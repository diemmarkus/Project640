# Project640
 Project640 is an equalizer app that removes the dust from our old-school LCD-640 Linienprojektor.

## Build Project640 (Windows)
### Compile dependencies
- `Qt` SDK or the compiled sources (>= 5.4.0)

### Compile Project640 (MSVC)
1. Open CMake GUI
2. set the Pong folder to `where is the source code`
3. choose a build folder
4. Set `QT_QMAKE_EXECUTABLE` by locating the qmake.exe
5. Hit `Configure` then `Generate` (twice)
6. Open the `Project640.sln` which is in your new build directory
7. Right-click the Pong project and choose `Set as StartUp Project`
8. Compile the Solution

### If anything did not work
- check if your Qt is set correctly (otherwise set the path to `qt_install_dir/qtbase/bin/qmake.exe`)
- check if your builds proceeded correctly

## Links
- [nomacs.org](https://nomacs.org)

