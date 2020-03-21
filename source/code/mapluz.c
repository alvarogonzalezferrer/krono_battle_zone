//--------------------------------------------------------
// mapluz.c
//
// Generador de mapa de adicion de colores
// y mapa de adicion de tonalidad gris.-
//
// Por Kronoman
//
// IMPRESCINDIBLE PARA HACER LUCES SUAVES QUE SE PUEDAN
// INTERSECTAR ENTRE SI Y EFECTOS DE LENS FLARE, ETC!
//--------------------------------------------------------
#ifndef MAPLUZ_C
#define MAPLUZ_C

//--------------------------------------------------------
// Esta rutina crea el mapa aditivo de luces
// ESENCIAL para el fundido de LUCES!!!!
// Es imprescindible!!! 
// debe estar definido globalmente: 
// COLOR_MAP greyscale_add_map;
//--------------------------------------------------------
void crear_mapa_aditivo_luces()
{
int x,y;

// setea los datos del mapa aditivo de luces
// de forma tal que a cada combinacion le corresponda
// la suma de ambas intensidades, maximo 255.
// explicacion:
// la intensidad de una luz va de 0 a 255 (representada por el color)
// esto crea un mapa sencillo, donde a cada color A con B le corresponde
// el indice de la paletta que es su suma A+B
// y NO su color sumado (Red+Red, Green+G, Blue+Blue) que seria su
// color sumado y no su indice, como queremos.
//
// Ejemplo: color 0 y 10 da color indice 10, no la suma de sus valores RGB
// como ocurre con la tabla creada abajo (crear_mapa_aditivo)

   for (x=0; x<256; x++)
      for (y=0; y<256; y++)
              greyscale_add_map.data[x][y] = MIN(x+y, 255);
}
//--------------------------------------------------------


//--------------------------------------------------------
// RGB crear_mapa_aditivo(PALETTE pal, int x, int y)
//
// Crea el COLOR_MAP de manera
// tal que al dibujar en modo TRANS, los colores
// se suman; muy bueno para combinar colores
// hacia un tinte que seria su suma RGB.
// Esto es lo que se llama un mapa de color aditivo
//
// Se le pasa una paleta, y el color #1 (x) y el #2 (y)
// y devuelve el valor RGB que quedaria.
// De buscar el color en la paletta se encarga Allegro.
// 
// Para llamar a esta funcion se debe hacer asi:
//
// create_color_table(&tabla_a_crear,
//                       paleta_a_usar,
//                       crear_mapa_aditivo,
//                       NULL);
// y para usar la 'tabla_a_crear' (paleta creada) en
// sus transparencias, usar:
// drawing_mode(DRAW_MODE_TRANS,NULL,0,0); // para dibujar transparente
// color_map = &tabla_a_crear; // eligo el mapa de color creado
//
// luego, al hacer cualquier circle, etc, se adicionaran los colores.
//--------------------------------------------------------

RGB crear_mapa_aditivo(PALETTE pal, int x, int y)
{
   int r, g, b;
   RGB rgb;

// adiciona los valores
   r = (int)pal[x].r + (int)pal[y].r;
   g = (int)pal[x].g + (int)pal[y].g;
   b = (int)pal[x].b + (int)pal[y].b;

// uso MIN para limitarlo al valor maximo 63, sino, si es > 63,  queda negro
   rgb.r = MIN(r, 63); 
   rgb.g = MIN(g, 63);
   rgb.b = MIN(b, 63);

return rgb; // devuelvo lo que cree para este color...
} // fin crear_mapa_aditivo
//--------------------------------------------------------

//--------------------------------------------------------
// PALETTE crear256grises()
// Crea una paleta de 256 tonos de gris
// repetidos cada 4, o sea, en realidad, 64 tonos de gris
// agrupados de a 4.
// Util para que se yo... ;^D
//
// Llamar:
//       crear256grises(paleta);
//
// Ok...
//--------------------------------------------------------
void crear256grises(PALLETE tmpal)
{
int i1, i2; // para el for

i2 = 0; // valor RGB
for (i1=0; i1<256; i1+=4)
      {
       tmpal[i1].r = tmpal[i1].g = tmpal[i1].b = i2;
       tmpal[i1+1].r = tmpal[i1+1].g = tmpal[i1+1].b = i2;
       tmpal[i1+2].r = tmpal[i1+2].g = tmpal[i1+2].b = i2;     
       tmpal[i1+3].r = tmpal[i1+3].g = tmpal[i1+3].b = i2;
       i2++;
      } // fin for i1

} // fin crear256grises()
//--------------------------------------------------------
#endif
