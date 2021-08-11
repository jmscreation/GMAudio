@echo off

REM Note: Please use 32bit GCC as Game Maker runs in 32bit mode only!

REM Set Paths Here:


set GCC_BIN_DIR=C:\MinGW-W64\mingw32-11.2.0\bin
set OUTPUT=AudioDLL.dll

REM C++ and G++ Paths

set CPP=%GCC_BIN_DIR%\c++
set GPP=%GCC_BIN_DIR%\g++

del %OUTPUT%

echo Compile Files...
%CPP% -O2 -Wall -std=c++20 -DBUILD_DLL  -c audio.cpp -o obj\audio.o
%CPP% -O2 -Wall -std=c++20 -DBUILD_DLL  -c main.cpp -o obj\main.o
%CPP% -O2 -Wall -std=c++20 -DBUILD_DLL  -c stb_vorbis.cpp -o obj\stb_vorbis.o

echo Export DLL...
%GPP% -shared -Wl,--dll obj\audio.o obj\main.o obj\stb_vorbis.o -o %OUTPUT% -L.\ -s -static -static-libstdc++ -static-libgcc -lportaudio-s -lwinmm -lsetupapi -luser32

if exist %OUTPUT% (
	echo Build Successful!
) else (
	echo Build Failed! See Log For Details
)
pause>nul