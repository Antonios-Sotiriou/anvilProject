# 3D Graphics Engine.
### The game engine is still under development...


## Windows Compilation
At windows we open the Project with Visual Studio.<br>
Visual Studio compiles the Project automatically.<br>
We can just run the .exe pressing the green Visual Studio play button.<br>

## Linux Compilation
Navigate in the project main folder.<br>
cd project/<br>

Build the Project with cmake.<br>
From inside project/ folder, run:<br>
cmake -B out/<br>

Navigate in the out/ folder.<br>
From inside out/ folder, run:<br>
make<br><br>

At this point the project will be compiled and will create an executable at this folder.
It is very possible that missing dependencies errors will arrise.
Those are vary from machine to machine so they must be handled individually.

### Possible missing dependencies:
build-essential<br>
libwayland-dev<br>
libxkbcommon-dev<br>
xorg-dev<br>


