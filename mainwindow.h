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
class Filter;
class WaveGenerator;
class TimeDomainPlot;
class FreqDomainPlot;

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
    // Connected explicitly in the constructor. Deliberately not named
    // on_<widget>_clicked: that form is reserved for connectSlotsByName, and
    // reusing it here would make Qt fire these twice.
    void loadWavFile();
    void startPlayback();
    void stopPlayback();
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
    Filter *filter;
    WaveGenerator *waveGenerator;
    TimeDomainPlot *timeDomainPlot;
    FreqDomainPlot *freqDomainPlot;

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

    void updateUIForPlaybackState(bool isPlaying);

    QTimer *graphUpdateTimer;
    void setupGraphUpdateTimer();
};

#endif // MAINWINDOW_H
