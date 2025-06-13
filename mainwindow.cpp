#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>
#include <QStatusBar>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_csPort(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("ComportFree v0.1");
    setGeometry(100, 100, 400, 300);

    ui->ui_mainLayout->setContentsMargins(0, 0, 0, 0); // // Remove margin
    ui->ui_mainLayout->setSpacing(0);
    ui->ui_mainLayout->addWidget(ui->tabWidget);

    ui->tab_basic->setLayout(ui->vLayout_basic_main);
    ui->vLayout_basic_main->setContentsMargins(0, 15, 0, 15); // 마진 제거


    //Create QStatusBar instance directly and add it to the layout
    m_statusBar = new QStatusBar(this); // Specify the parent widget
    ui->ui_mainLayout->addWidget(m_statusBar);  // Add to the bottom of VBoxLayout

    // Apply style to QStatusBar as well.
    m_statusBar->setStyleSheet(
        "QStatusBar { "
        "   background-color: #e0e0e0; "
        "   border-top: 1px solid #c0c0c0; "
        "   padding: 0px; "
        "   border: 1px solid block; " // Check QStatusBar's own boundaries
        "}"
        "QStatusBar::item { "
        "   border: none; "
        "}"
        );

    m_statusBar->showMessage("Ready", 0);


    /*
    // Timer init and connect
    m_displayTimer = new QTimer(this);
    connect(m_displayTimer, &QTimer::timeout, this, &MainWindow::slotProcessAndDisplayBuffer);
    m_displayTimer->setInterval(100); // process buffer every 100ms
    */
    //Limit log storage to 1000 lines; will be configurable in settings later.
    ui->plnTxtViewer->setMaximumBlockCount(1000);
    ui->plnTxtViewer->setReadOnly(true);

    //connections

    connect(&m_csPort, &CSerialport::sigDataReceived, this, &MainWindow::slotReadData);

    connect(ui->btnPortsInfo, &QPushButton::clicked, this, &MainWindow::slotBtnPortsInfo);
    connect(ui->btnOpenPort, &QPushButton::clicked, this, &MainWindow::slotBtnOpenPort);
    connect(ui->btnClosePort, &QPushButton::clicked, this, &MainWindow::slotBtnClosePort);
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::slotBtnSend);
    connect(ui->btnClearTxtBrower, &QPushButton::clicked, this, &MainWindow::slotBtnClearTxtBrower);

}


MainWindow::~MainWindow()
{

    m_csPort.close();

    delete ui;
}

void MainWindow::slotBtnPortsInfo()
{
    loadPorts();
}

void MainWindow::loadPorts()
{
    ui->cmbPorts->clear();
    foreach(auto &port, QSerialPortInfo::availablePorts()) {
        ui->cmbPorts->addItem(port.portName());
    }

    m_statusBar->showMessage("Searched for comports.", 0);
}


void MainWindow::slotBtnOpenPort()
{
    if(m_csPort.openPort(ui->cmbPorts->currentText())) {
        m_statusBar->showMessage("Port is opened successfully", 0);
    } else {
        m_statusBar->showMessage("Unable to open specified port ...", 0);
    }
}

void MainWindow::slotBtnClosePort()
{
    m_csPort.close();
    m_statusBar->showMessage("Port is closed.", 0);
}


void MainWindow::slotBtnSend()
{

    qint64 numBytes = m_csPort.write(ui->lnMessage->text().toUtf8());

    if(-1 == numBytes) {
        m_statusBar->showMessage("Something went wrong!", 0);
    }

}

void MainWindow::slotReadData(QByteArray _data)
{

    // Reads all available data from the serial port.
    auto receivedData = _data;

    /*
    // Adds data to the buffer (improves performance when processing large amounts of data).
    m_receivedBuffer.append(receivedData);

    // Batches UI updates for smoother performance when data arrives rapidly,
    // preventing UI freezes/hiccups.
    if (!m_displayTimer->isActive()) {
        m_displayTimer->start();
    }

    */

}


// rocesses and displays data in m_receivedBuffer
// void MainWindow::slotProcessAndDisplayBuffer()
// {
//     if (m_receivedBuffer.isEmpty()) {
//         m_displayTimer->stop(); // Stops the timer if the buffer is empty.
//         return;
//     }

//     // Variables to store HEX and ASCII strings
//     QString hexDisplay;
//     QString asciiDisplay;

//     // Process all data in the buffer.
//     QByteArray dataToProcess = m_receivedBuffer;
//     m_receivedBuffer.clear();

//     // Hexadecimal (HEX) display
//     hexDisplay = dataToProcess.toHex(' ').toUpper();

//     // ASCII display (detailed handling of control characters)
//     for (char byte : dataToProcess) {
//         if (byte >= 0x20 && byte <= 0x7E) { // // Printable ASCII character
//             asciiDisplay += byte;
//         } else {
//             /* Will be made configurable in the settings menu.

//             // Display specific control characters in detail
//             switch (static_cast<unsigned char>(byte)) {
//             case '\r':
//                 asciiDisplay += "[CR]"; // Carriage Return
//                 break;
//             case '\n':
//                 asciiDisplay += "[LF]"; // Line Feed
//                 break;
//             case '\t':
//                 asciiDisplay += "[TAB]"; // Tab
//                 break;
//             case '\0':
//                 asciiDisplay += "[NUL]"; // Null character
//                 break;
//             default:
//                 // Replace other non-printable characters with '.'
//                 asciiDisplay += '.';
//                 break;
//             }
//             */
//         }
//     }

//     //Will be made configurable in the settings menu.
//     bool bOutputHex = false;
//     QString lineToDisplay;

//     if(bOutputHex) {
//         // Hex Display
//         lineToDisplay = QString("%1").arg(hexDisplay);
//     } else {
//         // ASCII Display
//         lineToDisplay = QString("%1").arg(asciiDisplay);
//     }

//     // aAppends plain text using appendPlainText() function.
//     ui->plnTxtViewer->appendPlainText(lineToDisplay);

//     // Scrolls the text viewer to the latest content.
//     ui->plnTxtViewer->verticalScrollBar()->setValue(ui->plnTxtViewer->verticalScrollBar()->maximum());

// }


void MainWindow::slotBtnClearTxtBrower()
{
    ui->plnTxtViewer->clear();
}


























