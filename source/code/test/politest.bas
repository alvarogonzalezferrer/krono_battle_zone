DECLARE FUNCTION VerPuntoPoli! (X!, Y!)
'Esta rutina se fija si un punto esta dentro de un poligono
'Por Kronoman - 1 de Enero del 2001 - PRIMER DIA DEL MILENIO!!!!
'Basado en una rutina en Delphi
'
'FUNCIONA PERFECTAMENTE BIEN!!!! TRADUCIR A C++!!!!!
'


'Constante
PI = 3.141592654#
PI = PI / 180 'HAGO ESTO, Y ME AHORRO PONER EN LA FORMULA /180;


COLI = 4 'COLOR POLIGONO

'estas variables vana contener la posicion de los vertices del poligono
'en orden de giro de 0 a 360.
'creado en la matriz PoliX, PoliY de cantidad de puntos CantPoli
 
      
DIM SHARED CANTPOLI 'cantidad
DIM SHARED POLIX(361)     'x
DIM SHARED POLIY(361)     'y

RANDOMIZE TIMER

radio = INT(RND * 100) + 100'radio del poligono
rotacion = 45

'posicion del centro del poligono
CENTROX = 640 / 2
CENTROY = 480 / 2

SCREEN 12
lados = INT(RND * 10) + 3


redibujar:
CLS

'dar toda la vuelta fijando las coordenadas de los vertices
'y uniendo los puntos
vertices = 0'vertice actual 0 a 360 (su indice no tiene nada que ver con el angulo)
incang = 360 / lados 'incremento del angulo

FOR A = 0 TO 360 + incang STEP incang

'PEDIR LAS COORDENADAS DEL ANGULO
        xx1 = CENTROX
        yy1 = CENTROY
        LARGO = radio
        ang = A + rotacion 'le sumo x grados para que quede rotado
        'ajustar el angulo para que quede dentro de 0 a 360
        WHILE ang > 360
                ang = ang - 360
        WEND
        WHILE ang < 0
                ang = ang + 360
        WEND

        ang = INT(ang)

        GOSUB DEVOLVERCOORD:

'marca los lados
IF A = 0 THEN PSET (xx2, yy2), COLI ELSE LINE -(xx2, yy2), COLI

        POLIX(CANTPOLI) = xx2
       POLIY(CANTPOLI) = yy2
        CANTPOLI = CANTPOLI + 1
        IF CANTPOLI > 360 THEN CANTPOLI = 360
NEXT

'pintarlo
IF lados > 2 THEN PAINT (xx1, yy1), COLI, COLI



'SITUAR UN PUNTO AL AZAR Y DECIR SI ESTA DENTRO O FUERA
DO
RANDOMIZE TIMER
XP = INT(RND * 640) + 1
YP = INT(RND * 480) + 1

CIRCLE (XP, YP), 1, 15

' SI DA 0 ESTA FUERA, 1 ADENTRO
A = VerPuntoPoli(XP, YP)
IF A = 1 THEN
        PRINT "ESTA DENTRO    "
ELSE
        PRINT "ESTA FUERA      "
END IF
SLEEP 1
CIRCLE (XP, YP), 1, 0
LOCATE 1, 1: PRINT "............"
LOCATE 1, 1

LOOP WHILE INKEY$ = ""

END
'-----------------RUTINA AUXILIAR PERO NECESARIA------------------

DEVOLVERCOORD:
'PASAR EN XX1, YY1 LAS COORDENADAS ORIGEN DE LA LINEA
'EN LARGO EL LARGO Y EN ANG EL ANGULO; ENTONCES, DEVUELVO
'LA XX2, YY2 DONDE DEBE QUEDAR LA LINEA PARA TRAZAR EL ANGULO ESPECIFICADO
'HACER LINE[XX1,YY1]-[XX2,YY2] Y LISTO
xx1 = INT(xx1)
yy1 = INT(yy1)
LARGO = INT(LARGO)


yy2 = SIN(ang * PI) * LARGO 'SENO

yy2 = yy1 - yy2

xx2 = COS(ang * PI) * LARGO 'COSENO
xx2 = xx2 + xx1

'LOS NUMEROS DE COORDENADA PASAN A SER ENTEROS

xx2 = INT(xx2)
xx1 = INT(xx1)
yy2 = INT(yy2)
yy1 = INT(yy1)

RETURN
'LISTO

FUNCTION VerPuntoPoli (X, Y)
'Ve si el punto x,y esta dentro del poligono
'creado en la matriz PoliX, PoliY de cantidad de puntos CantPoli
'Devuelve 0 si esta fuera
'Devuelve 1 si esta dentro

RESULTADO = 0
 
  J = CANTPOLI
  FOR I = 1 TO CANTPOLI
    IF ((POLIY(I) <= Y) AND (Y < POLIY(J))) OR ((POLIY(J) <= Y) AND (Y < POLIY(I))) THEN
    
      IF (X < (POLIX(J) - POLIX(I)) * (Y - POLIY(I)) / (POLIY(J) - POLIY(I)) + POLIX(I)) THEN
          IF RESULTADO = 0 THEN RESULTADO = 1 ELSE RESULTADO = 0
     END IF
   
    END IF

    J = I
    NEXT


VerPuntoPoli = RESULTADO

'RUTINA ORIGINAL
'function PtInPoly(const Points : Array of TPoint; X,Y : Integer): Boolean;
'var
'  Count,
'  I,
'  J                 : Integer;
'begin
'  Result := False;
'  Count := Length(Points);
'  J := Count-1;
'  for I := 0 to Count-1 do begin
'    if ((Points[I].Y <=Y) and (Y < Points[J].Y)) or
'       ((Points[J].Y <=Y) and (Y < Points[I].Y)) then
'    begin
'      if (x < (Points[j].X - Points[i].X) * (y - Points[i].Y) /
'        (Points[j].Y - Points[i].Y) + Points[i].X) then
'          Result := not Result;
'    end;
'
'    J := I;
'  end;
'end;



END FUNCTION

