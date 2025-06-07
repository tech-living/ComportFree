#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , _serialPort(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if(_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
        _serialPort = nullptr;
    }

    delete ui;
}

void MainWindow::on_btnPortsInfo_clicked()
{
    loadPorts();
}

void MainWindow::loadPorts()
{
    ui->cmbPorts->clear();
    foreach(auto &port, QSerialPortInfo::availablePorts()) {
        ui->cmbPorts->addItem(port.portName());
    }
}


void MainWindow::on_btnOpenPort_clicked()
{
    if(_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
        _serialPort = nullptr;
    }

    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(ui->cmbPorts->currentText());
    _serialPort->setBaudRate(QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);

    if(_serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, "Result",
                                 "Port Opened successfully");
    } else {
        QMessageBox::critical(this, "Port Error",
                                 "Unable to open specified port ...");

    }

}

