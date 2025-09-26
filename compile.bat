echo off
make -C .\mod && cp .\mod\bin\ap.dll .\Reloadedii\legobatman1.apsupport
xcopy /Y /E /I .\Reloadedii\* .\export\LB1AP
