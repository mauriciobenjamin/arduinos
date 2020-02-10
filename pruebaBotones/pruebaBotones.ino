#define pinBotones A0

byte botonPresionado = false;
byte botonLiberado = false;
byte edoBoton = false;

void setup() {
  // put your setup code here, to run once:
  pinMode (pinBotones, INPUT);
  digitalWrite(pinBotones, LOW);
  Serial.begin(9600);
  Serial.println("Prueba botones");
}

void loop() {
  // put your main code here, to run repeatedly:
  byte boton;
  byte marcatiempo;
  int voltajeBoton = analogRead(pinBotones);
  Serial.println(voltajeBoton);
  delay (1000);
}
