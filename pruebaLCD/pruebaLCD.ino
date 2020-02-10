#include <LiquidCrystal.h>

#define BOTON_PIN_ADC           A0  // A0 is the boton ADC input
// ADC readings expected for the 5 botons on the ADC input
#define DER_ADC           6  // derecha
#define ARR_ADC            152  // up
#define ABJ_ADC          335  // down
#define IZQ_ADC          498  // left
#define SELEC_ADC        707  // derecha
#define HISTERESIS         3  // hysteresis for valid boton sensing window
//return values for leerBotones()
#define BOTON_NO               0  //
#define BOTON_DER              1  //
#define BOTON_ARR              2  //
#define BOTON_ABJ              3  //
#define BOTON_IZQ               4  //
#define BOTON_SELEC             5  //
/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte botonPresionado  = false;         //this will be true after a leerBotones() call if triggered
byte botonLiberado = false;         //this will be true after a leerBotones() call if triggered
byte botonEdo          = BOTON_NO;   //used by leerBotones() for detection of boton events
uint8_t etapas = 1;
/*--------------------------------------------------------------------------------------
  Init the LCD library with the LCD pins to be used
--------------------------------------------------------------------------------------*/
LiquidCrystal lcd( 11, 12, 5, 6, 9, 10 );   //Pins for the freetronics 16x2 LCD shield. LCD: ( RS, E, LCD-D4, LCD-D5, LCD-D6, LCD-D7 )

/*-------
Estados de la máquina de estados
------------------------------------------------*/
enum estados { INICIO=0, PROGRAMAR };
estados opEstados = INICIO;



void setup() {
  // put your setup code here, to run once:
  pinMode( BOTON_PIN_ADC, INPUT );         //ensure A0 is an input
   //set up the LCD number of columns and rows:
   lcd.begin( 16, 2 );
   //Print some initial text to the LCD.
   lcd.setCursor( 0, 0 );   //top left
   //          1234567890123456
   lcd.print( "PRUEBA" );
   //
   lcd.setCursor( 0, 1 );   //bottom left
   //          1234567890123456
   lcd.print( "    Ambiental   " );
   delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(leerBotones() != 0) {};
    lcd.clear();
    
    switch (opEstados) {
      case INICIO:
        inicio();
        break;
      case PROGRAMAR:
        programar();
        break;
    }
    uint8_t botones = leerBotones();
}

void inicio() {
  byte botones;
  lcd.clear();
  lcd.home();
  lcd.print("Derecha");
  while(!(botones & (BOTON_DER))) {
    botones = leerBotones();
  }
  opEstados = PROGRAMAR;
}

void programar() {
  byte botones;
  lcd.clear();
  lcd.home();
  lcd.print("Cuanto");
  lcd.setCursor(4,1);
  int cuanto=1;
  while(true) {
    botones = leerBotones();
    if (botones == BOTON_IZQ) {
      cuanto ++;
      delay(200);
    }
    if (botones == BOTON_DER) {
      cuanto --;
      delay(200);
    }
    if (botones == BOTON_ARR) {
      cuanto += 5;
      delay(200);
    }
    if (botones == BOTON_ABJ) {
      cuanto -= 5;
      delay(200);
    }
    lcd.setCursor(5,1);
    lcd.print(cuanto);
  }
}

byte leerBotones()
{
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
   //handle boton flags for just pressed and just released events
   if( ( botonEdo == BOTON_NO ) && ( botones != BOTON_NO ) )
   {
      //the boton was just pressed, set botonPresionado, this can optionally be used to trigger a once-off action for a boton press event
      //it's the duty of the receiver to clear these flags if it wants to detect a new boton change event
      botonPresionado  = true;
      botonLiberado = false;
   }
   if( ( botonEdo != BOTON_NO ) && ( botones == BOTON_NO ) )
   {
      botonPresionado  = false;
      botonLiberado = true;
   }

   //save the latest boton value, for change event detection next time round
   botonEdo = botones;

   return( botones );
}
