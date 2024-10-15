#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>

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
};

#endif // MAINWINDOW_H
