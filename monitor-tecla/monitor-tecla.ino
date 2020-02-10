#include <LiquidCrystal.h>

LiquidCrystal lcd( 5, 6, 9, 10, 11, 12 );
const int tecla = A0;
#define DER_ADC          1   // derecha 4
#define ARR_ADC          146  // up 154
#define ABJ_ADC          329  // down 344
#define IZQ_ADC          491  // left 513
#define SELEC_ADC        697  // derecha 728
#define NO_ADC           911  // Ningun boton 943
#define HISTERESIS       35
const byte BOTON_NO        = 0;
const byte BOTON_DER       = 1;
const byte BOTON_ARR       = 2;
const byte BOTON_ABJ       = 3;
const byte BOTON_IZQ       = 4;
const byte BOTON_SELEC     = 5;
int miTecla = 0;
int teclaSUM;
int teclaA;
int i;
byte botonEdo   = BOTON_NO;
byte botonEdoUlt = botonEdo;
byte boton = BOTON_NO;
uint voltAnt = BOTON_NO;
uint botonVolt = BOTON_NO;
long tiempoAnt;
const long tiempoRef = 30;
bool cambio = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  // put your setup code here, to run once:

}

void loop() {
  miTecla = analogRead(tecla);
  Serial.println(miTecla);
  /* Verficar voltaje de cada tecla*/
  lcd.setCursor(0,0);
  lcd.print(miTecla);
  delay(50);
  if(teclaA <= miTecla + 10 && teclaA >= miTecla - 10) {
    teclaSUM += miTecla;
    i++;
    lcd.setCursor(0,1); lcd.print("   ");
    lcd.print(teclaSUM/i);
    Serial.println(teclaSUM/i);
  } else {
    teclaSUM = 0;
    i = 0;
  }
  teclaA = miTecla;
  Serial.println(teclaA);
  /*Contador de pulsaciones por botón*/
  uint8_t botones = 0;
  byte derecha=0;
  byte izquierda=0;
  byte arriba=0;
  byte abajo=0;
  byte selec=0; //                       0123456789012345
  lcd.setCursor(5,0); lcd.print("D I Ar Ab S");
  while(true){
    botones = leerBotones();
    if(botones == BOTON_IZQ) {
      izquierda ++;
      lcd.setCursor(7,1); lcd.print(izquierda);
    }
    if(botones == BOTON_DER) {
      derecha ++;
      lcd.setCursor(5,1); lcd.print(derecha);
    }
    if(botones == BOTON_ARR) {
      arriba ++;
      lcd.setCursor(10,1); lcd.print(arriba);
    }
    if(botones == BOTON_ABJ) {
      abajo ++;
      lcd.setCursor(13,1); lcd.print(abajo);
    }
    if(botones == BOTON_SELEC) {
      selec ++;
      lcd.setCursor(15,1); lcd.print(selec);
    }
   }
  /*botones = leerBotones();

    if (botones == BOTON_IZQ) {
      Serial.println("IZQUIERDA");
    }
    if (botones == BOTON_DER) {
      Serial.println("DERECHA");
    }
    if (botones == BOTON_ABJ) {
      Serial.println("ABAJO");
    }
    if (botones == BOTON_ARR) {
      Serial.println("ARRIBA");
    }
    if (botones == BOTON_SELEC) {
      Serial.println("SELECCIONA");
    }
    Serial.println(millis()); Serial.print(" ");
    Serial.println(botonVolt);

    lcd.clear(); lcd.home(); lcd.print(botones);
    lcd.setCursor(0,1); lcd.print("Voltaje: "); lcd.print(botonVolt);*/

}

byte leerBotones() {
  botonVolt = analogRead(tecla);
  if(botonVolt <= voltAnt + HISTERESIS && botonVolt >= voltAnt - HISTERESIS){
    cambio = false;
  } else if(botonVolt <= voltAnt + 5 && botonVolt >= voltAnt) {
     cambio = false;
  } else {
    cambio = true;
  }
  if(cambio) {
    if(botonVolt <= ( DER_ADC + HISTERESIS ))
    {
      botonEdo = BOTON_DER;
    }
    else if(botonVolt >= ( ARR_ADC - HISTERESIS )
            && botonVolt <= ( ARR_ADC + HISTERESIS ) )
    {
      botonEdo = BOTON_ARR;
    }
    else if(botonVolt >= ( ABJ_ADC - HISTERESIS )
            && botonVolt <= ( ABJ_ADC + HISTERESIS ) )
    {
      botonEdo = BOTON_ABJ;
    }
    else if(botonVolt >= ( IZQ_ADC - HISTERESIS )
            && botonVolt <= ( IZQ_ADC + HISTERESIS ) )
    {
      botonEdo = BOTON_IZQ;
    }
    else if(botonVolt >= ( SELEC_ADC - HISTERESIS )
            && botonVolt <= ( SELEC_ADC + HISTERESIS ) )
    {
      botonEdo = BOTON_SELEC;
    } else {
      botonEdo = BOTON_NO;
    }
    if(botonVolt > NO_ADC) {
      botonEdo = BOTON_NO;
    }
    voltAnt = botonVolt;
  }
  if ((millis() - tiempoAnt) > tiempoRef && botonEdoUlt == BOTON_NO) {
    boton = botonEdo;
  } else {
    boton = BOTON_NO;
  }
  if (cambio) {
    tiempoAnt = millis();
  }
  if (!cambio) {
      boton = BOTON_NO;
  }
  botonEdoUlt = botonEdo;
  return boton;
}
