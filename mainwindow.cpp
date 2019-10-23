#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    //Set central widget
    centralWidget = new QWidget();
    this->setCentralWidget(centralWidget);


    //Make main layout
    QGridLayout *mainLayout = new QGridLayout(centralWidget);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    //Serial Port Widget
    mWidgetSerialPort = new CWidgetSerialPort(this);
    mainLayout->addWidget(mWidgetSerialPort, 0,0);

    //Control Panel
    mWidgetControlPanel = new CWidgetControlPanel(this);
    mainLayout->addWidget(mWidgetControlPanel, 0, 1);

    //Log Box Widget
    mWidgetLogBox = new CWidgetLogBox(this);
    mainLayout->addWidget(mWidgetLogBox, 2,0,1,2);
    mWidgetLogBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //SCOM Editor
    mWidgetSCOMEditor = new CWidgetSCOMEditor(this);
    mainLayout->addWidget(mWidgetSCOMEditor, 1,0,1,2);

    //SCOM List
    mWidgetScomList = new CWidgetScomList(this);
    mainLayout->addWidget(mWidgetScomList, 1, 2, 2, 1);


    //SCOM queue
    mScomQueue = new CScomQueue();


    //Connect Signals with Slots
    ConnectSignalSlots();
}

MainWindow::~MainWindow()
{

}

void MainWindow::ConnectSignalSlots()
{
    //Connect mWidgetControlPanel with mWidgetSerialPort
    connect(mWidgetControlPanel, SIGNAL(SignalButtonConnect(bool)), mWidgetSerialPort,SLOT(PublicSlotOpenClose(bool)));

    //Connect mWidgetControlPanel with mScomQueue
    connect(mWidgetControlPanel, SIGNAL(SignalButtonStart(bool)), mScomQueue, SLOT(PublicSlotStartSCOMQueue(bool)));

    //Connect mWidgetControlPanel with mWidgetScomEditor
    connect(mWidgetControlPanel, SIGNAL(SignalButtonSingleShot()), mWidgetSCOMEditor, SLOT(PublicSlotRequestScomEditorDataToScomQueue()));
    //Connect mWidgetControlPanel with mWidgetScomList
    connect(mWidgetControlPanel, SIGNAL(SignalButtonRun(bool)), mWidgetScomList, SLOT(PublicSlotScomQueueRunCancel(bool)));


    //Connect mWidgetSerialPort with mScomQueue
    connect(mWidgetSerialPort, SIGNAL(SignalCOMPortInfo(QString,QString)), mScomQueue, SLOT(PublicSlotConfigureSCOMWorker(QString,QString)));

    //Connect mWidgetScomEditorBox with mScomQueue
    connect(mWidgetSCOMEditor, SIGNAL(SignalResponseScomEditorDataToScomQueue(SCOMQueueItem)), mScomQueue, SLOT(PublicSlotInsertSCOMQueueAndShot(SCOMQueueItem)));

    //Connect with log box
    connect(mWidgetControlPanel, SIGNAL(SignalLogOutput(QString)), mWidgetLogBox, SLOT(PublicSlotNewLog(QString)));
    connect(mScomQueue, SIGNAL(SignalLogOutput(QString)), mWidgetLogBox, SLOT(PublicSlotNewLog(QString)));
    connect(mWidgetScomList, SIGNAL(SignalLogOutput(QString)), mWidgetLogBox, SLOT(PublicSlotNewLog(QString)));

    //Connect mWidgetScomList to mWidgetScomEditor
    //connect(mWidgetScomList, SIGNAL(SignalMoveScomQueueItemFromListToEditor(ScomEditData)), mWidgetSCOMEditor, SLOT());
    connect(mWidgetScomList, SIGNAL(SignalAskForScomQueueItemFromEditorToList()), mWidgetSCOMEditor, SLOT(PublicSlotRequestScomEditorDataToList()));
    connect(mWidgetSCOMEditor, SIGNAL(SignalResponseScomEditorDataToList(ScomEditData)), mWidgetScomList, SLOT(PublicSlotInsertScomQueueItemFromEditor(ScomEditData)));
    connect(mWidgetScomList, SIGNAL(SignalMoveScomQueueItemFromListToEditor(ScomEditData)), mWidgetSCOMEditor, SLOT(PublicSlotMoveScomEditorDataFromList(ScomEditData)));

    //Connect mWidgetScomList to mWidgetControlPanel
    connect(mWidgetScomList, SIGNAL(SignalScomQueueItemToEditorReady()), mWidgetControlPanel, SLOT(PublicSlotSingleShot()));
    connect(mWidgetScomList, SIGNAL(SignalScomQueueItemEmpty(bool)), mWidgetControlPanel, SLOT(PublicSlotRun(bool)));

    //Connect mScomQueue with mWidgetScomList
    connect(mScomQueue, SIGNAL(SignalScomQueueFinish(hd_common::e_Error)), mWidgetScomList,SLOT(PublicSlotScomQueueItemFinish(hd_common::e_Error)));
}
