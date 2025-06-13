#include "CSerialport.h"

CSerialport::CSerialport(QObject *parent)
    : QObject{parent}
    , m_serialPort(nullptr)
{

    // Timer init and connect
    m_displayTimer = new QTimer(this);
    connect(m_displayTimer, &QTimer::timeout, this, &CSerialport::slotProcessAndDisplayBuffer);
    m_displayTimer->setInterval(100); // process buffer every 100ms

}

CSerialport::~CSerialport()
{
    if(m_serialPort != nullptr) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }
}

bool CSerialport::openPort(QString _portname)
{
    if(m_serialPort != nullptr) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }

    m_serialPort = new QSerialPort(this);
    m_serialPort->setPortName(_portname);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

    if(m_serialPort->open(QIODevice::ReadWrite)) {
        connect(m_serialPort, &QSerialPort::readyRead, this, &CSerialport::slotReadData);
    }

    return m_serialPort->isOpen();
}

 qint64 CSerialport::write(QByteArray _data)
{
    if(m_serialPort == nullptr || !m_serialPort->isOpen()) {
        //m_statusBar->showMessage("Comport is not opened!", 0);
        return false;
    }

    return m_serialPort->write(_data);
}

void CSerialport::close()
{
    if(m_serialPort != nullptr) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }
}

void CSerialport::slotReadData()
{
    if(m_serialPort->isOpen()) {
        emit sigDataReceived(m_serialPort->readAll());
    }

    /*
    // Reads all available data from the serial port.
    auto receivedData = m_serialPort->readAll();

    // Adds data to the buffer (improves performance when processing large amounts of data).
    m_receivedBuffer.append(receivedData);

    // 데Batches UI updates for smoother performance when data arrives rapidly,
    // preventing UI freezes/hiccups.
    if (!m_displayTimer->isActive()) {
        m_displayTimer->start();
    }

*/
}

void CSerialport::slotProcessAndDisplayBuffer()
{
    if (m_receivedBuffer.isEmpty()) {
        m_displayTimer->stop(); // Stops the timer if the buffer is empty.
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
    //ui->plnTxtViewer->appendPlainText(lineToDisplay);

    // Scrolls the text viewer to the latest content.
    //ui->plnTxtViewer->verticalScrollBar()->setValue(ui->plnTxtViewer->verticalScrollBar()->maximum());

}
