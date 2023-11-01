#define BLYNK_AUTH_TOKEN  "7mWqQGFYYbS4BKiLjoEQ3g9WDrkz9nDA"
#define LINE_TOKEN  "STJ0PCEOB0MVb8hxFph0s5FBWy1wLFoSz32sBN8LnQi"

#include <TridentTD_LineNotify.h>

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME           "Device"
#define RXD2 16
#define TXD2 17

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <math.h>

char auth[] = "7mWqQGFYYbS4BKiLjoEQ3g9WDrkz9nDA";
char ssid[] = "WhoAmI";
char pass[] = "tanagon101";

int Button_V0 = 0; // button ON-OFF Automatic mode
int TotalDate_V2 = 0;
int StartDate_V4 = 0;

// tab2
int Day_V6 = 0;
int CountDay;
int Day;
String inputString= ""; // a string to hold incoming data 
boolean stringComplete= false; // whether the string is complete
String sMois = ""; // substring Mois
// int SoilMoistureFromSenser; // Receive the data from STM32

// Time in Hour entered by user Day1-7
int TimeHour_V8;
int TimeHourDay1;
int TimeHourDay2;
int TimeHourDay3;
int TimeHourDay4;
int TimeHourDay5;
int TimeHourDay6;
int TimeHourDay7;
int CurrentHour;

// Time in Mins entered by user Day1-7
int TimeMins_V9;
int TimeMinsDay1;
int TimeMinsDay2;
int TimeMinsDay3;
int TimeMinsDay4;
int TimeMinsDay5;
int TimeMinsDay6;
int TimeMinsDay7;
int CurrentMins;
int CurrentSec;

// Soil Moisture entered by user Day1-7
int SoilMoisture_V11 = 0;
int SoilMoistureDay1;
int SoilMoistureDay2;
int SoilMoistureDay3;
int SoilMoistureDay4;
int SoilMoistureDay5;
int SoilMoistureDay6;
int SoilMoistureDay7;

// the Fertilizer A inputted by user Day1-7
int FertilizerA_V13 = 0;
int FertilizerADay1;
int FertilizerADay2;
int FertilizerADay3;
int FertilizerADay4;
int FertilizerADay5;
int FertilizerADay6;
int FertilizerADay7;

// the Fertilizer B inputted by user Day1-7
int FertilizerB_V15 = 0;
int FertilizerBDay1;
int FertilizerBDay2;
int FertilizerBDay3;
int FertilizerBDay4;
int FertilizerBDay5;
int FertilizerBDay6;
int FertilizerBDay7;

int k; // k for loop Solenoid Valve
int i; // i for loop Fertilizer A
int j; // j for loop Fertilizer B

int CurrentTime_V18 = 0;
int CurrentDay_V20 = 0;
int SoilMoisture_V21 = 0;
int FertilizerASec_V23 = 0;
String StatusFertilizerA_V24 = "";
int FertilizerABSec_V26 = 0;
String StatusFertilizerB_V27 = "";
String StatusValve_V29 = "";
int Current_day;
int Current_month;
int Current_year;

int button_V31 = 0; // button ON-OFF Fertilizer A
int button_V32 = 0; // button ON-OFF Fertilizer B
int button_V33 = 0; // button ON-OFF Solenoid Valve
int CurrentTime_V35 = 0;
int CurrentDay_V36 = 0;
int SetTimeHour_V45 = 0;
int SetTimeMin_V46 = 0;
int SetTimeSec_V47 = 0;
int SetDayD_V48 = 0;
int SetDayM_V49 = 0;
int SetDayY_V50 = 0;
int Settime_V37 = 0;
int Setday_V39 = 0;

int TimeLineH;
int TimeLineM;
int PumpA;
int PumpB;
int Setmoi;
String Dayofweek;

BlynkTimer timer;
WidgetRTC rtc;
BlynkTimer mytimerrecivedata;
BlynkTimer mytimergetdatafromstm;
BlynkTimer mytimertostm;

// Digital clock display of the time
void clockDisplay() {
  CurrentHour = hour();
  CurrentMins = minute();
  CurrentSec = second();
  Current_day = day();
  Current_month = month();
  Current_year = year();
  
  int sec = 0;
  int mins = 0;
  int hours = 0;


  // Setting time and date
if (Settime_V37 == 1) {
  CurrentSec = SetTimeSec_V47;
  CurrentMins = SetTimeMin_V46;
  CurrentHour = SetTimeHour_V45;
}

if (Setday_V39 == 1) {
  Current_day = SetDayD_V48;
  Current_month = SetDayM_V49;
  Current_year = SetDayY_V50;
}

String currentTime = String(CurrentHour) + ":" + CurrentMins + ":" + CurrentSec;
String currentDate = String(Current_day) + " / " + Current_month + " / " + Current_year;

Serial.print("Current time: ");
Serial.print(currentTime);
Serial.print(" ");
Serial.print(currentDate);
Serial.println();

// Send time to the App
Blynk.virtualWrite(V35, currentTime);
Blynk.virtualWrite(V18, currentTime);

// Send date to the App
Blynk.virtualWrite(V20, currentDate);
Blynk.virtualWrite(V36, currentDate);

// Date converting
int yy;         // Last 2 digits of the year (i.e., 2016 would be 16)
int c;          // Century (i.e., 2016 would be 20)
int mTable;     // Month value based on calculation table
int SummedDate; // Add values combined in prep for Mod7 calc
int DoW;        // Day of the week value (0-6)
int leap;       // Leap Year or not
int cTable;     // Century value based on calculation table

// Leap Year Calculation
if ((fmod(Current_year, 4) == 0 && fmod(Current_year, 100) != 0) || (fmod(Current_year, 400) == 0)) {
  leap = 1;
} else {
  leap = 0;
}

// Limit results to year 1900-2299 (to save memory)
while (Current_year > 2299) {
  Current_year = Current_year - 400;
}

while (Current_year < 1900) {
  Current_year = Current_year + 400;
}

// Calculating century
c = Current_year / 100;

// Calculating two-digit year
yy = fmod(Current_year, 100);

// Century value based on Table
if (c == 19) {
  cTable = 1;
}

if (c == 20) {
  cTable = 0;
}

if (c == 21) {
  cTable = 5;
}

if (c == 22) {
  cTable = 3;
}

// Jan and Feb calculations affected by leap years
if (Current_month == 1) {
  if (leap == 1) {
    mTable = 6;
  } else {
    mTable = 0;
  }
}

if (Current_month == 2) {
  if (leap == 1) {
    mTable = 2;
  } else {
    mTable = 3;
  }
}

// Other months not affected and have set values
if (Current_month == 10) {
  mTable = 0;
}

if (Current_month == 8) {
  mTable = 2;
}

if (Current_month == 3 || Current_month == 11) {
  mTable = 3;
}

if (Current_month == 4 || Current_month == 7) {
  mTable = 6;
}

if (Current_month == 5) {
  mTable = 1;
}

if (Current_month == 6) {
  mTable = 4;
}

if (Current_month == 9 || Current_month == 12) {
  mTable = 5;
}

// Enter the data into the formula
SummedDate = Current_day + mTable + yy + (yy / 4) + cTable;

// Find remainder
DoW = fmod(SummedDate, 7);

// Output result
Serial.print(" = ");

// Remainder determines day of the week
if (DoW == 0) {
  Serial.println("Saturday");
  Day = 6;
}

if (DoW == 1) {
  Serial.println("Sunday");
  Day = 7;
}

if (DoW == 2) {
  Serial.println("Monday");
  Day = 1;
}

if (DoW == 3) {
  Serial.println("Tuesday");
  Day = 2;
}

if (DoW == 4) {
  Serial.println("Wednesday");
  Day = 3;
}

if (DoW == 5) {
  Serial.println("Thursday");
  Day = 4;
}

if (DoW == 6) {
  Serial.println("Friday");
  Day = 5;
}
BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(134, 122, 29, 169), 8080);
  LINE.setToken(LINE_TOKEN);
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)

  // Display digital clock every 10 seconds
  timer.setInterval(1000L, clockDisplay);
  mytimerrecivedata.setInterval(1000L, RecieveData);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  inputString.reserve(200);
  mytimergetdatafromstm.setInterval(100L, RecieveDataToSTM32F4);
  mytimertostm.setInterval(100L, sendDatatoSTM32F4);

  // tab1
  Blynk.virtualWrite(V1, "  TOTAL DATE:");
  Blynk.virtualWrite(V3, "  START DATE:");

  // tab2
  Blynk.virtualWrite(V5, " CONFIG");
  Blynk.virtualWrite(V7, "TIME:");
  Blynk.virtualWrite(V10, "SOIL MOISTURE(%):");
  Blynk.virtualWrite(V12, "FERTILIZER A:");
  Blynk.virtualWrite(V14, "FERTILIZER B:");

  // tab3
  Blynk.virtualWrite(V16, "  MONITOR");
  Blynk.virtualWrite(V17, " CURRENT TIME:");
  Blynk.virtualWrite(V19, " CURRENT DAY:");
  Blynk.virtualWrite(V22, " FERTILIZER A (Sec):");
  Blynk.virtualWrite(V25, " FERTILIZER B (Sec):");
  Blynk.virtualWrite(V28, " SOLENOID VALVE:");

  // tab4
  Blynk.virtualWrite(V30, " MANUAL");
  Blynk.virtualWrite(V34, " SETTIME");
  Blynk.virtualWrite(V38, " HH:");
  Blynk.virtualWrite(V40, " MM:");
  Blynk.virtualWrite(V41, " SS:");
  Blynk.virtualWrite(V42, " DAY:");
  Blynk.virtualWrite(V53, " MONTH:");
  Blynk.virtualWrite(V54, "YEAR:");
  Blynk.virtualWrite(V52, "AUTOMATIC MODE:");
}


void loop() {
  Blynk.run();
  timer.run();
  mytimerrecivedata.run();
  serialEvent();
  mytimergetdatafromstm.run();
  mytimertostm.run();

  // Stored the data inputted on the config screen tab to variables
  if (Day_V6 == 1) {
    // Data for Monday
    TimeHourDay1 = TimeHour_V8;
    TimeMinsDay1 = TimeMins_V9;
    SoilMoistureDay1 = SoilMoisture_V11;
    FertilizerADay1 = FertilizerA_V13;
    FertilizerBDay1 = FertilizerB_V15;
  }
  if (Day_V6 == 2) {
    // Data for Tuesday
    TimeHourDay2 = TimeHour_V8;
    TimeMinsDay2 = TimeMins_V9;
    SoilMoistureDay2 = SoilMoisture_V11;
    FertilizerADay2 = FertilizerA_V13;
    FertilizerBDay2 = FertilizerB_V15;
  }
  if (Day_V6 == 3) {
    // Data for Wednesday
    TimeHourDay3 = TimeHour_V8;
    TimeMinsDay3 = TimeMins_V9;
    SoilMoistureDay3 = SoilMoisture_V11;
    FertilizerADay3 = FertilizerA_V13;
    FertilizerBDay3 = FertilizerB_V15;
  }
  if (Day_V6 == 4) {
    // Data for Thursday
    TimeHourDay4 = TimeHour_V8;
    TimeMinsDay4 = TimeMins_V9;
    SoilMoistureDay4 = SoilMoisture_V11;
    FertilizerADay4 = FertilizerA_V13;
    FertilizerBDay4 = FertilizerB_V15;
  }
  if (Day_V6 == 5) {
    // Data for Friday
    TimeHourDay5 = TimeHour_V8;
    TimeMinsDay5 = TimeMins_V9;
    SoilMoistureDay5 = SoilMoisture_V11;
    FertilizerADay5 = FertilizerA_V13;
    FertilizerBDay5 = FertilizerB_V15;
  }
  if (Day_V6 == 6) {
    // Data for Saturday
    TimeHourDay6 = TimeHour_V8;
    TimeMinsDay6 = TimeMins_V9;
    SoilMoistureDay6 = SoilMoisture_V11;
    FertilizerADay6 = FertilizerA_V13;
    FertilizerBDay6 = FertilizerB_V15;
  }
  if (Day_V6 == 7) {
    // Data for Sunday
    TimeHourDay7 = TimeHour_V8;
    TimeMinsDay7 = TimeMins_V9;
    SoilMoistureDay7 = SoilMoisture_V11;
    FertilizerADay7 = FertilizerA_V13;
    FertilizerBDay7 = FertilizerB_V15;
  }
}

// Check whether the Automatic button is ON/OFF
// If the automatic is ON
if (Button_V0 == 1) {
  // Check date and then convert to day
  // Set CountDay to 0
  Serial.print("Day = ");
  Serial.print(Day);
  CountDay = 0;
  // Set startdate to Day
  Day = StartDate_V4;
  
  ///////////////////////////////////////
  Serial.print("Total date:");
  Serial.print(TotalDate_V2);
  Serial.print("Start Date:");
  Serial.print(StartDate_V4);
  Serial.print("\n");
  Serial.print("Day");
  Serial.print(Day);
  Serial.print("Soil Moisture from Sensor:");
  Serial.print(SoilMoistureFromSenser);
  Serial.print("\n");
  Serial.print("Time");
  Serial.print(CurrentHour + ":" + CurrentMins);
  Serial.print("\n");
  ///////////////////////////////////////
  
  for (CountDay = 0; CountDay <= TotalDate_V2; CountDay++) {
    if (Day == 1) {
      Dayofweek = String("Monday");
      // Check whether the set time matches the current time
      if (TimeHourDay1 == CurrentHour && TimeMinsDay1 == CurrentMins) {
        TimeLineH = TimeHourDay1;
        TimeLineM = TimeMinsDay1;
        // Check the soil moisture to open the Solenoid Valve
        // By comparing the soil moisture entered by the user with the soil moisture measured by the sensor
        Serial.print("Today Soil Moisture");
        Serial.print(SoilMoistureDay1);
        Serial.print("\n");
        Serial.print("Time for Pump A: Pump B");
        Serial.print(FertilizerADay1);
        Serial.print(":");
        Serial.print(FertilizerBDay1);
        Serial.print("\n");
        Setmoi = SoilMoistureDay1;
        
        if (SoilMoistureFromSenser < SoilMoistureDay1) {
          // The Solenoid Valve is open (ON) now!
          // Print the status "ON" to monitor
          Blynk.virtualWrite(V29, "ON");
          
          // No need to check whether FertilizerADay1 and FertilizerBDay1 are less than or equal to 10
          // because we use the numeric input that scopes the range between 0 to 10
          // The Fertilizer A and B pumps are active now!!!
          
          if (FertilizerADay1 == 0) {
            // Print the status of Fertilizer A is off
            Blynk.virtualWrite(V24, "OFF");
          } else {
            // Print the status of Fertilizer A is on
            Blynk.virtualWrite(V24, "ON");
            // The Fertilizer A will work for FertilizerADay1 seconds
            for (i = FertilizerADay1; i >= 0; i--) {
              // Print the value of FertilizerADay1 while decreasing to monitor
              sleep(1); // Wait for 1 second
              Blynk.virtualWrite(V23, i);
              PumpA = i;
            }
            // The Fertilizer A is off
            // Print the status of Fertilizer A is off
            Blynk.virtualWrite(V24, "OFF");
          }
          
          if (FertilizerBDay1 == 0) {
            // Print the status of Fertilizer B is off
            Blynk.virtualWrite(V27, "OFF");
          } else {
            // Print the status of Fertilizer B is on
            Blynk.virtualWrite(V27, "ON");
            // The Fertilizer B will work for FertilizerBDay1 seconds
            for (j = FertilizerBDay1; j >= 0; j--) {
              // Print the value of FertilizerBDay1 while decreasing to monitor
              sleep(1);
              Blynk.virtualWrite(V26, j);
              PumpB = j;
            }
            // The Fertilizer B is off
            // Print the status of Fertilizer B is off
            Blynk.virtualWrite(V27, "OFF");
          }
        }
        // The Solenoid Valve is off under the conditions that SoilMoistureFromSenser = SoilMoistureDay1
        // and FertilizerADay1 and FertilizerBDay1 are done
      }
      // The Solenoid Valve is OFF
      // Print the status "ON" to monitor
      Blynk.virtualWrite(V29, "OFF");
    }// End for Day1

    if (Day == 2) {
      Dayofweek = String("Tuesday");
      if (TimeHourDay2 == CurrentHour && TimeMinsDay2 == CurrentMins) {
        TimeLineH = TimeHourDay2;
        TimeLineM = TimeMinsDay2;
        Serial.print("Today Soil Moisture: ");
        Serial.print(SoilMoistureDay2);
        Serial.print("\n");
        Serial.print("Time for Pump A: Pump B: ");
        Serial.print(FertilizerADay2);
        Serial.print(":");
        Serial.print(FertilizerBDay2);
        Serial.print("\n");
        Setmoi = SoilMoistureDay2;

        if (SoilMoistureFromSenser < SoilMoistureDay2) {
         Blynk.virtualWrite(V29, "ON");

          if (FertilizerADay2 == 0) {
            Blynk.virtualWrite(V24, "OFF");
          }
          else {
            Blynk.virtualWrite(V24, "ON");
            for (i = FertilizerADay2; i >= 0; i--) {
             sleep(1);
             Blynk.virtualWrite(V23, i);
             PumpA = i;
          }
          Blynk.virtualWrite(V24, "OFF");
        }
        if (FertilizerBDay2 == 0) {
          Blynk.virtualWrite(V27, "OFF");
        } 
        else {
          Blynk.virtualWrite(V27, "ON");
          for (j = FertilizerBDay2; j >= 0; j--) {
            sleep(1);
            Blynk.virtualWrite(V26, j);
            PumpB = j;
          }
          Blynk.virtualWrite(V27, "OFF");
        }
    }
    Blynk.virtualWrite(V29, "OFF");
    }
  }//End of day2

  if (Day == 3) {
    Dayofweek = String("Wednesday");
    if (TimeHourDay3 == CurrentHour && TimeMinsDay3 == CurrentMins) {
      TimeLineH = TimeHourDay3;
      TimeLineM = TimeMinsDay3;
      Serial.print("Today Soil Moisture: ");
      Serial.print(SoilMoistureDay3);
      Serial.print("\n");
      Serial.print("Time for Pump A: Pump B: ");
      Serial.print(FertilizerADay3);
      Serial.print(":");
      Serial.print(FertilizerBDay3);
      Serial.print("\n");
      Setmoi = SoilMoistureDay3;
  
      if (SoilMoistureFromSenser < SoilMoistureDay3) {
        Blynk.virtualWrite(V29, "ON");
  
        if (FertilizerADay3 == 0) {
          Blynk.virtualWrite(V24, "OFF");
        } else {
          Blynk.virtualWrite(V24, "ON");
          for (i = FertilizerADay3; i >= 0; i--) {
            sleep(1);
            Blynk.virtualWrite(V23, i);
            PumpA = i;
          }
          Blynk.virtualWrite(V24, "OFF");
        }
  
        if (FertilizerBDay3 == 0) {
          Blynk.virtualWrite(V27, "OFF");
        } else {
          Blynk.virtualWrite(V27, "ON");
          for (j = FertilizerBDay3; j >= 0; j--) {
            sleep(1);
            Blynk.virtualWrite(V26, j);
            PumpB = j;
          }
          Blynk.virtualWrite(V27, "OFF");
        }
      }
  
      Blynk.virtualWrite(V29, "OFF");
    }
  }// End of day3

  if (Day == 4) {
    Dayofweek = String("Thursday");
    if (TimeHourDay4 == CurrentHour && TimeMinsDay4 == CurrentMins) {
      TimeLineH = TimeHourDay4;
      TimeLineM = TimeMinsDay4;
      Serial.print("Today Soil Moisture: ");
      Serial.print(SoilMoistureDay4);
      Serial.print("\n");
      Serial.print("Time for Pump A: Pump B: ");
      Serial.print(FertilizerADay4);
      Serial.print(":");
      Serial.print(FertilizerBDay4);
      Serial.print("\n");
      Setmoi = SoilMoistureDay4;
  
      if (SoilMoistureFromSenser < SoilMoistureDay4) {
        Blynk.virtualWrite(V29, "ON");
  
        if (FertilizerADay4 == 0) {
          Blynk.virtualWrite(V24, "OFF");
        } else {
          Blynk.virtualWrite(V24, "ON");
          for (i = FertilizerADay4; i >= 0; i--) {
            sleep(1);
            Blynk.virtualWrite(V23, i);
            PumpA = i;
          }
          Blynk.virtualWrite(V24, "OFF");
        }
  
        if (FertilizerBDay4 == 0) {
          Blynk.virtualWrite(V27, "OFF");
        } else {
          Blynk.virtualWrite(V27, "ON");
          for (j = FertilizerBDay4; j >= 0; j--) {
            sleep(1);
            Blynk.virtualWrite(V26, j);
            PumpB = j;
          }
          Blynk.virtualWrite(V27, "OFF");
        }
      }
  
      Blynk.virtualWrite(V29, "OFF");
    }
  }// End of day4

  if (Day == 5) {
    Dayofweek = String("Friday");
    if (TimeHourDay5 == CurrentHour && TimeMinsDay5 == CurrentMins) {
      TimeLineH = TimeHourDay5;
      TimeLineM = TimeMinsDay5;
      Serial.print("Today Soil Moisture: ");
      Serial.print(SoilMoistureDay5);
      Serial.print("\n");
      Serial.print("Time for Pump A: Pump B: ");
      Serial.print(FertilizerADay5);
      Serial.print(":");
      Serial.print(FertilizerBDay5);
      Serial.print("\n");
      Setmoi = SoilMoistureDay5;
  
      if (SoilMoistureFromSenser < SoilMoistureDay5) {
        Blynk.virtualWrite(V29, "ON");
  
        if (FertilizerADay5 == 0) {
          Blynk.virtualWrite(V24, "OFF");
          Serial.print("OFF");
        } else {
          Blynk.virtualWrite(V24, "ON");
          for (i = FertilizerADay5; i >= 0; i--) {
            sleep(1);
            Blynk.virtualWrite(V23, i);
            PumpA = i;
          }
          i = 0;
          Blynk.virtualWrite(V24, "OFF");
        }
  
        if (FertilizerBDay5 == 0) {
          Blynk.virtualWrite(V27, "OFF");
        } else {
          Blynk.virtualWrite(V27, "ON");
          for (j = FertilizerBDay5; j >= 0; j--) {
            sleep(1);
            Blynk.virtualWrite(V26, j);
            PumpB = j;
          }
          j = 0;
          Blynk.virtualWrite(V27, "OFF");
        }
      }
  
      Blynk.virtualWrite(V29, "OFF");
    }
  }// End of day5

  if (Day == 6) {
    Dayofweek = String("Saturday");
    if (TimeHourDay6 == CurrentHour && TimeMinsDay6 == CurrentMins) {
      TimeLineH = TimeHourDay6;
      TimeLineM = TimeMinsDay6;
      Serial.print("Today Soil Moisture: ");
      Serial.print(SoilMoistureDay6);
      Serial.print("\n");
      Serial.print("Time for Pump A: Pump B: ");
      Serial.print(FertilizerADay6);
      Serial.print(":");
      Serial.print(FertilizerBDay6);
      Serial.print("\n");
      Setmoi = SoilMoistureDay6;
  
      if (SoilMoistureFromSenser < SoilMoistureDay6) {
        Blynk.virtualWrite(V29, "ON");
        Serial.print("SOL = ON");
  
        if (FertilizerADay6 == 0) {
          Blynk.virtualWrite(V24, "OFF");
          Serial.print("A = OFF");
        } else {
          Blynk.virtualWrite(V24, "ON");
          Serial.print("A = ON");
          for (i = FertilizerADay6; i >= 0; i--) {
            sleep(1);
            Blynk.virtualWrite(V23, i);
            Serial.print(i);
            PumpA = i;
          }
          Blynk.virtualWrite(V24, "OFF");
          Serial.print("A = OFF");
        }
  
        if (FertilizerBDay6 == 0) {
          Blynk.virtualWrite(V27, "OFF");
          Serial.print("B = OFF");
        } else {
          Blynk.virtualWrite(V27, "ON");
          Serial.print("B = ON");
          for (j = FertilizerBDay6; j >= 0; j--) {
            sleep(1);
            Blynk.virtualWrite(V26, j);
            Serial.print(j);
            PumpB = j;
          }
          Blynk.virtualWrite(V27, "OFF");
          Serial.print("B = OFF");
        }
      }
  
      Blynk.virtualWrite(V29, "OFF");
    }
  }// End of day6

  if (Day == 7) {
    Dayofweek = String("Sunday");
    if (TimeHourDay7 == CurrentHour && TimeMinsDay7 == CurrentMins) {
      TimeLineH = TimeHourDay7;
      TimeLineM = TimeMinsDay7;
      Serial.print("Today Soil Moisture: ");
      Serial.print(SoilMoistureDay7);
      Serial.print("\n");
      Serial.print("Time for Pump A: Pump B: ");
      Serial.print(FertilizerADay7);
      Serial.print(":");
      Serial.print(FertilizerBDay7);
      Serial.print("\n");
      Setmoi = SoilMoistureDay7;
  
      if (SoilMoistureFromSenser < SoilMoistureDay7) {
        Blynk.virtualWrite(V29, "ON");
  
        if (FertilizerADay7 == 0) {
          Blynk.virtualWrite(V24, "OFF");
        } else {
          Blynk.virtualWrite(V24, "ON");
          for (i = FertilizerADay7; i >= 0; i--) {
            sleep(1);
            Blynk.virtualWrite(V23, i);
            PumpA = i;
          }
          Blynk.virtualWrite(V24, "OFF");
        }
  
        if (FertilizerBDay7 == 0) {
          Blynk.virtualWrite(V27, "OFF");
        } else {
          Blynk.virtualWrite(V27, "ON");
          for (j = FertilizerBDay7; j >= 0; j--) {
            sleep(1);
            Blynk.virtualWrite(V26, j);
            PumpB = j;
          }
          Blynk.virtualWrite(V27, "OFF");
        }
      }
  
      Blynk.virtualWrite(V29, "OFF");
    }
  
    // Increment to the next day
    Day++;
  }
 }
}

// If the automatic mode is off
if (Button_V0 == 0) {
  // Case 1: Manual mode
  if (button_V33 == 1) {
    // Print that the solenoid valve is on
    Blynk.virtualWrite(V29, "ON");

    // Set both pumps to off (0 sec)
    Blynk.virtualWrite(V24, "OFF"); // Pump A
    Blynk.virtualWrite(V27, "OFF"); // Pump B

    // Check if Pump A is on
    if (button_V31 == 1) {
      // Print that Pump A is on
      Blynk.virtualWrite(V24, "ON");
    }

    // Check if Pump B is on
    if (button_V32 == 1) {
      // Print that Pump B is on
      Blynk.virtualWrite(V27, "ON");
    }
  }
  // Case 2: Sleeping mode (doing nothing)
  else {
    Blynk.virtualWrite(V29, "OFF");
    Blynk.virtualWrite(V24, "OFF");
    Blynk.virtualWrite(V27, "OFF");
  }
}

// Get the data from STM32F4 and store it in Arduino
if (stringComplete) {
  stringComplete = false;
  Serial.println(inputString);
  sMois = inputString.substring(1, 5);
  SoilMoistureFromSenser = sMois.toInt();
  inputString = "";
}


// Print data to show on Serial Monitor
Serial.print("Monday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay1);
Serial.print(":");
Serial.print(TimeMinsDay1);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay1);
Serial.print(":");
Serial.print(FertilizerBDay1);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay1);
Serial.print("\n");
Serial.print("\n");

Serial.print("Tuesday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay2);
Serial.print(":");
Serial.print(TimeMinsDay2);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay2);
Serial.print(":");
Serial.print(FertilizerBDay2);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay2);
Serial.print("\n");
Serial.print("\n");

Serial.print("Wednesday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay3);
Serial.print(":");
Serial.print(TimeMinsDay3);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay3);
Serial.print(":");
Serial.print(FertilizerBDay3);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay3);
Serial.print("\n");
Serial.print("\n");

Serial.print("Thursday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay4);
Serial.print(":");
Serial.print(TimeMinsDay4);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay4);
Serial.print(":");
Serial.print(FertilizerBDay4);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay4);
Serial.print("\n");
Serial.print("\n");

Serial.print("Friday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay5);
Serial.print(":");
Serial.print(TimeMinsDay5);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay5);
Serial.print(":");
Serial.print(FertilizerBDay5);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay5);
Serial.print("\n");
Serial.print("\n");

Serial.print("Saturday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay6);
Serial.print(":");
Serial.print(TimeMinsDay6);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay6);
Serial.print(":");
Serial.print(FertilizerBDay6);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay6);
Serial.print("\n");
Serial.print("\n");

Serial.print("Sunday:");
Serial.print("\n");
Serial.print("  Working Time:");
Serial.print(TimeHourDay7);
Serial.print(":");
Serial.print(TimeMinsDay7);
Serial.print("  Time for Pump A: Pump B");
Serial.print(FertilizerADay7);
Serial.print(":");
Serial.print(FertilizerBDay7);
Serial.print("  Soil Moisture:");
Serial.print(SoilMoistureDay7);
Serial.print("\n");
Serial.print("\n");


// *Sending the data Mois to the following widget* //
void RecieveData() {
  Blynk.virtualWrite(V21, SoilMoistureFromSenser);
  Blynk.virtualWrite(V44, SoilMoistureFromSenser);
}

void serialEvent() {
  while (Serial2.available()) {
    char inChar = (char)Serial2.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

// *This part is receiving the data from STM32F4 (Moisture converted from 0-4095 to 0-99) *
void RecieveDataToSTM32F4() {
  Serial2.print("#%.1f");
  Serial2.print(SoilMoistureFromSenser);
  Serial2.print("\n");
}

BLYNK_WRITE(V2) {
  TotalDate_V2 = param.asInt(); // setting total date
}

BLYNK_WRITE(V4) {
  StartDate_V4 = param.asInt(); // setting start date
}

BLYNK_WRITE(V31) {
  button_V31 = param.asInt(); // button ON-OFF Fertilizer A
}

BLYNK_WRITE(V32) {
  button_V32 = param.asInt(); // button ON-OFF Fertilizer B
}

BLYNK_WRITE(V33) {
  button_V33 = param.asInt(); // button ON-OFF Solenoid Valve
}

BLYNK_WRITE(V6) {
  Day_V6 = param.asInt(); // setting select day
}

BLYNK_WRITE(V8) {
  TimeHour_V8 = param.asInt(); // setting HH
  TimeHour_V8 = TimeHour_V8 - 1;
}

BLYNK_WRITE(V9) {
  TimeMins_V9 = param.asInt(); // setting MM
  TimeMins_V9 = TimeMins_V9 - 1;
}

BLYNK_WRITE(V11) {
  SoilMoisture_V11 = param.asInt(); // Enter by users
}

BLYNK_WRITE(V13) {
  FertilizerA_V13 = param.asInt(); // setting FA
}

BLYNK_WRITE(V15) {
  FertilizerB_V15 = param.asInt(); // setting FB
}

String HM = String(TimeLineH) + ":" + TimeLineM;

BLYNK_WRITE(V0) {
  Button_V0 = param.asInt(); // button ON-OFF Automatic

  // This one is for LineNotify to send the data to the Line application if I click the button on/off automatic
  if (Button_V0 == 0) {
    LINE.notify("\nAutomatic System: OFF\n");
  } else {
    LINE.notify("\nAutomatic System: ON\nStart Time: " + HM + "\nMoisture: " + String(SoilMoistureFromSenser) + "\nSoil Moisture: Working");
  }
}

BLYNK_WRITE(V45) {
  SetTimeHour_V45 = param.asInt();
}

BLYNK_WRITE(V46) {
  SetTimeMin_V46 = param.asInt();
}

BLYNK_WRITE(V47) {
  SetTimeSec_V47 = param.asInt();
}

BLYNK_WRITE(V48) {
  SetDayD_V48 = param.asInt();
}

BLYNK_WRITE(V49) {
  SetDayM_V49 = param.asInt();
}

BLYNK_WRITE(V50) {
  SetDayY_V50 = param.asInt() + 2017;
}

BLYNK_WRITE(V37) {
  Settime_V37 = param.asInt();
}

BLYNK_WRITE(V39) {
  Setday_V39 = param.asInt();
}

// *This part is the data that sends to show in STM32F4* //
void sendDatatoSTM32F4() {
  Serial2.print("#FerA(status)=");
  Serial2.print(button_V31);
  Serial2.print("\n");

  Serial2.print("#FerB(status)=");
  Serial2.print(button_V32);
  Serial2.print("\n");

  Serial2.print("#Valve(status)=");
  Serial2.print(button_V33);
  Serial2.print("\n");

  Serial2.print("#SetHour=");
  Serial2.print(TimeHour_V8);
  Serial2.print("\n");

  Serial2.print("#SetMinute=");
  Serial2.print(TimeMins_V9);
  Serial2.print("\n");

  Serial2.print("#TotalDate=");
  Serial2.print(TotalDate_V2);
  Serial2.print("\n");

  Serial2.print("#StartDate=");
  Serial2.print(StartDate_V4);
  Serial2.print("\n");

  Serial2.print("#MoisSlide=");
  Serial2.print(SoilMoisture_V11);
  Serial2.print("\n");

  Serial2.print("#FerA(s)=");
  Serial2.print(FertilizerA_V13);
  Serial2.print("\n");

  Serial2.print("#FerB(s)=");
  Serial2.print(FertilizerB_V15);
  Serial2.print("\n");

  Serial2.print("#DayC=");
  Serial2.print(Day_V6);
  Serial2.print("\n");

  Serial2.print("#FerAS=");
  Serial2.print(button_V31);
  Serial2.print("\n");

  Serial2.print("#FerBS=");
  Serial2.print(button_V32);
  Serial2.print("\n");

  Serial2.print("#ValveS=");
  Serial2.print(button_V33);
  Serial2.print("\n");

  Serial2.print("#RHour=");
  Serial2.print(SetTimeHour_V45);
  Serial2.print("\n");

  Serial2.print("#RMinute=");
  Serial2.print(SetTimeMin_V46);
  Serial2.print("\n");

  Serial2.print("#RSecond=");
  Serial2.print(SetTimeSec_V47);
  Serial2.print("\n");

  Serial2.print("#Day=");
  Serial2.print(SetDayD_V48);
  Serial2.print("\n");

  Serial2.print("#Month=");
  Serial2.print(SetDayM_V49);
  Serial2.print("\n");

  Serial2.print("#Year=");
  Serial2.print(SetDayY_V50);
  Serial2.print("\n");

  Serial2.print("#DayC=");
  Serial2.print(Day_V6);
  Serial2.print("\n");

  Serial2.print("#Mode=");
  Serial2.print(Button_V0);
  Serial2.print("\n");
}
