/**
 **********************************************
 * @file: modbus_master.c
 **********************************************
 */
#ifdef __cplusplus /* This is here so that c++ compiler wont mess with overloading */
extern "C" {
#endif
/* Includes */
#include "modbus_master.h"
#include "modbus_general.h"
#include <stdint.h>
#include <assert.h>
/*#include <string.h> */

extern uint16_t const CRC16_LOOKUP [MODBUS_CRC16_LOOKUP_SIZE];

/**
 * @brief: This Calculates a CRC for an master message
 * @param pMsgRTU Msg Pointer
 * @retval: CRC value order as HiLo
 * @note: Do not forget that in a message, Lo part becomes first
 */
uint16_t MODBUS_MASTER_MSG_RTU_CrcCalculate (MODBUS_MASTER_MSG_RTU_TypeDef const * const pMsgRTU)
{
		UNUSED(pMsgRTU);
	assert ((void *)0 != pMsgRTU);
    assert (xMODBUS_MASTER_MSG_RTU_SECTION_isValid(pMsgRTU->Meta.Section));

    uint8_t tmp;
    uint16_t crc = 0xFFFF;
    uint8_t const * const pMsg = (uint8_t const * const)& (xMODBUS_MSG_RTU_getSlaveAddress(pMsgRTU));

		uint32_t const limit = (MODBUS_MASTER_MSG_RTU_TypeDefLength(pMsgRTU)) -2; /**< @note -2, Err Bytes */

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
 * @brief: This Validates Crc of a Master message. It calcultes crc of message
 * and checkf if it is same as the message has or not
 * @param pMsgRTU Msg Pointer
 * @retval: boolean, if it maches, returns True, else False
 */
int32_t MODBUS_MASTER_MSG_RTU_CrcValidate (MODBUS_MASTER_MSG_RTU_TypeDef const * const pMsgRTU)
{
    uint8_t const msgCrcLo = xMODBUS_MSG_RTU_getErrorLo(pMsgRTU);
    uint8_t const msgCrcHi = xMODBUS_MSG_RTU_getErrorHi(pMsgRTU);
    uint16_t const msgCalculatedCrcHiLo = MODBUS_MASTER_MSG_RTU_CrcCalculate (pMsgRTU);

		UNUSED(pMsgRTU);
		assert ((void *)0 != pMsgRTU);

    return (
        (xMODBUS_MSG_RTU_getByteHi(msgCalculatedCrcHiLo) == msgCrcHi) &&
        (xMODBUS_MSG_RTU_getByteLo(msgCalculatedCrcHiLo) ==  msgCrcLo)
    );
}

/**
 * @brief MODBUS_MASTER_MSG_RTU_TypeDefLength This Function returns length with error bytes
 * @note This length is same as with raw section of struct.
 * @param pSrcMsgRTU: Source
 * @return Length
 */
uint32_t MODBUS_MASTER_MSG_RTU_TypeDefLength (MODBUS_MASTER_MSG_RTU_TypeDef const * const pSrcMsgRTU)
{
    uint32_t length = 0;
    /* Check Which section Should be */
    MODBUS_MSG_RTU_SECTION_TypeDef secFunc;
    xMODBUS_MASTER_MSG_RTU_CommandSection(
        ( xMODBUS_MSG_RTU_getFunction(pSrcMsgRTU) ), &secFunc);

    assert (Unknown != secFunc);

    if (Determined == secFunc) /* Fixed Length */
        length = MODBUS_MASTER_MSG_RTU_SecDetermined_LENGTH;
    else /* varies respect to byteCount */
        length = MODBUS_MASTER_MSG_RTU_SecUndetermined_MINLENGTH + pSrcMsgRTU->Data.Undetermined.ByteCount;

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
int32_t MODBUS_MASTER_MSG_RTU_Raw2TypeDef (MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const * const pBuffer, uint32_t const Length)
{
  /* Message size cannot be greater than Max Length */
  assert (MODBUS_MSG_RTU_MAXSIZE_B >= Length);
  /* Copy raw section to typedef */
	uint8_t * const pHead = (uint8_t * const) &( xMODBUS_MASTER_MSG_RTU_getSlaveAddress(pDestMsgRTU) );

  uint32_t i = 0;
  for (; i < Length; ++i)
    pHead [i] = pBuffer [i];

  /* Contstruct meta */
  pDestMsgRTU->Meta.Length = MODBUS_MASTER_MSG_RTU_TypeDefLength (pDestMsgRTU);
  MODBUS_MSG_RTU_SECTION_TypeDef secFunc;

  xMODBUS_MASTER_MSG_RTU_CommandSection(
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
int32_t MODBUS_MASTER_MSG_RTU_TypeDef2Raw (MODBUS_MASTER_MSG_RTU_TypeDef const * const pSrcMsgRTU,
    uint8_t * const pBuffer, uint32_t const Length)
{
	uint32_t const typedefLength = MODBUS_MASTER_MSG_RTU_TypeDefLength(pSrcMsgRTU);
  /* Message size cannot be greater than Max Length */
  assert (MODBUS_MSG_RTU_MAXSIZE_B >= Length);
	assert (typedefLength <= Length);
  assert (Unknown != pSrcMsgRTU->Meta.Section);
  /* Copy raw section to typedef */
  uint8_t const * const pHead = (uint8_t *) &( xMODBUS_MASTER_MSG_RTU_getSlaveAddress(pSrcMsgRTU) );

  uint32_t i = 0;
  for (; i < typedefLength; ++i)
    pBuffer [i] = pHead [i];

	return (Length - typedefLength);
}

/**
 * @brief This Function generates a master message for determined commands
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress Slave adress
 * @param Function Function/Command
 * @param DataByte1 DataByte1
 * @param DataByte2 DataByte1
 * @param DataByte3 DataByte1
 * @param DataByte4 DataByte1
 */
void MODBUS_MASTER_MSG_RTU_commandDetermined (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const DataByte1,
    uint8_t const DataByte2,
    uint8_t const DataByte3,
    uint8_t const DataByte4
    )
{
  assert ( xMODBUS_MASTER_MSG_RTU_IsCommandSecDetermined(Function) );

  /* Set meta */
  pDestMsgRTU->Meta.Section = Determined;
  pDestMsgRTU->Meta.Length = MODBUS_MASTER_MSG_RTU_SecDetermined_LENGTH;
  /* Set raw data */
  pDestMsgRTU->Data.Determined.SlaveAddress = SlaveAddress;
  pDestMsgRTU->Data.Determined.Function = Function;

  pDestMsgRTU->Data.Determined.DataByte1.DataByte1 = DataByte1;
  pDestMsgRTU->Data.Determined.DataByte2.DataByte2 = DataByte2;
  pDestMsgRTU->Data.Determined.DataByte3.DataByte3 = DataByte3;
  pDestMsgRTU->Data.Determined.DataByte4.DataByte4 = DataByte4;
  /* Crc */
  uint16_t const crc = MODBUS_MASTER_MSG_RTU_CrcCalculate (pDestMsgRTU);
  xMODBUS_MASTER_MSG_RTU_setErrorHi(pDestMsgRTU, xMODBUS_MSG_RTU_getByteHi(crc));
  xMODBUS_MASTER_MSG_RTU_setErrorLo(pDestMsgRTU, xMODBUS_MSG_RTU_getByteLo(crc));
}

/**
 * @brief This Function generates a master message for undetermined commands
 * @param[out] pDestMsgRTU Destination Msg Pointer
 * @param SlaveAddress Slave adress
 * @param Function Function/Command
 * @param DataByte1 DataByte1
 * @param DataByte2 DataByte1
 * @param DataByte3 DataByte1
 * @param DataByte4 DataByte1
 * @param ByteArray ByteArray
 * @param Length ByteArray's Length
 */
void MODBUS_MASTER_MSG_RTU_commandUndetermined (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const Function,
    uint8_t const DataByte1,
    uint8_t const DataByte2,
    uint8_t const DataByte3,
    uint8_t const DataByte4,
    uint8_t const * const ByteArray,
    uint32_t const Length
    )
{
  assert ( xMODBUS_MASTER_MSG_RTU_IsCommandSecUndetermined(Function) );
  /* Set Meta */
  pDestMsgRTU->Meta.Section = Undetermined;
  pDestMsgRTU->Meta.Length =
    MODBUS_MASTER_MSG_RTU_SecUndetermined_MINLENGTH + Length;

  /* Set Raw Data */
  pDestMsgRTU->Data.Undetermined.SlaveAddress = SlaveAddress;
  pDestMsgRTU->Data.Undetermined.Function = Function;

  pDestMsgRTU->Data.Undetermined.DataByte1.DataByte1 = DataByte1;
  pDestMsgRTU->Data.Undetermined.DataByte2.DataByte2 = DataByte2;
  pDestMsgRTU->Data.Undetermined.DataByte3.DataByte3 = DataByte3;
  pDestMsgRTU->Data.Undetermined.DataByte4.DataByte4 = DataByte4;
  /* Set Byte Array */
  pDestMsgRTU->Data.Undetermined.ByteCount = Length;
  uint8_t * const pBytes =
    xMODBUS_MASTER_MSG_RTU_undeterminedGetBytesPointer(pDestMsgRTU);
  
  uint32_t i = 0;
  for (; i < Length; ++i)
    pBytes [i] = ByteArray [i];

  /* Crc */
  uint16_t const crc = MODBUS_MASTER_MSG_RTU_CrcCalculate (pDestMsgRTU);
  xMODBUS_MASTER_MSG_RTU_setErrorHi(pDestMsgRTU, xMODBUS_MSG_RTU_getByteHi(crc));
  xMODBUS_MASTER_MSG_RTU_setErrorLo(pDestMsgRTU, xMODBUS_MSG_RTU_getByteLo(crc));
}

/**
 * @brief: This Function generates a message for reading coils
 * @param SlaveAddress Address of Slave device
 * @param StartingAddressHi StartingAddressHi
 * @param StartingAddressLo StartingAddressLo
 * @param QuantityCoilsHi QuantityCoilsHi
 * @param QuantityCoilsLo QuantityCoilsLo
 */
void MODBUS_MASTER_MSG_RTU_commandReadCoils (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityCoilsHi,
    uint8_t const QuantityCoilsLo
    )
{
  MODBUS_MASTER_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_COILS,
      StartingAddressHi, StartingAddressLo,
      QuantityCoilsHi, QuantityCoilsLo);
}

/**
 * @brief: This Function generates a message for reading DiscreteInput
 * @param SlaveAddress Address of Slave device
 * @param StartingAddressHi StartingAddressHi
 * @param StartingAddressLo StartingAddressLo
 * @param QuantityInputsHi QuantityInputsHi
 * @param QuantityInputsLo QuantityInputsLo
 */
void MODBUS_MASTER_MSG_RTU_commandReadDiscreteInput (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityInputsHi,
    uint8_t const QuantityInputsLo
    )
{
  MODBUS_MASTER_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_DISCRETE_INPUT,
      StartingAddressHi, StartingAddressLo,
      QuantityInputsHi, QuantityInputsLo);
}

/**
 * @brief: This Function generates a message for reading Holding Registers
 * @param SlaveAddress Address of Slave device
 * @param StartingAddressHi StartingAddressHi
 * @param StartingAddressLo StartingAddressLo
 * @param QuantityRegistersHi QuantityRegistersHi
 * @param QuantityRegistersLo QuantityRegistersLo
 */
void MODBUS_MASTER_MSG_RTU_commandReadHoldingRegisters (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo
    )
{
  MODBUS_MASTER_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_HOLDING_REGISTERS,
      StartingAddressHi, StartingAddressLo,
      QuantityRegistersHi, QuantityRegistersLo);
}

/**
 * @brief: This Function generates a message for reading Input Registers
 * @param SlaveAddress Address of Slave device
 * @param StartingAddressHi StartingAddressHi
 * @param StartingAddressLo StartingAddressLo
 * @param QuantityRegistersHi QuantityRegistersHi
 * @param QuantityRegistersLo QuantityRegistersLo
 */
void MODBUS_MASTER_MSG_RTU_commandReadInputRegisters (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo
    )
{
  MODBUS_MASTER_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_READ_INPUT_REGISTERS,
      StartingAddressHi, StartingAddressLo,
      QuantityRegistersHi, QuantityRegistersLo);
}

/**
 * @brief: This Function generates a message for writing single coil
 * @param SlaveAddress Address of Slave device
 * @param CoilAddressHi CoilAddressHi
 * @param CoilAddressLo CoilAddressLo
 * @param WriteDataHi WriteDataHi
 * @param WriteDataLo WriteDataLo
 */
void MODBUS_MASTER_MSG_RTU_commandWriteSingleCoil (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    )
{
  MODBUS_MASTER_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_SINGLE_COIL,
      CoilAddressHi, CoilAddressLo,
      WriteDataHi, WriteDataLo);
}

/**
 * @brief: This Function generates a message for writing single register
 * @param SlaveAddress Address of Slave device
 * @param RegisterAddressHi RegisterAddressHi
 * @param RegisterAddressLo RegisterAddressLo
 * @param WriteDataHi WriteDataHi
 * @param WriteDataLo WriteDataLo
 */
void MODBUS_MASTER_MSG_RTU_commandWriteSingleRegister (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const RegisterAddressHi,
    uint8_t const RegisterAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    )
{
  MODBUS_MASTER_MSG_RTU_commandDetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_SINGLE_REGISTER,
      RegisterAddressHi, RegisterAddressLo,
      WriteDataHi, WriteDataLo);
}

/**
 * @brief: This Function generates a message for writing multiple coils
 * @param SlaveAddress Slave Address
 * @param CoilAddressHi CoilAddressHi
 * @param CoilAddressLo CoilAddressLo
 * @param QuantityCoilsHi QuantityCoilsHi
 * @param QuantityCoilsLo QuantityCoilsLo
 * @param WriteDataByteArray The bytes that will be send as writeData
 * @param Length Length of WriteDataByteArray
 */
void MODBUS_MASTER_MSG_RTU_commandWriteMultipleCoils (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const QuantityCoilsHi,
    uint8_t const QuantityCoilsLo,
    uint8_t const * const WriteDataByteArray,
    uint32_t const Length
    )
{
  MODBUS_MASTER_MSG_RTU_commandUndetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_MULTIPLE_COILS, CoilAddressHi, CoilAddressLo,
      QuantityCoilsHi, QuantityCoilsLo, WriteDataByteArray, Length);
}

/**
 */
void MODBUS_MASTER_MSG_RTU_commandWriteMultipleRegisters (
    MODBUS_MASTER_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const StartingAddressHi,
    uint8_t const StartingAddressLo,
    uint8_t const QuantityRegistersHi,
    uint8_t const QuantityRegistersLo,
    uint8_t const * const WriteDataByteArray,
    uint32_t const Length
    )
{
  MODBUS_MASTER_MSG_RTU_commandUndetermined (pDestMsgRTU, SlaveAddress,
      MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS, StartingAddressHi,
      StartingAddressLo,
      QuantityRegistersHi, QuantityRegistersLo, WriteDataByteArray, Length);
}

#ifdef __cplusplus /* This is here so that c++ compiler wont mess with overloading */
}
#endif
