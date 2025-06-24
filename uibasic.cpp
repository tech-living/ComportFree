#include "uibasic.h"
#include "ui_uibasic.h"

#include <QFileDialog>

UiBasic::UiBasic(QWidget *parent)
    : QWidget(parent)
    , m_csPort(nullptr)
    , ui(new Ui::UiBasic)
{
    ui->setupUi(this);


    //Limit log storage to 1000 lines; will be configurable in settings later.
    ui->plnTxtViewer->setMaximumBlockCount(1000);
    ui->plnTxtViewer->setReadOnly(true);

    //connections

    // connect(&m_csPort, &CSerialport::sigDataReceived, this, &MainWindow::slotReadData); //QByteArray Raw Data
    connect(&m_csPort, &CSerialport::sigDataReceivedString, this, &UiBasic::slotReadData); //QString

    connect(ui->btnPortsInfo, &QPushButton::clicked, this, &UiBasic::slotBtnPortsInfo);
    connect(ui->btnOpenPort, &QPushButton::clicked, this, &UiBasic::slotBtnOpenPort);
    connect(ui->btnClosePort, &QPushButton::clicked, this, &UiBasic::slotBtnClosePort);
    connect(ui->btnSend, &QPushButton::clicked, this, &UiBasic::slotBtnSend);
    connect(ui->btnClearTxtLog, &QPushButton::clicked, this, &UiBasic::slotBtnClearLog);
    connect(ui->btnSaveLog, &QPushButton::clicked, this, &UiBasic::slotBtnSaveLog);
    connect(ui->btnClose, &QPushButton::clicked, this, &QApplication::quit);

}

UiBasic::~UiBasic()
{
    m_csPort.close();
    delete ui;
}


void UiBasic::slotBtnPortsInfo()
{
    loadPorts();
}

void UiBasic::loadPorts()
{
    ui->cmbPorts->clear();

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(auto it = availablePorts.constBegin(); it != availablePorts.constEnd(); ++it) {
        ui->cmbPorts->addItem(it->portName());
    }

   // m_statusBar->showMessage(tr("Searched for comports."), 0);
}


void UiBasic::slotBtnOpenPort()
{
    QString strPortName = ui->cmbPorts->currentText();
    if(m_csPort.openPort(strPortName)) {
       // m_statusBar->showMessage(strPortName + tr(" Port is opened successfully"), 0);
    } else {
       // m_statusBar->showMessage(strPortName + tr(" Unable to open specified port ..."), 0);
    }
}

void UiBasic::slotBtnClosePort()
{
    m_csPort.close();
    //m_statusBar->showMessage(tr("Port is closed."), 0);
}

void UiBasic::slotBtnSaveLog()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save File"), "", tr("Text Files (*.txt)"));

    if(filePath.isEmpty()) {
        //m_statusBar->showMessage(tr("No file name selected."), 0);
        return;
    }


    QFile file(filePath);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //m_statusBar->showMessage(tr("File creation error."), 0);
        return;
    }

    QTextStream out(&file);
    out << m_strLogData;

    out.flush();
    file.close();
    //m_statusBar->showMessage(tr("Log file creation complete."), 0);
}


void UiBasic::slotBtnSend()
{
    qint64 numBytes = m_csPort.write(ui->lnMessage->text().toUtf8());

    if(-1 == numBytes) {
       // m_statusBar->showMessage(tr("Something went wrong!"), 0);
    }

}


void UiBasic::slotReadData(QString _data)
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


void UiBasic::slotBtnClearLog()
{
    ui->plnTxtViewer->clear();
    m_strLogData.clear();
}
