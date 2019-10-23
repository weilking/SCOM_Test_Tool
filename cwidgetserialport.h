#ifndef CWIDGETSERIALPORT_H
#define CWIDGETSERIALPORT_H

#include <QWidget>
#include <QSerialPort>
#include <QtWidgets>

//Debug prefix
#define CWidgetSerialPort_DEBUG_PREFIX ("CWidgetSerialPort: ")

class CWidgetSerialPort : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetSerialPort(QWidget *parent = 0);

private:

    //Label for COM Port
    QLabel *qLabelCOMPort;

    //Label for Baudrate
    QLabel *qLabelCOMBaudrate;

    //Combobox for COM Port
    QComboBox *qComboBoxCOMPort;

    //Combobox for Baudrate
    QComboBox *qComboBoxBaudrate;

    //Connect widget signals to target slot
    void InitializeSignalConnection(void);

signals:

    void SignalCOMPortInfo(QString qStringCOMPort, QString qStringBaudrate);

public slots:

    void PublicSlotOpenClose(bool bOpen);

    void PublicSlotCOMPortInquiry(void);



};

#endif // CWIDGETSERIALPORT_H
