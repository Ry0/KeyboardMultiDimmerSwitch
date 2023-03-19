#include <hidboot.h>
#include <usbhub.h>
#include "hap/hap_manager.h"
#include "system/system_util.h"
#include "wifi_info.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#define LED 2

USB Usb;
// USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

system_util Sys;

hap_manager Hap;

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown(uint8_t mod, uint8_t key);
    void OnKeyUp(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key){
      MODIFIERKEYS mod;
      *((uint8_t*)&mod) = m;
      Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
      Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
      Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
      Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

      Serial.print(" >");
      PrintHex<uint8_t>(key, 0x80);
      Serial.print("< ");

      Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
      Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
      Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
      Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
      Serial.print("DN ");
      PrintKey(mod, key);
      uint8_t c = OemToAscii(mod, key);

      if (c)
        OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{

      MODIFIERKEYS beforeMod;
      *((uint8_t*)&beforeMod) = before;

      MODIFIERKEYS afterMod;
      *((uint8_t*)&afterMod) = after;

      if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
        Serial.println("LeftCtrl changed");
      }
      if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
        Serial.println("LeftShift changed");
      }
      if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
        Serial.println("LeftAlt changed");
      }
      if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
        Serial.println("LeftGUI changed");
      }

      if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
        Serial.println("RightCtrl changed");
      }
      if (beforeMod.bmRightShift != afterMod.bmRightShift) {
        Serial.println("RightShift changed");
      }
      if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
        Serial.println("RightAlt changed");
      }
      if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
        Serial.println("RightGUI changed");
      }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
      Serial.print("UP ");
      PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
    Serial.print("ASCII: ");
    Serial.println((char)key);
    digitalWrite(LED, LOW);
    switch ((char)key) {
    case '1':
        Serial.printf("execute button (id = 0)\n");
        Hap.button_callback(0, button_event_single_press);
        break;
    case '2':
        Serial.printf("execute button (id = 2)\n");
        Hap.button_callback(1, button_event_single_press);
        break;
    case '3':
        Serial.printf("execute button (id = 3)\n");
        Hap.button_callback(2, button_event_single_press);
        break;
    case '4':
        Serial.printf("execute button (id = 4)\n");
        Hap.button_callback(3, button_event_single_press);
        break;
    case '5':
        Serial.printf("execute button (id = 5)\n");
        Hap.button_callback(4, button_event_single_press);
        break;
    case '6':
        Serial.printf("execute button (id = 6)\n");
        Hap.button_callback(5, button_event_single_press);
        break;
    case '7':
        Serial.printf("execute button (id = 7)\n");
        Hap.button_callback(6, button_event_single_press);
        break;
    case '8':
        Serial.printf("execute button (id = 8)\n");
        Hap.button_callback(7, button_event_single_press);
        break;
    default:
        Serial.printf("Not allocated.\n");
        break;
    }
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
};


KbdRptParser Prs;

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);   // Set pin mode
    delay(1000);

#if !defined(__MIPSEL__)
    while (!Serial)
        ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
    
    Serial.println("Start");
    
    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay(200);

    HidKeyboard.SetReportParser(0, &Prs);

    // Try to connect wifi.
    Sys.connect_wifi(ssid, password);

    digitalWrite(LED, HIGH);

    // Associating GUI with HAP
    Hap.initialize(8, false);
}

void loop()
{
    Usb.Task();
}