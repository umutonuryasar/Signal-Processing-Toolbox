#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer *timer;

    QVector<double> time, signal;
    QVector<double> fftFreqSamp, fftOutput;

    double t = 0;

    void generateWave(QVector<double> &wave);
    void plotWave(QVector<double> &wave);

    void computeFFT(const QVector<double> &inputData);
    void plotFFT(QVector<double> &fftFreqSamp, QVector<double> &fftOutput);

    bool validateInputs();

private slots:
    void on_generatorButton_start_clicked();
    void on_generatorButton_pause_clicked();
    void on_generatorButton_stop_clicked();
    void updater();

};
#endif // MAINWINDOW_H
