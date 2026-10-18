@echo off
echo ========================================================
echo  Subir ScarletUtils 2.113 a GitHub para compilacion CI
echo ========================================================
echo.
set /p REPO_NAME="Nombre de tu repositorio en GitHub (por defecto: scarletutils-2113): "
if "%REPO_NAME%"=="" set REPO_NAME=scarletutils-2113

git remote remove origin 2>nul
git remote add origin https://github.com/TpdeaX/%REPO_NAME%.git

echo.
echo Subiendo rama main a GitHub...
git push -u origin main

echo.
if %errorlevel% equ 0 (
    echo ========================================================
    echo  Listo! Ahora entra a:
    echo  https://github.com/TpdeaX/%REPO_NAME%/actions
    echo  Espera unos 2 minutos y descarga 'scarletutils_2113.dll'.
    echo ========================================================
) else (
    echo Error al subir. Asegurate de haber creado el repo en https://github.com/new primero.
)
pause
