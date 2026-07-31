// wavreader.cpp
#include "wavreader.h"
#include <QFile>
#include <QDataStream>
#include <QtGlobal>
#include <cstring>

namespace {

const quint16 FormatPcm = 0x0001;
const quint16 FormatFloat = 0x0003;
const quint16 FormatExtensible = 0xFFFE;

bool readChunkId(QDataStream &in, char *id)
{
    return in.readRawData(id, 4) == 4;
}

// skipRawData() takes an int, so a chunk size close to 4 GB has to be skipped
// in several steps instead of overflowing into a negative count.
bool skipBytes(QDataStream &in, qint64 count)
{
    while (count > 0) {
        int step = static_cast<int>(qMin<qint64>(count, 1 << 20));
        int skipped = in.skipRawData(step);
        if (skipped <= 0)
            return false;
        count -= skipped;
    }
    return true;
}

double readOneSample(QDataStream &in, quint16 audioFormat, quint16 bitsPerSample)
{
    if (audioFormat == FormatFloat) {
        if (bitsPerSample == 64) {
            double value;
            in >> value;
            return value;
        }
        float value;
        in >> value;
        return value;
    }

    switch (bitsPerSample) {
    case 8: {
        // 8 bit PCM is stored unsigned with 128 as the zero level.
        quint8 sample;
        in >> sample;
        return (static_cast<int>(sample) - 128) / 128.0;
    }
    case 16: {
        qint16 sample;
        in >> sample;
        return sample / 32768.0;
    }
    case 24: {
        quint8 b0, b1, b2;
        in >> b0 >> b1 >> b2;
        qint32 sample = (static_cast<qint32>(b2) << 16)
                        | (static_cast<qint32>(b1) << 8)
                        | static_cast<qint32>(b0);
        if (sample & 0x800000)
            sample -= 0x1000000; // sign extend
        return sample / 8388608.0;
    }
    case 32: {
        qint32 sample;
        in >> sample;
        return sample / 2147483648.0;
    }
    default:
        return 0.0;
    }
}

// Decodes the data chunk into mono samples. Multi channel files are downmixed
// by averaging the channels, so one entry equals one frame and the time axis
// stays in sync with the playback position.
bool decodeSamples(QDataStream &in, quint32 dataSize, quint16 audioFormat,
                   quint16 numChannels, quint16 bitsPerSample, QVector<double> &samples)
{
    if (numChannels == 0 || bitsPerSample == 0 || bitsPerSample % 8 != 0)
        return false;

    if (audioFormat == FormatFloat) {
        if (bitsPerSample != 32 && bitsPerSample != 64)
            return false;
        in.setFloatingPointPrecision(bitsPerSample == 64 ? QDataStream::DoublePrecision
                                                         : QDataStream::SinglePrecision);
    } else if (audioFormat == FormatPcm) {
        if (bitsPerSample != 8 && bitsPerSample != 16 && bitsPerSample != 24 && bitsPerSample != 32)
            return false;
    } else {
        return false; // compressed formats are not supported
    }

    const qint64 frameSize = static_cast<qint64>(bitsPerSample / 8) * numChannels;
    const qint64 frameCount = static_cast<qint64>(dataSize) / frameSize;
    if (frameCount <= 0)
        return false;

    samples.resize(frameCount);

    for (qint64 frame = 0; frame < frameCount; ++frame) {
        double sum = 0.0;
        for (quint16 channel = 0; channel < numChannels; ++channel)
            sum += readOneSample(in, audioFormat, bitsPerSample);

        if (in.status() != QDataStream::Ok) {
            // Truncated file: keep whatever decoded cleanly.
            samples.resize(frame);
            return frame > 0;
        }

        samples[frame] = sum / numChannels;
    }

    return true;
}

} // namespace

bool WaveReader::readWavFile(const QString &filePath, QVector<double> &samples, double &sampleRate)
{
    samples.clear();
    sampleRate = 0.0;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    char id[4];
    quint32 size;

    if (!readChunkId(in, id) || strncmp(id, "RIFF", 4) != 0)
        return false;

    in >> size;

    if (!readChunkId(in, id) || strncmp(id, "WAVE", 4) != 0)
        return false;

    quint16 audioFormat = 0;
    quint16 numChannels = 0;
    quint16 bitsPerSample = 0;
    bool haveFormat = false;
    bool haveData = false;

    while (!in.atEnd()) {
        if (!readChunkId(in, id))
            break;

        in >> size;
        if (in.status() != QDataStream::Ok)
            break;

        if (strncmp(id, "fmt ", 4) == 0) {
            if (size < 16)
                return false;

            quint32 sampleRateTemp, byteRate;
            quint16 blockAlign;

            in >> audioFormat >> numChannels >> sampleRateTemp >> byteRate >> blockAlign >> bitsPerSample;
            if (in.status() != QDataStream::Ok)
                return false;

            if (sampleRateTemp == 0 || numChannels == 0)
                return false;

            sampleRate = sampleRateTemp;
            haveFormat = true;

            qint64 remaining = static_cast<qint64>(size) - 16;
            if (audioFormat == FormatExtensible && remaining >= 24) {
                // WAVE_FORMAT_EXTENSIBLE keeps the real format in the first two
                // bytes of the sub format GUID.
                quint16 extensionSize, validBitsPerSample, subFormat;
                quint32 channelMask;
                in >> extensionSize >> validBitsPerSample >> channelMask >> subFormat;
                if (in.status() != QDataStream::Ok)
                    return false;
                audioFormat = subFormat;
                remaining -= 10;
            }

            if (!skipBytes(in, remaining))
                return false;
        }
        else if (strncmp(id, "data", 4) == 0) {
            if (!haveFormat)
                return false;
            if (!decodeSamples(in, size, audioFormat, numChannels, bitsPerSample, samples))
                return false;
            haveData = true;
            break;
        }
        else {
            if (!skipBytes(in, size))
                break;
        }

        // RIFF chunks are word aligned: an odd size is followed by a pad byte.
        if (size % 2 != 0)
            skipBytes(in, 1);
    }

    file.close();
    return haveData && !samples.isEmpty();
}
