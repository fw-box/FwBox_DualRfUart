
#include "FwBox_DualRfUart.h"


FwBox_DualRfUart::FwBox_DualRfUart()
{
  FwBox_DualRfUart::SerialSensor = 0;
}

FwBox_DualRfUart::FwBox_DualRfUart(SoftwareSerial* serialSensor)
{
  FwBox_DualRfUart::SerialSensor = serialSensor;
}

FwBox_DualRfUart::~FwBox_DualRfUart()
{
}

void FwBox_DualRfUart::begin()
{
  FwBox_DualRfUart::SerialSensor = new SoftwareSerial(13, 15); // RX:D7, TX:D8
  FwBox_DualRfUart::SerialSensor->begin(9600);
}

RF_DATA_FORMAT* FwBox_DualRfUart::getData()
{
  return (RF_DATA_FORMAT*)&(FwBox_DualRfUart::RfData[0]);
}

void FwBox_DualRfUart::getAddressKey(char* buffer)
{
  RF_DATA_FORMAT* p_rf = FwBox_DualRfUart::getData();
  sprintf(buffer, "%02X%02X%02X", p_rf->ADDRESS2, p_rf->ADDRESS1, p_rf->KEY);
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
  int start_pos = currentIndex - 6;
  RF_DATA_FORMAT* p_rf = (RF_DATA_FORMAT*)(&(data[start_pos]));
  if((p_rf->SIG_FD == 0xfd) &&(p_rf->SIG_DF == 0xdf)) {
/*
    UnifiedLcd.setCursor(0, 0);
    if(p_rf->FREQ == 0xf3) {
      UnifiedLcd.print("315M ");
      Serial.print("315M ");
    }
    else if(p_rf->FREQ == 0xf4) {
      UnifiedLcd.print("433M ");
      Serial.print("433M ");
    }

    UnifiedLcd.print(p_rf->ADDRESS2, HEX);
    UnifiedLcd.print(" ");
    UnifiedLcd.print(p_rf->ADDRESS1, HEX);
    UnifiedLcd.print(" ");
    UnifiedLcd.print(p_rf->KEY, HEX);
    UnifiedLcd.print(" ");
    UnifiedLcd.print(p_rf->PARAM, HEX);
    UnifiedLcd.print("     ");

    Serial.print(p_rf->ADDRESS2, HEX);
    Serial.print(" ");
    Serial.print(p_rf->ADDRESS1, HEX);
    Serial.print(" ");
    Serial.print(p_rf->KEY, HEX);
    Serial.print(" ");
    Serial.println(p_rf->PARAM, HEX);
*/
    return true;
  }

  return false;
}
