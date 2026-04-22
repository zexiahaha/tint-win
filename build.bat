@echo off

mkdir .\build
pushd .\build
cl -Zi ..\tint.c
popd
