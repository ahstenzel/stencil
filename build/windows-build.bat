@ECHO OFF
SET mypath=%~dp0..\

:: Generate build files
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --preset="windows-default" -H%mypath% -G "Visual Studio 17 2022" -T host=x64 -A x64

:: Build test executable & run it
cmake --build %mypath%out\build\windows-default -j 10 
:: && %mypath%bin\static\AMD64\Debug\stencil.exe