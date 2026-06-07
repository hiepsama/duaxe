$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$mingwBin = "c:\Users\Admin\Downloads\Dev-Cpp 5.7.1\MinGW32\bin"

if (Test-Path $mingwBin) {
	$env:PATH = "$mingwBin;" + $env:PATH
}

Set-Location $projectRoot
g++.exe -Iinclude main.cpp Car.cpp Camera.cpp glad.c -Llib -static-libgcc -static-libstdc++ -lglfw3 -lgdi32 -lopengl32 -o RacingGame.exe 2>&1
