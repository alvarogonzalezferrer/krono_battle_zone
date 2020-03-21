// Prueba de atan para ver unas cositas...
// (c) 2001, Kronoman
#include <stdio.h>
#include <conio.h>
#include <math.h>

#define K_PI 0.017453292  // pi / 180, para psar grad a rad y vs


int main ()
{
int x1,y1,x2,y2, d1;
double t;
clrscr();
    printf ("\nX1:");
    scanf("%d", &x1 );
    printf ("\nY1:");
    scanf("%d", &y1 );
    printf ("\n-----\nX2:");
    scanf("%d", &x2 );
    printf ("\nY2:");
    scanf("%d", &y2 );
    printf ("\n:");

    d1 = (x2 - x1);

    if (d1 == 0)
    {
    if (y1 < y2 )  printf ("90\n"); else printf("270\n");
    }
    else
    {
        t = (y2 - y1) ;
        t = t / (double) d1;

        printf (" (%d - %d) / (%d - %d) \n " , y2, y1, x2, x1 );

        printf ("Tan: %f \n", t);

        t = atan(t) / K_PI ; // divido por pi/180 para pasar rad a grados
        printf ("Atan(t) / (PI / 180) = %f", t);
    }

return 0;
}
