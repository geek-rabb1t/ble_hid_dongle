#include "common.h"
#include "Adafruit_TinyUSB.h"

class UsbHidKeyboardPeripheral {
  public:
    void setup(void);
    void report(hid_keyboard_report_t* report);
    

  private:
    Adafruit_USBD_HID usbHid;
    static const uint8_t REPORT_ID = 0;
    static const uint8_t POLLING_INTERVAL_MS = 2;
    
};

extern UsbHidKeyboardPeripheral UsbKeyboard;
