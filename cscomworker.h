#ifndef CSCOMWORKER_H
#define CSCOMWORKER_H

#include <QSerialPort>
#include <QTimer>
#include <QList>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QTime>

#include "common_inc.h"

//============================================================================================================
// Defines
//============================================================================================================

//SCOM
#define SCOM_MSG_DATA_MAX_LENGTH (74)
#define SCOM_MSG_FRAME_LENGTH (SCOM_MSG_DATA_MAX_LENGTH + 8)
#define SCOM_MSG_TIMEOUT (1000)

//Serial port
#define SCOM_PORT_ID_DEFAULT (3)

//Debug prefix
#define CScomWorker_DEBUG_PREFIX ("CScomWorker: ")


//============================================================================================================
// Typedefs
//============================================================================================================

namespace HD_SCOM {

    ///SCOM Message Command ID
    struct SCOM_CMD_ID
    {
        unsigned char id[3];
    };

    ///SCOM Message struct
    struct SCOM_MSG
    {
        unsigned char att;
        unsigned char address;
        unsigned char len;
        SCOM_CMD_ID cmdid;
        unsigned char ihchk;
        unsigned char data[SCOM_MSG_DATA_MAX_LENGTH];
        unsigned char idchk;

    };

    enum SCOM_STAGE
    {
        STAGE_ATT = 0,
        STAGE_ADDR,
        STAGE_CMDID,
        STAGE_LEN,
        STAGE_IHCHK,
        STAGE_DATA,
        STAGE_IDCHK,
        STAGE_FINISH
    };

    enum SCOM_ATT
    {
        SCOM_ATT_CMD = 0x07,
        SCOM_ATT_ACK = 0x06,
        SCOM_ATT_NACK = 0x15,
        SCOM_ATT_INVALID = 0xFF
    };

}

//============================================================================================================
// Classes
//============================================================================================================

namespace HD_SCOM {

    class CScomWorker : public QObject
    {
        Q_OBJECT
        ///Property: COM Port ID
        //Q_PROPERTY(int iPort READ iPort WRITE setPort NOTIFY portChanged)
        ///Property: BaudRate
        //Q_PROPERTY(QSerialPort::BaudRate enumBaudrate READ enumBaudrate WRITE setBaudrate NOTIFY baudrateChanged)
        ///Property: Communication Timeout
       // Q_PROPERTY(int iCommTimeout READ iCommTimeout WRITE setCommTimeout)

    public:
        CScomWorker();
        ~CScomWorker();

        ///Function set serial port
        hd_common::e_Error Set_Serial_Port(QSerialPort *qSerialPort);

        ///Function start SCOM Worker
        hd_common::e_Error Start(void);

        ///Function stop SCOM Worker
        hd_common::e_Error Stop(void);

        ///Function to reset SCOM Worker
        hd_common::e_Error Reset(void);

        ///Function to send raw data
        hd_common::e_Error Send_Raw_Data(QByteArray &dataArray);

        ///Function to send raw data
        hd_common::e_Error Send_Raw_Data(unsigned char* data, unsigned char len);

        ///Function to build SCOM message
        hd_common::e_Error Build_SCOM_Message(unsigned char Address, QByteArray CmdID, SCOM_MSG *Message_out);

        ///Function to build SCOM message
        hd_common::e_Error Build_SCOM_Message(unsigned char Address, QByteArray CmdID, QByteArray Data, SCOM_MSG *Message_out);

        ///Function to send SCOM message
        hd_common::e_Error Send_SCOM_Message(SCOM_MSG &message);

        ///Function to build and send SCOM message
        hd_common::e_Error Send_SCOM_Message(unsigned char Address, QByteArray CmdID, QByteArray Data);

        ///Function to read SCOM message
        hd_common::e_Error Read_SCOM_Message(SCOM_MSG *message);


    private:
        ///SerialPort Object for SCOM communicatoin
        QSerialPort* m_SCOM_PORT;

        ///Timer used for message sent-out
        QTimer m_Message_Timer;

        /// SCOM Receive Buffer
        QList<SCOM_MSG> m_List_SCOM_Receive_Buffer;

        ///Temp SCOM buffer during receiving
        SCOM_MSG m_SCOM_Message_Ongoing_Buffer;

        SCOM_STAGE m_SCOM_Message_Ongoing_Stage;

        ///Raw data buffer
        QByteArray u8a_Received_RAW_Data;

    private slots:
        void Read_Raw_Data(void);

        void Read_Timeout_Routine(void);

    signals:

        void Message_Available(int num);

    };

}



#endif // CSCOMWORKER_H
