// angulo.h Header para angulo.c
// Contiene definiciones y los prototipos de las rutinas

// Primero me fijo si no definio esto previamente...
#ifndef ANGULO__H
#define ANGULO__H


// tablas pre-calculadas

// esto NECESITA ser optimizado!!!
// KT_COS, KT_SIN, KT_TAN es por lo de Krono Tabla Coseno, Seno, Tangente... ok?
double KT_COS[361]; // 0-360 = 361
double KT_SIN[361]; // 0-360 = 361
double KT_TAN[361]; // 0-360 = 361

// definicion de PI/180 como constante para acelerar todo...
#define KRONO_PI      0.017453292

// definicion de la relacion entre 255/360 para pasar
// de grados normales a 255 y viceversa
// angulo 0-360 * GRADOSRELAT = angulo de 0-255
// angulo 0-255 / GRADOSRELAT = angulo de 0-360
#define GRADOSRELAT   0.708333333

// Definicion de estructura usada para las coordenadas
// Tambien la usa la rutina de coordenadas de angulo
typedef struct
{
 int x1, y1; // coordenada x, y origen
 int x2, y2;    // coordenada x,y resultante
 int ang; // angulo usado
 int largo; // largo usado
} ANGULO_TYPE;

//-----------------------------------------------------------
// Prototipos de las rutinas
//----------------------------------------------------------- 
int arregla_ang(int angulo); // ajusta a rango 0-360
int pasa360a255(int angulo); // pasa de 360 a 255
int pasa255a360(int angulo); // pasa de 255 a 360

// devuelve coordenadas para una linea en angulo
ANGULO_TYPE dame_angc360(int ang, int largo, int xx1, int yy1);

// elige un numero entre num1 y num2 al azar
int al_azar(int num1, int num2);


#endif // fin de ver si ya definio todo

