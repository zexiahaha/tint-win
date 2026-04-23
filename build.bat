@echo off

mkdir .\build
pushd .\build
cl -Zi ..\tint.c User32.lib
popd
