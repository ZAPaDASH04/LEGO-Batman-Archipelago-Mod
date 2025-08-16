rem Run compile.bat and wait for it to finish
call .\compile.bat

xcopy /Y /E /I .\Reloadedii\* .\export\LB1AP
