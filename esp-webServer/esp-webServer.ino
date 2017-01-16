/*
  Instala un WebServer simple en el ESP
  La dirección IP está configurada como estática, para saber
  siempre a qué URL acceder.

  Debemos conocer el DHCP, nombre de red y contraseña Wifi
  antes de proceder.

  Conociendo la IP, podremos controlar el ESP de esta forma,
  para encender y apagar sólo un pin GPIO:
  http://10.0.1.20/gpio/0 // Apaga el pin
  http://10.0.1.20/gpio/1 // Enciende el pin


  Este codigo es parte del ejemplo con el que controlamos
  arduino desde una señal enviada por el ESP, que a su vez
  recibe la instrucción – request – desde un cliente web

  Autor: Sergio Majluf
  Enero de 2017
*/

#include <ESP8266WiFi.h>
#include "password.h"


// Configura la red con IP estática
IPAddress ip(10, 0, 1, 20);
IPAddress gateway(10, 0, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Ref http://www.esp8266.com/viewtopic.php?p=21391#sthash.6d3OXDZ1.dpuf


// Crea el servidor y asigna el puerto 80 (default web)
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // usaremos el pin GPIO2
  // En esta palca podemos usar 2, 4, 12, 16
  pinMode(2, OUTPUT);

  // Escribe LOW al pint 2
  digitalWrite(2, 0);

  // Conectar al Wifi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi conectado");

  // Start the server
  server.begin();
  Serial.println("Servidor iniciado");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Si hay un cliente conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Esperar que el cliente envíe datos
  Serial.println("nuevo cliente");
  while (!client.available()) {
    delay(1);
  }

  // Leer la primera linea del request hasta que encuentra un
  // "carriage return", es decir, un salto de linea
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Compara el request con lo que nosotros esperamos
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("Request invalido. Error 404");
    client.stop();
    return;
  }

  // Asigna el valor de val al GPIO2
  digitalWrite(2, val);

  client.flush();

  // Preparar la respuesta para el header HTTP
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE html><html> <head> <meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no'> <title>Controlador GPIO</title> <link href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'> <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js' integrity='sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa' crossorigin='anonymous'></script> </head> <body><div class='container'><div class='page-header'> <h1 class=''>ESP Control</h1><p>Controla el universo desde tu ESP/Arduino</p></div><div class='panel panel-default'> <div class='panel-heading'> <h3 class='panel-title'>GPIO est&aacute; ";
  s += (val) ? "ON" : "OFF";
  s += "</h3> </div> <div class='panel-body'> <div class='btn-group btn-group-justified' role='group' aria-label=''><a class='btn";
  s += (val) ? " btn-success" : "";
  s += "' href='/gpio/1'>ON </a><a class='btn";
  s += (val) ? "" : " btn-success";
  s += "' href='/gpio/0'>OFF</a></div> </div></div></div></body></html>";


  // Envia la respuesta HTTP al cliente
  client.print(s);
  delay(1);
  Serial.println("Cliente desconectado");

  // El cliente será desconectado cuando la función acabe y
  // el objeto 'cliente' será destruido. Listo para volver a empezar
}

