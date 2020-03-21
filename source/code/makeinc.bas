'Crea el archivo incluir.h con los archivos h y c que hay
SHELL "dir /b /on *.h > tmp~1"
SHELL "dir /b /on *.c >> tmp~1"
OPEN "tmp~1" FOR INPUT AS #1
OPEN "incluir.h" FOR OUTPUT AS #2
PRINT #2, "#ifndef INCLUIR_H"
PRINT #2, "#define INCLUIR_H"
PRINT #2, ""
DO WHILE NOT EOF(1)

    LINE INPUT #1, a$
    a$ = LCASE$(a$)
    PRINT #2, "#include " + CHR$(34) + a$ + CHR$(34)
    PRINT "#include " + CHR$(34) + a$ + CHR$(34)
LOOP
PRINT #2, ""
PRINT #2, "#endif"
CLOSE
KILL "tmp~1"
SYSTEM

