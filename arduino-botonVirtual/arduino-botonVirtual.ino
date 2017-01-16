/*
  Un botón conectado al Arduino controla un LED
  Un INPUT conectado al Arduino controla un OUTPUT

  Este codigo es parte del ejemplo con el que controlamos
  Arduino desde una señal externa enviada por el ESP,
  que a su vez recibe la instrucción – request – desde un cliente web

  Autor: Sergio Majluf
  Enero de 2017

*/

const int botonVirtual = 2;
const int ledPin =  13;
int estadoBoton = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  // Usamos Pullup para conectar directamente ambos MCU
  pinMode(botonVirtual, INPUT_PULLUP);
}

void loop() {
  estadoBoton = digitalRead(botonVirtual);
  // si estadoBoton es HIGH
  if (estadoBoton == HIGH) {
    // Enciende LED
    digitalWrite(ledPin, HIGH);
  } else {
    // Apaga LED
    digitalWrite(ledPin, LOW);
  }
}
