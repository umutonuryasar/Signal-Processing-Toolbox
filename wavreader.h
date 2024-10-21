#ifndef WAVREADER_H
#define WAVREADER_H

#include <QString>
#include <QVector>

class WaveReader
{
public:
    static bool readWavFile(const QString& filePath, QVector<double>& samples, double& sampleRate);
};

#endif // WAVREADER_H
