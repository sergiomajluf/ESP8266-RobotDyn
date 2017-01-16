# ESP8266-RobotDyn
Tutorial para configurar un Arduino con ESP8266 y controla dispositivos via Web. Internet de las Cosas o IoT



Paso 0: puente usb CH340.
Lo primero es tener funcionando el puente usb CH340.
En mi caso, además descubrí que sólo puedo usar Arduino IDE 1.6.9
Si uso la versión más reciente, no me funciona nada que no sea Genuino/Arduino de verdad.

Una vez que estamos OK, en la lista de Puertos veremos algo así:

Primer Paso: ESP

Para configurar el ESP, hay que instalar una librería:
https://github.com/esp8266/Arduino

Probé varias alternativas, y la única que me funciona hasta ahora es la del módulo Genérico. Esto no calza con la descripción de la placa que dice que tiene 8Mb, mientras que el ESP-01 creo que tiene 1 o 2 a lo sumo. Pero funciona.

En el hardware, los DIP Switches:
Upload: 00001110
Normal: 00001100

En esta tabla están todas las combinaciones “válidas”:

El código que vamos a subir al ESP es un Servidor Web.
El ESP estará esperando nuevos clientes. Cuando llegue un cliente, comparará la URL con un patrón conocido, y si el patrón calza, asignará un estado a una variable var.

Luego, responderá al cliente con un contenido html.

El código completo está en Github.

Para subir el código, poner los dip switches en modo upload, y presionar ambos botones Reset. Mientras presionas, darle upload al código (a la antigua usanza de Arduino), y soltarlos unos segundos después.

El upload se demora más que en modo Arduino normal, pero verás el avance en la consola, en letras rojas:

Para bootear el ESP en modo normal, debes cambiar el switches #7 a OFF. Esto porque el ESP debe tener el pin CHPD a tierra. Eso hace el switch 7.

Extrañamente, al no cambiar de vuelta el switch a veces funciona igual, pero no debería...

Tras hacer eso, tenemos un ESP conectado a la IP 10.0.1.20 (modificar para que calce con tu RED), que recibe un request web del tipo:
http://10.0.1.20/gpio/0
http://10.0.1.20/gpio/1

Ahora debiéramos prender un LED desde el ESP, pero eso lo haremos más adelante. Con el output de la consola ya sé qué el software está funcionando.

Segundo Paso: Arduino
Luego volví a cambiar los switches:
0011000

Esto pone la placa en modo “Arduino” normal.
Le puedes subir cualquier sketch. Voy a probar con un blink (desde los ejemplos de Arduino, sólo modificado para que imprima en la consola el estado del LED) que imprime en Serial.

Ahora, como el UART (USB) está conectado al arduino y no al ESP, no podemos ver la respuesta serial del ESP, aunque sigue estando ahí.

Nota: este pantallazo lo tomé antes de modificar el html para darle una mejor UI

Tercer Paso: controlar el LED desde ESP
Ahora hay que identificar cuál PIN físico es el que el servidor Web está modificando. Por el código sé que es el GPIO #2. En la placa están los pines de Arduino, pero también unos pines exclusivos del ESP. Si miras la placa por atrás
 vemos en la segunda fila GPIO 02, 12, 02 de nuevo. También se ve el 14 y el 16.

El microcontrolador ESP tiene muchos GPIO, creo que 10 0 más, pero depende en qué placa va, a veces se exponen sólo unos pocos, como este caso.

Volviendo al Web Server, está programado para encender y apagar el Pin 2 desde una URL

URL para apagar:

URL para encender:
Si notaste que cambié el cableado, fue el cable a tierra, saltándome la resistencia para que el LED brillara más en la foto no más  :)

Ojo que todo este tiempo lo que vemos en la consola serial del Arduino sigue siendo código Arduino.

Lo que vamos a hacer es leer un pin digital del arduino, y según el estado de ese pin, hacer algo en otro pin del arduino. Por ejemplo, encender un led, o controlar un relé 😉

Entonces, a cambiar el código Arduino.

Cuarto Paso: un botón virtual
Sabemos que el ESP prende y apaga un PIN. Eso a fin de cuentas, es un botón.
Para probar, creamos código Arduino donde un botón controla el estado de otro pin GPIO

Este código de Arduino simplemente lee un pin digital, y si el pin está HIGH, enciende el Led 13. Ojo que estamos usando INPUT_PULLUP en el pin que leemos (el pin botonVirtual #2) para simplificar la conexión eléctrica.

Recuerda que para subir código al Arduino, ahora los switches deben estar en ese modo: 0011000

Con esté código cargado en el Arduino, podemos probar. En este gif vemos que hay DOS LEDs que prenden y apagan simultáneamente:

El LED amarillo está conectado al ESP, y se enciende porque el GPIO02 cambia a HIGH.

El LED azul en cambio, es el LED 13 del Arduino, y se enciende porque el software lo controla. Sólo cuando el pin 2 del Arduino está HIGH, hacemos que el pin13 lo esté.

¿No es un despropósito hacer esa vuelta si podríamos controlar todo directamente desde el ESP? Sí.

La gracia estará cuando hagamos que el código de Arduino sea más complejo. Por ejemplo que tenga un timer. O que lea una combinación de otros pines. O que maneje simultáneamente varios pines OUTPUT con sólo uno INPUT… etc.

Quinto Paso: adiós LED, hola relé

Con el código OK, eliminamos todas las conexiones, y las reemplazamos por el relé. Esta placa requiere al menos 3 conexiones: tierra, 5v, y 1 pin digital por cada uno de los cuatro revés.

En este caso, ese pin digital es el 13. Podríamos usar cualquier otro, pero el código Arduino que está cargado ya está así.

¡¡¡Sube el volumen!!!

Finalmente, algo que hicimos antes, pero no habíamos mostrado.
La interfaz desde un móvil, se ve así:

