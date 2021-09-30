#include "common.h"
#include "BleHidKeyboardCentral.h"
#include "UsbHidKeyboardPeripheral.h"


void setup() {
  
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
  #endif

  // UserSwitchをクリアボタンとして設定
  pinMode(7, INPUT_PULLUP);

  // BLE Central HID Serviceのセットアップ
  BleCentral.setup(keyboardReportCallback);

  // USB HID Keyboardのセットアップ
  UsbKeyboard.setup();
}


void loop() {

  // UserSwを3秒以上、押し続けたらペアリング情報を削除
  if (switchKeepsPressing()) {
    BleCentral.clearBonds();
  }

  delay(10);
}

// スイッチが一定時間以上、押し続けられているか判定する
bool switchKeepsPressing() {
  bool result = false;
  bool switchStatus = (digitalRead(7) == LOW);
  static bool prevSwitchStatus = false;
  static unsigned long startTime = 0;
  static bool keepPressing = false;

  if (switchStatus && !prevSwitchStatus) {
    startTime = millis();
    keepPressing = false;
  }

  if (switchStatus && prevSwitchStatus && !keepPressing) {
    if (millis() - startTime >= 3000) {
      result = true;

      // 連続して処理を行わないため、ボタンを押し続けた場合は無視する
      keepPressing = true;
    }
  }

  prevSwitchStatus = switchStatus;
  return result;
}

// 最後の入力を記録しておく
hid_keyboard_report_t lastReport = { 0 };

void keyboardReportCallback(hid_keyboard_report_t* report) {
  
  // 入力内容の変更をチェック
  if ( memcmp(&lastReport, report, sizeof(hid_keyboard_report_t)) ) {// TODO 必要？

    #ifdef SERIAL_DEBUG 
      Serial.print(report->modifier);Serial.print("  ");
      for(uint8_t i=0; i<6; i++) {
        Serial.print(report->keycode[i]);Serial.print(" ");
      }
      Serial.println(" ");
    #endif

    UsbKeyboard.report(report);
  }

  // 最後の入力内容を更新
  memcpy(&lastReport, report, sizeof(hid_keyboard_report_t));  
}
