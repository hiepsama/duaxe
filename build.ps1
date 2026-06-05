$env:PATH = "c:\Users\Admin\Downloads\Dev-Cpp 5.7.1\MinGW32\bin;" + $env:PATH
Set-Location "c:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game"
g++.exe -Iinclude main.cpp Car.cpp Camera.cpp glad.c -Llib -static-libgcc -static-libstdc++ -lglfw3 -lgdi32 -lopengl32 -o RacingGame.exe 2>&1
