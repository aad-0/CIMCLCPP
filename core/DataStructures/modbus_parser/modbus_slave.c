/**
 **********************************************
 * @file: modbus_slave.c
 **********************************************
 */
#ifdef __cplusplus /* this is here so c++ compiler wont mess with overloading */
extern "C" {
#endif
/* Includes */
#include "modbus_slave.h"
#include "modbus_general.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>

extern uint16_t const CRC16_LOOKUP [MODBUS_CRC16_LOOKUP_SIZE];

/**
 * @brief: This Calculates a CRC for an slave message
 * @param[in] pMsgRtu: Msg Pointer
 * @retval: CRC value order as HiLo
 * @note: Do not forget that in message, Lo part becomes first
 */
uint16_t MODBUS_SLAVE_MSG_RTU_CrcCalculate (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pMsgRTU)
{
    UNUSED(pMsgRTU);
    assert ((void *)0 != pMsgRTU);
    assert (xMODBUS_SLAVE_MSG_RTU_SECTION_isValid(pMsgRTU->Meta.Section));

    uint8_t tmp = 0;
    uint16_t crc = 0xFFFF;
    uint8_t const * const pMsg = (uint8_t const * const)& (xMODBUS_MSG_RTU_getSlaveAddress(pMsgRTU));


    uint32_t const limit = (MODBUS_SLAVE_MSG_RTU_TypeDefLength(pMsgRTU)) -2; /* -2, Err Bytes */

    uint32_t i = 0;
    for (i = 0; i < limit; ++i)
    {
        tmp = * ((uint8_t const *)
                ( ((uint8_t const *)pMsg) +i));
        tmp ^= crc;
        crc >>= 8;
        crc ^= CRC16_LOOKUP [tmp];
    }

    return crc;
}

/**
 * @brief: This Validates Crc of a Slave message. It calculates messages crc and checks
 * if it is same as the receviced mesasages crc
 * @param[in] pMsgRtu: Msg Pointer
 * @retval: boolean, if it maches, returns True, else False
 */
int32_t
MODBUS_SLAVE_MSG_RTU_CrcValidate (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pMsgRTU)
{
    uint8_t const msgCrcLo = xMODBUS_MSG_RTU_getErrorLo(pMsgRTU);
    uint8_t const msgCrcHi = xMODBUS_MSG_RTU_getErrorHi(pMsgRTU);
    uint16_t const msgCalculatedCrcHiLo = MODBUS_SLAVE_MSG_RTU_CrcCalculate (pMsgRTU);

    UNUSED(pMsgRTU);
    assert ((void *)0 != pMsgRTU);

    return (
        (xMODBUS_MSG_RTU_getByteHi(msgCalculatedCrcHiLo) == msgCrcHi) &&
        (xMODBUS_MSG_RTU_getByteLo(msgCalculatedCrcHiLo) ==  msgCrcLo)
    );
}

/**
 * @brief MODBUS_SLAVE_MSG_RTU_TypeDefLength: This Function returns length with error bytes
 * @note This length is same as with raw bytes
 * @param pSrcMsgRTU: Source
 * @return Length
 */
uint32_t MODBUS_SLAVE_MSG_RTU_TypeDefLength (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pSrcMsgRTU)
{
    uint32_t length = 0;
    /* Check Which section is On */
    uint8_t secFunc;
    xMODBUS_SLAVE_MSG_RTU_CommandSection(
        ( xMODBUS_MSG_RTU_getFunction(pSrcMsgRTU) ), &secFunc);

    assert (Unknown != secFunc);

    if (Determined == secFunc) /* Fixed Length */
      length = MODBUS_SLAVE_MSG_RTU_SecDetermined_LENGTH;
    else if (Exception == secFunc)
      length = MODBUS_SLAVE_MSG_RTU_SecException_LENGTH;
    else /* varies respect to byteCount */
        length = MODBUS_SLAVE_MSG_RTU_SecUndetermined_MINLENGTH + pSrcMsgRTU->Data.Undetermined.ByteCount;

    return length;
}

/**
 * @brief: This function generates meta information and copies raw data into the typedef
 * @param pDestMsgRTU Destination typedef
 * @param pBuffer Source Buffer
 * @param Length Length of pBuffer
 * @retval difference between BUffers Length and calculated Typedefs Length
 * @note this function may 'function' faulty when bytecount is not placed in the pBuffer
 */
int32_t MODBUS_SLAVE_MSG_RTU_Raw2TypeDef (MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const * const pBuffer, uint32_t const Length)
{
  /* Message size cannot be greater than Max Length */
  assert (MODBUS_MSG_RTU_MAXSIZE_B >= Length);
  /* Copy raw section to typedef */
  uint8_t * const pHead = (uint8_t *) &( xMODBUS_SLAVE_MSG_RTU_getSlaveAddress(pDestMsgRTU) );

  uint32_t i = 0;
  for (; i < Length; ++i)
    pHead [i] = pBuffer [i];

  /* Contstruct meta */
  pDestMsgRTU->Meta.Length = MODBUS_SLAVE_MSG_RTU_TypeDefLength (pDestMsgRTU);
  MODBUS_MSG_RTU_SECTION_TypeDef secFunc;

  xMODBUS_SLAVE_MSG_RTU_CommandSection(
      ( xMODBUS_MSG_RTU_getFunction(pDestMsgRTU) ), &secFunc);

  assert (Unknown != secFunc);
  pDestMsgRTU->Meta.Section = secFunc;

  return (Length - (pDestMsgRTU->Meta.Length));
}

/**
 * @brief: This function coppies raw info to a char/uint8/byte array 
 * @param pSrcMsgRTU Destination typedef
 * @param pBuffer Source Buffer
 * @param Length Length of pBuffer
 * @retval difference between BUffers Length and calculated Typedefs Length
 */
int32_t MODBUS_SLAVE_MSG_RTU_TypeDef2Raw (MODBUS_SLAVE_MSG_RTU_TypeDef const * const pSrcMsgRTU,
    uint8_t * const pBuffer, uint32_t const Length)
{
	uint32_t const typedefLength = MODBUS_SLAVE_MSG_RTU_TypeDefLength(pSrcMsgRTU);
  /* Message size cannot be greater than Max Length */
  assert (MODBUS_MSG_RTU_MAXSIZE_B >= Length);
	assert (typedefLength <= Length);
  assert (Unknown != pSrcMsgRTU->Meta.Section);
  /* Copy raw section to typedef */
  uint8_t const * const pHead = (uint8_t *) &( xMODBUS_SLAVE_MSG_RTU_getSlaveAddress(pSrcMsgRTU) );

  uint32_t i = 0;
  for (; i < typedefLength; ++i)
    pBuffer [i] = pHead [i];

	return (Length - typedefLength);
}

/**
 * @brief: This function  generates slave message for determined commands
 * @param[out] pDestMsgRTU  Destination Msg Pointer
 * @param SlaveAddress Slave Address
 * @param Function Command/Function
 * @param DataByte1 DataByte1
 * @param DataByte2 DataByte2
 * @param DataByte3 DataByte3
 * @param DataByte4 DataByte4
 */
void MODBUS_SLAVE_MSG_RTU_commandDetermined ( MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const DataByte1,
    uint8_t const DataByte2,
    uint8_t const DataByte3,
    uint8_t const DataByte4
    )
{
  assert ( xMODBUS_SLAVE_MSG_RTU_IsCommandSecDetermined(Function) );

  /* Set meta */
  pDestMsgRTU->Meta.Section = Determined;
  pDestMsgRTU->Meta.Length = MODBUS_SLAVE_MSG_RTU_SecDetermined_LENGTH;
  /* Set raw data */
  pDestMsgRTU->Data.Determined.SlaveAddress = SlaveAddress;
  pDestMsgRTU->Data.Determined.Function = Function;

  pDestMsgRTU->Data.Determined.DataByte1.DataByte1 = DataByte1;
  pDestMsgRTU->Data.Determined.DataByte2.DataByte2 = DataByte2;
  pDestMsgRTU->Data.Determined.DataByte3.DataByte3 = DataByte3;
  pDestMsgRTU->Data.Determined.DataByte4.DataByte4 = DataByte4;
  /* Crc */
  uint16_t const crc = MODBUS_SLAVE_MSG_RTU_CrcCalculate (pDestMsgRTU);
  xMODBUS_SLAVE_MSG_RTU_setErrorHi(pDestMsgRTU, xMODBUS_MSG_RTU_getByteHi(crc));
  xMODBUS_SLAVE_MSG_RTU_setErrorLo(pDestMsgRTU, xMODBUS_MSG_RTU_getByteLo(crc));
}

/**
 * @brief This Function generates a slave message for undetermined commands
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress Slave adress
 * @param Function Function/Command
 * @param ByteArray ByteArray
 * @param Length ByteArray's Length
 */
void MODBUS_SLAVE_MSG_RTU_commandUndetermined (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const * const ByteArray,
    uint32_t const Length
    )
{
  /* Set meta */
  pDestMsgRTU->Meta.Section = Undetermined;
  pDestMsgRTU->Meta.Length =
    MODBUS_SLAVE_MSG_RTU_SecUndetermined_MINLENGTH + Length;

  /* Set Raw Data */
  pDestMsgRTU->Data.Undetermined.SlaveAddress = SlaveAddress;
  pDestMsgRTU->Data.Undetermined.Function = Function;

  /* Set Byte Array */
  pDestMsgRTU->Data.Undetermined.ByteCount = Length;
  uint8_t * const pBytes =
  xMODBUS_SLAVE_MSG_RTU_undeterminedGetBytesPointer(pDestMsgRTU);
  
  uint32_t i = 0;
  for (; i < Length; ++i)
    pBytes [i] = ByteArray [i];

  /* Crc */
  uint16_t const crc = MODBUS_SLAVE_MSG_RTU_CrcCalculate (pDestMsgRTU);
  xMODBUS_SLAVE_MSG_RTU_setErrorHi(pDestMsgRTU, xMODBUS_MSG_RTU_getByteHi(crc));
  xMODBUS_SLAVE_MSG_RTU_setErrorLo(pDestMsgRTU, xMODBUS_MSG_RTU_getByteLo(crc));
}

/**
 * @brief This Function generates slave's exception message
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress Slave adress
 * @param Function Function/Command
 * @param ExceptionCode Exception Code
 *    MODBUS_EXCEPTION_ILLEGAL_FUNCTION                         (0x01U)
 *    MODBUS_EXCEPTION_ILLEGAL_DATA_ACCESS                      (0x02U)
 *    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE                       (0x03U)
 *    MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE                     (0x04U)
 *    MODBUS_EXCEPTION_ACKNOWLEDGE                              (0x05U)
 *    MODBUS_EXCEPTION_SLAVE_DEVICE_BUSY                        (0x06U)
 *    MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE                     (0x07U)
 *    MODBUS_EXCEPTION_MEMORY_PARITY_ERROR                      (0x08U)
 *    MODBUS_EXCEPTION_GATEAWAY_PATH_UNAVAILABLE                (0x0AU)
 *    MODBUS_EXCEPTION_GATEAWAY_TARGET_DEVICE_FAILED_TO_RESPOND (0x0BU)
 */
void MODBUS_SLAVE_MSG_RTU_commandException ( MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const ExceptionCode
    )
{
  /* Set meta */
  pDestMsgRTU->Meta.Section = Exception;
  pDestMsgRTU->Meta.Length =
    MODBUS_SLAVE_MSG_RTU_SecException_LENGTH;

  /* Set Raw Data */
  pDestMsgRTU->Data.Exception.SlaveAddress = SlaveAddress;
  pDestMsgRTU->Data.Exception.Function = Function | MODBUS_EXCEPTION_MASK;

  /* Set Byte Array */
  pDestMsgRTU->Data.Exception.ExceptionCode = ExceptionCode;
  
  /* Crc */
  uint16_t const crc = MODBUS_SLAVE_MSG_RTU_CrcCalculate (pDestMsgRTU);
  xMODBUS_SLAVE_MSG_RTU_setErrorHi(pDestMsgRTU, xMODBUS_MSG_RTU_getByteHi(crc));
  xMODBUS_SLAVE_MSG_RTU_setErrorLo(pDestMsgRTU, xMODBUS_MSG_RTU_getByteLo(crc));
}

/* Determined command function */
/**
 * @brief This Function generates slaves response to write single coil
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param CoilAddressHi Coil AddressHi
 * @param CoilAddressLo Coil AddressLo
 * @param WriteDataHi WriteDataHi
 * @param WriteDataLo WriteDataLo
 */
void MODBUS_SLAVE_MSG_RTU_commandWriteSingleCoil (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    )
{
  MODBUS_SLAVE_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_SINGLE_COIL,
      CoilAddressHi, CoilAddressLo, WriteDataHi, WriteDataLo);
}

/**
 * @brief This Function generates slaves response to write single register
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param RegisterAddressHi Register AddressHi
 * @param RegisterAddressLo Register AddressLo
 * @param WriteDataHi WriteDataHi
 * @param WriteDataLo WriteDataLo
 */
void MODBUS_SLAVE_MSG_RTU_commandWriteSingleRegister (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const RegisterAddressHi,
    uint8_t const RegisterAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    )
{
  MODBUS_SLAVE_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_SINGLE_REGISTER,
      RegisterAddressHi, RegisterAddressLo, WriteDataHi, WriteDataLo);
}

/**
 * @brief This Function generates slaves response to write multiple coils
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param CoilAddressHi Coil AddressHi
 * @param CoilAddressLo Coil AddressLo
 * @param QuantityCoilsHi QuantityCoilsHi
 * @param QuantityCoilsLo QuantityCoilsLo
 */
void MODBUS_SLAVE_MSG_RTU_commandWriteMultipleCoils (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const QuantityCoilsHi,
    uint8_t const QuantityCoilsLo
    )
{
  MODBUS_SLAVE_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_SINGLE_REGISTER,
      CoilAddressHi, CoilAddressLo, QuantityCoilsHi, QuantityCoilsLo);
}

/**
 * @brief This Function generates slaves response to write multiple registers
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param StartingAddressHi Starting AddressHi
 * @param StartingAddressLo Starting AddressLo
 * @param QuantityRegistersHi QuantityRegistersHi
 * @param QuantityRegistersLo QuantityRegistersLo
 */
void MODBUS_SLAVE_MSG_RTU_commandWriteMultipleRegisters (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo
    )
{
  MODBUS_SLAVE_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_SINGLE_REGISTER,
      StartingAddressHi, StartingAddressLo,
      QuantityRegistersHi, QuantityRegistersLo);
}
/* Undetermineds */
/**
 * @brief This Function generates slaves response to read coils
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param DataByteArray ByteArray of Data
 * @param Length Length of DataByteArray
 */
void MODBUS_SLAVE_MSG_RTU_commandReadCoils (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    )
{
  MODBUS_SLAVE_MSG_RTU_commandUndetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_COILS,
      DataByteArray,
      Length
      );
}

/**
 * @brief This Function generates slaves response to read discrete input
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param DataByteArray ByteArray of Data
 * @param Length Length of DataByteArray
 */
void MODBUS_SLAVE_MSG_RTU_commandReadDiscreteInput(
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    )
{
  MODBUS_SLAVE_MSG_RTU_commandUndetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_DISCRETE_INPUT,
      DataByteArray,
      Length
      );
}

/**
 * @brief This Function generates slaves response to read holding registers
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param DataByteArray ByteArray of Data
 * @param Length Length of DataByteArray
 */
void MODBUS_SLAVE_MSG_RTU_commandReadHoldingRegister (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    )
{
  MODBUS_SLAVE_MSG_RTU_commandUndetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_HOLDING_REGISTERS,
      DataByteArray,
      Length
      );
}

/**
 * @brief This Function generates slaves response to read Input Register
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress SlaveAddress
 * @param DataByteArray ByteArray of Data
 * @param Length Length of DataByteArray
 */
void MODBUS_SLAVE_MSG_RTU_commandReadInputRegister (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    )
{
  MODBUS_SLAVE_MSG_RTU_commandUndetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_INPUT_REGISTERS,
      DataByteArray,
      Length
      );
}
#ifdef __cplusplus /* this is here so c++ compiler wont mess with overloading */
}
#endif
