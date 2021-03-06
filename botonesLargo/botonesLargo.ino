#include <Arduino.h>
#include <LiquidCrystal.h>

#define BOTON_PIN_ADC           A0  // A0 is the boton ADC input
// ADC readings expected for the 5 botons on the ADC input
#define DER_ADC            8  // derecha
#define ARR_ADC          154  // up
#define ABJ_ADC          338  // down
#define IZQ_ADC          500  // left
#define SELEC_ADC        708  // derecha
#define HISTERESIS         7  // hysteresis for valid boton sensing window
//return values for leerBotones()
const byte BOTON_NO       = 0;
const byte BOTON_DER      = 1;
const byte BOTON_DER_LAR  = 2;
const byte BOTON_ARR      = 3;
const byte BOTON_ARR_LAR  = 4;
const byte BOTON_ABJ      = 5;
const byte BOTON_ABJ_LAR  = 6;
const byte BOTON_IZQ      = 7;
const byte BOTON_IZQ_LAR  = 8;
const byte BOTON_SELEC    = 9;
const byte BOTON_SELEC_LAR= 10;

LiquidCrystal lcd( 11, 12, 5, 6, 9, 10 );

byte botonPresionado  = false;         //this will be true after a leerBotones() call if triggered
byte botonLiberado = false;         //this will be true after a leerBotones() call if triggered
byte botonEdo      = BOTON_NO;   //used by leerBotones() for detection of boton events
byte botonEdoUlt = botonEdo;
long ulTimer = 0;
long tiner = millis();
long retrasoBut = 50;
long apretarLargo = 500;
bool largo = false;

byte leerBotones();
byte idenBoton();
uint8_t botones = 0;

void setup()
{
  //boton adc input
  Serial.begin(9600);
  pinMode( BOTON_PIN_ADC, INPUT );         //ensure A0 is an input
  lcd.begin( 16, 2 );
  lcd.clear();
}

void loop()
{
  Serial.println("Estamos leyendo el boton");
  lcd.setCursor(0,0); lcd.print("Boton");
    botones = idenBoton();
    int voltaje = analogRead(BOTON_PIN_ADC);
    if (botones == BOTON_NO) {
      while (botones != BOTON_NO) {
        botones = idenBoton();
      }
    }
    if (botones != botonEdoUlt && botones != BOTON_NO)
    {
      ulTimer = millis();

      if (millis() - ulTimer > apretarLargo && largo == true) {
        largo = false;
        botonEdo = botones;
        Serial.print(botones); Serial.print(" : "); Serial.print(voltaje); Serial.print(" : "); Serial.print("Largo");
        lcd.setCursor(6,0); lcd.print(botones);
        lcd.setCursor(0,1); lcd.print(voltaje);
        lcd.setCursor(8,1); lcd.print("LARGO");
      }
    }

    if (botones == BOTON_NO && botonEdoUlt != BOTON_NO) {
      if (largo == false) {
        largo = true;
      } else {
        botonEdo = botones;
        Serial.print(botones); Serial.print(" : "); Serial.println(voltaje); Serial.print(" : "); Serial.print("Corto");
        lcd.setCursor(6,0); lcd.print(botonEdoUlt);
        lcd.setCursor(0,1); lcd.print(voltaje);
        lcd.setCursor(8,0); lcd.print("CORTO");
      }
    }

    if (botones == BOTON_NO && botonEdoUlt == BOTON_NO) {
      largo = false;
    }

    /*
    if (botones != BOTON_NO && botones == botonEdoUlt)
    {
      botonPresionado = true;
      botonLiberado = false;
      largo = true;
      if ((millis()-ulTimer) > apretarLargo)
      {
      }
    if ((millis()-ulTimer) > retrasoBut)
    {
      botones = idenBoton();
      if (botones != botonEdo && botones == BOTON_NO)
      {
        botonEdo = botones;
        Serial.print(botones);
        Serial.print(" : "); Serial.println(voltaje);
        lcd.setCursor(0,0); lcd.print(botones);
        lcd.setCursor(0, 1); lcd.print(voltaje);
      }
    }
    if (botones == BOTON_NO && botonEdoUlt != BOTON_NO)
    {
      botonLiberado = true;
      botonPresionado = false;
    }
  } */
  botonEdoUlt = botones;
}

byte idenBoton() {
  int botonVoltaje;
  uint8_t botones = BOTON_NO;   // return no boton pressed if the below checks don't write to btn

  //read the boton ADC pin voltage
  botonVoltaje = analogRead( BOTON_PIN_ADC );
  //sense if the voltage falls within valid voltage windows
  if( botonVoltaje <= ( DER_ADC + HISTERESIS ) && botonVoltaje >= (DER_ADC - HISTERESIS))
  {
    botones = BOTON_DER;
  }
  else if(   botonVoltaje >= ( ARR_ADC - HISTERESIS )
          && botonVoltaje <= ( ARR_ADC + HISTERESIS ) )
  {
    botones = BOTON_ARR;
  }
  else if(   botonVoltaje >= ( ABJ_ADC - HISTERESIS )
          && botonVoltaje <= ( ABJ_ADC + HISTERESIS ) )
  {
    botones = BOTON_ABJ;
  }
  else if(   botonVoltaje >= ( IZQ_ADC - HISTERESIS )
          && botonVoltaje <= ( IZQ_ADC + HISTERESIS ) )
  {
    botones = BOTON_IZQ;
  }
  else if(   botonVoltaje >= ( SELEC_ADC - HISTERESIS )
          && botonVoltaje <= ( SELEC_ADC + HISTERESIS ) )
  {
    botones = BOTON_SELEC;
  }
  return( botones );
}

