// Archivo include para startgam.c
// Kodigo que crea el mapa del juego
// setea los efectos, luces, etc y posicion de los jugadores
// CONTIENE MUCHAS DEFINICIONES QUE AFECTAN A CASI TODO EL JUEGO!!
// Por Kronoman
// 4 de Enero de 2001 - 2:20am
// DJGPP + Allegro

// Primero me fijo si no definio esto previamente...
#ifndef STARTGAM__H
#define STARTGAM__H

//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------
int dame_sprite_piso(int n);
void dibujar_rectangulo_mapa_1(BITMAP *bmp, int x1,int y1);
void dibujar_el_mapa();
void crear_mapa_juego();
void setear_mapa_juego();
long distancia_entre_jugadores();
void iniciar_juego_azar();
void mostrame_mapa_para_debug();
void situa_jug_azar(int numjug);
void crear_poli_jug(int numjug);
void setea_jug(int numjug);

//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

//--------------------------------------------------------
// Definicion de quien controla los jugadores
int JUG_HUMANO[3] = {TRUE,TRUE,TRUE}; // False = PC; True = Teclado (Humano) (0..1..2) (0 reservado uso futuro)

// Definiciones de los sprites
#define SPR_X_MAX       20 // tama¤o ancho
#define SPR_Y_MAX       20 // tama¤o alto

// definicion jugador
#define JUG_X_MAX       25 // tama¤o ancho
#define JUG_Y_MAX       25 // tama¤o alto
#define JUG_R_MAX       25 // radio del sprite (al girarlo, para las comprobaciones)
                           // debe ser = al ancho o alto maximo

// definiciones de velocidades
// velocidades lineales
#define MAX_VEL_TA       50 // velocidad maxima (pixels) del tanque
                             // NOTAR QUE ESTE VALOR SE DIVIDE POR 10
                             // CUANDO SE UTILIZA, ASIQUE UN VALOR DE 100
                             // SERIA EN REALIDAD 10 MAXIMO. OK?
                             // ES ASI PORQUE SI NO ACELERA DEMASIADO RAPIDO
                             // Y ES INCONTROLABLE
                             // ENTONCES, LOS VALORES DE ABAJO SERIAN
                             // 0.4 Y 0.1 EN REALIDAD, PERO DEBEN DEJARSE
                             // EN 4 Y 1, SIN DECIMALES. OK?

#define ACC_TA            4 // velocidad de aceleracion
#define FRICCION          2 // friccion (desaceleracion) del tanque

// velocidades de giro
#define VEL_GTA           4  // velocidad de giro (grados) del tanque
#define VEL_GTO           8  // velocidad de giro (grados) de la torreta
#define VEL_INE           2  // velocidad de giro del vector de inercia

// definicion de la energia
#define MAX_ENERGIA       150

// Definiciones de los tipos de clima
// para el generador de clima en tiempo real
#define NORMAL          0  // normal
#define LLUVIA          1  // lluvia 
#define NIEVE           2  // nieve
#define ARENA           3  // tormenta de arena
#define GRANIZO         4  // granizo
#define NIEBLA          5  // niebla (buenisimo el efecto!)


// hora del dia
#define DE_DIA          0  // de dia
#define DE_NOCHE        1  // de noche

// tipo de terreno
#define TIERRA          0 
#define PASTO           1 
#define BARRO           2 
#define AGUA            3
// si se agregan nuevos tipos de piso, agregarlos entre BARRO y AGUA,
// AGUA debe quedar a lo ultimo de los tipos de terreno, porque el terreno
// se basa en que son los <= AGUA

// tipos de paredes
#define LADRILLO        4 
#define MADERA          5 
#define ROCA1           6
#define ROCA2           7
#define ROCA3           8

// estructura del mapa
typedef struct {
        int clima; // tipo de clima
        int hora; // hora del dia
        long int tiempo_a_jugar; // tiempo a jugar la partida en segundos, -666 para infinito
        long int puntaje[3]; // 1 es el puntaje del jugador 1, 2 el del jug 2; ok?
                             // puntaje se mide en cantidad de muertes producidas al contrario
        int terreno_gral;  // tipo de terreno en gral (tierra, pasto, barro o agua)
                           // con esto se llena el mapa inicial, y se reemplazan las paredes rotas
     } MAPA_JUEGO_TYPE;

// defino el mapa - global
MAPA_JUEGO_TYPE info_mapa_juego;

// creo el array que contiene las casillas del mapa
// el mapa es de 1000x1000 pixeles
// dividido en una grilla de 20x20 pixels, o sea 50x50 grilla = 2500

int grilla_map[50][50]; // MAPA [0-49] = 50
int grilla_mapE[50][50]; // ENERGIA [0-49] = 50

// esto se organiza asi:
// grilla_map[x][y] = tipo de terreno
// grilla_mapE[x][y] = energia que le queda para romperse en caso de ser una pared


// typedef estructura de los jugadores
// contiene la info de los jugadores, como su posicion, energia,
// armamento, angulo, velocidad, etc...

typedef struct {
    int energia; // energia 0 a MAX_ENERGIA
    int posx; // pos x 0 a 1000 - ancho
    int posy; // pos x 0 a 1000 - alto
    int vel; // velocidad 0 a 10 * [speed power up]
    int ang_dir; // angulo del cuerpo de 0-255 (255 =360 grados)
    int ang_inercia; // angulo de la inercia 0-255
    int ang_tor; // angulo de la torreta de 0-255
    int arma; // tipo de arma: 0=ametralladora, 1,2,3=ca¤ones, 4=lanza llama
    int balas; // cantidad de balas del armamento (la ametralladora es infinita...)

    int premio; // premio que tiene; 0 = nada > 0 = premio
    int c_premio; // cantidad de premio que le queda (se descuenta por tick de juego)

    // poligono del tanque, para comprobaciones de juego
    // son 11 puntos, del 0 al 10 (el 0 cuenta, por eso son 11)
    // el 10 contiene el punto de disparo de la torreta
    // el 0 es un punto relativo al 10, pero alejado unos 20 pixeles
    // sirve para simular la luz de la torreta
    // el resto 1-9 son los bordes del tanque
    int xpol[11];
    int ypol[11];

    // tiene torreta? en caso de querer usar autos en vez de tanques
    // si es FALSE, solo dibuja el cuerpo, y dispara desde el frente,
    // y la luz sale del frente y no de la torreta
    int tiene_torreta;

    // aqui almacena las variables anteriores
    // en caso de que el tanque se superponga con algo, restaura esto ;D
    int posxold, posyold, ang_dirold, angtor_old; // posx, posy, ang_dir, ang_tor

    // si juega de noche, puede apagar las luces del tanque/auto
    // setear a FALSE para apagarlas
    int luz_prendida;

    // variable para medir el tiempo entre disparos y no disparar muy rapido
    unsigned long deciseg; // se sincroniza con la de game.c => "deciseg";

} JUGADOR_TYPE;

// defino global

JUGADOR_TYPE jugador_info[3]; // 1 = jugador 1, 2 = jugador 2 ok? 0=reservado uso futuro

#endif // fin de ver si ya definio todo


