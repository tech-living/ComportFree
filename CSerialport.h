#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class CSerialport : public QObject
{
    Q_OBJECT
public:
    explicit CSerialport(QObject *parent = nullptr);
    ~CSerialport();

    bool openPort(QString _portname);
    qint64 write(QByteArray _data);
    void close();

private:
    QSerialPort *m_serialPort;

    QByteArray m_receivedBuffer;         // receive buffer
    QTimer* m_displayTimer = nullptr;    // Display update Timer


private slots:
    void slotReadData();
    void slotProcessAndDisplayBuffer(); // slot for signal(QTimer::timeout())

signals:
    void sigDataReceived(QByteArray _data);

};

#endif // CSERIALPORT_H
