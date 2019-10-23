#ifndef CWIDGETCONTROLPANEL_H
#define CWIDGETCONTROLPANEL_H

#include <QWidget>
#include <QtWidgets>

class CWidgetControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetControlPanel(QWidget *parent = nullptr);

private:
    //Button to Connect/Disconnect SerialPort
    QPushButton* qPushButtonConnect;

    //Button to shot single SCOM message
    QPushButton* qPushButtonSingleShot;

    //Button to start/stop SCOM message queue worker
    QPushButton* qPushButtonStart;

    //Button to run commmand list
    QPushButton* qPushButtonRun;

signals:
    SignalButtonConnect(bool bConnectDisconnect);

    SignalButtonStart(bool bStartStop);

    SignalButtonSingleShot(void);

    SignalButtonRun(bool bRunCancel);

    //signal to output to log box
    SignalLogOutput(QString sLog);

private slots:
    void PrivateSlotConnect(void);

    void PrivateSlotStart(void);

    void PrivateSlotSingleShot(void);

    void PrivateSlotRun(bool bRunCancel);

public slots:

    void PublicSlotConnect(bool bConnectDisconnect);

    void PublicSlotStart(bool bStartStop);

    void PublicSlotSingleShot(void);

    void PublicSlotRun(bool bRunCancel);

};

#endif // CWIDGETCONTROLPANEL_H
