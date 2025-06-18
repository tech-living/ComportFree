#ifndef UIBASIC_H
#define UIBASIC_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QScrollBar>

#include "CSerialport.h"

namespace Ui {
class UiBasic;
}

class UiBasic : public QWidget
{
    Q_OBJECT

public:
    explicit UiBasic(QWidget *parent = nullptr);
    ~UiBasic();

private:
    Ui::UiBasic *ui;

    void loadPorts();
    //QSerialPort *m_serialPort;
    CSerialport m_csPort;

    QString m_strLogData;

private slots:
    void slotBtnPortsInfo();
    void slotBtnOpenPort();
    void slotBtnSend();
    void slotBtnClearLog();
    void slotBtnClosePort();
    void slotBtnSaveLog();

    void slotReadData(QString _data);

};

#endif // UIBASIC_H
