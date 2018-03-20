## Synopsis

A small starter kit for developing your own Pacman game.

Only external dependency: [SDL](http://www.libsdl.org) version 2.

Supports Windows, Linux, macOS.

## C++ compilers and SDL2

### Linux
Install `gcc` or `clang`. One of the two is probably already installed. Install SDL2-dev (on Ubuntu, the package is called `libsdl2-dev`).
Point `-I /opt/local/include -L /opt/local/lib` in `compile_and_run_pacman.sh` to the location of the SDL2 header files and library binaries.

### macOS
Install XCode from the App Store. Open the terminal and run `xcode-select --install`. You should now have the program `clang++`: that's your C++ compiler!

Download the development libraries of SDL2 from their [website](http://www.libsdl.org/download-2.0.php) (it's a `.dmg` file). Open the dmg and copy the `SDL2.framework` to `/Library/Frameworks` (to go directly to a path in Finder, press command+shift+g). The framework may need to be signed. To sign the framework (is this necessary?), open up a terminal, go to `/Library/Frameworks/SDL2.framework/`, and sign the framework using the command `codesign -f -s - SDL2`.

Make sure that the directories used in the `compile_and_run_pacman_osx.sh` script match with the location of your SDL2 installation.

If you get errors like `fatal error: 'SDL2/SDL.h' file not found`, you probably have to run `xcode-select --install` again (may happen after updating XCode).

### Windows
Download Visual Studio Community (it's free). During installation, select for "workload" the "Desktop development with C++"; that should install all needed components.

Create a Visual Studio project from existing source and point it to this pacmanLib directory. Add the `include` directory to the "Additional Include Directories". (Note: it's smart to apply these settings to "All Configurations" in the Configurations dropdown box...)

Download the development libraries (for Visual C++!) of SDL2 from their [website](http://www.libsdl.org/download-2.0.php). You can unzip the binary package anywhere, but the provided batch script assumes you have unpacked it in the `C:\SDL2\` directory (i.e. such that you have e.g. the `C:\SDL2\include` dir).
Inside the `include` directory, create a subdir `SDL2` and move all SDL2 header files into this `include\SDL2\` dir. Add the include directory (that contains this `SDL2`) directory to the "Additional Include Directories" of your Visual Studio project. Add the SDL2 `lib\x86` (or `lib\x64` when using the 64bit compiler) directory to the "Additional Library Directories", and add `SDL2.lib` and `SDL2main.lib` to the "Additional Dependencies" setting under Linker->Input.
Now you should be able to build (F7) the pacman program. Finally, copy the `lib\x86\SDL2.dll` file (or `lib\x64\SDL2.dll`) to your project (it's needed when running pacman.exe). Hit the play button and you should see the pacman maze!
If anything is unclear: use Google, and look e.g. here: http://gigi.nullneuron.net/gigilabs/setting-up-sdl2-with-visual-studio-2015/.

You should also be able to build `pacman.exe` from the commandline. Use the `compile_and_run_pacman.bat` script for that. You should run that script from the `x64 Native Tools Command Prompt for VS2017` terminal (when installing Visual Studio, it creates a shortcut for that in the Windows launch menu).

## Tooling support

A few extra files provide support for the following C++ tooling:
- `compile_and_run_pacman*`: commandline scripts that invoke the compiler to build the pacman program and run it. You may need to modify them a bit to use the correct compiler and to locate the SDL2 library files.
- `_clang-format`: [clang-format](https://clang.llvm.org/docs/ClangFormat.html). Many popular editors and IDEs support clang-format through e.g. plugins.
- `Doxyfile`: [doxygen](http://www.doxygen.org). The `Doxyfile` configures Doxygen to use Graphviz to generate e.g. class inheritance diagrams. The Graphviz software and installation instructions can be found on [the Graphviz website](http://www.graphviz.org).

## Directory structure

```
├── include/    The header files of your project
├── resources/  Media files, such as the bitmap with pacman images
├── src/
│   ├── your_code_here.cpp
│   └── ...
└── visualstudio/  Visual Studio project files that might work for you (copy to top directory before use)
```

## License

All files are in the public domain. Relicense at your own convenience.
