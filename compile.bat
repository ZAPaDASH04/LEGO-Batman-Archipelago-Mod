
rem start "" cmd /c "i686-w64-mingw32-g++ -m32 -o \"C:\Program Files (x86)\Steam\steamapps\common\Lego Batman\injector.exe\" .\injector\src\injector.cpp 1>injector_log.txt 2>&1"
rem start "" cmd /c "i686-w64-mingw32-g++ -shared -m32 -o \"C:\Program Files (x86)\Steam\steamapps\common\Lego Batman\mod.dll\" .\hack\src\mod.cpp -static-libgcc -static-libstdc++ 1>mod_log.txt 2>&1"

rem set "GAMEPATH=C:\Progra~2\Steam\steamapps\common\Lego Batman"
rem start "" cmd /c "i686-w64-mingw32-g++ -m32 -o \"%GAMEPATH%\injector.exe\" .\injector\src\injector.cpp 1>injector_log.txt 2>&1"
rem start "" cmd /c "i686-w64-mingw32-g++ -shared -m32 -o \"%GAMEPATH%\mod.dll\" .\hack\src\mod.cpp -static-libgcc -static-libstdc++ >mod_log.txt 2>&1"


rem i686-w64-mingw32-g++ -m32 -o "C:\Program Files (x86)\Steam\steamapps\common\Lego Batman\LB1I.exe" .\injector\src\injector.cpp -static-libgcc -static-libstdc++ -static 1>injector_log.txt 2>&1
make -C .\injector && cp .\injector\bin\LB1I.exe  "C:\Program Files (x86)\Steam\steamapps\common\Lego Batman\LB1I.exe"

rem i686-w64-mingw32-g++ -shared -m32 -o "C:\Program Files (x86)\Steam\steamapps\common\Lego Batman\ap.dll" .\hack\src\mod.cpp -static-libgcc -static-libstdc++ -static 1>mod_log.txt 2>&1
make -C .\hack && cp .\hack\bin\ap.dll "C:\Program Files (x86)\Steam\steamapps\common\Lego Batman\ap.dll"
