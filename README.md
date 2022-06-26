# potato-chess

A chess engine written in C++ and compiled to Web Assembly with an html frontend

Compiled by [Emscripten](https://github.com/emscripten-core/emscripten)

Frontend code written with HTML and Javascript that can be used in the browser or as a desktop app with electron.

Compile to Web Assembly using `em++ -lembind -o engine.js -O2 *.cpp` and packaged using electron-builder and electron-packager