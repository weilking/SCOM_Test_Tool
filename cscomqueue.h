#ifndef CSCOMQUEUE_H
#define CSCOMQUEUE_H

#include <QObject>
#include "cscomworker.h"
#include "common_inc.h"

//============================================================================================================
// Defines
//============================================================================================================


//Debug prefix
#define CScomQueue_DEBUG_PREFIX ("CScomQueue: ")

//============================================================================================================
// Typedefs
//============================================================================================================
namespace HD_SCOM {
    struct SCOMQueueItem
    {
        SCOM_MSG mScomMsgSend;
        SCOM_MSG mScomMsgExpectedAnswer;
        unsigned int uintDelayAfter;
        unsigned int uintMaxRepeatTime;
        bool bCritical;
        bool bResult;
    };
}

//============================================================================================================
// Classes
//============================================================================================================

namespace HD_SCOM {

    class CScomQueue : public QObject
    {
        Q_OBJECT
    public:
        CScomQueue();
        ~CScomQueue();

        //Start Queue
        hd_common::e_Error Start(void);

        //Stop Queue
        hd_common::e_Error Stop(void);

        //Execute SCOM Queue Item from Front Send
        hd_common::e_Error ExecuteSend(void);

        // Compare answer with expected target
        hd_common::e_Error CompareAnswerWithTarget(SCOM_MSG& mSCOMAnswer, SCOM_MSG& mSCOMTarget);

        // Output debug log
        void OutputDebugLog(QString sFunction, QString sType, hd_common::e_Error Code);

    private:
        //SCOM execution queue
        QList<SCOMQueueItem> mListSCOMQueue;

        //Combined SCOM Port
        QSerialPort mSerialPortConnected;

        //SCOM Worker
        CScomWorker mScomWorker;

    signals:

        //signal to notify the case is started
        void SignalScomQueueStartStop(bool bOpen);

        //signal to notify the case is finished
        void SignalScomQueueFinish(hd_common::e_Error eRet);

        //signal to output to log box
        void SignalLogOutput(QString sLog);


    public slots:
        //Configure SCOM Worker Serial port
        void PublicSlotConfigureSCOMWorker(QString qStringCOMPort, QString qStringBaudrate);

        //Insert SCOM command to tail
        void PublicSlotInsertSCOMQueue(SCOMQueueItem message);

        //Insert SCOM Queue Item to Front and execute immediately
        void PublicSlotInsertSCOMQueueAndShot(SCOMQueueItem message);

        //Delete SCOM command at tail
        void PublicSlotDeleteSCOMQueueTail(void);

        //Delete SCOM command at front
        void PublicSlotDeleteSCOMQueueFront(void);

        //Start/Stop SCOM Queue with SCOM worker
        void PublicSlotStartSCOMQueue(bool bStartStop);

        //Execute SCOM Queue Item from Front Read
        void PublicSlotExecuteRead(int number);




    };
}
#endif // CSCOMQUEUE_H
