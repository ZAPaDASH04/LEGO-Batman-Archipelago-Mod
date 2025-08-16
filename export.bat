rem Run compile.bat and wait for it to finish
call .\compile.bat

xcopy /Y /E /I .\Reloadedii\* .\export\LB1AP

tar -a -c -f ".\export\LB1APv0.1.1a.zip" ".\export\LB1AP"
