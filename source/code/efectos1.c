//--------------------------------------------------------
// efectos1.c
// efectos1.h
// rutinas de efectos visuales simples
// Huellas del tanque, Proyector de particulas, Quemazon de las paredes
// y vaporizacion de paredes };^P
// NOTAR QUE SI VER_PARTI == FALSE, NO SE VEN LAS PARTICULAS!!!
//--------------------------------------------------------

#ifndef EFECTOS1_C
#define EFECTOS1_C

//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------

// SISTEMA DE PARTICULAS
static int cantidad_particulas = 0; // cantidad de particulas

// NOTAR QUE EL TIPO 'fixed' pertenece a Allegro
// es para usar punto flotante falso, o sea un 'float' entero ;D
// usar itofix, etc para convertir
static struct {
 fixed x;  // posicion x particula 0-1000
 fixed y;  // posicion y particula 0-1000
 fixed dx; // direccion x (punto flotante)
 fixed dy; // direccion y (punto flotante)
 int vida; // vida que le queda a la particula, al llegar a 0 es liberada
 int color; // color 0-255
 int tipo; // tipo de particula
	   // 0 = punto, 1 = circulo, 2 = chispa
} particula[MAX_PARTICULAS]; // desde 0 a MAX_PARTICULAS - 1

//--------------------------------------------------------
// void dejar_huella_t(int numjug)
// marca en el bitmap Areajuego_bmp huellas en la posicion del tanque
// El poligono que rodea al tanque debe haber sido creado previamente
// a llamar a esto. Ok?
//--------------------------------------------------------
void dejar_huella_t(int numjug)
{

circlefill(Areajuego_bmp, 
           jugador_info[numjug].xpol[5], 
           jugador_info[numjug].ypol[5],random()%2+1,
           getpixel(Areajuego_bmp, 
           jugador_info[numjug].xpol[5], 
           jugador_info[numjug].ypol[5]));

circlefill(Areajuego_bmp, 
           jugador_info[numjug].xpol[7], 
           jugador_info[numjug].ypol[7],random()%2+1,
           getpixel(Areajuego_bmp, 
           jugador_info[numjug].xpol[7], 
           jugador_info[numjug].ypol[7]));

} // fin dejar_huella_t
//--------------------------------------------------------

//--------------------------------------------------------
// Grafica la pared completamente destruida
// Usar luego de destruir la pared
// Las coordenadas x, y son en GRILLA 0..49, 0..49
//--------------------------------------------------------
void destruir_pared(BITMAP *bmp, int x, int y)
{
BITMAP *tmp; // bitmap para crear la pared destruida...
int x1, y1; // para el for

// calculo la pos real en pixeles
x *= SPR_X_MAX;
y *= SPR_Y_MAX;


// quemar el bmp real
quemar_pared(bmp, x + (SPR_X_MAX / 2),y + (SPR_Y_MAX / 2), random()%12 + 12, BALAZO);
quemar_pared(bmp, x + (SPR_X_MAX / 2),y + (SPR_Y_MAX / 2), random()%10 + 10, PULVERIZAR);

// crear bmp con tama¤o de los sprites
    tmp = create_bitmap(SPR_X_MAX, SPR_Y_MAX);
// poner el piso en tmp

blit(krono_dato[ dame_sprite_piso(info_mapa_juego.terreno_gral) ].dat,
     tmp, 0, 0, 0, 0, SPR_X_MAX, SPR_Y_MAX);

// ahora, copiar paso a paso, partes si, partes no al bmp de pantalla
// de esa manera, queda todo cool...
for (x1 = 0; x1 < tmp->w; x1++) 
 for (y1 = 0; y1 < tmp->h; y1++) 
    if (random () % 100 < 65 )
        {
        putpixel (bmp, x1 + x, y1 + y, getpixel(tmp, x1, y1) ); // remover pared con piso
        }
    else
        {
        if ( (x1 + x) < (bmp->w - 1) ) // evita unos puntos rosas sospechosos... ;^P
            {
            agregar_particulas(x1+ x, y1+ y,
                               random()%10 + 15,
                               random()%10 + 5,
                               3,
                               getpixel(bmp, x1 + x, y1 + y),
                               getpixel(bmp, x1 + x, y1 + y));         // agregar particulas de destruccion en color de la pared...
            }
        }

destroy_bitmap(tmp); // liberar memoria


}


//--------------------------------------------------------
// 'Quema' un area del mapa en x,y (coordenadas de pixel!)
// fuerza es la potencia de la quemazon, 10 queda bien
// tipo_efecto es BALAZO o PULVERIZAR, y define el efecto
// grafico del impacto en la pared.-
//--------------------------------------------------------
void quemar_pared(BITMAP *bmp, int x, int y, int fuerza, int tipo_efecto)
{
int i = 0;
int c = random() % fuerza + 30;
int z1, z2, z3, z4;

int x1,y1,x2,y2;


// hace un efecto 'licuadora' que parece que se desmorona la pared...
for (i=0; i < c; i++)
    {
    // elegir desplazamiento al azar
        z1 = random() % fuerza;
        z2 = random() % fuerza;        
        z3 = random() % fuerza;        
        z4 = random() % fuerza;        
    if ((random()%10) < 5) z1 = -z1 ;
    if ((random()%10) < 5) z2 = -z2 ;
    if ((random()%10) < 5) z3 = -z3 ;
    if ((random()%10) < 5) z4 = -z4 ;


    x1 = x + z1;
    x2 = x + z3;

    y1 = y + z2;
    y2 = y + z4;

// comprobar que no salga fuera de pantalla!
    if (x1 > bmp->w - 1) x1 = bmp->w - 1;
    if (x2 > bmp->w - 1) x2 = bmp->w - 1;
    if (x1 < 0 ) x1 = 0;
    if (x2 < 0 ) x2 = 0;

    if (y1 > bmp->h - 1) y1 = bmp->h - 1;
    if (y2 > bmp->h - 1) y2 = bmp->h - 1;
    if (y1 < 0 ) y1 = 0;
    if (y2 < 0 ) y2 = 0;

        switch (tipo_efecto) {
        case PULVERIZAR:
            circlefill(bmp,
                       x1,
                       y1,
                       (random() % 2) + 1,
                       getpixel(bmp, x2, y2) );
        break;
        case BALAZO:
             putpixel (bmp, x1, y1, getpixel(bmp, x2,y2) );
        break;
        }

    }

}


//--------------------------------------------------------
// reset_particulas()
// esto 'resetea' el sistema de particulas
// llamar cada vez que termina el juego
// de lo contrario habra particulas al empezar a jugar de nuevo
//--------------------------------------------------------
void reset_particulas()
{
int i1; // para el for

cantidad_particulas = 0; // cantidad de particulas
for (i1=1; i1 < MAX_PARTICULAS; i1++)
{
 particula[i1].x = 0;
 particula[i1].y = 0;
 particula[i1].dx = 0;
 particula[i1].dx = 0;
 particula[i1].vida = 0;
 particula[i1].color = 0;
 particula[i1].tipo = 0;
}

} // fin reset_particulas()
//--------------------------------------------------------

//--------------------------------------------------------
//void dibujar_particulas(BITMAP *bmp, int x, int y)
//Rutina que dibuja las particulas ajustadas a la ventana x, y
//Pasarle el bitmap donde se desea que lo dibuje
//-------------------------------------------------------- 
void dibujar_particulas(BITMAP *bmp, int x, int y)
{
int i1; // para el for

// si las particulas estan OFF, abortar
if (VER_PARTI == FALSE) return;

// notar que no comprueba si estan dentro de pantalla
// de eso se encargan las rutinas de graficos
// seria al pedo - gastar 2 comprobaciones
for (i1=1; i1 < cantidad_particulas; i1++)
{

if (particula[i1].vida < 1) particula[i1].tipo = -1; // particula muerta

if (particula[i1].tipo == 0)
            putpixel (bmp,
                     fixtoi(particula[i1].x) - x,
                     fixtoi(particula[i1].y) - y,
                     particula[i1].color);

if (particula[i1].tipo == 1)
            circlefill (bmp,
                      fixtoi(particula[i1].x) - x,
                      fixtoi(particula[i1].y) - y, 1,
                      particula[i1].color);

if (particula[i1].tipo == 2)
                line (bmp,
                      fixtoi(particula[i1].x) - x,
                      fixtoi(particula[i1].y) - y,
                      fixtoi(fadd(particula[i1].x, particula[i1].dx)) - x ,
                      fixtoi(fadd(particula[i1].y, particula[i1].dy)) - y,
                      particula[i1].color);
// nota: fadd(x,y) es igual a x+y, pero con fixed... :^)


} // fin for
// LISTO!

} // fin dibujar_particulas
//--------------------------------------------------------



//--------------------------------------------------------
// void mover_particulas()
// Rutina que MUEVE las particulas 
// y libera la memoria de las particulas que ya no se usan
// para permitir el uso de nuevas
// llamar cuando actualiza la logica del juego
//-------------------------------------------------------- 
void mover_particulas()
{
int i1, i2; // para el for, etc

// si las particulas estan OFF, abortar
if (VER_PARTI == FALSE) return;


i2 = cantidad_particulas;

for (i1=1; i1<cantidad_particulas; i1++)
{
// uso fadd porque son numeros fixed
// igual los podes sumar con +, pero no comprueba el overflow
//particula[i1].x +=  particula[i1].dx;
//particula[i1].y +=  particula[i1].dy;

particula[i1].x = fadd(particula[i1].x, particula[i1].dx);
particula[i1].y = fadd(particula[i1].y, particula[i1].dy);


particula[i1].vida--;

if (particula[i1].vida < 1)
    {
    // particula muerta, cambiar con la ultima viva el lugar
    // y eliminar
     particula[i1] = particula[i2];
     i2--;
     if (i2<0) i2=0;
    }
} // fin for
cantidad_particulas = i2;
// LISTO!
} // fin mover_particulas
//--------------------------------------------------------

//--------------------------------------------------------
//void agregar_particulas(int x, int y, int vida, cantidad
//                           int tipo, int colorr1,
//                           int colorr2)
//
// Esta rutina agrega particulas
// en la  posicion 'x,y' de duracion 'vida', de 'cantidad'
// de 'tipo', y en un rango de color de 'colorr1' a 'colorr2'
// 'tipo' puede ser:
// 0 = puntos
// 1 = circulos
// 2 = lineas
// 3 = al azar
// 4 = combinado 0 y 1
// 5 = combinados 0,1,2
//
// ATENTO: esto debe ser:
// color1 < colorr2 
//
// *** NOTA ***: USA LAS TABLAS PRECALCULADAS 'KT_TAN'
//--------------------------------------------------------

void agregar_particulas(int x, int y, int vida,
                        int cantidad, int tipo, int colorr1,
                        int colorr2)
{
int i1, i2; // para el for, etc

// si las particulas estan OFF, abortar
if (VER_PARTI == FALSE) return;

if (cantidad_particulas>MAX_PARTICULAS-1) return;

// crear hasta el maximo posible de particulas
if (cantidad_particulas + cantidad > MAX_PARTICULAS-1)
                          cantidad = MAX_PARTICULAS - 1 - cantidad_particulas;

if (tipo==3) tipo = random () % 3;


for (i1=cantidad_particulas; i1<cantidad_particulas + cantidad; i1++)
{
particula[i1].x = itofix(x);
particula[i1].y = itofix(y);

// elegir color
i2 = al_azar(colorr1, colorr2);
particula[i1].color = i2;

// velocidad
particula[i1].dx = ftofix((float)(al_azar(-500, 500)) / 100);
particula[i1].dy = ftofix((float)(al_azar(-500, 500)) / 100);

particula[i1].vida = vida;

particula[i1].tipo = tipo;

// combinar 1 y 0...
if (tipo==4) particula[i1].tipo = random () % 2;

// combinar 0,1,2
if (tipo==5) particula[i1].tipo = random () % 3;


} // fin for

cantidad_particulas = cantidad_particulas + cantidad;
// comprobacion de seguridad
if (cantidad_particulas>MAX_PARTICULAS-1) cantidad_particulas=MAX_PARTICULAS-1;

// LISTO!
} // fin agregar_particulas 
//--------------------------------------------------------
#endif
