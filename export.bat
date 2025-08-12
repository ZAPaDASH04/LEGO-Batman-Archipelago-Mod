rem Run compile.bat and wait for it to finish
call .\compile.bat

rem Copy files from injector\bin to export
rem xcopy /Y /E /I .\injector\bin\* .\export\

rem Copy files from hack\bin to export
rem xcopy /Y /E /I .\hack\bin\* .\export\
rem cp .\hack\bin\ap.dll
xcopy /Y /E /I .\Reloadedii\* .\export\
