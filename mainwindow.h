#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QtMultimedia>
#include <QAudioFormat>
#include <QMediaDevices>

class FFT;
class WaveGenerator;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generatorButton_start_clicked();
    void on_generatorButton_stop_clicked();
    void updater();
    void on_loadWavButton_clicked();
    void on_playButton_clicked();
    void on_stopButton_clicked();
    void on_progressSlider_valueChanged(int value);
    void updatePlaybackInfo();
    void updateGraphs();
    void setupGraphs();

private:
    bool validateInputs();
    void generateWave();
    void updateSignal();
    void plotWave();
    void computeFFT();
    void plotFFT();

    Ui::MainWindow *ui;
    QTimer *timer;
    FFT *fft;
    WaveGenerator *waveGenerator;

    QVector<double> time;
    QVector<double> signal;
    QVector<double> fftOutput;
    QVector<double> fftFreqSamp;

    bool isRunning;
    double currentTime;

    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    QTimer *playbackTimer;
    QVector<double> wavSamples;
    double wavSampleRate;
    qint64 wavDuration;

    void setupAudioPlayback();
    void updateUIForPlaybackState(bool isPlaying);

    QTimer *graphUpdateTimer;
    void setupGraphUpdateTimer();
};

#endif // MAINWINDOW_H
