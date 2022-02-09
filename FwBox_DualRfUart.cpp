
#include "FwBox_DualRfUart.h"


FwBox_DualRfUart::FwBox_DualRfUart()
{
  //
  // Default : SoftwareSerial
  //
  FwBox_DualRfUart::SerialSensor = 0;
  FwBox_DualRfUart::SerialType = SERIAL_TYPE_SOFTWARE;

  FwBox_DualRfUart::SingleDual = RF_UART_SINGLE;
}

FwBox_DualRfUart::FwBox_DualRfUart(SoftwareSerial* serialSensor)
{
  FwBox_DualRfUart::SerialSensor = serialSensor;
  FwBox_DualRfUart::SerialType = SERIAL_TYPE_SOFTWARE;

  FwBox_DualRfUart::SingleDual = RF_UART_SINGLE;
}

FwBox_DualRfUart::FwBox_DualRfUart(HardwareSerial* serialSensor)
{
  FwBox_DualRfUart::SerialSensor = serialSensor;
  FwBox_DualRfUart::SerialType = SERIAL_TYPE_HARDWARE;

  FwBox_DualRfUart::SingleDual = RF_UART_SINGLE;
}

FwBox_DualRfUart::~FwBox_DualRfUart()
{
}

void FwBox_DualRfUart::setSingleDual(int singleDual)
{
  FwBox_DualRfUart::SingleDual = singleDual;
}

void FwBox_DualRfUart::begin()
{
  if (FwBox_DualRfUart::SerialSensor == 0) {
    //
    // Default : SoftwareSerial
    //
    FwBox_DualRfUart::SerialType = SERIAL_TYPE_SOFTWARE;
    FwBox_DualRfUart::SerialSensor = new SoftwareSerial(13, 15); // RX:D7, TX:D8
    SoftwareSerial* pss = (SoftwareSerial*)(FwBox_DualRfUart::SerialSensor);
    pss->begin(9600);
  }
  else {
    if (FwBox_DualRfUart::SerialType == SERIAL_TYPE_SOFTWARE) {
      SoftwareSerial* pss = (SoftwareSerial*)(FwBox_DualRfUart::SerialSensor);
      pss->begin(9600);
    }
    else {
      HardwareSerial* pss = (HardwareSerial*)(FwBox_DualRfUart::SerialSensor);
      pss->begin(9600);
    }
  }
}

void FwBox_DualRfUart::begin(const int pinRx, const int pinTx)
{
  //
  // Default : SoftwareSerial
  //
  FwBox_DualRfUart::SerialType = SERIAL_TYPE_SOFTWARE;
  FwBox_DualRfUart::SerialSensor = new SoftwareSerial(pinRx, pinTx);
  SoftwareSerial* pss = (SoftwareSerial*)(FwBox_DualRfUart::SerialSensor);
  pss->begin(9600);
}

RF_DATA_FORMAT_SINGLE* FwBox_DualRfUart::getDataSingle()
{
  return (RF_DATA_FORMAT_SINGLE*)&(FwBox_DualRfUart::RfData[0]);
}

RF_DATA_FORMAT_DUAL* FwBox_DualRfUart::getDataDual()
{
  return (RF_DATA_FORMAT_DUAL*)&(FwBox_DualRfUart::RfData[0]);
}

void FwBox_DualRfUart::getAddressKey(char* buffer)
{
  if (FwBox_DualRfUart::SingleDual == RF_UART_SINGLE) {
    RF_DATA_FORMAT_SINGLE* p_rf = FwBox_DualRfUart::getDataSingle();
    sprintf(buffer, "%02X%02X%02X", p_rf->ADDRESS2, p_rf->ADDRESS1, p_rf->KEY);
  }
  else {
    RF_DATA_FORMAT_DUAL* p_rf = FwBox_DualRfUart::getDataDual();
    sprintf(buffer, "%02X%02X%02X", p_rf->ADDRESS2, p_rf->ADDRESS1, p_rf->KEY);
  }
}

bool FwBox_DualRfUart::handle()
{
  if(FwBox_DualRfUart::SerialSensor == 0) {
    return false;
  }

  while(FwBox_DualRfUart::SerialSensor->available()) {
    char c = FwBox_DualRfUart::SerialSensor->read();
    RfData[DataIndex] = c;

    //Serial.print(c, HEX);
    //Serial.print(' ');
    
    if(c == 0xdf) {
      if(parseData(&(RfData[0]), DataIndex) == true) {
        DataIndex = 0;
        //Serial.print("<true>\r\n");
        return true;
      }
    }

    DataIndex++;

    if(DataIndex >= MAX_RF_DATA_SIZE)
      DataIndex = 0;
  }

  //Serial.print("<false>\r\n");
  return false;
}

//
// Data format :
//
//     ***RF_UART_SINGLE***
//     Offset  Data  Description
//     0       0xfd  header
//     2             Address 2
//     3             Address 1
//     4             Key
//     5             Signal Width
//     6       0xdf  Tailer
//
//     ***RF_UART_DUAL***
//     Offset  Data  Description
//     0       0xfd  header
//     1             Frequency - 0xf3:315M, 0xf4:433M
//     2             Address 2
//     3             Address 1
//     4             Key
//     5             Signal Width
//     6       0xdf  Tailer
//
bool FwBox_DualRfUart::parseData(char* data, int currentIndex)
{
  int start_pos = currentIndex - 5;

  if (FwBox_DualRfUart::SingleDual == RF_UART_SINGLE)
    start_pos = currentIndex - 5;
  else
    start_pos = currentIndex - 6;

  if (start_pos < 0)
    return false;

  if (FwBox_DualRfUart::SingleDual == RF_UART_SINGLE) {
    RF_DATA_FORMAT_SINGLE* p_rf = (RF_DATA_FORMAT_SINGLE*)(&(data[start_pos]));
    if((p_rf->SIG_FD == 0xfd) &&(p_rf->SIG_DF == 0xdf)) {
      return true;
    }
  }
  else {
    RF_DATA_FORMAT_DUAL* p_rf = (RF_DATA_FORMAT_DUAL*)(&(data[start_pos]));
    if((p_rf->SIG_FD == 0xfd) &&(p_rf->SIG_DF == 0xdf)) {
      return true;
    }
  }

  return false;
}
