@ECHO OFF
REM BAT PARA COMPILAR EL JUEGO
REM APRENDE A USAR LOS MAKEFILES GIL!!
REM -s es para achicar el tamaño del ejecutable
REM -O3 activa mucho la optimizacion para velocidad
REM ver las FAQ de DJGPP, seccion 8.14

REM ---- 2020 ----
REM Marzo 2020 - agregue salvar errores y warnings con REDIR -o etc 
REM gcc -s -O3 -Wall principa.c -o  ..\bin\kronobz.exe -lalleg
REM compilar
echo Compilando... espere por favor
gcc -v
REM redir -o gcc_err.txt -eo 
gcc -s -O3 -Wall principa.c -o  ..\bin\kronobz.exe -lalleg

REM crear un exe stand alone, todo esto es opcional
REM exe2coff ..\bin\kronobz.exe
REM copy /b C:\djgpp\bin\cwsdstub.exe + ..\bin\kronobz  ..\bin\kronobz.exe
REM UPX crashea DosBOX
REM upx ..\bin\kronobz.exe
REM del ..\bin\kronobz



