'Kronoman - 31-Diciembre-2000 (ULTIMO DIA DEL MILENIO!!)
'(VICTORIA SE ESTA BA¥ANDO)
'
'Peque¤o test para el manejo del vehiculo
'Representa el vehiculo como un punto.
'El vehiculo es movido por dos vectores:
'* Velocidad
'* Inercia
'
'Ambos tienen un angulo, y una fuerza
'Cuando la velocidad cambia su angulo, la inercia hace lo mismo
'pero mas lento, hacia el angulo que tiene ahora la nueva velocidad
'De esa manera, el vehiculo 'derrapa'
'
'controles: A,S,D,W mover; BARRA para borrar path SI/NO
'

'CONST pi1 = 3.141592654#
'CONST PI = pi1 / 180

CONST KRONOPI = .017453292#

CONST TRUE = 1
CONST FALSE = 0

borrarpath = TRUE
'posicion centrada
xp = 160
yp = 100
'direccion (angulo, 90 es hacia arriba)
dir = 90
'inercia (angulo, idem anterior)
inercia = 90

'alterando estos valores se consige la sensacion de derrape para cada vehiculo

'velocidad de alteracion de la direccion
vdir = 10
'velocidad de alteracion de la inercia
viner = 5

'velocidad del vehiculo en cuestion
velocidad = 0


SCREEN 7, 0, 0, 0
CLS

TT = TIMER

DO
d$ = UCASE$(INKEY$)

'acelerar
IF d$ = "W" THEN velocidad = velocidad + 2
IF d$ = "S" THEN velocidad = velocidad - 4 'frenar

'borrar on /off
IF d$ = " " THEN IF borrarpath = TRUE THEN borrarpath = FALSE ELSE borrarpath = TRUE
IF velocidad > 0 THEN
'doblar
IF d$ = "A" THEN dir = dir + (vdir * velocidad / 10)
IF d$ = "D" THEN dir = dir - (vdir * velocidad / 10)
END IF
IF d$ = CHR$(27) THEN EXIT DO

IF TIMER - TT >= .1 THEN
TT = TIMER
SCREEN 7, 0, 1, 0
PCOPY 2, 1

'MOVER

'verificar
IF dir > 360 THEN dir = dir - 360
IF dir < 0 THEN dir = dir + 360

'mover la inercia hacia donde debe ser
IF inercia > dir - vdir THEN inercia = inercia - viner


IF inercia < dir + vdir THEN inercia = inercia + viner

IF inercia >= dir - vdir AND inercia <= dir + vdir THEN inercia = dir

'friccion de frenado = .3
IF velocidad > 0 THEN velocidad = velocidad - .3
IF velocidad < 0 THEN velocidad = 0
IF velocidad > 15 THEN velocidad = 15'limitador de velocidad


'tomar hacia donde mueve la velocidad
xx1 = xp
yy1 = yp
largo = velocidad
ang = dir

GOSUB devolvercoord:

'mover con la velocidad
xp = xx2
yp = yy2

'tomar hacia donde mueve la inercia
xx1 = xp
yy1 = yp
largo = velocidad / 4  '1/3 de la velocidad... ;D
ang = inercia

GOSUB devolvercoord:

'mover con la inercia
xp = xx2
yp = yy2


'que no se salga
IF xp < 0 THEN xp = 0
IF yp < 0 THEN yp = 0
IF xp > 319 THEN xp = 319
IF yp > 199 THEN yp = 199


'info
LOCATE 20, 1
PRINT "D§"; dir; "I§"; inercia; "V"; velocidad


'dejar la 'estela' del circulito
IF borrarpath = FALSE THEN
        SCREEN 7, 0, 2, 0
        'DIBUJAR
        CIRCLE (xp, yp), 2, 15
        SCREEN 7, 0, 1, 0
ELSE
        PCOPY 3, 2
END IF


'DIBUJAR
CIRCLE (xp, yp), 2, 15




'dibuja un poligono

'centra
xx1 = xp
yy1 = yp
largo = 15
ang = dir
GOSUB devolvercoord
xx1 = xx2
yy1 = yy2
largo = 10
ang = dir - 90
GOSUB devolvercoord

xx1 = xx2
yy1 = yy2
largo = 20
ang = dir + 90
GOSUB devolvercoord
LINE (xx1, yy1)-(xx2, yy2), 13
xz1 = xx1
yz1 = yy1


xx1 = xx2
yy1 = yy2
largo = 30
ang = dir + 180
GOSUB devolvercoord
LINE (xx1, yy1)-(xx2, yy2), 14

xx1 = xx2
yy1 = yy2
largo = 20
ang = dir + 270
GOSUB devolvercoord
LINE (xx1, yy1)-(xx2, yy2), 15

'cierra el poligono con las vars que prev almacene
xx1 = xx2
yy1 = yy2
LINE (xx1, yy1)-(xz1, yz1), 12


'dibuja las direcciones
'angulo
xx1 = xp
yy1 = yp
largo = (velocidad * 4) + 3'para que se vea bien
ang = dir
GOSUB devolvercoord:
LINE (xx1, yy1)-(xx2, yy2), 10

'inercia
xx1 = xp
yy1 = yp
largo = (velocidad * 2) + 2'para que se vea bien
ang = inercia
GOSUB devolvercoord:
LINE (xx1, yy1)-(xx2, yy2), 11


PCOPY 1, 0

END IF


LOOP


END


'----------------RUTINA AUXILIAR PERO NECESARIA------------------

devolvercoord:
'PASAR EN XX1, YY1 LAS COORDENADAS ORIGEN DE LA LINEA
'EN LARGO EL LARGO Y EN ANG EL ANGULO; ENTONCES, DEVUELVO
'LA XX2, YY2 DONDE DEBE QUEDAR LA LINEA PARA TRAZAR EL ANGULO ESPECIFICADO
'HACER LINE[XX1,YY1]-[XX2,YY2] Y LISTO
DO WHILE ang > 360 OR ang < 0
IF ang > 360 THEN ang = ang - 360
IF ang < 0 THEN ang = ang + 360
LOOP

xx1 = INT(xx1)
yy1 = INT(yy1)
largo = INT(largo)


yy2 = SIN(ang * KRONOPI) * largo 'SENO

yy2 = yy1 - yy2

xx2 = COS(ang * KRONOPI) * largo 'COSENO
xx2 = xx2 + xx1

'LOS NUMEROS DE COORDENADA PASAN A SER ENTEROS

xx2 = INT(xx2)
xx1 = INT(xx1)
yy2 = INT(yy2)
yy1 = INT(yy1)

RETURN
'LISTO

