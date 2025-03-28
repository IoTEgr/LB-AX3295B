cd /d %~dp0
MakeSPIBin.exe %1 Res.bin
@echo.
@if exist z:\ (copy /y DestBin.bin z:\)

















