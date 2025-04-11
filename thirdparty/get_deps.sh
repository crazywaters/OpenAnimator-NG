#!/bin/sh
[ ! -d stb ]        && git clone https://github.com/nothings/stb.git                      ./stb
[ ! -d miniz ]      && git clone https://github.com/richgel999/miniz.git                  ./miniz

#[ ! -d mathc ]      && git clone https://github.com/kattkieru/mathc.git                   ./mathc
#[ ! -d sdl3 ]       && git clone https://github.com/libsdl-org/SDL.git                    ./sdl3        && git checkout release-3.2.2

#[ ! -d sdl3_ttf ]   && git clone https://github.com/libsdl-org/SDL_ttf.git                ./sdl3_ttf
#[ ! -d sdl3_gfx ]   && git clone https://github.com/sabdul-khabir/SDL3_gfx.git            ./sdl3_gfx
[ ! -d sdl3_image ] && git clone --recursive https://github.com/libsdl-org/SDL_image.git  ./sdl3_image  && git checkout release-3.2.0
#[ ! -d cjson ]      && git clone https://github.com/DaveGamble/cJSON                      ./cjson

#[ ! -d clay ]       && git clone https://github.com/nicbarker/clay/                       ./clay

#mkdir -p dcimgui
#pushd dcimgui
#[ ! -d imgui ]         && git clone https://github.com/ocornut/imgui                      ./imgui
#[ ! -d dear-bindings ] && git clone https://github.com/dearimgui/dear_bindings            ./dear_bindings
#popd

# [ ! -d spirv_shadercross ] && git clone https://github.com/KhronosGroup/SPIRV-Cross.git ./spirv_shadercross
# [ ! -d sdl3_shadercross ]  && git clone https://github.com/libsdl-org/SDL_shadercross.git ./sdl3_shadercross && pushd sdl3_shadercross/external && sh ./download.sh && popd

