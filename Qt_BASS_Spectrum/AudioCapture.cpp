#include "AudioCapture.h"
#include <QDebug>
#include <QTimer>

AudioCapture::AudioCapture(QObject *parent) :
    QObject(parent),
    mId(-1),
    mIsRunning(false)
{
    qRegisterMetaType< QList<AudioDeviceInfo> >("QList<AudioDeviceInfo>");
    qRegisterMetaType< QVector<double> >("QVector<double>");

    BASS_SetConfig(BASS_CONFIG_UNICODE, true);
    if(!BASS_Init(0, 44100, 0, 0, NULL))
    {
        qCritical() << Q_FUNC_INFO << "BASS_Init failed" << BASS_ErrorGetCode();
        mIsInited = false;
        return;
    }
    mIsInited = true;

    QTimer::singleShot(25, this, SLOT(slotTimerGetFFT()));
}

// WASAPI callback - not doing anything with the data
DWORD CALLBACK DuffRecording(void *buffer, DWORD length, void *user)
{
    Q_UNUSED(buffer);
    Q_UNUSED(length);
    Q_UNUSED(user);
    return TRUE; // continue recording
}

void AudioCapture::populateDeviceList()
{
    if(!mIsInited)
        return;

    QList<AudioDeviceInfo> list;
    for(int i=0; i<MAX_DEVICE_ID; i++)
    {
        BASS_WASAPI_DEVICEINFO inf;
        if(BASS_WASAPI_GetDeviceInfo(i, &inf))
        {
            if((inf.flags & BASS_DEVICE_ENABLED) && (inf.flags & BASS_DEVICE_LOOPBACK))
            {
                AudioDeviceInfo adinf;
                adinf.id = i;
                adinf.name = inf.name;
                list.push_back(adinf);
            }
        }
    }

    emit signalDeviceList(list);
}

void AudioCapture::openDevice(int id)
{
    if(mIsRunning)
    {
        if(id == mId)
            return;

        BASS_WASAPI_Free();
    }

    mId = id;

    if(!BASS_WASAPI_Init(mId, 0, 0, BASS_WASAPI_BUFFER, 1, 0.1f, &DuffRecording, NULL))
    {
        qCritical() << Q_FUNC_INFO << "BASS_WASAPI_Init failed" << BASS_ErrorGetCode();
        mIsRunning = false;
        return;
    }

    BASS_WASAPI_Start();
    mIsRunning = true;
}

void AudioCapture::slotTimerGetFFT()
{
    if(mIsRunning)
    {
        BASS_WASAPI_GetData(mFFT, BASS_DATA_FFT2048);
        QVector<double> fftdata;
        for(int i=0; i<1024; i++)
            fftdata.push_back(mFFT[i]);
        emit signalFFTData(fftdata);
    }
    QTimer::singleShot(25, this, SLOT(slotTimerGetFFT()));
}
