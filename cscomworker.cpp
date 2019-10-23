#include "cscomworker.h"

using namespace HD_SCOM;
using namespace hd_common;

QT_USE_NAMESPACE

/**
 * @brief HD_SCOM::CScomWorker::CScomWorker
 */
HD_SCOM::CScomWorker::CScomWorker()
{


    //Configure COM
    m_SCOM_PORT = NULL;

    //Configure Timer
    m_Message_Timer.setInterval(SCOM_MSG_TIMEOUT);

    m_SCOM_Message_Ongoing_Buffer.att = SCOM_ATT_INVALID;
}


/**
 * @brief HD_SCOM::CScomWorker::~CScomWorker
 */
HD_SCOM::CScomWorker::~CScomWorker()
{
    //Close linked port
    if(m_SCOM_PORT !=NULL && m_SCOM_PORT->isOpen())
    {
        m_SCOM_PORT->close();
    }
}




e_Error CScomWorker::Set_Serial_Port(QSerialPort *qSerialPort)
{
    e_Error e_Ret = e_OK;

    if(qSerialPort != NULL)
    {
        m_SCOM_PORT = qSerialPort;
    }

    else
    {
        e_Ret = e_FAIL;
    }

    return e_Ret;

}

/**
 * @brief CScomWorker::Start
 * @return
 */
e_Error CScomWorker::Start()
{
    e_Error e_Ret = e_OK;

    if(!m_SCOM_PORT->open(QIODevice::ReadWrite))
    {
        e_Ret = e_SCOM_ERROR_OPEN_PORT;
        qDebug() << CScomWorker_DEBUG_PREFIX << "e_SCOM_ERROR_OPEN_PORT" << '\n';
    }

    //Connect Signal and Object

    QObject::connect(m_SCOM_PORT,SIGNAL(bytesWritten(qint64)),&m_Message_Timer,SLOT(start()));
    QObject::connect(m_SCOM_PORT,SIGNAL(readyRead()),this,SLOT(Read_Raw_Data()));
    QObject::connect(&m_Message_Timer,SIGNAL(timeout()),this,SLOT(Read_Timeout_Routine()));


    m_SCOM_Message_Ongoing_Stage = STAGE_ATT;

    return e_Ret;

}


/**
 * @brief CScomWorker::Stop
 * @return
 */
e_Error CScomWorker::Stop()
{

    e_Error e_Ret = e_OK;

    //Reset CScomWorker
    Reset();

    if(m_SCOM_PORT->isOpen())
    {
        m_SCOM_PORT->close();
    }

    return e_Ret;
}

/**
 * @brief CScomWorker::Reset
 * @return
 */
e_Error CScomWorker::Reset()
{
    e_Error e_Ret = e_OK;

    //Clear receiving buffer
    m_SCOM_PORT->clear();

    //Stop message timer
    if(m_Message_Timer.isActive())
    {
        m_Message_Timer.stop();
    }

    //Clear Receive Buffer
    m_List_SCOM_Receive_Buffer.clear();

    //
    if(m_SCOM_Message_Ongoing_Buffer.att != SCOM_ATT_INVALID)
    {
        m_SCOM_Message_Ongoing_Buffer.att = SCOM_ATT_INVALID;
    }

    //Reset working stage
    m_SCOM_Message_Ongoing_Stage = STAGE_ATT;

    QObject::disconnect(m_SCOM_PORT,SIGNAL(bytesWritten(qint64)),this,0);
    QObject::disconnect(m_SCOM_PORT,SIGNAL(readyRead()),this,0);


    return e_Ret;
}

/**
 * @brief CScomWorker::Send_Raw_Data
 * @param dataArray
 * @return
 */
e_Error CScomWorker::Send_Raw_Data(QByteArray &dataArray)
{
    e_Error e_Ret = e_OK;


    qint64 i64_writtenBytes = m_SCOM_PORT->write(dataArray);

    if(i64_writtenBytes < dataArray.length())
    {
        e_Ret = e_SCOM_ERROR_WRITE;
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Send_Raw_Data]" << "e_SCOM_ERROR_WRITE" << '\n';
        return e_Ret;
    }

    m_SCOM_PORT->flush();

    if(!m_SCOM_PORT->waitForBytesWritten(50))
    {
        e_Ret = e_SCOM_ERROR_WRITE;
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Send_Raw_Data]" << "e_SCOM_ERROR_WRITE_TIMEOUT" << '\n';
        return e_Ret;
    }
    return e_Ret;
}

/**
 * @brief CScomWorker::Send_Raw_Data
 * @param data
 * @param len
 * @return
 */
e_Error CScomWorker::Send_Raw_Data(unsigned char *data, unsigned char len)
{
    e_Error e_Ret = e_OK;


    qint64 i64_writtenBytes = m_SCOM_PORT->write((char*)data,len);

    if(i64_writtenBytes < len)
    {
        e_Ret = e_SCOM_ERROR_WRITE;
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Send_Raw_Data]" << "e_SCOM_ERROR_WRITE" << '\n';
        return e_Ret;
    }
    m_SCOM_PORT->flush();

    if(!m_SCOM_PORT->waitForBytesWritten(50))
    {
        e_Ret = e_SCOM_ERROR_WRITE;
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Send_Raw_Data]" << "e_SCOM_ERROR_WRITE_TIMEOUT" << '\n';
        return e_Ret;
    }

    return e_Ret;
}

/**
 * @brief CScomWorker::Build_SCOM_Message
 * @param Address
 * @param CmdID
 * @param Message_out
 * @return
 */
e_Error CScomWorker::Build_SCOM_Message(unsigned char Address, QByteArray CmdID, SCOM_MSG *Message_out)
{
    e_Error e_Ret = e_OK;

    if(CmdID.length() != 3)
    {
        e_Ret = e_SCOM_COMMAND_ID_INV;
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Build_SCOM_Message]" << "e_SCOM_COMMAND_ID_INV" << '\n';
        return e_Ret;
    }

    unsigned char u8_ihchk = 0;

    //Calculate header checksum
    u8_ihchk += SCOM_ATT_CMD;
    u8_ihchk += (unsigned char)CmdID.at(0);
    u8_ihchk += (unsigned char)CmdID.at(1);
    u8_ihchk += (unsigned char)CmdID.at(2);
    u8_ihchk += Address;

    u8_ihchk = ~u8_ihchk;

    //fill in data to message_out
    Message_out->address = Address;
    Message_out->att = SCOM_ATT_CMD;
    Message_out->cmdid.id[0] = (unsigned char)CmdID.at(0);
    Message_out->cmdid.id[1] = (unsigned char)CmdID.at(1);
    Message_out->cmdid.id[2] = (unsigned char)CmdID.at(2);
    Message_out->len = 0;
    Message_out->ihchk = u8_ihchk;

    return e_Ret;
}

/**
 * @brief CScomWorker::Build_SCOM_Message
 * @param Address
 * @param CmdID
 * @param Data
 * @param Message_out
 * @return
 */
e_Error CScomWorker::Build_SCOM_Message(unsigned char Address, QByteArray CmdID, QByteArray Data, SCOM_MSG *Message_out)
{
    e_Error e_Ret = e_OK;

    if(CmdID.length() != 3)
    {
        e_Ret = e_SCOM_COMMAND_ID_INV;
        qDebug() << CScomWorker_DEBUG_PREFIX << "e_SCOM_COMMAND_ID_INV" << '\n';
        return e_Ret;
    }

    unsigned char u8_ihchk = 0;
    unsigned char u8_idchk = 0;

    //Calculate header checksum
    u8_ihchk += SCOM_ATT_CMD;
    u8_ihchk += CmdID.at(0);
    u8_ihchk += CmdID.at(1);
    u8_ihchk += CmdID.at(2);
    u8_ihchk += Address;
    u8_ihchk += Data.size();

    u8_ihchk = ~u8_ihchk;

    //Calculate data checksum
    if(Data.size() > 0)
    {
        for(int i=0;i<Data.size();i++)
        {
            u8_idchk += Data.at(i);
        }
        u8_idchk = ~u8_idchk;
    }

    //fill in data to message_out
    Message_out->address = Address;
    Message_out->att = SCOM_ATT_CMD;
    Message_out->cmdid.id[0] = CmdID.at(0);
    Message_out->cmdid.id[1] = CmdID.at(1);
    Message_out->cmdid.id[2] = CmdID.at(2);
    Message_out->len = Data.size();
    Message_out->ihchk = u8_ihchk;

    if(Data.size() > 0)
    {
        memcpy(Message_out->data,Data.data(),Data.size());
        Message_out->idchk = u8_idchk;
    }

    return e_Ret;

}

e_Error CScomWorker::Send_SCOM_Message(SCOM_MSG &message)
{
    e_Error e_Ret = e_OK;

    //Check if there is a message is sent and is wait for reply.
    if(m_Message_Timer.isActive())
    {
        e_Ret = e_SCOM_SEND_MESSAGE_TOO_FREQUENT;
        qDebug() << CScomWorker_DEBUG_PREFIX << "e_SCOM_SEND_MESSAGE_TOO_FREQUENT" << '\n';
        return e_Ret;
    }

    QByteArray qba_write;

    qba_write += message.att;
    qba_write += message.address;
    qba_write += message.cmdid.id[0];
    qba_write += message.cmdid.id[1];
    qba_write += message.cmdid.id[2];
    qba_write += message.len;
    qba_write += message.ihchk;

    //if length > 0
    if(message.len > 0)
    {
        qba_write.append((const char*)message.data,message.len);

        qba_write += message.idchk;
    }


    e_Ret = Send_Raw_Data(qba_write);

    return e_Ret;

}

e_Error CScomWorker::Send_SCOM_Message(unsigned char Address, QByteArray CmdID, QByteArray Data)
{
    e_Error e_Ret = e_OK;

    //Check if there is a message is sent and is wait for reply.
    if(m_Message_Timer.isActive())
    {
        e_Ret = e_SCOM_SEND_MESSAGE_TOO_FREQUENT;
        qDebug() << CScomWorker_DEBUG_PREFIX << "e_SCOM_SEND_MESSAGE_TOO_FREQUENT" << '\n';
        return e_Ret;
    }

    SCOM_MSG SM_Message;

    e_Ret = Build_SCOM_Message(Address,CmdID,Data,&SM_Message);

    if(e_Ret != e_OK)
    {
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Send_SCOM_Message]" << "e_SCOM_ERROR_BUILD_SCOM_MESSAGE" << '\n';
        return e_Ret;
    }

    e_Ret = Send_SCOM_Message(SM_Message);
    if(e_Ret != e_OK)
    {
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Send_SCOM_Message]" << "e_SCOM_ERROR_WRITE" << '\n';
    }

    return e_Ret;
}

e_Error CScomWorker::Read_SCOM_Message(SCOM_MSG *message)
{
    e_Error e_Ret = e_OK;

    //CHeck if receiving is  ongoing
    if(m_Message_Timer.isActive())
    {

    }

    if(m_List_SCOM_Receive_Buffer.isEmpty())
    {
        e_Ret = e_SCOM_READ_MESSAGE_NO_MESSAGE;
        qDebug() << CScomWorker_DEBUG_PREFIX << "[Read_SCOM_Message]" << "e_SCOM_READ_MESSAGE_NO_MESSAGE" << '\n';
        return e_Ret;
    }

    memcpy(message,&m_List_SCOM_Receive_Buffer.front(),sizeof(SCOM_MSG));
    m_List_SCOM_Receive_Buffer.pop_front();

    qDebug() << CScomWorker_DEBUG_PREFIX << "[Read_SCOM_Message]" << "e_OK" << '\n';

    return e_Ret;
}

void HD_SCOM::CScomWorker::Read_Raw_Data(void)
{

    u8a_Received_RAW_Data += m_SCOM_PORT->readAll();

    //Verify if the data is empty
    if(u8a_Received_RAW_Data.isEmpty())
    {
       qDebug() << "No available Data!\n";
       return;
    }

    while(!u8a_Received_RAW_Data.isEmpty())
    {
        switch(m_SCOM_Message_Ongoing_Stage)
        {
        case STAGE_ATT:
            switch (u8a_Received_RAW_Data.at(0))
            {
            case SCOM_ATT_CMD:
            case SCOM_ATT_ACK:
            case SCOM_ATT_NACK:
                //Add 1st bytes to att
                m_SCOM_Message_Ongoing_Buffer.att = u8a_Received_RAW_Data.at(0);
                //remove the first bytes
                u8a_Received_RAW_Data.remove(0,1);
                //Set current stage to ADDR
                m_SCOM_Message_Ongoing_Stage = STAGE_ADDR;
                break;
            default:
                //if the first bytes is invalid value
                //discard all data, reset receiving stage
                m_SCOM_Message_Ongoing_Buffer.att = SCOM_ATT_INVALID;
                u8a_Received_RAW_Data.clear();
                m_SCOM_Message_Ongoing_Stage = STAGE_ATT;

                qDebug() << "Invalid att header value!\n";
                return;
            }

        case STAGE_ADDR:
            //if no data is available, return and wait for new data.
            if(u8a_Received_RAW_Data.isEmpty())
            {
                qDebug() << "Waiting for ADDR value!\n";
                return;
            }

            //Add 1st bytes to Addr
            m_SCOM_Message_Ongoing_Buffer.address = u8a_Received_RAW_Data.at(0);
            //remove the first bytes
            u8a_Received_RAW_Data.remove(0,1);
            //Set current stage to CMDID
            m_SCOM_Message_Ongoing_Stage = STAGE_CMDID;
            break;

        case STAGE_CMDID:
            //if data length is smaller than 3, return and wait for new data.
            if(u8a_Received_RAW_Data.length()< 3 )
            {
                qDebug() << "Waiting for CMDID value!\n";
                return;
            }

            //Add first three bytes to cmdid
            m_SCOM_Message_Ongoing_Buffer.cmdid.id[0] = u8a_Received_RAW_Data.at(0);
            m_SCOM_Message_Ongoing_Buffer.cmdid.id[1] = u8a_Received_RAW_Data.at(1);
            m_SCOM_Message_Ongoing_Buffer.cmdid.id[2] = u8a_Received_RAW_Data.at(2);

            //remove the first bytes
            u8a_Received_RAW_Data.remove(0,3);
            //Set current stage to LEN
            m_SCOM_Message_Ongoing_Stage = STAGE_LEN;
            break;

        case STAGE_LEN:
            //if no data is available, return and wait for new data.
            if(u8a_Received_RAW_Data.isEmpty())
            {
                qDebug() << "Waiting for LEN value!\n";
                return;
            }

            //Add 1st bytes to Len
            m_SCOM_Message_Ongoing_Buffer.len = u8a_Received_RAW_Data.at(0);
            //remove the first bytes
            u8a_Received_RAW_Data.remove(0,1);
            //Set current stage to IHCHK
            m_SCOM_Message_Ongoing_Stage = STAGE_IHCHK;
            break;

        case STAGE_IHCHK:
            //if no data is available, return and wait for new data.
            if(u8a_Received_RAW_Data.isEmpty())
            {
                qDebug() << "Waiting for IHCHK value!\n";
                return;
            }

            //Add 1st bytes to Len
            m_SCOM_Message_Ongoing_Buffer.ihchk = u8a_Received_RAW_Data.at(0);
            //remove the first bytes
            u8a_Received_RAW_Data.remove(0,1);
            //Set current stage to DATA
            if(m_SCOM_Message_Ongoing_Buffer.len > 0)
            {
                m_SCOM_Message_Ongoing_Stage = STAGE_DATA;
            }
            else
            {
                //Set current stage to FINISH
                m_SCOM_Message_Ongoing_Stage = STAGE_FINISH;
            }
            break;

        case STAGE_DATA:
            //if no data is available, return and wait for new data.
            if(u8a_Received_RAW_Data.length() < m_SCOM_Message_Ongoing_Buffer.len )
            {
                qDebug() << "Waiting for DATA value!\n";
                return;
            }

            //Copy data to data buffer
            memcpy(m_SCOM_Message_Ongoing_Buffer.data,u8a_Received_RAW_Data.data(),m_SCOM_Message_Ongoing_Buffer.len);

            //remove the first bytes
            u8a_Received_RAW_Data.remove(0,m_SCOM_Message_Ongoing_Buffer.len);
            //Set current stage to DATA
            m_SCOM_Message_Ongoing_Stage = STAGE_IDCHK;
            break;

        case STAGE_IDCHK:
            //if no data is available, return and wait for new data.
            if(u8a_Received_RAW_Data.isEmpty())
            {
                qDebug() << "Waiting for IDCHK value!\n";
                return;
            }

            //Add 1st bytes to ihchk
            m_SCOM_Message_Ongoing_Buffer.idchk = u8a_Received_RAW_Data.at(0);
            //remove the first bytes
            u8a_Received_RAW_Data.remove(0,1);
            //Set current stage to FINISH
            m_SCOM_Message_Ongoing_Stage = STAGE_FINISH;
            break;



        default:

            break;
        }

        //When message receiving is completed
        if(m_SCOM_Message_Ongoing_Stage == STAGE_FINISH)
        {
            //push the SCOM message to list
            m_List_SCOM_Receive_Buffer.push_back(m_SCOM_Message_Ongoing_Buffer);

            m_Message_Timer.stop();

            //emit signal
            emit Message_Available(m_List_SCOM_Receive_Buffer.size());

            qDebug() << "Finish one complete SCOM message!\n";
            m_SCOM_Message_Ongoing_Buffer.att = SCOM_ATT_INVALID;

            m_SCOM_Message_Ongoing_Stage = STAGE_ATT;
         }
    }

}

void CScomWorker::Read_Timeout_Routine()
{

    //Stop timeout timer count
    m_Message_Timer.stop();

    //Clear up the read buffer in serialport
    m_SCOM_PORT->clear(QSerialPort::Input);

    //Set communication direction to all direction
    //m_SCOM_PORT->setOpenMode(QIODevice::ReadWrite);

    if(m_SCOM_Message_Ongoing_Buffer.att != SCOM_ATT_INVALID)
    {
        m_SCOM_Message_Ongoing_Buffer.att = SCOM_ATT_INVALID;
    }

    //emit signal
    emit Message_Available(0);


    qDebug() << CScomWorker_DEBUG_PREFIX << "Message Receiving Timeout!" << '\n';

    return;
}
