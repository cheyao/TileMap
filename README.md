# Tile Map

A template SDL3 cross-platform project

## Dependencies:
SDL3
SDL3_image

## Building on mac and linux:

```
git clone https://github.com/cheyao/TileMap
cd TileMap
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
To make release on mac:
```
./mac-patch-dylib.sh
```
This will patch the binary to include SDL in the bundle

## Web 

Compile and install `SDL` and `SDL_image` as you would but with emscripten. (You need static libs `-DBUILD_SHARED_LIBS=OFF`)

```
mkdir build && cd build
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
emmake make 
```
When building there should be errors saying for example that you need png library, so go to the `external` folder of `SDL_image`, run `./download.sh` and go to the directories of the missing libraryies and build with cmake & emscripten.

## Linux 32 bit
```
export CFLAGS="-m32"
export CXXFLAGS="-m32"
export LDFLAGS="-m32"
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## For windows on linux
```
mkdir build && cd build
x86_64-w64-mingw32-cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

See https://cheyao.github.io/cross-compile-windows.html

## Emscriptien 
```
mkdir build && cd build
source ~/emsdk/emsdk_env.sh
emcmake -DCMAKE_BUILD_TYPE=Release ..
emmake
```

See https://cheyao.github.io/cross-compile-web.html
