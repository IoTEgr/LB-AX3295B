rd ax32xx\sd /s /q

rd bwlib\fs\make /s /q
rd bwlib\fs\src /s /q

rd multimedia\avi\make /s /q
rd multimedia\avi\src /s /q

rd multimedia\mp3\make /s /q
rd multimedia\mp3\src /s /q

rd nes_src /s /q

cd /d %~dp0

del %~dp0\multimedia\imageL\*.c

del %~dp0\multimedia\user_effect\*.c

rd ax32_platform_demo\Debug  /s /q
rd ax32_platform_demo\.codelite  /s /q
rd ax32_platform_demo\.clang  /s /q

pause