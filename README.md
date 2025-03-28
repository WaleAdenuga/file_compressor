File compression and decompression program
Initially based on lossless compression and decompression

CMake outputs the binary (program build ie fcmp.exe) in C:\Tools\fcmp

Add to PATH in environment variables to use it in command line as intended

OpenCV is required for image compression
Make sure mingw64 is installed and added to PATH

Download from https://opencv.org/releases/, run the executable which extracts the folder into a directory of your choosing.

Open your system's environment variables and add the bin directory (eg C:\Tools\opencv\build\bin) to the PATH.  

But hey, the precompiled libraries are not for mingw
so you'd have to download the sources from the same website to generate them.

2 directories - C:/Tools/opencv - where the exe extracted to 
AND
C:/Tools/opencv-source - for the source files

Then you run
"cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:/Tools/opencv-mingw C:/Tools/opencv-source"

where C:/Tools/opencv-mingw is the directory where it would be installed

After, you run "mingw32-make -j4" and "mingw32-make install"

You then add "C:\Tools\opencv-mingw\x64\mingw\bin" to the PATH

If using vscode, you can add "C:/Tools/opencv/build/include" to the includePath of c_cpp_properties.json to use intellisense