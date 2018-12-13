# Pong
Pong is a homage to the famous arcade game Pong with the capability of old-school controllers using an Arduino Uno board. 

## Build Pong (Windows)
### Compile dependencies
- `Qt` SDK or the compiled sources (>= 5.4.0)

### Compile Pong (MSVC)
1. Open CMake GUI
2. set the Pong folder to `where is the source code`
3. choose a build folder
4. Set `QT_QMAKE_EXECUTABLE` by locating the qmake.exe
5. Hit `Configure` then `Generate` (twice)
6. Open the `Pong.sln` which is in your new build directory
7. Right-click the Pong project and choose `Set as StartUp Project`
8. Compile the Solution

### If anything did not work
- check if your Qt is set correctly (otherwise set the path to `qt_install_dir/qtbase/bin/qmake.exe`)
- check if your builds proceeded correctly

## Prepare Arduino Uno
1. get the Arduino IDE [1]
2. plugin your Arduino Uno Board (via USB)
3. Mount the Board (`Tools > Port > COMxx`)
4. Load the controller code (`Pong > src > Arduino > controller > controller.ino`)
5. Compile `CTRL+U`
6. Upload `CTRL+R`

## Links
- [1] https://www.arduino.cc/en/Main/Software
- [nomacs.org](http://nomacs.org)

