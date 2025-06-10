#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>
#include <QStatusBar>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_serialPort(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("ComportFree v0.1");

    // Timer init and connect
    m_displayTimer = new QTimer(this);
    connect(m_displayTimer, &QTimer::timeout, this, &MainWindow::slotProcessAndDisplayBuffer);
    m_displayTimer->setInterval(100); // process buffer every 100ms

    //Limit log storage to 1000 lines; will be configurable in settings later.
    ui->plnTxtViewer->setMaximumBlockCount(1000);
    ui->plnTxtViewer->setReadOnly(true);

}

MainWindow::~MainWindow()
{
    if(m_serialPort != nullptr) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
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
    if(m_serialPort != nullptr) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }

    m_serialPort = new QSerialPort(this);
    m_serialPort->setPortName(ui->cmbPorts->currentText());
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

    if(m_serialPort->open(QIODevice::ReadWrite)) {
        //QMessageBox::information(this, "Result", "Port Opened successfully");

        connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::slotReadData);
    } else {
        QMessageBox::critical(this, "Port Error",
                                 "Unable to open specified port ...");
    }

}

void MainWindow::on_btnClosePort_clicked()
{
    if(m_serialPort != nullptr && m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}


void MainWindow::on_btnSend_clicked()
{
    if(m_serialPort == nullptr || !m_serialPort->isOpen()) {
        QMessageBox::critical(this, "Port Error",
                              "Comport is not opened!");
        return;
    }

    m_serialPort->write(ui->lnMessage->text().toUtf8());
}

void MainWindow::slotReadData()
{

    if(m_serialPort == nullptr || !m_serialPort->isOpen()) {
        // To be displayed in the status bar.
        qDebug() << "Comport is not opened!";
        // QMessageBox::critical(this, "Port Error", "Comport is not opened!");
        return;
    }

    // Reads all available data from the serial port.
    auto receivedData = m_serialPort->readAll();

    // Adds data to the buffer (improves performance when processing large amounts of data).
    m_receivedBuffer.append(receivedData);

    // 데Batches UI updates for smoother performance when data arrives rapidly,
    // preventing UI freezes/hiccups.
    if (!m_displayTimer->isActive()) {
        m_displayTimer->start();
    }

}

// rocesses and displays data in m_receivedBuffer
void MainWindow::slotProcessAndDisplayBuffer()
{
    if (m_receivedBuffer.isEmpty()) {
        m_displayTimer->stop(); // 버퍼가 비면 타이머를 멈춥니다.
        return;
    }

    // Variables to store HEX and ASCII strings
    QString hexDisplay;
    QString asciiDisplay;

    // Process all data in the buffer.
    QByteArray dataToProcess = m_receivedBuffer;
    m_receivedBuffer.clear();

    // Hexadecimal (HEX) display
    hexDisplay = dataToProcess.toHex(' ').toUpper();

    // ASCII display (detailed handling of control characters)
    for (char byte : dataToProcess) {
        if (byte >= 0x20 && byte <= 0x7E) { // // Printable ASCII character
            asciiDisplay += byte;
        } else {
            /* Will be made configurable in the settings menu.

            // Display specific control characters in detail
            switch (static_cast<unsigned char>(byte)) {
            case '\r':
                asciiDisplay += "[CR]"; // Carriage Return
                break;
            case '\n':
                asciiDisplay += "[LF]"; // Line Feed
                break;
            case '\t':
                asciiDisplay += "[TAB]"; // Tab
                break;
            case '\0':
                asciiDisplay += "[NUL]"; // Null character
                break;
            default:
                // Replace other non-printable characters with '.'
                asciiDisplay += '.';
                break;
            }
            */
        }
    }

    //Will be made configurable in the settings menu.
    bool bOutputHex = false;
    QString lineToDisplay;

    if(bOutputHex) {
        // Hex Display
        lineToDisplay = QString("%1").arg(hexDisplay);
    } else {
        // ASCII Display
        lineToDisplay = QString("%1").arg(asciiDisplay);
    }

    // aAppends plain text using appendPlainText() function.
    ui->plnTxtViewer->appendPlainText(lineToDisplay);

    // Scrolls the text viewer to the latest content.
    ui->plnTxtViewer->verticalScrollBar()->setValue(ui->plnTxtViewer->verticalScrollBar()->maximum());

}


void MainWindow::on_btnClearTxtBrower_clicked()
{
    ui->plnTxtViewer->clear();
}


























