@echo off
REM Cambiar a UTF-8 para mejor soporte de caracteres especiales
chcp 65001 >nul

echo === COMPILANDO PNTD SISTEMA CON MinGW ===
echo.

REM Verificar que g++ esté disponible
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    exit /b 1
)

echo ✓ g++ encontrado
echo.

REM Crear directorio build si no existe
if not exist build mkdir build

echo Compilando con g++...
echo.

REM Compilación de todos los módulos principales
g++ -std=c++17 -Wall -I"src/estructuras" -I"src/modulos" -I"src/utilidades" ^
    src/main.cpp ^
    src/modulos/RegistroUsuarios.cpp ^
    src/modulos/GestionCursos.cpp ^
    src/utilidades/ValidacionDatos.cpp ^
    src/modulos/GestorCertificados.cpp ^
    src/utilidades/PersistenciaCSV.cpp ^
    -o build/PNTD.exe

REM Verificar si la compilación fue exitosa
if exist build\PNTD.exe (
    echo.
    echo ✓ Compilacion exitosa con MinGW
    echo Ejecutable creado: build/PNTD.exe
    echo.
    echo ¿Desea ejecutar el programa? (s/n)
    set /p respuesta=
    if /i "%respuesta%"=="s" (
        echo.
        echo === EJECUTANDO PNTD ===
        build\PNTD.exe
    )
) else (
    echo.
    echo ✗ Error en la compilacion
    echo Revise los errores mostrados arriba
)

echo.
pause