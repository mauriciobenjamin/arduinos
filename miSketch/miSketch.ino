// Librerias de sensor de temperatura DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

// Libreria del controlador PID
#include <PID_v1.h>

// Librearías para la pantalla OLED SSD1306 de 128x64px
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Valor del pin de termometro
#define ONE_WIRE_BUS 5

// Declaración del display via conexión I2C conectando en los pines SDA, SCL
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Valores para la configuración del sensor del termometro
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempSensor;

// Variables de control para el PID
double Input, Setpoint, Output;
// Inicialización del PID 
PID miPid(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);

int ventana = 10000;
unsigned long ventanaInicial;

#define PinRele 6

void setup() {
  ventanaInicial = millis();

  // Temperatura que se debe de mantener
  Setpoint = 45;
  
  // Configuración de la ventana de tiempo ya que la biblioteca PID esta diseñada para salidas analogas, y se debe adaptar para el uso de relevadores
  miPid.SetOutputLimits(0, ventana);
  // Inicia el PID
  miPid.SetMode(AUTOMATIC);

  Serial.begin(9600);
  Serial.println("Prueba del sensor de temperatura");
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Fallo la localización de la SSD1306"));
    for(;;);
    }
  
  display.display(); // muestra el splashscreen de adafruit
  delay(2000);
  display.clearDisplay();

  pinMode(PinRele, OUTPUT);
  digitalWrite(PinRele, LOW);

  sensors.begin();
  if(!sensors.getAddress(tempSensor, 0))
  {
    Serial.println("El sensor no esta disponible");
    display.clearDisplay();
    display.setTextSize(2); // Establece el tamaño de texto en 2
    display.setCursor(0, 0);
    display.println(F("El sensor no esta disponible"));
  }
  sensors.setResolution(tempSensor, 12);
  sensors.setWaitForConversion(false);
}

void loop() {
  sensors.requestTemperatures();
  Input = sensors.getTempC(tempSensor);
  Serial.print("Temperatura: ");
  Serial.println(Input);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0,0);
  display.print(F("Objetivo: "));
  display.println(Setpoint);
  display.setCursor(0,16);
  display.setTextSize(2);
  display.print(F("T: "));
  display.println(Input);
  display.display();

  miPid.Compute();

  unsigned long now = millis();

  if (now - ventanaInicial > ventana) {
    ventanaInicial += ventana;
  }
  if (Output > now - ventanaInicial) digitalWrite(PinRele, LOW);
  else digitalWrite(PinRele, HIGH);
}
