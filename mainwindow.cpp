#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include "GlobalVariable.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_csPort(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    extern const QString strVerision;

    setWindowTitle("ComportFree " + strVerision);
    setGeometry(300, 100, 600, 450);

    ui->ui_mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margin
    ui->ui_mainLayout->setSpacing(0);
    ui->ui_mainLayout->addWidget(ui->tabWidget);

    ui->tab_basic->setLayout(ui->vLayout_basic_main);
    ui->vLayout_basic_main->setContentsMargins(0, 15, 0, 15);

    ui->tab_about->setLayout(ui->vLayout_about_main);
    m_uiAbout = new UiAbout(ui->tabWidget);
    ui->vLayout_about_main->addWidget(m_uiAbout);

    ui->tabWidget->setCurrentIndex(0);

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

    m_statusBar->showMessage(tr("Ready"), 0);

    //Limit log storage to 1000 lines; will be configurable in settings later.
    ui->plnTxtViewer->setMaximumBlockCount(1000);
    ui->plnTxtViewer->setReadOnly(true);

    //connections

    // connect(&m_csPort, &CSerialport::sigDataReceived, this, &MainWindow::slotReadData); //QByteArray Raw Data
    connect(&m_csPort, &CSerialport::sigDataReceivedString, this, &MainWindow::slotReadData); //QString

    connect(ui->btnPortsInfo, &QPushButton::clicked, this, &MainWindow::slotBtnPortsInfo);
    connect(ui->btnOpenPort, &QPushButton::clicked, this, &MainWindow::slotBtnOpenPort);
    connect(ui->btnClosePort, &QPushButton::clicked, this, &MainWindow::slotBtnClosePort);
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::slotBtnSend);
    connect(ui->btnClearTxtLog, &QPushButton::clicked, this, &MainWindow::slotBtnClearLog);
    connect(ui->btnSaveLog, &QPushButton::clicked, this, &MainWindow::slotBtnSaveLog);

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

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(auto it = availablePorts.constBegin(); it != availablePorts.constEnd(); ++it) {
        ui->cmbPorts->addItem(it->portName());
    }

    m_statusBar->showMessage(tr("Searched for comports."), 0);
}


void MainWindow::slotBtnOpenPort()
{
    QString strPortName = ui->cmbPorts->currentText();
    if(m_csPort.openPort(strPortName)) {
        m_statusBar->showMessage(strPortName + tr(" Port is opened successfully"), 0);
    } else {
        m_statusBar->showMessage(strPortName + tr(" Unable to open specified port ..."), 0);
    }
}

void MainWindow::slotBtnClosePort()
{
    m_csPort.close();
    m_statusBar->showMessage(tr("Port is closed."), 0);
}

void MainWindow::slotBtnSaveLog()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save File"), "", tr("Text Files (*.txt)"));

    if(filePath.isEmpty()) {
        m_statusBar->showMessage(tr("No file name selected."), 0);
        return;
    }


    QFile file(filePath);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_statusBar->showMessage(tr("File creation error."), 0);
        return;
    }

    QTextStream out(&file);
    out << m_strLogData;

    out.flush();
    file.close();
    m_statusBar->showMessage(tr("Log file creation complete."), 0);
}


void MainWindow::slotBtnSend()
{
    qint64 numBytes = m_csPort.write(ui->lnMessage->text().toUtf8());

    if(-1 == numBytes) {
        m_statusBar->showMessage(tr("Something went wrong!"), 0);
    }

}


void MainWindow::slotReadData(QString _data)
{

    // Reads all available data from the serial port.
    auto receivedData = _data;

    //Will be made configurable in the settings menu.
    bool bTimeStemp = true;

    if(bTimeStemp) {
        QString timeStamp = QDateTime::currentDateTime().toString("[HH:mm:ss.zzz] ");
        m_strLogData.append(timeStamp + receivedData);
    } else {
        m_strLogData.append(receivedData);
    }
    m_strLogData.append("\n");


    // aAppends plain text using appendPlainText() function.
    ui->plnTxtViewer->appendPlainText(receivedData);

    // Scrolls the text viewer to the latest content.
    ui->plnTxtViewer->verticalScrollBar()->setValue(ui->plnTxtViewer->verticalScrollBar()->maximum());
}


void MainWindow::slotBtnClearLog()
{
    ui->plnTxtViewer->clear();
    m_strLogData.clear();
}


























