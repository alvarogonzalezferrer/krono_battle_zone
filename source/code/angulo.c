//----------------------------------------------------------- 
// angulo.c << RUTINAS DE CALCULO MATEMATICO EN GENERAL
// Codigo que contiene rutinas muy utiles para el calculo
// de posiciones de x,y de angulos, etc
// Ademas, incluye la funcion 'potencia' para potenciar un numero a otro
// y 'al_azar' que permite elegir un numero al azar dentro de un rango
// Todo por Kronoman
// (c) 2001
// DJGPP
//----------------------------------------------------------- 

#ifndef ANGULO_C
#define ANGULO_C

//-----------------------------------------------------------
// RUTINAS - CODIGO
//----------------------------------------------------------- 

//-----------------------------------------------------------
// Rutina que devuelve un angulo dentro del rango de 0-360
// aun si el angulo esta fuera de rango
//-----------------------------------------------------------
int arregla_ang(int angulo)
{
        int i;
if ((angulo>-1) && (angulo<361)) return angulo; // si esta bien, devolverlo igual

        i = angulo - (360 * (angulo/360));
        if (i>360) i -= 360;
        if (i<0)   i += 360;
        return i;
}
//-----------------------------------------------------------


//----------------------------------------------------------- 
// Rutina que devuelve un angulo dentro del rango de 0-255
// dandole un angulo de 0-360; util para rutinas de allegro
// Luego convertir este entero a fixed usando itofix(xxx)
//-----------------------------------------------------------
int pasa360a255(int angulo)
{
        int i;
        i = arregla_ang(angulo); // acomodo el angulo a 0-360
//        i = i * 255 / 360;
        i = i * GRADOSRELAT; // relacion de 0.70833333 etc, predefinida
        return i;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
// Rutina que devuelve un angulo dentro del rango de 0-360
// dandole un angulo de 0-255; util para rutinas de allegro
// Pasar un entero convirtiendo con fixtoi(xxx)
//-----------------------------------------------------------
int pasa255a360(int angulo)
{
        int i;
i = angulo;
// ajustar
if (i>255) i -= 255;
if (i<0)   i += 255;

//        i = i * 360 / 255;
        i = i / GRADOSRELAT; // relacion de 0.70833333 etc, predefinida
        return i;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
// Rutina que devuelve una estructura con las coordenada x,y destino
// para trazar una linea de x1, y1 en angulo ang con largo 
// el angulo va en un valor de 0 a 360
// USA TABLAS PRE-CALCULADAS: KT_SIN, KT_COS!
//-----------------------------------------------------------

ANGULO_TYPE dame_angc360(int ang, int largo, int xx1, int yy1)
{
ANGULO_TYPE tmp; // estructura temporal

// seteo la estructura
tmp.x1 = xx1;
tmp.y1 = yy1;

tmp.ang = arregla_ang(ang); // toma el angulo en rango 0-360 solamente
tmp.largo = largo;

// calculo la x2, y2

// tmp.y2 = sin(tmp.ang * KRONO_PI) * tmp.largo;
tmp.y2 = KT_SIN[tmp.ang] * tmp.largo; // uso tabla pre-calculada
tmp.y2 = tmp.y1 - tmp.y2;

// tmp.x2 = cos(tmp.ang * KRONO_PI) * tmp.largo;
tmp.x2 = KT_COS[tmp.ang] * tmp.largo; // tabla pre-calculada
tmp.x2 = tmp.x2 + tmp.x1;

return tmp;
}
//----------------------------------------------------------- 

//----------------------------------------------------------- 
//Funcion potencia
//Pasarle dos numeros, y devuelve el 1ero elevado al 2ndo
// Devuelve long asique ojo!
//----------------------------------------------------------- 

long potencia(int base, int exponente)
{
   long temporal = 1;        /* Valor que voy hallando */
   int i;                   /* Para bucles */

   for(i=1; i<=exponente; i++)
     temporal *= base;

   return temporal;
}
//----------------------------------------------------------- 

//----------------------------------------------------------- 
//Funcion al_azar
//Pasarle dos numeros, y devuelve uno al azar entre ellos
// pueden ser negativos o positivos
// SIEMPRE pasar el menor primero y el mayor despues
// El generador de numeros aleatorios debe estar previamente iniciado
//-----------------------------------------------------------
int al_azar(int num1, int num2)
{
int i1;
// si estan al reves, dar vuelta 
if (num1 > num2)
{
    i1 = num2;
    num2 = num1;
    num1 = i1;
}

    i1 = num2 - num1 + 1;
    i1 = (random() % i1) + num1;
    return i1;
}
//----------------------------------------------------------- 


#endif
