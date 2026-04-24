@echo off

if not exist .\build mkdir .\build
pushd .\build
cl -Zi ..\tint.c User32.lib Comctl32.lib
popd
