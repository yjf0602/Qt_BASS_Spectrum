#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AudioCapture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotDeviceListUpdate(QList<AudioDeviceInfo> list);
    void slotComboBoxChanged();
    void slotNewFFTData(QVector<double> fftdata);

private:
    Ui::MainWindow *ui;

    AudioCapture *mAudioCapture;
    QList<AudioDeviceInfo> mDeviceList;
};
#endif // MAINWINDOW_H
