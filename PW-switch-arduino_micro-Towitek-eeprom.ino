#include <EEPROM.h>

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;
//----------------------

//-------------pw2--------------
#include "Keyboard.h"
int i;
int z = 0;
int key[5];
String keyvalue;
int bytes_from_RFID = 5;
String Password_from_eeprom;
String Password1_from_eeprom = "12345678901234";
//---------------------------------


boolean eeprom_is_addr_ok(int addr) {
return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}
//----------------------
boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {
// counter
int i;
if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
return false;
}
for (i = 0; i < numBytes; i++) {
EEPROM.write(startAddr + i, array[i]);
}
return true;
}
//-----------------------------
boolean eeprom_write_string(int addr, const char* string) {

int numBytes; // actual number of bytes to be written

numBytes = strlen(string) + 1;
return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}
//------------------------------
boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
byte ch; // byte read from eeprom
int bytesRead; // number of bytes read so far
if (!eeprom_is_addr_ok(addr)) { // check start address
return false;
}

if (bufSize == 0) { // how can we store bytes in an empty buffer ?
return false;
}

if (bufSize == 1) {
buffer[0] = 0;
return true;
}
bytesRead = 0; // initialize byte counter
ch = EEPROM.read(addr + bytesRead); // read next byte from eeprom
buffer[bytesRead] = ch; // store it into the user buffer
bytesRead++; // increment byte counter

while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {
// if no stop condition is met, read the next byte from eeprom
ch = EEPROM.read(addr + bytesRead);
buffer[bytesRead] = ch; // store it into the user buffer
bytesRead++; // increment byte counter
}
// make sure the user buffer has a string terminator, (0x00) as its last byte
if ((ch != 0x00) && (bytesRead >= 1)) {
buffer[bytesRead - 1] = 0;
}
return true;
}
//-------------------------------------------
const int BUFSIZE = 15;
char buf[BUFSIZE];
char buf1[BUFSIZE];
String myString; 
String myString1 = "";
char myStringChar[BUFSIZE];
char myStringChar1[BUFSIZE];
//boolean passwort_eingabe = false;
boolean endlos = false;
unsigned long start = millis();
char ch ="";
//-----------------------------------------------------------------


void setup()
{
//------------------PW2---------------------
Serial1.begin(9600); // start serial to RFID reader
Keyboard.begin();  
//-----------------------------------------

Serial.begin(9600);

passwort_eingabe ();
//--------------------------------------
pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT); 

// Serial.println("Saving variable string to address 15");
//myString="12345678901234";
//myString.toCharArray(myStringChar, BUFSIZE); //convert string to char array
//strcpy(buf, myStringChar);
//12345678901234
//eeprom_write_string(15, buf); 

//Serial.print("Reading string from address 0: "); 
eeprom_read_string(0, buf, BUFSIZE);
Password_from_eeprom = buf;
//Serial.println(buf);
//Serial.print("Reading string from address 15: ");
//eeprom_read_string(15, buf1, BUFSIZE);
//Password1_from_eeprom = buf1;
//Serial.println(buf);

}



//---------------------------------------------------------------------
void loop()
{
 int sensorVal = digitalRead(2);

 if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }



  
//-------------PW2-------------------
if (Serial1.available() > 0)
 {
i = Serial1.read();
z ++;
// -------------6 = 5+1(bytes)
if (z < (bytes_from_RFID + 1) )
  {
Serial.print(i, DEC);
Serial.print(" ");
key[z] =i;
// -----------------5 bytes from RFID
if (z == bytes_from_RFID)
   {
 Serial.println();
 Serial1.end();

 keyvalue = (key[1] );
 keyvalue = (keyvalue + key[2]);
 keyvalue = (keyvalue + key[3]);
 keyvalue = (keyvalue + key[4]);
 keyvalue = (keyvalue + key[5]);
 if (keyvalue == "12409413828")
   {
//------- \n after password means Newline or ENTER
if (sensorVal == HIGH) {
    Keyboard.print(Password_from_eeprom +"\n");
  } else {
    Keyboard.print(Password1_from_eeprom +"\n");
  }
 
   }
// -----------------5 Seconds lock
 delay(5000);
//--------------------Thats not nice but it works.
 Serial1.begin(9600);
 
z=0;
   }
  }
 }
}

//------------------------------------


boolean passwort_eingabe()
{
// while( endlos == false)
while (millis() - start <= 15000)
 {
  myString1.reserve(16);
if( Serial.available())
 {   
   
ch = Serial.read();
if (ch != 10)
{
myString1 = (myString1 + ch);
//---------------------------Warum dieser delay?---Keine Ahnung---------------
delay(10);
}
    if (ch == 10)
   { 
      
      
       if (myString1 != "")
          {
            
          myString1.toCharArray(myStringChar1, BUFSIZE); //convert string to char array
          strcpy(buf, myStringChar1);
          eeprom_write_string(0, buf);
          break;
          }
       }
 }
 }
return true ;
}


