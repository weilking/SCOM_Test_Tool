#include "cwidgetserialport.h"

#include <QSerialPortInfo>


CWidgetSerialPort::CWidgetSerialPort(QWidget *parent) : QWidget(parent)
{

    //Create new layout
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    //(0,0) Add Label Text - COM Port
    qLabelCOMPort = new QLabel(this);
    qLabelCOMPort->setText("COM Port");

    mainLayout->addWidget(qLabelCOMPort, 0, 0);

    //(1,0) Add Label Text - Baudrate
    qLabelCOMBaudrate = new QLabel(this);
    qLabelCOMBaudrate->setText("Baudrate");

    mainLayout->addWidget(qLabelCOMBaudrate, 1, 0);

    //(0,1) Add ComboBox - COM Port
    qComboBoxCOMPort = new QComboBox(this);
    mainLayout->addWidget(qComboBoxCOMPort, 0, 1);
    //Scan all available ports and add to list

    QList<QSerialPortInfo> qListSerialPortInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &qSerialPortInfoTmp, qListSerialPortInfo)
    {
        if(false == qSerialPortInfoTmp.isNull())
        {
           qComboBoxCOMPort->addItem(qSerialPortInfoTmp.portName());
        }
    }

    //(1,0) Add ComboBox - Baudrate
    qComboBoxBaudrate = new QComboBox(this);
    mainLayout->addWidget(qComboBoxBaudrate, 1, 1);
    //Add the first port
    QList<qint32> qListBaudrate;

    if(!qListSerialPortInfo.isEmpty())
    {
        qListBaudrate = qListSerialPortInfo.at(0).standardBaudRates();
    }


    //Add baudrate to combobox
    foreach (const qint32 &i32TmpBaudrate, qListBaudrate)
    {
        qComboBoxBaudrate->addItem(QString::number(i32TmpBaudrate,10));

    }
    //Set default baudrate to 9600
    qComboBoxBaudrate->setCurrentIndex(qComboBoxBaudrate->findText("9600"));


    //Finish layout setting
    setLayout(mainLayout);

    //Initialize Signal-slot Connection between widgets
    InitializeSignalConnection();
}

void CWidgetSerialPort::InitializeSignalConnection()
{

}

void CWidgetSerialPort::PublicSlotOpenClose(bool bOpen)
{
    //Enable/Disable two Combobox
    qComboBoxCOMPort->setEnabled(!bOpen);
    qComboBoxBaudrate->setEnabled(!bOpen);

    //emit signal
    emit SignalCOMPortInfo(qComboBoxCOMPort->currentText(), qComboBoxBaudrate->currentText());
}

void CWidgetSerialPort::PublicSlotCOMPortInquiry()
{
    //emit current serial port setting in combobox
    emit SignalCOMPortInfo(qComboBoxCOMPort->currentText(), qComboBoxBaudrate->currentText());
}
