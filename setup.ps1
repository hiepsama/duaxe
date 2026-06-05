$ErrorActionPreference = "Stop"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

$dir = "c:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game"
Write-Host "Creating folders..."
New-Item -ItemType Directory -Force -Path "$dir\include\glad" | Out-Null
New-Item -ItemType Directory -Force -Path "$dir\include\KHR" | Out-Null
New-Item -ItemType Directory -Force -Path "$dir\lib" | Out-Null

Write-Host "Downloading GLAD..."
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/JoeyDeVries/LearnOpenGL/master/src/glad.c" -OutFile "$dir\glad.c"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/JoeyDeVries/LearnOpenGL/master/includes/glad/glad.h" -OutFile "$dir\include\glad\glad.h"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/JoeyDeVries/LearnOpenGL/master/includes/KHR/khrplatform.h" -OutFile "$dir\include\KHR\khrplatform.h"

Write-Host "Downloading GLFW..."
Invoke-WebRequest -Uri "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN32.zip" -OutFile "$dir\glfw.zip"
Expand-Archive -Path "$dir\glfw.zip" -DestinationPath "$dir\glfw_extracted" -Force
Copy-Item -Path "$dir\glfw_extracted\glfw-3.3.8.bin.WIN32\include\GLFW" -Destination "$dir\include" -Recurse -Force
Copy-Item -Path "$dir\glfw_extracted\glfw-3.3.8.bin.WIN32\lib-mingw\libglfw3.a" -Destination "$dir\lib" -Force

Write-Host "Downloading GLM..."
Invoke-WebRequest -Uri "https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip" -OutFile "$dir\glm.zip"
Expand-Archive -Path "$dir\glm.zip" -DestinationPath "$dir\glm_extracted" -Force
Copy-Item -Path "$dir\glm_extracted\glm-0.9.9.8\glm" -Destination "$dir\include" -Recurse -Force

Write-Host "Cleaning up..."
Remove-Item -Path "$dir\glfw.zip" -Force
Remove-Item -Path "$dir\glfw_extracted" -Recurse -Force
Remove-Item -Path "$dir\glm.zip" -Force
Remove-Item -Path "$dir\glm_extracted" -Recurse -Force

Write-Host "Done!"
