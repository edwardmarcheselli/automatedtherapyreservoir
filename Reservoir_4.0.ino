#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Adafruit_MAX31855.h>

//NOTE
//all protocols are built around the same type of function structure
//each protocol for cooling, heating, and compression runs a while loop until the timer set by the user runsout
//each protocol has the ability to stop the session at any time or pause and resume the session

//reservoir cooling is the only protocol that runs a loop that is based on a sensor value and not a timer

//COMPONENTS
const int heater1 = 5;
const int peltier = 4;
const int pump1_in = 10;  //Agitator
const int pump1_out = 9; //Agitator
const int pump2_in = 11;  //Sleeve Pump
const int pump2_out = 6;  //Sleeve Pump
const int fan_in = 7;
const int fan_out = 8;

//Sleeve Heat Sensor Pin
#define MAXDO   A4
#define MAXCS   A3
#define MAXCLK  A2

//Reservoir Heat Sensor Pin
#define ONE_WIRE_BUS A5

//Sensor Initialization Setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

//VARIABLES
//int com = 0;
int timer = 0;                //time sent by user for therapy session time limit
int water_temp = 0;           //the reservoir water temp level limit
int heat_level = 0;           //the sleeve heat level limit
int level = 0;                //analog value when incrementing pump rate to reach compression level
int comp_level = 0;           //the compression level limit
unsigned long ntime = 0;      //starts the new time relative to the CPU clock
unsigned long current = 0;    //stores the current time of a session
unsigned long adjust = 0;     //stores the time relative to the CPU clock

int test1 = 0;
int test2 = 0;
int code = 0;                 //codes sent from the GUI to the arduino code to communicate which functions to run

int fsrPin = A0;              // the FSR and 10K pulldown are connected to a0
int fsrReading;               // the analog reading from the FSR resistor divider
int fsrVoltage;               // the analog reading converted to voltage
unsigned long fsrResistance;  // The voltage converted to resistance, can be very big so make "long"
unsigned long fsrConductance;
long fsrForce;

double sle_temp;              //sleeve temperature

const byte numChars = 32;     //bytes for the char passed from the GUI
char receivedChars[numChars]; //character array for code passed from GUI

boolean newData = false;      //boolean to communicate when the Serial receives new data

//Sample codes for the three sensors
//PRES SENS CODE
/*
   fsrVoltage = map(analogRead(fsrPin), 0, 1023, 0, 5000);
   pressense(fsrVoltage);
*/
//RES TEMP CODE
/*
   sensors.requestTemperatures();
   sensors.getTempCByIndex(0);
*/

//SLEEVE TEMP CODE
/*
   sle_temp = thermocouple.readCelsius();
*/


void setup() {
  // put your setup code here, to run once:
  //Initialize ports as outputs for signal generation
  //Sensor ports are initialized through their library calls above
  pinMode(heater1, OUTPUT);
  pinMode(peltier, OUTPUT);
  pinMode(pump1_in, OUTPUT);
  pinMode(pump1_out, OUTPUT);
  pinMode(pump2_in, OUTPUT);
  pinMode(pump2_out, OUTPUT);
  pinMode(fan_in, OUTPUT);
  pinMode(fan_out, OUTPUT);

  Serial.begin(115200);         //begin serial communication with the raspberry pi at 115200 baud rate
  //Serial.println(1000);
  delay(1000);                  //delay rest of code 1 second to make sure serial communication is initialized properly
}


void loop() {
  // put your main code here, to run repeatedly:
  //Set code to 0 and wait for a code to come from the GUI/user
  
  code = 0;
  newData = false;
  recvWithStartEndMarkers();
  
  //if statement splits message sent from gui into the code which sepcifies the function and the timer using ":" to divide these two values
  
  if (newData == true) {
    //char str[] = "5454:3434:2424";
    char delim[2] = ":";
    char *com = strtok(receivedChars, delim);
    char *code_time = strtok(NULL, delim);
    //char *code_lev = strtok(NULL, delim);
    code = atoi(com);
    timer = atoi(code_time);
    newData = false;
  }

  //Code if statements which start each protocol for the therapy sessions

  if (code == 6969) {
    start_cooling();
  }

  if (code == 6868) {
    active_cooling(timer);
  }

  if (code == 2323) {
    high_heating(timer);
  }

  if (code == 2424) {
    medium_heating(timer);
  }

  if (code == 2525) {
    low_heating(timer);
  }

  if (code == 5454) {
    high_comp(timer);
  }

  if (code == 5555) {
    medium_comp(timer);
  }

  if (code == 5656) {
    low_comp(timer);
  }

  //safety if statements that allow user to stop any protocols, in the event that the program accidentally reenters the main loop in the event of an error
  
  if (code == 7070) {
    analogWrite(peltier, 0);
    digitalWrite(fan_in, LOW);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 0);
    analogWrite(pump1_out, 0 );
    analogWrite(pump2_in, 0);
    analogWrite(pump2_out, 0);
    analogWrite(heater1, 0);
  }

  if (code == 7171) {
    analogWrite(peltier, 0);
    digitalWrite(fan_in, LOW);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 0);
    analogWrite(pump1_out, 0 );
    analogWrite(pump2_in, 0);
    analogWrite(pump2_out, 0);
    analogWrite(heater1, 0);
  }

  if (code == 7373) {
    analogWrite(peltier, 0);
    digitalWrite(fan_in, LOW);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 0);
    analogWrite(pump1_out, 0 );
    analogWrite(pump2_in, 0);
    analogWrite(pump2_out, 0);
    analogWrite(heater1, 0);
  }

  if (code == 7474) {
    analogWrite(peltier, 0);
    digitalWrite(fan_in, LOW);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 0);
    analogWrite(pump1_out, 0 );
    analogWrite(pump2_in, 0);
    analogWrite(pump2_out, 0);
    analogWrite(heater1, 0);
  }

  if (code == 7676) {
    analogWrite(peltier, 0);
    digitalWrite(fan_in, LOW);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 0);
    analogWrite(pump1_out, 0 );
    analogWrite(pump2_in, 0);
    analogWrite(pump2_out, 0);
    analogWrite(heater1, 0);
  }

  if (code == 7878) {
    analogWrite(peltier, 0);
    digitalWrite(fan_in, LOW);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 0);
    analogWrite(pump1_out, 0 );
    analogWrite(pump2_in, 0);
    analogWrite(pump2_out, 0);
    analogWrite(heater1, 0);
  }
}
//Serial Calls
/*
   Cool Start 6969

   Cool Therapy Start 6868
   Cool Pause 7070
   Cool Stop 7171

   High Heat 2323
   Medium Heat 2424
   Low Heat 2525

   Heat Start 7272
   Heat Pause 7373
   Heat Stop 7474

   High Comp 5454
   Medium Comp 5555
   Low Comp 5656

   Comp Start 7575
   Comp Pause 7676
   Comp Stop 7878
*/

//START COOLING PRE-THERAPY
int start_cooling() {
  water_temp = 3;
  int code = 0;
  sensors.requestTemperatures();
                                                             //filters out any erroneous sensors readings
  while (sensors.getTempCByIndex(0) == -127) {
    sensors.requestTemperatures();
  }
                                                            //turn on peltier and reduce water temperature until the water reaches 3 degrees celsius
  while (sensors.getTempCByIndex(0) > 3) {
    sensors.requestTemperatures();
    while (sensors.getTempCByIndex(0) == -127) {
      sensors.requestTemperatures();
    }
    analogWrite(peltier, 255);
    digitalWrite(fan_in, HIGH);
    digitalWrite(fan_out, LOW);
    analogWrite(pump1_in, 100);
    analogWrite(pump1_out, 0 );

    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      char *code_time = strtok(NULL, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      timer = atoi(code_time);
      newData = false;
    }

    if (code == 7070) {
      code = 0;
      analogWrite(peltier, 0);
      digitalWrite(fan_in, LOW);
      digitalWrite(fan_out, LOW);
      analogWrite(pump1_in, 0);
      analogWrite(pump1_out, 0 );
      return 0;
    }

    if (code == 7171) {
      code = 0;
      analogWrite(peltier, 0);
      digitalWrite(fan_in, LOW);
      digitalWrite(fan_out, LOW);
      analogWrite(pump1_in, 0);
      analogWrite(pump1_out, 0 );
      return 0;
    }

  }

  while (code != 6868) {

    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      char *code_time = strtok(NULL, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      timer = atoi(code_time);
      newData = false;
    }

                                                    //keep reservoir temperature at 3 degrees celsius while waiting for user to run cooling protocol
    sensors.requestTemperatures();
    while (sensors.getTempCByIndex(0) == -127) {
      sensors.requestTemperatures();
    }
    if (sensors.getTempCByIndex(0) < water_temp) {
      sensors.requestTemperatures();
      while (sensors.getTempCByIndex(0) == -127) {
        sensors.requestTemperatures();
      }
      analogWrite(peltier, 0);
      digitalWrite(fan_in, HIGH);
      digitalWrite(fan_out, LOW);
      analogWrite(pump1_in, 100);
      analogWrite(pump1_out, 0);
    }
    if (sensors.getTempCByIndex(0) > water_temp) {
      sensors.requestTemperatures();
      while (sensors.getTempCByIndex(0) == -127) {
        sensors.requestTemperatures();
      }
      analogWrite(peltier, 255);
      digitalWrite(fan_in, HIGH);
      digitalWrite(fan_out, LOW);
      analogWrite(pump1_in, 200);
      analogWrite(pump1_out, 0);
    }
  }

  if (code == 6868) {
    active_cooling(timer);
  }
}

//START THERAPY ACTIVE COOLING
int active_cooling(long timer1) {
  water_temp = 3;
  code = 0;
  int convert = 60;
  timer1 = timer1 * convert;
  ntime = (unsigned long)timer1;
  current = millis();
  adjust = millis() - current;

  //run the cooling protocol for the specified time period
  
  while (adjust < ntime) {
    adjust = millis() - current;

    sensors.requestTemperatures();
    while (sensors.getTempCByIndex(0) == -127) {
      sensors.requestTemperatures();
    }
    if (sensors.getTempCByIndex(0) < water_temp) {
      sensors.requestTemperatures();
      while (sensors.getTempCByIndex(0) == -127) {
        sensors.requestTemperatures();
      }
      analogWrite(peltier, 0);
      digitalWrite(fan_in, HIGH);
      digitalWrite(fan_out, LOW);
      analogWrite(pump1_in, 200);
      analogWrite(pump1_out, 0);
      analogWrite(pump2_in, 200);
      analogWrite(pump2_out, 0);
    }
    if (sensors.getTempCByIndex(0) > water_temp) {
      sensors.requestTemperatures();
      while (sensors.getTempCByIndex(0) == -127) {
        sensors.requestTemperatures();
      }
      analogWrite(peltier, 255);
      digitalWrite(fan_in, HIGH);
      digitalWrite(fan_out, LOW);
      analogWrite(pump1_in, 200);
      analogWrite(pump1_out, 0);
      analogWrite(pump2_in, 200);
      analogWrite(pump2_out, 0);
    }

    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }

    //gives the user the ability to stop the session at any time
    
    if (code == 7171) {
      code = 0;
      analogWrite(peltier, 0);
      analogWrite(pump1_in, 0);
      analogWrite(pump1_out, 0);
      analogWrite(pump2_in, 0);
      analogWrite(pump2_out, 0);
      digitalWrite(fan_in, LOW);
      digitalWrite(fan_out, LOW);
      return 0;
    }

    //gives the user the ability to pause the session at any time
    
    if (code == 7070) {
      code = 0;
      while (code != 6868) {

        newData = false;
        recvWithStartEndMarkers();
        if (newData == true) {
          //char str[] = "5454:3434:2424";
          char delim[2] = ":";
          char *com = strtok(receivedChars, delim);
          //char *code_lev = strtok(NULL, delim);
          code = atoi(com);
          newData = false;
        }


        analogWrite(peltier, 100);
        digitalWrite(fan_in, HIGH);
        digitalWrite(fan_out, LOW);
        analogWrite(pump1_in, 100);
        analogWrite(pump1_out, 0);
        analogWrite(pump2_in, 0);
        analogWrite(pump2_out, 0);
        ntime = ntime + millis();
      }
    }
  }
  analogWrite(peltier, 0);
  analogWrite(pump1_in, 0);
  analogWrite(pump1_out, 0);
  analogWrite(pump2_in, 0);
  analogWrite(pump2_out, 0);
  digitalWrite(fan_in, LOW);
  digitalWrite(fan_out, LOW);
  return;
}

//STOP COOLING
int stop_cooling() {
  analogWrite(peltier, 0);
  analogWrite(pump1_in, 50);
  analogWrite(pump1_out, 0);
  analogWrite(pump2_in, 0);
  analogWrite(pump2_out, 0);
  digitalWrite(fan_in, HIGH);
  digitalWrite(fan_out, LOW);
  return;
}

//HEATING
int heating(int heat_level, long timer2) {
  code = 0;
  int convert = 60;
  timer2 = timer2 * convert;
  ntime = (unsigned long)timer2;
  current = millis();
  adjust = millis() - current;
  while (adjust < ntime) {
    adjust = millis() - current;
    analogWrite(heater1, 100);
/*    if (thermocouple.readCelsius() < heat_level) {
      analogWrite(heater1, 100);
    }
    if (thermocouple.readCelsius() > heat_level) {
      analogWrite(heater1, 0);
    }
*/
    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }

    if (code == 7474) {
      analogWrite(heater1, 0);
      return 0;
    }

    if (code == 7373) {
      code = 0;
      while (code != 7272) {

        newData = false;
        recvWithStartEndMarkers();
        if (newData == true) {
          //char str[] = "5454:3434:2424";
          char delim[2] = ":";
          char *com = strtok(receivedChars, delim);
          //char *code_lev = strtok(NULL, delim);
          code = atoi(com);
          newData = false;
        }

        char str = receivedChars;
        char delim[2] = ":";
        char *com1 = strtok(str, delim);
        int code = atoi(com1);

        analogWrite(heater1, 0);
        ntime = ntime + millis();
      }
    }
  }
  analogWrite(heater1, 0);
  return;
}


//HIGH HEAT
int high_heating(long timer20) {
  heat_level = 160;
  code = 0;

  while (code != 7474) {
    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }
    if (code == 7272) {
      heating(heat_level, timer20);
    }
    if (code == 7474) {
      return;
    }
  }
  return;
}

//MEDIUM HEAT
int medium_heating(long timer21) {
  heat_level = 140;
  code = 0;

  while (code != 7474) {

    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }


    if (code == 7272) {
      heating(heat_level, timer21);
    }
    if (code == 7474) {
      return;
    }
  }
  return;
}

//LOW HEAT
int low_heating(long timer22) {
  heat_level = 120;
  code = 0;

  while (code != 7474) {

    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }


    if (code == 7272) {
      heating(heat_level, timer22);
    }
    if (code == 7474) {
      return;
    }
  }
  return;
}

//COMPRESSION
int compression(int comp_level, long timer3) {
  int convert = 60;
  timer3 = timer3 * convert;
  ntime = (unsigned long)timer3;
  current = millis();
  adjust = millis() - current;
  while (adjust < ntime) {
    adjust = millis() - current;
    fsrVoltage = map(analogRead(fsrPin), 0, 1023, 0, 5000);
    level = 0;
    while (pressens(fsrVoltage) < comp_level) {
      fsrVoltage = map(analogRead(fsrPin), 0, 1023, 0, 5000);
      analogWrite(pump2_in, level);
      analogWrite(pump2_out, 0);
      level += 10;


      newData = false;
      recvWithStartEndMarkers();
      if (newData == true) {
        //char str[] = "5454:3434:2424";
        char delim[2] = ":";
        char *com = strtok(receivedChars, delim);
        //char *code_lev = strtok(NULL, delim);
        code = atoi(com);
        newData = false;
      }

      if (code == 7878) {
        return;
      }

      if (code == 7676) {
        code = 0;
        while (code != 7575) {

          newData = false;
          recvWithStartEndMarkers();
          if (newData == true) {
            //char str[] = "5454:3434:2424";
            char delim[2] = ":";
            char *com = strtok(receivedChars, delim);
            //char *code_lev = strtok(NULL, delim);
            code = atoi(com);
            newData = false;
          }


          level = 0;
          analogWrite(pump2_in, level);
          analogWrite(pump2_out, 0);
          ntime = ntime + millis();
        }
      }
    }
    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }

    if (code == 7878) {
      return;
    }

    if (code == 7676) {
      code = 0;
      while (code != 7575) {

        newData = false;
        recvWithStartEndMarkers();
        if (newData == true) {
          //char str[] = "5454:3434:2424";
          char delim[2] = ":";
          char *com = strtok(receivedChars, delim);
          //char *code_lev = strtok(NULL, delim);
          code = atoi(com);
          newData = false;
        }


        level = 0;
        analogWrite(pump2_in, level);
        analogWrite(pump2_out, 0);
        ntime = ntime + millis();
      }
    }
  }
  analogWrite(pump2_in, 0);
  analogWrite(pump2_out, 0);
  return;

}

//HIGH COMP
int high_comp(long timer10) {
  comp_level = 5;
  code = 0;

  while (code != 7878) {
    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }


    if (code == 7575) {
      compression(comp_level, timer10);
    }
    if (code == 7878) {
      return;
    }
  }
  return;
}

//MEDIUM COMP
int medium_comp(long timer11) {
  comp_level = 4;
  code = 0;

  while (code != 7878) {
    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }


    if (code == 7575) {
      compression(comp_level, timer11);
    }
    if (code == 7878) {
      return;
    }
  }
  return;
}

//LOW COMP
int low_comp(long timer12) {
  comp_level = 3;
  code = 0;

  while (code != 7878) {
    newData = false;
    recvWithStartEndMarkers();
    if (newData == true) {
      //char str[] = "5454:3434:2424";
      char delim[2] = ":";
      char *com = strtok(receivedChars, delim);
      //char *code_lev = strtok(NULL, delim);
      code = atoi(com);
      newData = false;
    }


    if (code == 7575) {
      compression(comp_level, timer12);
    }
    if (code == 7878) {
      return;
    }
  }
  return;
}

//SERIAL FILTER
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}


//Pressure Sensor
int pressens(int fsrVoltage) {
  if (fsrVoltage == 0) {
    //Serial.println("No pressure");
  }
  else {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 10000;                // 10K resistor
    fsrResistance /= fsrVoltage;
    //Serial.print("FSR resistance in ohms = ");
    //Serial.println(fsrResistance);

    fsrConductance = 1000000;           // we measure in micromhos so
    fsrConductance /= fsrResistance;
    //Serial.print("Conductance in microMhos: ");
    //Serial.println(fsrConductance);

    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {

      fsrForce = fsrConductance / 80;
      //Serial.print("Force in Newtons: ");
      //Serial.println(fsrForce);
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      //Serial.print("Force in Newtons: ");
      //Serial.println(fsrForce);
    }
  }
  return fsrForce;
}
