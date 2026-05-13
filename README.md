# tint-win

[中文](README.zh-CN.md)

A tiny Win32 thing that makes other windows transparent.

This is mostly me poking at C and the Windows API. It lists visible desktop
windows, lets you pick one, changes its opacity, and tries to put everything
back when you are done.

That's it. That's the program.

## Building

You need MSVC / Visual Studio Build Tools.

Open a Developer Command Prompt, or any shell where `cl` works, then run:

```bat
build.bat
```

The exe ends up here:

```text
build\tint.exe
```

## What it does

- lists normal visible desktop windows
- shows the selected window title and process name
- applies opacity with a slider
- restores the current window
- restores all windows touched by the program
- tries to restore modified windows on exit

## What it does not do

A lot.

This is not trying to be a polished desktop app. It is a small C/Win32 learning
project that happens to be useful enough to keep around.

There is no installer, no tray icon, no settings file, no auto update, no fancy
anything. Maybe later. Maybe never.

## Code shape

The code is split into a few small pieces:

- `tint.c` - Win32 entry point and message handling
- `tint_app.h` - shared app state and control ids
- `tint_window_list.*` - finding windows
- `tint_opacity.*` - applying and restoring opacity
- `tint_ui.*` - creating controls
- `tint_view.*` - putting text/status into the UI

## Notes

I made this because apparently I wanted to fight Win32 for a while.

Win32 feels so old that it looks like a dinosaur fossil sitting on my desk, and
the official Microsoft docs often read like somebody carved the answer into
oracle bones and forgot to include the translation.

This uses layered window attributes under the hood. Some windows may not behave
the way you expect, because Windows desktop apps are Windows desktop apps.

If something looks weird, press restore.

If that does not help, well, now we have learned something.

## License

MIT.
