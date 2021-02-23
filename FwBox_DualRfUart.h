
#ifndef __FWBOX_DUALRFUART__
#define __FWBOX_DUALRFUART__


#include <Arduino.h>
#include "SoftwareSerial.h"

#define DUAL_RF_UART_FREQ_315M 0xf3
#define DUAL_RF_UART_FREQ_433M 0xf4

#define MAX_RF_DATA_SIZE 16

#define MAX_RF_DATA_SIZE 16

#pragma pack(push)
#pragma pack(1)
struct RF_DATA_FORMAT {
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
      SoftwareSerial* SerialSensor;
      char RfData[MAX_RF_DATA_SIZE];
      int DataIndex = 0;
    public:
      FwBox_DualRfUart();
      FwBox_DualRfUart(SoftwareSerial* serialSensor);
      ~FwBox_DualRfUart();
      void begin();
      bool parseData(char* data, int currentIndex);
      bool handle();
      RF_DATA_FORMAT* getData();
      void getAddressKey(char* buffer);
  };

#endif // __FWBOX_DUALRFUART__
