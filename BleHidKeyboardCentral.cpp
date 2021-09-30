#include "BleHidKeyboardCentral.h"


BleHidKeyboardCentral BleCentral;

void scanCallback(ble_gap_evt_adv_report_t* report) {
  Bluefruit.Central.connect(report);
}

void connectCallback(uint16_t conn_handle) {
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  #ifdef SERIAL_DEBUG 
    Serial.println("Connected");
    Serial.print("Discovering HID  Service ... ");
  #endif

  if ( BleCentral.bleHid.discover(conn_handle) ) {
    #ifdef SERIAL_DEBUG 
      Serial.println("Found it");
    #endif

    // ペアリングする
    conn->requestPairing();

  } else {
    #ifdef SERIAL_DEBUG 
      Serial.println("Found NONE");
    #endif
    
    // HIDサービスが見つからない場合は切断
    conn->disconnect();
  }
}

void connectionSecuredCallback(uint16_t conn_handle) {
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if ( !conn->secured() ) {
    conn->requestPairing();
    return;
  }

  BleCentral.connHandle = conn_handle;
  BleCentral.bleHid.setBootMode(true);

  if ( BleCentral.bleHid.keyboardPresent() ) BleCentral.bleHid.enableKeyboard();

  #ifdef SERIAL_DEBUG 
    Serial.println("Secured");

    uint8_t hidInfo[4];
    BleCentral.bleHid.getHidInfo(hidInfo);

    Serial.printf("HID version: %d.%d\n", hidInfo[0], hidInfo[1]);
    Serial.print("Country code: "); Serial.println(hidInfo[2]);
    Serial.printf("HID Flags  : 0x%02X\n", hidInfo[3]);
    Serial.println("Ready to receive from peripheral");
  #endif
  
}

void disconnectCallback(uint16_t conn_handle, uint8_t reason) {

  #ifdef SERIAL_DEBUG 
    Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
  #endif      
}


void BleHidKeyboardCentral::setup(BLEClientHidAdafruit::kbd_callback_t kbdCallback) {

  Bluefruit.begin(0, 1);
  Bluefruit.setName("BLE Keyboard dongle");

  // BLE Central Hid Serivceの初期化
  bleHid.begin();
  bleHid.setKeyboardReportCallback(kbdCallback);

  // デバイスが接続されていない場合はLEDを点滅させる。
  Bluefruit.setConnLedInterval(250);

  // Callbackの設定
  Bluefruit.Central.setConnectCallback(connectCallback);
  Bluefruit.Central.setDisconnectCallback(disconnectCallback);
  Bluefruit.Security.setSecuredCallback(connectionSecuredCallback);

  // BLEデバイスのスキャンを開始
  Bluefruit.Scanner.setRxCallback(scanCallback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80);
  Bluefruit.Scanner.filterService(bleHid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);

  connHandle = 0; 
};


void BleHidKeyboardCentral::clearBonds(void) {

  #ifdef SERIAL_DEBUG 
    Serial.println("clear bonds");
  #endif

  if (Bluefruit.connected() != 0) {
    BLEConnection* conn = Bluefruit.Connection(connHandle);
    conn->disconnect();
  }
  Bluefruit.Central.clearBonds();
};
