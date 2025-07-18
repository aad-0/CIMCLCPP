#include <stdio.h>
#include <stdlib.h>
#include "modbus_general.h"
#include "modbus_master.h"
#include "modbus_slave.h"
#include <string.h>

#define reset(X) memset( ((X)), 0x00, sizeof(*X))

int modbus_master_test  ();
int modbus_slave_test ();

int
main (void)
{
  fprintf(stdout, "Size: %p %p", (void *)sizeof(MODBUS_MASTER_MSG_RTU_TypeDef),
      (void *)sizeof(MODBUS_SLAVE_MSG_RTU_TypeDef));

  fprintf(stdout, "\n################### master ##########\n");
  modbus_master_test();
  fprintf(stdout, "\n################### master ##########\n");
  fprintf(stdout, "\n################### slave ##########\n");
  modbus_slave_test();
  fprintf(stdout, "\n################### slave ##########\n");
  return 0;
}
 
int
modbus_master_test()
{
  uint16_t err = 0;
  int32_t ret;

  MODBUS_MASTER_MSG_RTU_TypeDef msg = {0};
#if 0
  msg.Meta.Length = MODBUS_MASTER_MSG_RTU_SecDetermined_LENGTH;
  msg.Meta.Section = Determined;

  msg.Data.Determined.SlaveAddress = 0x04;
  msg.Data.Determined.Function = 0x01;
  msg.Data.Determined.DataByte1.StartingAddressHi = 0x00;
  msg.Data.Determined.DataByte2.StartingAddressLo = 0x0a;
  msg.Data.Determined.DataByte3.QuantityCoilsHi = 0x00;
  msg.Data.Determined.DataByte4.QuantityCoilsLo = 0x0d;

  err = MODBUS_MASTER_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_MASTER_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_MASTER_MSG_RTU_TypeDefLength(&msg));


  msg.Meta.Section = Undetermined;
  msg.Meta.Length = MODBUS_MASTER_MSG_RTU_SecUndetermined_MINLENGTH + 4;
  msg.Data.Undetermined.SlaveAddress = 0x11;
  msg.Data.Undetermined.Function = 0x10;
  msg.Data.Undetermined.DataByte1.DataByte1 = 0x00;
  msg.Data.Undetermined.DataByte2.DataByte2 = 0x01;
  msg.Data.Undetermined.DataByte3.DataByte3 = 0x00;
  msg.Data.Undetermined.DataByte4.DataByte4 = 0x02;
  msg.Data.Undetermined.ByteCount = 0x04;

  xMODBUS_MASTER_MSG_RTU_undeterminedSetByteHi(&msg, 0, 0x00);
  xMODBUS_MASTER_MSG_RTU_undeterminedSetByteLo(&msg, 0, 0x0A);

  xMODBUS_MASTER_MSG_RTU_undeterminedSetByteHi(&msg, 1, 0x01);
  xMODBUS_MASTER_MSG_RTU_undeterminedSetByteLo(&msg, 1, 0x02);

  xMODBUS_MASTER_MSG_RTU_setErrorHi(&msg, 0xF0);
  xMODBUS_MASTER_MSG_RTU_setErrorLo(&msg, 0xC6);

  err = MODBUS_MASTER_MSG_RTU_CrcCalculate (&msg);

  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_MASTER_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_MASTER_MSG_RTU_TypeDefLength(&msg));
#endif

  reset(&msg);
  MODBUS_MASTER_MSG_RTU_commandReadCoils(&msg, 04, 00, 0x0a, 00, 0x0d);

  err = MODBUS_MASTER_MSG_RTU_CrcCalculate (&msg);

  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_MASTER_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_MASTER_MSG_RTU_TypeDefLength(&msg));

  reset(&msg);
  uint8_t arr [] = {0xcd, 0x01};
  MODBUS_MASTER_MSG_RTU_commandWriteMultipleCoils (&msg, 0x11, 0x00, 0x13, 0x00, 0x0a, arr, 2);
  err = MODBUS_MASTER_MSG_RTU_CrcCalculate (&msg);

  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_MASTER_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_MASTER_MSG_RTU_TypeDefLength(&msg));

  reset(&msg);
  uint8_t raw1 [] = {0x04, 0x01, 0x00, 0x0a, 0x00, 0x0d, 0xdd, 0x98};
  ret = MODBUS_MASTER_MSG_RTU_Raw2TypeDef (&msg, raw1, 11);

  err = MODBUS_MASTER_MSG_RTU_CrcCalculate (&msg);

  fprintf (stdout, "Ret, %d\n", ret);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_MASTER_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_MASTER_MSG_RTU_TypeDefLength(&msg));
  reset(&msg);

  uint8_t raw2 [] = {0x11, 0x0f, 0x00, 0x13, 0x00, 0x0a, 0x02, 0xcd, 0x01, 0xbf, 0x0b};
  ret = MODBUS_MASTER_MSG_RTU_Raw2TypeDef (&msg, raw2, 11);

  err = MODBUS_MASTER_MSG_RTU_CrcCalculate (&msg);

  fprintf (stdout, "Ret, %d\n", ret);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_MASTER_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_MASTER_MSG_RTU_TypeDefLength(&msg));

  return 0;
}

int
modbus_slave_test ()
{
  uint16_t err = 0;
  int32_t ret;

  MODBUS_SLAVE_MSG_RTU_TypeDef msg = {0};
#if 0
  msg.Meta.Length = MODBUS_SLAVE_MSG_RTU_SecDetermined_LENGTH;
  msg.Meta.Section = Determined;

  msg.Data.Determined.SlaveAddress = 0x11;
  msg.Data.Determined.Function = 0x05;
  msg.Data.Determined.DataByte1.StartingAddressHi = 0x00;
  msg.Data.Determined.DataByte2.StartingAddressLo = 0xac;
  msg.Data.Determined.DataByte3.QuantityCoilsHi = 0xff;
  msg.Data.Determined.DataByte4.QuantityCoilsLo = 0x00;

  xMODBUS_MSG_RTU_setErrorHi (&msg, 0x8B);
  xMODBUS_MSG_RTU_setErrorLo (&msg, 0x4e);

  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_SLAVE_MSG_RTU_TypeDefLength(&msg));

  reset(&msg);
  msg.Meta.Length = MODBUS_SLAVE_MSG_RTU_SecDetermined_LENGTH;
  msg.Meta.Section = Undetermined;

  msg.Data.Undetermined.SlaveAddress = 0x04;
  msg.Data.Undetermined.Function = 0x01;
  msg.Data.Undetermined.ByteCount = 0x02;
  xMODBUS_MSG_RTU_undeterminedSetByteHi(&msg, 0, 0x0A);
  xMODBUS_MSG_RTU_undeterminedSetByteLo(&msg, 0, 0x11);

  xMODBUS_MSG_RTU_setErrorHi (&msg, 0x50);
  xMODBUS_MSG_RTU_setErrorLo (&msg, 0xb3);

  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_SLAVE_MSG_RTU_TypeDefLength(&msg));


  reset(&msg);
  msg.Meta.Length = MODBUS_SLAVE_MSG_RTU_SecException_LENGTH;
  msg.Meta.Section = Exception;

  msg.Data.Exception.SlaveAddress = 0x0A;
  msg.Data.Exception.Function = 0x01 | 0x80;
  msg.Data.Exception.ExceptionCode = 0x02;

  xMODBUS_MSG_RTU_setErrorHi (&msg, 0x53);
  xMODBUS_MSG_RTU_setErrorLo (&msg, 0xb0);

  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  fprintf(stdout, "Length %d\n", MODBUS_SLAVE_MSG_RTU_TypeDefLength(&msg));
#endif

  reset(&msg);
  MODBUS_SLAVE_MSG_RTU_commandException(&msg, 0xA, 0x01, 0x02);

  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  reset(&msg);
  /*
void MODBUS_SLAVE_MSG_RTU_commandWriteSingleCoil (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const CoilAddressHi,
    uint8_t const CoilAddressLo,
    uint8_t const WriteDataHi,
    uint8_t const WriteDataLo
    );
   */
  MODBUS_SLAVE_MSG_RTU_commandWriteSingleCoil(&msg, 0x11, 0, 0xAC, 0xff, 0x00);

  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));
  /*
void MODBUS_SLAVE_MSG_RTU_commandReadCoils (
    MODBUS_SLAVE_MSG_RTU_TypeDef * const pDestMsgRTU,
    uint8_t const SlaveAddress,
    uint8_t const * const DataByteArray,
    uint32_t const Length
    );
   */
  uint8_t arr [] = {0x0A, 0x11};
  MODBUS_SLAVE_MSG_RTU_commandReadCoils(&msg, 0x04, arr, 0x02);

  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  reset(&msg);
  uint8_t raw1 [] = {0x11, 0x0f, 0x00, 0x13, 0x00, 0x0a, 0x02, 0xcd, 0x01, 0xbf, 0x0b};
  ret = MODBUS_SLAVE_MSG_RTU_Raw2TypeDef(&msg, raw1, 11);
  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Ret, %d\n", ret);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  reset(&msg);
  uint8_t raw2 [] = {0x04, 01, 02, 0x0a, 0x11, 0xb3, 0x50};
  ret = MODBUS_SLAVE_MSG_RTU_Raw2TypeDef(&msg, raw2, 11);
  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Ret, %d\n", ret);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  reset(&msg);
  uint8_t raw3 [] = {0x0a, 0x81, 0x02, 0xb0, 0x53};
  ret = MODBUS_SLAVE_MSG_RTU_Raw2TypeDef(&msg, raw3, 5);
  err = MODBUS_SLAVE_MSG_RTU_CrcCalculate (&msg);
  fprintf (stdout, "Ret, %d\n", ret);
  fprintf (stdout, "Err, %X\n", err);
  fprintf (stdout, "Val: %d\n", MODBUS_SLAVE_MSG_RTU_CrcValidate(&msg));

  return 0;
}
