#include <EEPROM.h>

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;
const int BUFSIZE = 15;

//-------------pw2--------------
#include <Keyboard.h>

int i;
int z = 0;
int key[5];
String keyvalue;
int bytes_from_RFID = 5;
String Password_from_eeprom;

// Eingänge
int Taster = 2;

//Ausgänge
int LEDboard = 13;
int LEDtaster = 12;
int LED1 = 11;
int LED2 = 10;
int LED3 = 9;

//allg, Variablen

char buf[BUFSIZE];
char buf1[BUFSIZE];
String myString;
String myString1 = "";
char myStringChar[BUFSIZE];
char myStringChar1[BUFSIZE];
//boolean passwort_eingabe = false;
boolean endlos = false;
unsigned long start = millis();
char ch = "";

//zaehler LED

int buttonState = 0;
int counter = 0;
int buttonStateHIGH = 0;

//--------------Funktionen-----------------------


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

boolean passwort_eingabe()
{

  //in 30 Sekunden sollte eine Verbindung aufgebaut sein
  //wenn nicht, dann Betrieb mit Netzteil
  //       delay (30000);
  // while( endlos == false)

  myString1.reserve(16);


  while (millis() - start <= 30000)
  {

    if ( Serial.available())
    {

      ch = Serial.read();

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
  return true ;
}


void setup() {

  pinMode(Taster, INPUT_PULLUP);

  pinMode(LEDboard, OUTPUT);
  pinMode(LEDtaster, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);



  //------------------PW2---------------------
  Serial1.begin(9600); // start serial to RFID reader
  Keyboard.begin();
  //-----------------------------------------

  Serial.begin(9600);

  passwort_eingabe ();
  //--------------------------------------

  eeprom_read_string(0, buf, BUFSIZE);
  Password_from_eeprom = buf;

}

void loop() {

  int wertTaster = digitalRead(Taster);

  if (wertTaster == HIGH) {
    digitalWrite(LEDboard, LOW);
    //digitalWrite(LED1,HIGH);
    //digitalWrite(LED2,HIGH);
    //digitalWrite(LED3,HIGH);
    //digitalWrite(LEDtaster,HIGH);

  } else {
    digitalWrite(LEDboard, HIGH);
  }






  buttonState = digitalRead(Taster);
  if (buttonState == HIGH)
  {
    buttonStateHIGH = 1;

  }
  if (buttonState == LOW && buttonStateHIGH == 1)
  {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    counter++;
    buttonStateHIGH = 0;
  }
  digitalWrite(counter + 7, HIGH);
  if (counter == 4)
  {
    counter = 0;

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
      key[z] = i;
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

        if (keyvalue == "1173154251101")
          Keyboard.print(Password_from_eeprom);
        else if (keyvalue == "12409413828")
        {
          digitalWrite(12, HIGH);

          //------- \n after password means Newline or ENTER
          if (counter == 0)
          {
            Keyboard.print("Passwort 2\n");
          }
          else if (counter == 1)
          {
            Keyboard.print("Passwort 1\n");
           // Keyboard.print(Password_from_eeprom);
          }


          else if (counter == 2)
          {
            Keyboard.print("Passwort 3\n");
          }


          else if (counter == 3)
          {
            Keyboard.print("Passwort 4\n");
          }

        }
        // -----------------5 Seconds lock
        delay(5000);
        digitalWrite(12, LOW);
        //--------------------Thats not nice but it works.
        Serial1.begin(9600);

        z = 0;
      }
    }
  }
}


