call ..\..\SetEnv.bat
call cclean4ecl.cmd


rem pause

rem set FKP=%~dp0freertos
rem set
pause

rem cmake -S src -B . -G "Eclipse CDT4 - Ninja" -D CMAKE_BUILD_TYPE=Debug -D FREERTOS_KERNEL_PATH=%FKP%
cmake -B . -G "Eclipse CDT4 - Ninja" -D CMAKE_BUILD_TYPE=Debug

pause


