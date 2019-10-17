#include <ArduinoJson.h>

//Gas Sensor Pins
#define F2600 A1
#define MQ135 A2
#define F2602 A3

//Gas Sensor Load Resistance (RL)
#define RL_F2600 7.5
#define RL_MQ135 10
#define RL_F2602 7.5
    
/*2600 GASSES*/
float CO_A = 1.144997421;
float CO_B = -0.21687423;

float CH4_A = 1.05777824;
float CH4_B = -0.0795645;

/*2602 GASSES*/
float NH3_A = 0.92372844;
float NH3_B = -0.291578925;

float H2S_A = 0.38881036;
float H2S_B = -0.35010059;

float VOC_A = 0.3220722;
float VOC_B = -0.6007520;


/*MQ-135 GASSES*/
//MQ-135 CO2
float A_MQ135_CO2 = 112.89808; 
float B_MQ135_CO2 = -2.868463517;  


//MQ-135 NOx
float A_MQ135_NOx = 34.69756084;  
float B_MQ135_NOx = -3.422829698; 

void setup() 
{
Serial.begin(9600); // opens serial port, sets data rate128000 bps
}

void loop() 
{
  float VRL_F2600; 
  float Rs_F2600; 
  float Ro_F2600 = 11.26;
  float ratio_F2600;
   
  VRL_F2600 = analogRead(F2600)*(5.0/1023.0); 
  Rs_F2600 = ((5.0/VRL_F2600)-1)*(RL_F2600);
  ratio_F2600 = Rs_F2600/Ro_F2600;


  float ppm_CO = CO_A * pow(ratio_F2600, CO_B);
  float ppm_CH4 = CH4_A * pow(ratio_F2600, CH4_B);

  float VRL_MQ135; 
  float Rs_MQ135; 
  float Ro_MQ135 = 20.1;
  float ratio_MQ135;

  VRL_MQ135 = analogRead(MQ135)*(5.0/1023.0); 
  Rs_MQ135 = ((5.0/VRL_MQ135)-1)*(RL_MQ135); 
  ratio_MQ135 = Rs_MQ135/Ro_MQ135;
  
  
  float ppm_CO2 = A_MQ135_CO2 * pow(ratio_MQ135, B_MQ135_CO2);
  float ppm_NOx = A_MQ135_NOx * pow(ratio_MQ135, B_MQ135_NOx);

  float VRL_F2602; 
  float Rs_F2602; 
  float Ro_F2602 = 64.88;
  float ratio_F2602;
   
  VRL_F2602 = analogRead(F2602)*(5.0/1023.0); 
  Rs_F2602 = ((5.0/VRL_F2602)-1)*(RL_F2602); 
  ratio_F2602 = Rs_F2602/Ro_F2602;

  float ppm_VOC = VOC_A * pow(ratio_F2602, VOC_B);
  float ppm_H2S = H2S_A * pow(ratio_F2602, H2S_B);  
  float ppm_NH3 = NH3_A * pow(ratio_F2602, NH3_B);
  
  // put your main code here, to run repeatedly:


  DynamicJsonBuffer jBuffer; 
  JsonObject& root = jBuffer.createObject();

  root["CO2"] = ppm_CO2;
  root["CO"] = ppm_CO;
  root["CH4"] = ppm_CH4;
  root["NOx"] = ppm_NOx;
  root["NH3"] = ppm_NH3;
  root["H2S"] = ppm_H2S;
  root["VOC"] = ppm_VOC;
  
  root.prettyPrintTo(Serial);
  Serial.println("");


delay(18000);
}
