#include <Arduino.h>
#include "UsbHidKeyboardPeripheral.h"


UsbHidKeyboardPeripheral UsbKeyboard;

uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()
};

void UsbHidKeyboardPeripheral::setup(void) {

  // USB HID Keyboardのセットアップ
  TinyUSBDevice.setProductDescriptor("Ble Keyboard Dongle");
  usbHid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
  usbHid.setPollInterval(POLLING_INTERVAL_MS);
  usbHid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usbHid.setReportCallback(NULL, NULL);
  usbHid.begin();
  
  while( !TinyUSBDevice.mounted() ) delay(1);  
}

void UsbHidKeyboardPeripheral::report(hid_keyboard_report_t* report) {

  // サスペンド中なら、デバイスを起こす
  if ( TinyUSBDevice.suspended() ) {
    TinyUSBDevice.remoteWakeup();
  }

  // QMK firmwareのS()関数などで複数キー同時押しを行うと、送信漏れが発生する場合がある
  // 前回の送信が終わっていない場合は、一度だけ送信を遅延する
  if ( !usbHid.ready() ) {
    delay(POLLING_INTERVAL_MS);
    if ( !usbHid.ready() ) {
      #ifdef SERIAL_DEBUG 
        Serial.println("not ready");
      #endif
      
      return;    
    }
  }

  usbHid.keyboardReport(REPORT_ID, report->modifier, report->keycode);
}
