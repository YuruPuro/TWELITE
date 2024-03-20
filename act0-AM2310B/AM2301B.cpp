#include <TWELITE>
#include <NWK_SIMPLE>// ネットワークサポート
#include "bitmapdata.h"
#include "AM2301B.h"

static uint8_t AM2301BADDR = 0x38 ; // 0111000

// 初期化
void AM2301B::init() {
}

bool AM2301B::readSencer( ) {
  bool result = true ;
  uint8_t stat = 0 ;
  uint8_t readData[8] ;
  int len = 0 ;
  
 if (auto && wrt = Wire.get_writer(AM2301BADDR)) {
    wrt << 0x71;
  } else {
    return false ;
  }
  if (auto&& rdr = Wire.get_reader(AM2301BADDR, 1)) {
    rdr >> stat;
  } else {
    return false ;
  }
  if (stat != 0x18) { // BUSY FLAG Check
    Serial << "stat " << (int)stat << crlf ; 
    if (auto && wrt = Wire.get_writer(AM2301BADDR)) {
        wrt << 0xBA;
        delay(100) ;
    }
    return false ;
  }

 if (auto && wrt = Wire.get_writer(AM2301BADDR)) {
    wrt << 0xAC;
    wrt << 0x33;
    wrt << 0x00;
  } else {
    return false ;
 }
  delay(80); // wait SecnserRead

  if (auto&& rdr = Wire.get_reader(AM2301BADDR, 7)) {
    for (int i=0;i<7;i++) {
      rdr >> readData[i];
    }
  } else {
    return false ;
  }

  // ----- CRC8 -----CRC[7:0]= 1 + x^4 + x^5 + x^8  : B1 0011 0001
  uint8_t crc = 0xFF;
  for (int i = 0; i < 6 ; i++) {
    uint8_t b = readData[i];
    crc ^= b;
    for (int x = 0; x < 8; x++) {
      if (crc & 0x80) {
        crc <<= 1;
        crc ^= 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
 
  uint32_t t1,t2,t3 ;
  t1 = readData[1] ; t2 = readData[2] ; t3 = readData[3] & 0xF0 ;
  uint32_t h = t1 << 12 | t2 << 4 | t3 >> 4 ; 

  t1 = readData[3] & 0x0F; t2 = readData[4] ; t3 = readData[5] ;
  uint32_t t = t1 << 16 | t2 << 8 | t3 ;

  // ----- 取得データ格納 -----
  humidity = (h * 100 ) >> 20 ;
  temperature = (( t * 200) >> 18) / 4. - 50;
  uint8_t cr = readData[6] ;

  result = (cr == crc) ? true : false ;
  Serial << "AM2301B result " << result ; 
  Serial << "  humidity " << (int)humidity  ; 
  Serial << "  temperature " << (float)temperature << crlf ; 

  return result ;
}
