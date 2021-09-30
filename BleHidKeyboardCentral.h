#include "common.h"
#include <bluefruit.h>

class BleHidKeyboardCentral {
  public:
    void setup(BLEClientHidAdafruit::kbd_callback_t kbdCallback);
    void clearBonds(void);

  private:
    BLEClientHidAdafruit bleHid;
    uint16_t connHandle;

    friend void scanCallback(ble_gap_evt_adv_report_t* report);
    friend void connectCallback(uint16_t conn_handle);
    friend void connectionSecuredCallback(uint16_t conn_handle);
    friend void disconnectCallback(uint16_t conn_handle, uint8_t reason);
    
};

extern BleHidKeyboardCentral BleCentral;
