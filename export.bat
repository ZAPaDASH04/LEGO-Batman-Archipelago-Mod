rem Run compile.bat and wait for it to finish
call .\compile.bat
xcopy /Y /E /I .\Reloadedii\* .\export\LB1AP
cp .\README.md .\export\LB1AP\README.md

powershell Compress-Archive -Path ".\export\LB1AP\*" -DestinationPath ".\export\LB1APv0.1.1a.zip" -Force
