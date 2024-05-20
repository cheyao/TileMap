# Tile Map

Just testing how tile maps work
And animations

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

## Web 

Compile and install `SDL` and `SDL_image` as you would but with emscripten. (You need static libs `-DBUILD_SHARED_LIBS=OFF`)

```
mkdir build && cd build
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
emmake make 
```
When building there should be errors saying for example that you need png library, so go to the `external` folder of `SDL_image`, run `./download.sh` and go to the directories of the missing libraryies and build with cmake & emscripten.
