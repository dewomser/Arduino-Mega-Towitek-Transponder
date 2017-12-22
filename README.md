# Arduino-Mega-Towitek-Transponder
How to communicate with TWT 2021  and Arduino Mega und Micro

Arduino Micro is in keyboard-mode.
Passwordsafe is Arduino. No Server needed.

Wiring Transponder: red -> + (5 or 3.3V) | blue -> GND | yellow -> RX

## password2-switch...
Transponder Code shown in  serial Monitor.
Input Pin2 :PW can be switched
PW-length=15 bytes 
PW-store EEPROM Addr 0
Password is changeable within first 20 seconds after powersupply connected via Arduino serial Monitor or bash: echo "secretPW123" > /dev/ttyACM0
Blogpost: https://untergang.de/index.php/linux-blog/passwortmanager-mit-arduino-2.html
