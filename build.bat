@echo off

REM Set Paths Here:

set GCC_BIN_DIR=C:\MinGW-W64\mingw32-10.2.0\bin
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
%GPP% -shared -Wl,--dll -Llibraries obj\audio.o obj\main.o obj\stb_vorbis.o -o %OUTPUT% -s -static-libstdc++ -static-libgcc -static -static-libstdc++ -static-libgcc -static  -lportaudio-s -lwinmm -luser32 -lsetupapi

if exist %OUTPUT% (
	echo Build Successful!
) else (
	echo Build Failed! See Log For Details
)
pause>nul