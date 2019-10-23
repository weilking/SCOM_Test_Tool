#ifndef COMMON_INC_H
#define COMMON_INC_H

//============================================================================================================
// Defines
//============================================================================================================

//============================================================================================================
// Typedefs
//============================================================================================================

namespace hd_common
{
    enum e_Error
    {
        e_OK = 0x0000,
        e_FAIL = 0x0001,

        //SCOM
        e_SCOM_CHECKSUM_HEADER_INV = 0x0100,
        e_SCOM_CHECKSUM_DATA_INV = 0x0101,
        e_SCOM_DATA_LENGTH_INV = 0x0102,
        e_SCOM_HEADER_INV = 0x0103,
        e_SCOM_ERROR_CHANGE_BAUDRATE = 0x0104,
        e_SCOM_ERROR_OPEN_PORT = 0x0105,
        e_SCOM_COMMAND_ID_INV = 0x0106,
        e_SCOM_SEND_MESSAGE_TOO_FREQUENT = 0x0107,
        e_SCOM_ERROR_WRITE = 0x0108,
        e_SCOM_ERROR_BUILD_SCOM_MESSAGE = 0x0109,
        e_SCOM_READ_MESSAGE_NO_MESSAGE = 0x010A,
        e_SCOM_READ_MESSAGE_READ_ONGOING = 0x010B,
        e_SCOM_ERROR_CHANGE_PORT_NAME = 0x010C,


        //SCOM Queue
        e_SCOM_ITEM_QUEUE_EMPTY = 0x0200,
        e_SCOM_ANSWER_MISMATCH_ATT = 0x201,
        e_SCOM_ANSWER_MISMATCH_LEN = 0x202,
        e_SCOM_ANSWER_MISMATCH_DATA = 0x203
    };



}

#endif // COMMON_INC_H

