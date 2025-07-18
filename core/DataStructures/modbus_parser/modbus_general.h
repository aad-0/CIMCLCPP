/**
 **********************************************
 * @file: modbus_general.h
 * @brief This file has general defines and typedefs
 * for modbus protocol.
 * These files provides parsing and generating protocol messages
 * for master and slave
 ************************************************
 * @ref:
 * Reference(s):
 * https://logicio.com/HTML/ioext-modbuscommands.htm
 * https://www.modbustools.com/modbus.html#Function01
 * modbus crc16
 **********************************************
 */
#ifndef MODBUS_GENERAL_H
#define MODBUS_GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <stdint.h>
/* Debug Stuff */
#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED() return -192
#endif

#ifndef UNUSED
#define UNUSED(__X) (void) (__X)
#endif

/* typedefs */
/**
 * @brief This enum determines the type of message for master and slave
 * Undetermineds' size can be vary
 * Determineds' size is determined, fixed
 * Exception is specific for slave messages, it has fixed size
 * Unkown is for error cases
 */
typedef enum MODBUS_MSG_RTU_SECTION_e
{
	Undetermined = 0x01,
	Determined = 0x02,
	Exception = 0x80, /**< @note slave specific */
	Unknown = 0xFF
} MODBUS_MSG_RTU_SECTION_TypeDef;

/* General */
#define MODBUS_MSG_RTU_MAXSIZE_B (256U) /**< Maximum size/length of modbus messag */
#define MODBUS_CRC16_LOOKUP_SIZE (256U) /**< size of lookup table */

#define MODBUS_MSG_RTU_MINLENGTH (4U) /**< With Error bytes */

/* Commands */
/* MODBUS RTCU COMMANDS */
/**
 * @todo there are more messages, add them to commend
 * @brief
 * Master
 * The MODBUS commands that the RTCU device uses to read and write I/O in remote devices.
 * Command Name Description
 * 02 (0x02) Reads discrete inputs. 				This is used to read the digital inputs from the devices.
 * 04 (0x04) Reads input registers. 				This is used to read the analog inputs from the devices.
 * 15 (0x0F) Writes multiple coils. 				This is used to set the digital outputs for the devices.
 * 16 (0x10) Writes multiple holding registers.	This is used to set the analog outputs for the devices.
 * Reference See: https://www.modbustools.com/modbus.html#Function01
 */
#define MODBUS_COMMAND_READ_COILS				        (0x01U)
#define MODBUS_COMMAND_READ_DISCRETE_INPUT		  (0x02U)
#define MODBUS_COMMAND_READ_HOLDING_REGISTERS	  (0x03U)
#define MODBUS_COMMAND_READ_INPUT_REGISTERS		  (0x04U)

#define MODBUS_COMMAND_WRITE_SINGLE_COIL	      (0x05U)
#define MODBUS_COMMAND_WRITE_SINGLE_REGISTER    (0x06U)
#define MODBUS_COMMAND_WRITE_MULTIPLE_COILS		  (0x0FU)
#define MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS (0x10U)
#define MODBUS_ERR_COMMAND_UNKOWN	              (0xFFU)

/**
 * @brief Exceptions
 * @ref https://www.simplymodbus.ca/exceptions.htm
 * Reference: https://www.simplymodbus.ca/exceptions.htm
 * Exceptions
 */
#define MODBUS_EXCEPTION_MASK                                     (0x80U)

#define MODBUS_EXCEPTION_ILLEGAL_FUNCTION                         (0x01U)
#define MODBUS_EXCEPTION_ILLEGAL_DATA_ACCESS                      (0x02U)
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE                       (0x03U)
#define MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE                     (0x04U)
#define MODBUS_EXCEPTION_ACKNOWLEDGE                              (0x05U)
#define MODBUS_EXCEPTION_SLAVE_DEVICE_BUSY                        (0x06U)
#define MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE                     (0x07U)
#define MODBUS_EXCEPTION_MEMORY_PARITY_ERROR                      (0x08U)
#define MODBUS_EXCEPTION_GATEAWAY_PATH_UNAVAILABLE                (0x0AU)
#define MODBUS_EXCEPTION_GATEAWAY_TARGET_DEVICE_FAILED_TO_RESPOND (0x0BU)

/* Memory Areas */
/* 1 Bit */
/* size in bytes: 1250 */
#define MODBUS_MEMORY_AREA_COIL_RANGE_Lo	(1U)
#define MODBUS_MEMORY_AREA_COIL_RANGE_Hi	(10000U)
/* #define MODBUS_MEMORY_AREA_COIL_PHYS_LENGTH	(1250U) */

/* size in bytes: 1250 */
#define MODBUS_MEMORY_AREA_INPUT_RANGE_Lo	(10001U)
#define MODBUS_MEMORY_AREA_INPUT_RANGE_Hi (20000U)
/* #define MODBUS_MEMORY_AREA_INPUT_PHYS_LENGTH	(1250U) */
/* 16 Bit-per register, split to 8 */
#define MODBUS_MEMORY_AREA_INPUT_REGISTERS_RANGE_Lo (30001U)
#define MODBUS_MEMORY_AREA_INPUT_REGISTERS_RANGE_Hi (40000U)

#define MODBUS_MEMORY_AREA_HOLDING_REGISTERS_RANGE_Lo (40001U)
#define MODBUS_MEMORY_AREA_HOLDING_REGISTERS_RANGE_Hi (50000U)
/* Function Like Macros */
/**
 * @brief checks if it is a valid command or not
 * @param __COMMAND  Command
 * @retval: Boolean wheter it is a command or not
 * @note to check command behaviours Aka (Un)Determined,
 *  see IsCommandSecXXX macros
 */
#define xMODBUS_MSG_RTU_IsCommand(__COMMAND)	\
(	\
    (MODBUS_COMMAND_READ_COILS                == (__COMMAND)) ||	\
    (MODBUS_COMMAND_READ_DISCRETE_INPUT       == (__COMMAND)) ||	\
    (MODBUS_COMMAND_READ_HOLDING_REGISTERS    == (__COMMAND))	|| \
		(MODBUS_COMMAND_READ_INPUT_REGISTERS      == (__COMMAND)) || \
		(MODBUS_COMMAND_WRITE_SINGLE_COIL         == (__COMMAND))	||	\
		(MODBUS_COMMAND_WRITE_SINGLE_REGISTER     == (__COMMAND))	||	\
		(MODBUS_COMMAND_WRITE_MULTIPLE_COILS      == (__COMMAND))	||	\
		(MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS  == (__COMMAND))	\
    )

/**
 * @brief This check if the command is an exception of command
 * @note Exception of commands are masked with 0x80 Aka MODBUS_EXCEPTION_MASK
 * further info, see reference
 * @ref https://www.simplymodbus.ca/exceptions.htm
 * @param __COMMAND Command
 * @note There are no or'ed commands definitions for exception
 * This macro is expected to use on raw information
 */
#define xMODBUS_MSG_RTU_IsCommandException(__COMMAND) \
( (__COMMAND) & (MODBUS_EXCEPTION_MASK) )

/**
 * @brief get any high byte of 16-bit
 * @param 16-bit variable
 * @retval First byte of 16-bit variable
 */
#define xMODBUS_MSG_RTU_getByteHi(__BYTE)	\
(( (__BYTE) & 0xFF00) >> 8)

/**
 * @brief get any low byte of 16-bit
 * @param 16-bit variable
 * @retval First byte of 16-bit variable
 */
#define xMODBUS_MSG_RTU_getByteLo(__BYTE)	\
(( (__BYTE) & 0x00FF))

/**
 * @brief This function-like macro returns slave address of struct
 * @param __MSG__ Struct pointer
 * @note Any of master and slave's msg struct can be given as parameter,
 * These macros are shared between two of structs, yet have their own macros
 * that defined to these generic ones to provide easier interface for user
 * @retval slave address, uint8_t, char
 */
#define xMODBUS_MSG_RTU_getSlaveAddress(__MSG__)  \
((__MSG__))->Data.Determined.SlaveAddress /**< Determined part is used, but feel free to use for undetermined ones aswell
If you have any further questions, see implementation of struct*/

/**
 * @brief This function-like macro returns function/command of struct
 * @param __MSG__ Struct pointer
 * @retval slave address, uint8_t, char
 */
#define xMODBUS_MSG_RTU_getFunction(__MSG__)  \
((__MSG__))->Data.Determined.Function /**< Determined part is used, but feel free to use for undetermined ones aswell
If you have any further questions, see implementation of struct*/

/**
 * @brief This function-like macro returns errorHi Byte
 * @param __MSG__ Struct Pointer
 * @retval Error High Byte Aka ErrorHi uint8_t, char
 * @note For Undeterministic sections, errorHi and errorLo bytes are not included in the definition of struct
 * These macros have more generic approach to get Error Bytes. Therefore, these macros can be used for master and slave
 * with any section
 * @note This macro relies on to meta part of message
 */
#define xMODBUS_MSG_RTU_getErrorHi(__MSG__)  \
( (uint8_t const *) &( ((__MSG__))->Data.RawData ) ) [ ((__MSG__))->Meta.Length -1]

/**
 * @brief This function-like macro returns errorHi Byte
 * @param __MSG__ Struct Pointer
 * @retval Error Low Byte Aka ErrorLo uint8_t, char
 * @note For Undeterministic sections, errorHi and errorLo bytes are not included in the definition of struct
 * These macros have more generic approach to get Error Bytes. Therefore, these macros can be used for master and slave
 * with any section
 * @note This macro relies on to meta part of message
 */
#define xMODBUS_MSG_RTU_getErrorLo(__MSG__)  \
( (uint8_t const *) &( ((__MSG__))->Data.RawData ) ) [ ((__MSG__))->Meta.Length -2]

/**
 * @brief This function-like macro sets errorHi Byte
 * This macro used to construct new messages
 * @note Dont forget that modbus uses CRC16, and have low byte first in protocol
 * @param __MSG__ Struct Pointer
 * @note This macro relies on to meta part of message
 * @retval None
 */
#define xMODBUS_MSG_RTU_setErrorHi(__MSG__, __VAL)  \
( ( (uint8_t *) &( ((__MSG__))->Data.RawData ) ) [ ((__MSG__))->Meta.Length -1] ) = ((__VAL))

/**
 * @brief This function-like macro sets errorLo Byte
 * @param __MSG__ Struct Pointer
 * This macro used to construct new messages
 * @note Dont forget that modbus uses CRC16, and have low byte first in protocol
 * @note This macro relies on to meta part of message
 * @retval None
 */
#define xMODBUS_MSG_RTU_setErrorLo(__MSG__, __VAL)  \
( ( (uint8_t *) &( ((__MSG__))->Data.RawData ) ) [ ((__MSG__))->Meta.Length -2] ) = ((__VAL))

/**
 * @brief This function-like macro returns head of bytes
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @note This macro gives direct access to bytes section of undeterministic messages
 * @param __MSG__ Struct pointer
 * @retval uint8_t/char pointer
 */
#define xMODBUS_MSG_RTU_undeterminedGetBytesPointer(__MSG__)  \
( ( (uint8_t *) &( ((__MSG__))->Data.Undetermined.ByteCount ) ) +1 )

/**
 * @brief This function-like macro sets High part of data section, which used in undetermined section
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @param __BYTEIDX index of byte
 * @param __VAL value to set
 * @retval None
 * @note This macro is useful to generate messages with WriteDataHi and Lo
 */
#define xMODBUS_MSG_RTU_undeterminedSetByteHi(__MSG__, __BYTEIDX, __VAL)     \
do {                                                                      \
uint8_t * pBytes = xMODBUS_MSG_RTU_undeterminedGetBytesPointer( ((__MSG__)) );  \
pBytes [ (__BYTEIDX *2) ] = __VAL;                                      \
} while (0)

/**
 * @brief This function-like macro sets Low part of data section, which used in undetermined section
 * @note This macro should be used when undetermined section is in use, enabled in meta section of struct
 * @param __MSG__ Struct pointer
 * @param __BYTEIDX index of byte
 * @param __VAL value to set
 * @retval None
 * @note This macro is useful to generate messages with WriteDataHi and Lo
 */
#define xMODBUS_MSG_RTU_undeterminedSetByteLo(__MSG__, __BYTEIDX, __VAL)     \
do {                                                                      \
uint8_t * pBytes = xMODBUS_MSG_RTU_undeterminedGetBytesPointer( ((__MSG__)) );  \
pBytes [ ((__BYTEIDX *2) +1) ] = __VAL;                                 \
} while (0)

#ifdef __cplusplus
}
#endif
#endif /* MODBUS_GENERAL_H */
