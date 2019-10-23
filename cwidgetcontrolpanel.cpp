#include "cwidgetcontrolpanel.h"

CWidgetControlPanel::CWidgetControlPanel(QWidget *parent) : QWidget(parent)
{

    QGridLayout *qGridLayoutMain = new QGridLayout();


    //(0,0) Connect button

    qPushButtonConnect = new QPushButton(this);
    qPushButtonConnect->setText("Connect");

    qGridLayoutMain->addWidget(qPushButtonConnect, 0,0);

    //(0, 1) Single Shot Button

    qPushButtonSingleShot = new QPushButton(this);
    qPushButtonSingleShot->setText("Single Shot");
    qPushButtonSingleShot->setEnabled(false);

    qGridLayoutMain->addWidget(qPushButtonSingleShot, 0,1);


    //(1, 0) Start Button

    qPushButtonStart = new QPushButton(this);
    qPushButtonStart->setText("Start");
    qPushButtonStart->setEnabled(false);

    qGridLayoutMain->addWidget(qPushButtonStart, 1,0);


    //(1,1) Run Button
    qPushButtonRun = new QPushButton(this);
    qPushButtonRun->setText("Run Script");
    qPushButtonRun->setCheckable(true);
    qPushButtonRun->setChecked(false);
    qPushButtonRun->setEnabled(false);

    qGridLayoutMain->addWidget(qPushButtonRun, 1,1);

    //Finish layout setting
    setLayout(qGridLayoutMain);

    //Connect signal
    connect(qPushButtonConnect, SIGNAL(clicked()), this, SLOT(PrivateSlotConnect()));
    connect(qPushButtonStart, SIGNAL(clicked()), this, SLOT(PrivateSlotStart()));
    connect(qPushButtonSingleShot, SIGNAL(clicked()), this, SLOT(PrivateSlotSingleShot()));
    connect(qPushButtonRun, SIGNAL(clicked(bool)), this, SLOT(PrivateSlotRun(bool)));
}

//Private slot: Connect Serial Port
void CWidgetControlPanel::PrivateSlotConnect()
{
    if(qPushButtonConnect->text() == "Connect")
    {
        //Set PushButton Connect text to "Disconnect"
        qPushButtonConnect->setText("Disconnect");

        //Enable Start and Single Shot Button
         qPushButtonStart->setEnabled(true);

         //emit Connect signal
         emit SignalButtonConnect(true);

         emit SignalLogOutput("[STATUS]CWidgetControlPanel:[PrivateSlotConnect]Serial Port Connect");
    }
    else
    {
        //Set PushButton Connect text to "Disconnect"
        qPushButtonConnect->setText("Connect");

        //If Script Run
        if(qPushButtonRun->isChecked())
        {
            emit SignalButtonRun(false);
        }

        qPushButtonRun->setEnabled(false);
        qPushButtonRun->setChecked(false);
        qPushButtonRun->setText("Run Script");

        //Disable Start and Single Shot Button
         qPushButtonStart->setEnabled(false);
         qPushButtonSingleShot->setEnabled(false);

         qPushButtonStart->setText("Start");

         //emit Stop signal
         emit SignalButtonStart(false);
         //emit Disconnect signal
         emit SignalButtonConnect(false);


         emit SignalLogOutput("[STATUS]CWidgetControlPanel:[PrivateSlotConnect]Serial Port Disconnect");
    }

}

//Private Slot: Start SCOM Queue/Worker
void CWidgetControlPanel::PrivateSlotStart()
{
    if(qPushButtonStart->text() == "Start")
    {
        qPushButtonStart->setText("Stop");

        qPushButtonSingleShot->setEnabled(true);
        qPushButtonRun->setEnabled(true);

        //emit start signal
        emit SignalButtonStart(true);
    }
    else
    {
        qPushButtonStart->setText("Start");

        qPushButtonSingleShot->setEnabled(false);

        //If Script Run
        if(qPushButtonRun->isChecked())
        {
            emit SignalButtonRun(false);
        }

        qPushButtonRun->setEnabled(false);
        qPushButtonRun->setChecked(false);
        qPushButtonRun->setText("Run");

        //emit stop signal
        emit SignalButtonStart(false);
    }

}

//Private Slot: Send single shot message from SCOM editor
void CWidgetControlPanel::PrivateSlotSingleShot()
{
    emit SignalButtonSingleShot();
}

//Private Slot: Start Script sending
void CWidgetControlPanel::PrivateSlotRun(bool bRunCancel)
{
    //If canceled
    if(!bRunCancel)
    {
        qPushButtonRun->setText("Run Script");
    }
    else
    {
        qPushButtonRun->setText("Cancel");

    }
    //emit Run/Cancel Script signal
    emit SignalButtonRun(bRunCancel);
}

void CWidgetControlPanel::PublicSlotConnect(bool bConnectDisconnect)
{

}

void CWidgetControlPanel::PublicSlotStart(bool bStartStop)
{

}

void CWidgetControlPanel::PublicSlotSingleShot()
{
    emit SignalButtonSingleShot();
}

void CWidgetControlPanel::PublicSlotRun(bool bRunCancel)
{
    qPushButtonRun->setText("Run Script");
    qPushButtonRun->setChecked(false);
}
