#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include <QObject>
#include <QList>
#include "bass.h"
#include "basswasapi.h"

#define MAX_DEVICE_ID   100

struct AudioDeviceInfo
{
    QString name;
    int id;
    AudioDeviceInfo() {}
};

class AudioCapture : public QObject
{
    Q_OBJECT
public:
    explicit AudioCapture(QObject *parent = nullptr);
    void populateDeviceList();
    void openDevice(int id);

public slots:
    void slotTimerGetFFT();

signals:
    void signalDeviceList(QList<AudioDeviceInfo> list);
    void signalFFTData(QVector<double> fftdata);

private:
    bool mIsInited;
    int mId;
    int mIsRunning;
    float mFFT[1024];
};

#endif // AUDIOCAPTURE_H
