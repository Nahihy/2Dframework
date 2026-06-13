My final project for my uni course.
It's a lib to easily create 2D graphical games, based on a randerer lib to make base (2D) OpenGL easier and object orianted and a datatbase lib to save data on files.

Please see the docs for more info.

The three of them are made with only c and cmake and those are the tools needed to compile it.
The master branch is for unix/unix-like systems (linux/bsd/macos) and windows branch for windows.

On windows there's a bug that doesn't happen consistenly for me where the textures aren't getting copied to where the executable exists and instead to the build dir's root, so if no textures are getting detected that's probably why.

usage:
to compile and use the library all you need is a c compiler and cmake.

cmake can be installed from the website cmake.org, and for the compiler you can use gcc clang or msvc etc.
to edit in vscode you can install the cmake tools extension and it should work.
to edit in visual studio use the visual studio installer app, click on modify and inside the c++ desktop development section check the c++ cmake tools.

for every other editor you just need to setup cmake support for it and have a c compiler.

