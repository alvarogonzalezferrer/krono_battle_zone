'grafico de seno (se puede graficar cos o tan, reemplazando sin[x] por cos[x] o tan[x]
'(c) 2001, Kronoman
SCREEN 12
'escala x,y
sx = 30
sy = 10

'periodo de la onda
per = 0

'escala del grafico [recomendado 0.1; 1 = super rapido]
escala = .5

'primera vuelta?
vuelta = 0
'modo de dibujo: 0 = puntos, 1 = lineas
modo = 1


FOR x = 0 TO 640 / sx STEP escala
    y = SIN(x + per)

IF modo = 0 THEN
    PSET ((x * sx), (y * sy) + 240), 15
ELSE
    IF vuelta = 0 THEN
        vuelta = 1
        PSET ((x * sx), (y * sy) + 240), 15
    ELSE
        LINE -((x * sx), (y * sy) + 240), 15
    END IF
END IF

    IF INKEY$ <> "" THEN END

NEXT

