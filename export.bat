REM Run compile.bat and wait for it to finish
call .\compile.bat

REM Copy files from injector\bin to export
xcopy /Y /E /I .\injector\bin\* .\export\

REM Copy files from hack\bin to export
xcopy /Y /E /I .\hack\bin\* .\export\
