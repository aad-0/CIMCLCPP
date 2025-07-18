/**
 **********************************************
 * @file: modbus_slave.h
 **********************************************
 */
#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "modbus_general.h"
#define MODBUS_MSG_RTU_ERR_BIGSIZE	(-1U)
/* TypeDefs */
/**
 * @brief Slave Msg Struct
 * this struct can express any slave messages/respones
 * @note note that this struct should be used with help of macros defined in the header
 * @note There are generic macros that does not state for any relation for master and slave. That macros should be safe to use with
 * this struct, but for sake of readibility and writiblity, there are alternative macros that does state relation for master and slave.
 * It  highly enncouraged to use of them
 */
typedef struct MODBUS_SLAVE_MSG_RTU_s
{
    struct
    {
			MODBUS_MSG_RTU_SECTION_TypeDef Section; /**< Section, can be either determined, undetermined or exception which is specific
to slave messages. Unknown is also a enum in MODBUS_MSG_RTU_SECITON_TypeDef, but is reserved for errors */
			uint32_t Length; /**< Length of message. all functions are tries to calculate the length of the message instead of relying to meta,
bur ir is encouraged to keep length updated and correct */
		} Meta; /**< This Holds Meta Information about struct.*/
    // Actual Message Part
    union
    {
      uint8_t RawData[MODBUS_MSG_RTU_MAXSIZE_B]; /**< Data Part Can be acessed as byte or with structs */

      struct
      {
				uint8_t SlaveAddress; /**< Slave Address, Every message has this information as first byte of message*/
        uint8_t Function; /**< Function, Every message has this information as second byte of message */
        union
        {
					uint8_t StartingAddressHi;
          uint8_t CoilAddressHi;
          uint8_t RegisterAddressHi;
					uint8_t DataByte1; /**< For customizability @note it encourged to use this when writing generic functions*/
				} DataByte1; /**< This is first byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
          uint8_t StartingAddressLo;
          uint8_t CoilAddressLo;
          uint8_t RegisterAddressLo;
					uint8_t DataByte2; /**< For customizability @note it encourged to use this when writing generic functions*/
				} DataByte2; /**< This is second byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
          uint8_t QuantityCoilsHi;
          uint8_t QuantityInputsHi;
          uint8_t QuantityRegistersHi;
          uint8_t WriteDataHi;
					uint8_t DataByte3; /**< For customizability @note it encourged to use this when writing generic functions*/
				} DataByte3; /**< This is third syte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
          uint8_t QuantityCoilsLo;
          uint8_t QuantityInputsLo;
          uint8_t QuantityRegistersLo;
          uint8_t WriteDataLo;
					uint8_t DataByte4; /**< For customizability @note it encourged to use this when writing generic functions*/
				} DataByte4; /**< This is fourth byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
        // end section
				uint8_t ErrorLo; /**< Modbus uses CRC16 and sends Low byte first */
				uint8_t ErrorHi; /**< High part uf CRC16 */
			} Determined; /**
			 * @brief Determined This struct holds definition of determined sized, slave messages/respones. Therefore errorHi and errorLo
			 * bytes can be accessed through this struct. Macros are still useable and adviced.
			 */

      struct
      {
        uint8_t SlaveAddress; /**< Slave Address, Every message has this information as first byte of message*/
        uint8_t Function; /**< Function, Every message has this information as second byte of message */
				uint8_t ByteCount; /**< User should access Data and Error Sections with macros */
			} Undetermined; /**
			 * @brief Undetermined This struct does not hold any information, error bytes are not defined here
			 * the bytearray is expected after bytecount with length less than MODBUS_MSG_RTU_MAXSIZE_B Aka 256
			 * @note 256 is the size of struct, so the space left for bytearray is less than 256.
			 * @note User can access to that bytearray with a macro defined as xMODBUS_MSG_RTU_undeterminedGetBytesPointer
			 * this macro also has master and slave definitons.
			 * @note User can access to error byttes with a defined as xMODBUS_MSG_RTU_getErrorHi and xMODBUS_MSG_RTU_getErrorLo
			 * this macro has master and slave definitions
			 * to use these macros, Length in the meta section should be set correct
			 */

      struct
      {
				uint8_t SlaveAddress; /**< Slave Address, Every message has this information as first byte of message*/
				uint8_t Function; /**< Function, Every message has this information as second byte of message */
				uint8_t ExceptionCode; /** This part holds exception code which are defined in modbus_general.h with MODBUS_EXPECTİON_X*/
        // end section
				uint8_t ErrorLo; /**< Modbus uses CRC16 and sends Low byte first */
				uint8_t ErrorHi; /**< High part uf CRC16 */
      } Exception;

    } Data;

} MODBUS_SLAVE_MSG_RTU_TypeDef;

/**< @note Error Bytes are included */
#define MODBUS_SLAVE_MSG_RTU_SecDetermined_LENGTH       (8U) /**< Fixed Length when determined section is in use */
#define MODBUS_SLAVE_MSG_RTU_SecUndetermined_MINLENGTH  (5U) /**< This is minimum length which assumed that byteCount set as zero(0) */
#define MODBUS_SLAVE_MSG_RTU_SecException_LENGTH        (5U) /**< Fixed Length */
#define MODBUS_SLAVE_MSG_RTU_MINLENGTH MODBUS_SLAVE_MSG_RTU_SecUndetermined_MINLENGTH /**< smallest of the lengths,
this helps to read from a buffer just enough to continue */

/* Function Like Macros */
/**
 * @brief This Function-like macro checks section whether valid or not
 * @note Unkown is not included, since this macro just checks for messages that can exist
 * @retval Boolean
 */
#define xMODBUS_SLAVE_MSG_RTU_SECTION_isValid(__SEC) \
  ( ( Undetermined  == (__SEC) )  ||                  \
    ( Determined    == (__SEC) )  ||                  \
    ( Exception     == (__SEC) )                      \
  )

/**
 * @brief: Retuns if command is section of 0x05 or 0x06
 * @param Command: Command
 * @retval: True if it is related Command, False otherwise
 */
#define xMODBUS_SLAVE_MSG_RTU_IsCommandSecDetermined(__COMMAND)	    \
    (                                                               \
    ( MODBUS_COMMAND_WRITE_SINGLE_COIL        == (__COMMAND) )  ||  \
    ( MODBUS_COMMAND_WRITE_SINGLE_REGISTER    == (__COMMAND) )  ||  \
    ( MODBUS_COMMAND_WRITE_MULTIPLE_COILS     == (__COMMAND) )  ||  \
    ( MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS == (__COMMAND) )      \
    )

/**
 * @brief: Retuns if command is section of 0x01, 0x02, 0x03, 0x04, 0x0f 0x10
 * @param Command: Command
 * @retval: True if it is related Command, False otherwise
 */
#define xMODBUS_SLAVE_MSG_RTU_IsCommandSecUndetermined(__COMMAND)	  \
    (	                                                              \
    ( MODBUS_COMMAND_READ_COILS               == (__COMMAND) )  ||	\
    ( MODBUS_COMMAND_READ_DISCRETE_INPUT      == (__COMMAND) )  || 	\
    ( MODBUS_COMMAND_READ_HOLDING_REGISTERS   == (__COMMAND) )  || 	\
    ( MODBUS_COMMAND_READ_INPUT_REGISTERS     == (__COMMAND) )     	\
    )

/**
 * @brief Returns which section is valid respect to function
 * @note If it is an excetion of an command, excpetion should be masked
 * @param Command: Command
 * @param[out]:
 *            1 : Undetermined
 * 	          2 : Determined
 * 	          4 : Exception
 * 	          8 : Unknown
 * @retval: None
 */
#define xMODBUS_SLAVE_MSG_RTU_CommandSection(__COMMAND, __RET__)	      \
do	{	                                                                  \
    *((__RET__)) = Unknown;	                                            \
    if (xMODBUS_SLAVE_MSG_RTU_IsCommandSecDetermined(__COMMAND))        \
        *((__RET__)) = Determined;                                      \
    else if (xMODBUS_SLAVE_MSG_RTU_IsCommandSecUndetermined(__COMMAND))	\
				*((__RET__)) = Undetermined;	                                  \
		else if (xMODBUS_MSG_RTU_IsCommandException( (__COMMAND) ))         \
        *((__RET__)) = Exception;                                       \
    } while (0);

/**
 * @brief This checkf is command is an exception of command
 * @param __COMMAND Command
 */
#define xMODBUS_SLAVE_MSG_RTU_IsCommandException xMODBUS_MSG_RTU_IsCommandException

/**
 * @brief This function-like macro returns slave address of struct
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct pointer
 * @retval slave address, uint8_t, char
 */
#define xMODBUS_SLAVE_MSG_RTU_getSlaveAddress xMODBUS_MSG_RTU_getSlaveAddress

/**
 * @brief This function-like macro returns function of struct
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct pointer
 * @retval slave address, uint8_t, char
 */
#define xMODBUS_SLAVE_MSG_RTU_getFunction xMODBUS_MSG_RTU_getFunction

/**
 * @brief This function-like macro returns errorHi Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval Error High Byte Aka ErrorHi uint8_t, char
 * @note See explaination about struct
 */
#define xMODBUS_SLAVE_MSG_RTU_getErrorHi xMODBUS_MSG_RTU_getErrorHi

/**
 * @brief This function-like macro returns errorLo Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval Error High Byte Aka ErrorLo, uint8_t, char
 * @note See explaination about struct
 */
#define xMODBUS_SLAVE_MSG_RTU_getErrorLo xMODBUS_MSG_RTU_getErrorLo

/**
 * @brief This function-like macro sets errorHi Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval None
 * @note See explaination about struct
 */
#define xMODBUS_SLAVE_MSG_RTU_setErrorHi xMODBUS_MSG_RTU_setErrorHi

/**
 * @brief This function-like macro sets errorLo Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval None
 * @note See explaination about struct
 */
#define xMODBUS_SLAVE_MSG_RTU_setErrorLo xMODBUS_MSG_RTU_setErrorLo

/**
 * @brief This function-like macro returns head of bytes
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @retval uint8_t/char pointer
 */
#define xMODBUS_SLAVE_MSG_RTU_undeterminedGetBytesPointer xMODBUS_MSG_RTU_undeterminedGetBytesPointer

/**
 * @brief This function-like macro sets High part of data section, which used in undetermined section
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @param __BYTEIDX index of byte
 * @param __VAL value to set
 * @retval None
 */
#define xMODBUS_SLAVE_MSG_RTU_undeterminedSetByteHi xMODBUS_MSG_RTU_undeterminedSetByteHi

/**
 * @brief This function-like macro sets Low part of data section, which used in undetermined section
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @param __BYTEIDX index of byte
 * @param __VAL value to set
 * @retval None
 */
#define xMODBUS_SLAVE_MSG_RTU_undeterminedSetByteLo xMODBUS_MSG_RTU_undeterminedSetByteLo

/* Shared Functions */
uint16_t MODBUS_SLAVE_MSG_RTU_CrcCalculate (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pMsgRTU);
int32_t MODBUS_SLAVE_MSG_RTU_CrcValidate (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pMsgRTU);
uint32_t MODBUS_SLAVE_MSG_RTU_TypeDefLength (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pSrcMsgRTU);

int32_t MODBUS_SLAVE_MSG_RTU_Raw2TypeDef (MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const * const pBuffer, uint32_t const Length);

int32_t MODBUS_SLAVE_MSG_RTU_TypeDef2Raw (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pDestMsgRTU,
    uint8_t * const pBuffer, uint32_t const Length);

/* Generic command functions */
void MODBUS_SLAVE_MSG_RTU_commandDetermined ( MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const DataByte1,
    uint8_t const DataByte2,
    uint8_t const DataByte3,
    uint8_t const DataByte4
    );

void MODBUS_SLAVE_MSG_RTU_commandUndetermined ( MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const * const ByteArray,
    uint32_t const Length
    );

void MODBUS_SLAVE_MSG_RTU_commandException ( MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const ExceptionCode
    );

/* Determined command function */
void MODBUS_SLAVE_MSG_RTU_commandWriteSingleCoil (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    );

void MODBUS_SLAVE_MSG_RTU_commandWriteSingleRegister (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    );

void MODBUS_SLAVE_MSG_RTU_commandWriteMultipleCoils (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const QuantityCoilsHi,
    uint8_t const QuantityCoilsLo
    );

void MODBUS_SLAVE_MSG_RTU_commandWriteMultipleRegisters (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo
    );

/* Undeterministic */
void MODBUS_SLAVE_MSG_RTU_commandReadCoils (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    );

void MODBUS_SLAVE_MSG_RTU_commandReadDiscreteInput(
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    );

void MODBUS_SLAVE_MSG_RTU_commandReadHoldingRegister (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    );

void MODBUS_SLAVE_MSG_RTU_commandReadInputRegister (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    );

#ifdef __cplusplus
}
#endif
#endif // MODBUS_SLAVE_H
