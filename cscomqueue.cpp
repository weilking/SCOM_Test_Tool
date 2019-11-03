#include "cscomqueue.h"

using namespace HD_SCOM;
using namespace hd_common;

CScomQueue::CScomQueue()
{
    //Connect Signal and Object
}

CScomQueue::~CScomQueue()
{

}

e_Error CScomQueue::Start()
{
    e_Error eRet = e_OK;

    //Start SCOM Worker
    eRet = mScomWorker.Start();
    if(e_OK != eRet)
    {
        OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
        emit SignalScomQueueStartStop(false);
    }
    else
    {
        OutputDebugLog(__FUNCTION__, "PASS" , eRet);
        emit SignalScomQueueStartStop(true);

    }
    //clear message list
    mListSCOMQueue.clear();

    bTerminate = false;

    connect(&mDelayTimer,SIGNAL(timeout()),this,SLOT(DelayTimeoutRoutine()));

    return eRet;
}

e_Error CScomQueue:: Stop()
{
    //Stop SCOM Worker

    e_Error eRet = e_OK;

    eRet = mScomWorker.Stop();

    OutputDebugLog(__FUNCTION__, "PASS" , eRet);

    bTerminate = true;

    return eRet;
}

e_Error CScomQueue::ExecuteSend()
{
    e_Error eRet = e_OK;
    SCOMQueueItem mSCOMQueueItemExecute;


    //Execute single SCOM message item

    //Pop up the first item

    if(mListSCOMQueue.isEmpty())
    {
        eRet = e_SCOM_ITEM_QUEUE_EMPTY;
        return eRet;
    }

    mSCOMQueueItemExecute = mListSCOMQueue.front();


    //Send SCOM command
    eRet = mScomWorker.Send_SCOM_Message(mSCOMQueueItemExecute.mScomMsgSend);
    if(e_OK != eRet)
    {
        //Fail to Send
        OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
        return eRet;
    }


    //Connect SCOMWorker Message Available signal to Execute Read
    QObject::connect(&mScomWorker,SIGNAL(Message_Available(int)),this, SLOT(PublicSlotExecuteRead(int)));

    return eRet;
}

e_Error CScomQueue::CompareAnswerWithTarget(SCOM_MSG &mSCOMAnswer, SCOM_MSG &mSCOMTarget)
{
    e_Error eRet = e_OK;

    if(mSCOMAnswer.att != mSCOMTarget.att)
    {
        eRet = e_SCOM_ANSWER_MISMATCH_ATT;
        OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
    }
    else if(mSCOMTarget.len > 0)
    {
        if(mSCOMAnswer.len != mSCOMTarget.len)
        {
            eRet = e_SCOM_ANSWER_MISMATCH_LEN;
            OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
        }
        else
        {
            if(0 != memcmp(mSCOMAnswer.data, mSCOMTarget.data, mSCOMAnswer.len))
            {
                eRet = e_SCOM_ANSWER_MISMATCH_DATA;
                OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
            }
        }
    }
    else
    {
        //No need to compare response with expected data
        //Do Nothing
    }


    return eRet;
}

void CScomQueue::OutputDebugLog(QString sFunction, QString sType, e_Error code)
{
    QString sStringOutput;

    sStringOutput = QString("[%3]%1[%2][0x%4]").arg(CScomQueue_DEBUG_PREFIX, sFunction, sType, QString::number(code,16));

    qDebug() << sStringOutput;

    //Emit signal to log box receiving slot
    emit SignalLogOutput(sStringOutput);

    return;
}

void CScomQueue::OutputDebugLog(QString sFunction, QString sType, QString sText)
{
    QString sStringOutput;

    sStringOutput = QString("[%3]%1[%2][%4]").arg(CScomQueue_DEBUG_PREFIX, sFunction, sType, sText);

    qDebug() << sStringOutput;

    //Emit signal to log box receiving slot
    emit SignalLogOutput(sStringOutput);

    return;
}

void CScomQueue::DelayTimeoutRoutine()
{
    mDelayTimer.stop();
    //delayed emit scom queue finish, only for e_OK
    emit SignalScomQueueFinish(e_OK);
}

void CScomQueue::PublicSlotExecuteRead(int iNumber)
{
    e_Error eRet = e_OK;
    SCOMQueueItem mSCOMQueueItemExecute;
    SCOM_MSG mSCOMMsg;


    mSCOMQueueItemExecute = mListSCOMQueue.front();
    if(iNumber == 0)
    {
        //No message is received, Receive Timeout
        eRet = e_SCOM_READ_MESSAGE_NO_MESSAGE;
        OutputDebugLog(__FUNCTION__, "ERROR" , eRet);

    }
    else
    {
        //Read returned SCOM command
        eRet = mScomWorker.Read_SCOM_Message(&mSCOMMsg);
        if(e_OK != eRet)
        {
            //Fail to read
            OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
        }
        else
        {
            //Compare with expected value
            eRet = CompareAnswerWithTarget(mSCOMMsg, mSCOMQueueItemExecute.mScomMsgExpectedAnswer);
            if(eRet != e_OK)
            {
                OutputDebugLog(__FUNCTION__, "ERROR" , eRet);
            }
        }
    }

    //Disconnect signal to slot
    QObject::disconnect(&mScomWorker,SIGNAL(Message_Available(int)),this, 0);

     //Remove the front item in queue
     mListSCOMQueue.removeFirst();

     //If repeat time is not zero && not wait until success, reinsert the message again.
     if(eRet != e_OK)
     {
         if(mSCOMQueueItemExecute.uintMaxRepeatTime > 1)
         {
             -- mSCOMQueueItemExecute.uintMaxRepeatTime;
             if(!bTerminate)
             {
                PublicSlotInsertSCOMQueueAndShot(mSCOMQueueItemExecute);
                return;
             }
         }
     }
     else if(eRet == e_OK && !mSCOMQueueItemExecute.bWaitUntil)
     {
         if(mSCOMQueueItemExecute.uintMaxRepeatTime > 1)
         {
             -- mSCOMQueueItemExecute.uintMaxRepeatTime;
             if(!bTerminate)
             {
                PublicSlotInsertSCOMQueueAndShot(mSCOMQueueItemExecute);
                return;
             }
         }
     }
     else
     {
         if(mSCOMQueueItemExecute.uintDelayAfter > 0)
         {
             //start delay timer
            mDelayTimer.setInterval(mSCOMQueueItemExecute.uintDelayAfter);
            mDelayTimer.start();

            QString sTmp;
            sTmp = QString("Delay %1 ms").arg(mSCOMQueueItemExecute.uintDelayAfter);

            OutputDebugLog(__FUNCTION__, "STATUS" , sTmp);
            return;
         }

     }

     //emit signal success/fail
     emit SignalScomQueueFinish(eRet);

     return;
}


//Public slot to terminate queue and shot
void CScomQueue::PublicSlotTerminateQueueAndShot(bool bRunCancel)
{
    bTerminate = !bRunCancel;

    //If delay is running
    if(mDelayTimer.isActive())
    {
        //call timeout routine directly
        mDelayTimer.stop();
    }
}

//Public Slot to receive serial port change signal
void CScomQueue::PublicSlotConfigureSCOMWorker(QString qStringCOMPort, QString qStringBaudrate)
{
    if(mSerialPortConnected.isOpen())
    {
        mSerialPortConnected.close();
    }

    //Reconfigure the serial port
    mSerialPortConnected.setPortName(qStringCOMPort);
    mSerialPortConnected.setBaudRate(qStringBaudrate.toInt(NULL, 10));

    mScomWorker.Set_Serial_Port(&mSerialPortConnected);
}

void CScomQueue::PublicSlotInsertSCOMQueue(SCOMQueueItem message)
{
    //Add new item at end of list
    mListSCOMQueue.append(message);
}

void CScomQueue::PublicSlotInsertSCOMQueueAndShot(SCOMQueueItem message)
{
    mListSCOMQueue.insert(0, message);

    e_Error eRet = e_OK;
    eRet = ExecuteSend();
    if(eRet != e_OK)
    {
        OutputDebugLog(__FUNCTION__,"ERROR", eRet);
    }
}

void CScomQueue::PublicSlotDeleteSCOMQueueTail()
{
    mListSCOMQueue.removeLast();
}

void CScomQueue::PublicSlotDeleteSCOMQueueFront()
{
    mListSCOMQueue.removeFirst();
}

void CScomQueue::PublicSlotStartSCOMQueue(bool bStartStop)
{
    if(bStartStop == true)
    {
        //Call Start
        Start();
    }
    else
    {
        //Call Stop
        Stop();
    }
}

