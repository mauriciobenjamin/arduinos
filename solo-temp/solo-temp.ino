#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ONE_WIRE_BUS 5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SECS_PER_MIN (60UL)
#define SECS_PER_HOUR (3600UL)
#define cardSelect 4
#define PinRele 6

#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (_time_ / SECS_PER_HOUR)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempSensor;

File registro;

#define ONE_WIRE_BUS 5

double Input;
unsigned long acumulado;
enum estados { CALENTAR = 0, ENFRIAR};
estados estado = CALENTAMIENTO;

#define PinRele 6

void setup() {
  acumulado = millis()/1000;
  Serial.begin(9600);
  Serial.println("Prueba del sensor de temperatura");
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Fallo la localización de la SSD1306"));
    for(;;);
    }

  Serial.println("Iniciando la tarjeta SD...");
  if (!SD.begin(cardSelect)) {
    Serial.println(F("Fallo la tarjeta o no hay"));
    while(1);
  }
  Serial.println("Tarjeta inicializada");
  
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
  
  Serial.println("Tiempo, Temperatura");
}

void loop() {
  sensors.requestTemperatures();
  Input = sensors.getTempC(tempSensor);

  display.clearDisplay();
  display.setCursor(10,16);
  display.setTextSize(2);
  display.print(F("T: "));
  display.println(Input);
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(10,34);

  unsigned long now = millis()/1000;
  String tiempo = time(now);
  display.println(tiempo);
  display.display();
  if (now - acumulado > 5) {
    acumulado += 5;
    String data = tiempo + ", " + String(Input);
    registro = SD.open("registro.csv", FILE_WRITE);
    if(registro) {
      registro.println(data);
      registro.close();
      Serial.println(data);
    } else {
      Serial.println("Error al registrar");
    }
  }

  if(estado == CALENTAR) {
    calentar(Input, now);
  }
}

String time(unsigned long val) {
  int horas = numberOfHours(val);
  int minutos = numberOfMinutes(val);
  int segundos = numberOfSeconds(val);
  String texto = String(digits(horas) + ":" + digits(minutos) + ":" + digits(segundos));
  return texto;
}

String digits(int num) {
  if(num < 10){
    return String("0" + String(num));
  } else {
    return String(num);
  }
}

void calentar(tem) {
  if(tem >= 94) {
    digitalWrite(pinRele, LOW);//cambiar el estado del relevador
    estado = ENFRIAR;
  } else {
    //relevador prendido
    digitalWrite(pinRele, HIGH);
  }
}

