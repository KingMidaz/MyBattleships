@echo off
cls
:start
Battleships.exe -b "..\Bots\MyBot\x64\Release" "..\Bots\C#" --clog --pretty --debug -s 2
pause
goto start