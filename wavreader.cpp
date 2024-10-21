// wavreader.cpp
#include "wavreader.h"
#include <QFile>
#include <QDataStream>

bool WaveReader::readWavFile(const QString& filePath, QVector<double>& samples, double& sampleRate)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    char id[4];
    in.readRawData(id, 4);

    if (strncmp(id, "RIFF", 4) != 0)
        return false;

    quint32 size;
    in >> size;

    in.readRawData(id, 4);
    if (strncmp(id, "WAVE", 4) != 0)
        return false;

    while (!in.atEnd()) {
        in.readRawData(id, 4);
        in >> size;

        if (strncmp(id, "fmt ", 4) == 0) {
            quint16 audioFormat, numChannels;
            quint32 sampleRateTemp, byteRate;
            quint16 blockAlign, bitsPerSample;

            in >> audioFormat >> numChannels >> sampleRateTemp >> byteRate >> blockAlign >> bitsPerSample;

            sampleRate = sampleRateTemp;

            in.skipRawData(size - 16);
        }
        else if (strncmp(id, "data", 4) == 0) {
            int numSamples = size / 2;
            samples.resize(numSamples);

            for (int i = 0; i < numSamples; ++i) {
                qint16 sample;
                in >> sample;
                samples[i] = sample / 32768.0; // Normalize 16-bit to -1.0 1.0
            }

            break;
        }
        else {
            in.skipRawData(size);
        }
    }

    file.close();
    return true;
}
