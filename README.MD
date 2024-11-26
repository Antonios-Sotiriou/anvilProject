# 3D Graphics Engine.
### The game engine is still under development...


## Windows Compilation
At windows we open the Project with Visual Studio.
Visual Studio compiles the Project automatically.
We can just run the .exe pressing the green Visual Studio play button.

## Linux Compilation
Navigate in the project main folder.
cd project/

Build the Project with cmake.
From inside project/ folder, run:
cmake -B out/

Navigate in the out/ folder.
From inside out/ folder, run:
make

At this point the project will be compiled and will create an executable at this folder.
It is very possible that missing dependencies errors will arrise.
Those are vary from machine to machine so they must be handled individually.

Possible missing dependencies:
build-essential
libwayland-dev
libxkbcommon-dev
xorg-dev


