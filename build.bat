@echo off

if not exist .\build mkdir .\build
pushd .\build
cl -Zi ..\tint.c ..\tint_window_list.c ..\tint_opacity.c ..\tint_ui.c ..\tint_view.c User32.lib Comctl32.lib
popd
