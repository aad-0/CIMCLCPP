/**
 **********************************************
 * @file: modbus_master.h
 * @brief: This is a header file of modbus_master.c file
 * This file provides shared functions, typedefs and definitions
 * to handle master messages
 **********************************************
 */
#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "modbus_general.h"

/* TypeDefs */
/**
 * @brief Master message struct
 * this struct can express any king of message, see MODBUS_MSG_RTU_SECITON_Typedef,
 * with Data section. This struct provides easier access to members and meta section
 * which can be used to control statemachines
 * @note user can eliminate usage of Section part, but dont forget that some functions
 * in modbus_general.h depends on Length of Meta
 * @note user should follow reference of modbus message structure to generate messages with this
 * class. Or use prewritten functons. Feel free to add your own command functions. macros should handle them
 * well.
 */
typedef struct MODBUS_MASTER_MSG_RTU_s
{
    struct
    {
      MODBUS_MSG_RTU_SECTION_TypeDef Section;
      uint32_t Length;
    } Meta; /**< This Holds Meta Information about struct.
                @note These meta information cannot be set together.
                This Meta information is used in state machined to generate related response*/
    /* Actual Message Part */
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
					uint8_t DataByte1; /**< for customizability */
				} DataByte1; /**< This is first byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */

      
        union
        {
          uint8_t StartingAddressLo;
          uint8_t CoilAddressLo;
          uint8_t RegisterAddressLo;
					uint8_t DataByte2; /**< for customizability */
				} DataByte2; /**< This is second byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */

      
        union
        {
          uint8_t QuantityCoilsHi;
          uint8_t QuantityInputsHi;
          uint8_t QuantityRegistersHi;
          uint8_t WriteDataHi;
					uint8_t DataByte3; /**< for customizability */
				} DataByte3; /**< This is third byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
          uint8_t QuantityCoilsLo;
          uint8_t QuantityInputsLo;
          uint8_t QuantityRegistersLo;
          uint8_t WriteDataLo;
					uint8_t DataByte4; /**< for customizability */
				} DataByte4; /**< This is fourth byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
        /* end section */
        uint8_t ErrorLo;
        uint8_t ErrorHi;
			} Determined; /**< This member should use to generate and parse determined commands/functions see definitions and macros
to see which functions are determined or not */

      struct
      {
        uint8_t SlaveAddress; /**< Slave Address, Every message has this information as first byte of message*/
        uint8_t Function; /**< Function, Every message has this information as second byte of message */
        union
        {
            uint8_t StartingAddressHi;
            uint8_t CoilAddressHi;
            uint8_t RegisterAddressHi;
						uint8_t DataByte1; /**< for customizability */
				} DataByte1; /**< This is first byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
            uint8_t StartingAddressLo;
            uint8_t CoilAddressLo;
            uint8_t RegisterAddressLo;
						uint8_t DataByte2; /**< for customizability */
				} DataByte2; /**< This is second byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
            uint8_t QuantityCoilsHi;
            uint8_t QuantityInputsHi;
            uint8_t QuantityRegistersHi;
            uint8_t WriteDataHi;
						uint8_t DataByte3; /**< for customizability */
				} DataByte3; /**< This is third byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */
      
        union
        {
            uint8_t QuantityCoilsLo;
            uint8_t QuantityInputsLo;
            uint8_t QuantityRegistersLo;
            uint8_t WriteDataLo;
            uint8_t DataByte4; /* for customizability */
				} DataByte4; /**< This is fourth byte of a determined message, this byte can be called in different ways respoect to union
This provides easy message parsing and generation. Also It provides easier code to read */

        uint8_t ByteCount;
        /**< User should access Data and Error Sections with macros */
			} Undetermined; /**< This member should be used to generate and parse undetermined commands/functions. To see which commands
are undetermined or not, see macros defined in the header file
@note to access byte array that comes after ByteCount, user should use macros */

		} Data; /**< Raw data where it holds. It has an array of size max of message length and it manages data in that array with
 members like determined and undetermined */

} MODBUS_MASTER_MSG_RTU_TypeDef;


#define MODBUS_MASTER_MSG_RTU_SecDetermined_LENGTH (8U) /**< Fixed Length when determined section is in use */
#define MODBUS_MASTER_MSG_RTU_SecUndetermined_MINLENGTH (9U) /**< With Error Bytes, @note Error Bytes has not set in the struct def */
#define MODBUS_MASTER_MSG_RTU_MINLENGTH MODBUS_MASTER_MSG_RTU_SecDetermined_LENGTH //MODBUS_MASTER_MSG_RTU_Sec0x0F_0x10_MINLENGTH

/* Function Like Macros */
/**
 * @brief This Function-like macro checks section whether valid or not for master
 * @retval Boolean
 */
#define xMODBUS_MASTER_MSG_RTU_SECTION_isValid(__SEC) \
  ( ( Undetermined  == (__SEC) ) ||                   \
    ( Determined    == (__SEC) )                      \
  )

/** 
 * @brief Function-like macro, returns true if command is related with determined section
 * @param __COMMAND Command of modbus
 * @retval Boolean
 */
#define xMODBUS_MASTER_MSG_RTU_IsCommandSecDetermined(__COMMAND)  \
    (	                                                            \
    (MODBUS_COMMAND_READ_COILS              == (__COMMAND) )  ||  \
    (MODBUS_COMMAND_READ_DISCRETE_INPUT     == (__COMMAND) )  ||  \
    (MODBUS_COMMAND_READ_HOLDING_REGISTERS  == (__COMMAND) )  ||  \
    (MODBUS_COMMAND_READ_INPUT_REGISTERS    == (__COMMAND) )  ||  \
    (MODBUS_COMMAND_WRITE_SINGLE_COIL       == (__COMMAND) )  ||  \
    (MODBUS_COMMAND_WRITE_SINGLE_REGISTER   == (__COMMAND) ) 		  \
    )

/** 
 * @brief Function-like macro, returns true if command is related with undetermined section
 * @param __COMMAND Command of modbus
 * @retval Boolean
 */
#define xMODBUS_MASTER_MSG_RTU_IsCommandSecUndetermined(__COMMAND)    \
    (															                                    \
       (MODBUS_COMMAND_WRITE_MULTIPLE_COILS     == (__COMMAND) )  ||  \
       (MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS == (__COMMAND) )      \
    )

/**
 * @brief Returns which section is valid respect to function
 * @param Command: Command
 * @param[out]: MODBUS_MASTER_MSG_RTU_SECTION_TypeDef aka enum MODBUS_MASTER_MSG_RTU_SECTION_e
 *            1 : Undetermined
 * 	          2 : Determined
 * 	          -1 : Unknown
 * @retval: None
 */
#define xMODBUS_MASTER_MSG_RTU_CommandSection(__COMMAND, __RET__)	      \
do	{                                                                   \
    *((__RET__)) = Unknown;                             	              \
    if (xMODBUS_MASTER_MSG_RTU_IsCommandSecUndetermined(__COMMAND))     \
        *((__RET__)) = Undetermined;	                                  \
    else if (xMODBUS_MASTER_MSG_RTU_IsCommandSecDetermined(__COMMAND))	\
        *((__RET__)) = Determined;	                                    \
    } while (0);

/**
 * @brief This function-like macro returns slave address of struct
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct pointer
 * @retval slave address, uint8_t, char
 */
#define xMODBUS_MASTER_MSG_RTU_getSlaveAddress xMODBUS_MSG_RTU_getSlaveAddress

/**
 * @brief This function-like macro returns function of struct
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct pointer
 * @retval slave address, uint8_t, char
 */
#define xMODBUS_MASTER_MSG_RTU_getFunction xMODBUS_MSG_RTU_getFunction

/**
 * @brief This function-like macro returns errorHi Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval Error High Byte Aka ErrorHi uint8_t, char
 * @note See explaination about struct
 */
#define xMODBUS_MASTER_MSG_RTU_getErrorHi xMODBUS_MSG_RTU_getErrorHi

/**
 * @brief This function-like macro returns errorLo Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval Error High Byte Aka ErrorLo, uint8_t, char
 * @note See explaination about struct
 */
#define xMODBUS_MASTER_MSG_RTU_getErrorLo xMODBUS_MSG_RTU_getErrorLo

/**
 * @brief This function-like macro sets errorHi Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval None
 * @note See explaination about struct
 */
#define xMODBUS_MASTER_MSG_RTU_setErrorHi xMODBUS_MSG_RTU_setErrorHi

/**
 * @brief This function-like macro sets errorLo Byte
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @param __MSG__ Struct Pointer
 * @retval None
 * @note See explaination about struct
 */
#define xMODBUS_MASTER_MSG_RTU_setErrorLo xMODBUS_MSG_RTU_setErrorLo

/**
 * @brief This function-like macro returns head of bytes
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @retval uint8_t/char pointer
 */
#define xMODBUS_MASTER_MSG_RTU_undeterminedGetBytesPointer xMODBUS_MSG_RTU_undeterminedGetBytesPointer

/**
 * @brief This function-like macro sets High part of data section, which used in undetermined section
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @param __BYTEIDX index of byte
 * @param __VAL value to set
 * @retval None
 */
#define xMODBUS_MASTER_MSG_RTU_undeterminedSetByteHi xMODBUS_MSG_RTU_undeterminedSetByteHi

/**
 * @brief This function-like macro sets Low part of data section, which used in undetermined section
 * @note This macro is shared one's wrapper-like definition. xMODBUS_(MASTER/SLAVE)MSG_X
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @param __BYTEIDX index of byte
 * @param __VAL value to set
 * @retval None
 */
#define xMODBUS_MASTER_MSG_RTU_undeterminedSetByteLo xMODBUS_MSG_RTU_undeterminedSetByteLo

/* Shared Functions */
uint16_t MODBUS_MASTER_MSG_RTU_CrcCalculate (MODBUS_MASTER_MSG_RTU_TypeDef const * const pMsgRTU);
int32_t MODBUS_MASTER_MSG_RTU_CrcValidate (MODBUS_MASTER_MSG_RTU_TypeDef const * const pMsgRTU);
uint32_t MODBUS_MASTER_MSG_RTU_TypeDefLength (MODBUS_MASTER_MSG_RTU_TypeDef const * const pSrcMsgRTU);
int32_t MODBUS_MASTER_MSG_RTU_Raw2TypeDef (MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const * const pBuffer, uint32_t const Length);

int32_t MODBUS_MASTER_MSG_RTU_TypeDef2Raw (MODBUS_MASTER_MSG_RTU_TypeDef const * const pDestMsgRTU,
    uint8_t * const pBuffer, uint32_t const Length);

/* Generic command functions */
void MODBUS_MASTER_MSG_RTU_commandUndetermined (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const slaveAddress,
    uint8_t const Function,
    uint8_t const DataByte1,
    uint8_t const DataByte2,
    uint8_t const DataByte3,
    uint8_t const DataByte4,
    uint8_t const * const ByteArray,
    uint32_t const Length
    );

void MODBUS_MASTER_MSG_RTU_commandDetermined (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const DataByte1,
    uint8_t const DataByte2,
    uint8_t const DataByte3,
    uint8_t const DataByte4
    );
/* Specific command functions */
/* Determined */
void MODBUS_MASTER_MSG_RTU_commandReadCoils (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityCoilsHi,
    uint8_t const QuantityCoilsLo
    );

void MODBUS_MASTER_MSG_RTU_commandReadDiscreteInput (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityInputsHi,
    uint8_t const QuantityInputsLo
    );

void MODBUS_MASTER_MSG_RTU_commandReadHoldingRegisters (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo
    );

void MODBUS_MASTER_MSG_RTU_commandReadInputRegisters (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo
    );

void MODBUS_MASTER_MSG_RTU_commandWriteSingleCoil (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    );

void MODBUS_MASTER_MSG_RTU_commandWriteSingleRegister (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const RegisterAddressHi,
    uint8_t const RegisterAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    );

/* Undetermined */
void MODBUS_MASTER_MSG_RTU_commandWriteMultipleCoils (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const QuantityCoilsHi,
    uint8_t const QuantityCoilsLo,
    uint8_t const * const WriteDataByteArray,
    uint32_t const Length
    );

void MODBUS_MASTER_MSG_RTU_commandWriteMultipleRegisters (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo,
    uint8_t const * const WriteDataByteArray,
    uint32_t const Length
    );

#ifdef __cplusplus
}
#endif
#endif /* MODBUS_MASTER */
