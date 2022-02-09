
#ifndef __FWBOX_DUALRFUART__
#define __FWBOX_DUALRFUART__


#include <Arduino.h>
#include "SoftwareSerial.h"
#include "HardwareSerial.h"

#define RF_UART_SINGLE 1
#define RF_UART_DUAL 2

#define DUAL_RF_UART_FREQ_315M 0xf3
#define DUAL_RF_UART_FREQ_433M 0xf4

#define MAX_RF_DATA_SIZE 16

#define SERIAL_TYPE_SOFTWARE 1
#define SERIAL_TYPE_HARDWARE 2

#pragma pack(push)
#pragma pack(1)
struct RF_DATA_FORMAT_SINGLE {
    uint8_t SIG_FD;
    uint8_t ADDRESS2;
    uint8_t ADDRESS1;
    uint8_t KEY;
    uint8_t PARAM;
    uint8_t SIG_DF;
};
struct RF_DATA_FORMAT_DUAL {
    uint8_t SIG_FD;
    uint8_t FREQ;
    uint8_t ADDRESS2;
    uint8_t ADDRESS1;
    uint8_t KEY;
    uint8_t PARAM;
    uint8_t SIG_DF;
};
#pragma pack(pop)

  class FwBox_DualRfUart {
    private:
      Stream* SerialSensor;
      char RfData[MAX_RF_DATA_SIZE];
      int DataIndex = 0;
      int SerialType = SERIAL_TYPE_SOFTWARE;
      int SingleDual = RF_UART_SINGLE;
    public:
      FwBox_DualRfUart();
      FwBox_DualRfUart(SoftwareSerial* serialSensor);
      FwBox_DualRfUart(HardwareSerial* serialSensor);
      ~FwBox_DualRfUart();
      void setSingleDual(int singleDual);
      void begin();
      void begin(const int pinRx, const int pinTx);
      bool parseData(char* data, int currentIndex);
      bool handle();
      RF_DATA_FORMAT_SINGLE* getDataSingle();
      RF_DATA_FORMAT_DUAL* getDataDual();
      void getAddressKey(char* buffer);
  };

#endif // __FWBOX_DUALRFUART__
