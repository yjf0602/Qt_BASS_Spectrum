#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->addGraph();
    ui->plot->xAxis->setRange(0, 600);
    ui->plot->yAxis->setRange(0, 0.1);

    mAudioCapture = new AudioCapture(this);

    connect(mAudioCapture, SIGNAL(signalDeviceList(QList<AudioDeviceInfo>)),
            this, SLOT(slotDeviceListUpdate(QList<AudioDeviceInfo>)));

    connect(mAudioCapture, SIGNAL(signalFFTData(QVector<double>)),
            this, SLOT(slotNewFFTData(QVector<double>)));

    mAudioCapture->populateDeviceList();

    connect(ui->comboBox, SIGNAL(activated(int)),
            this, SLOT(slotComboBoxChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotDeviceListUpdate(QList<AudioDeviceInfo> list)
{
    mDeviceList = list;

    QStringList deviceNameList;
    for(int i=0; i<mDeviceList.size(); i++)
    {
        deviceNameList.push_back(mDeviceList.at(i).name);
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(deviceNameList);
}

void MainWindow::slotComboBoxChanged()
{
    int id = mDeviceList.at(ui->comboBox->currentIndex()).id;
    mAudioCapture->openDevice(id);
}

void MainWindow::slotNewFFTData(QVector<double> fftdata)
{
    QVector<double>keys;
    for(int i=0; i<1024; i++)
        keys.push_back(i);
    ui->plot->graph(0)->setData(keys, fftdata);
    ui->plot->replot();
}
