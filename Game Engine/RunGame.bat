@echo off
cls
:start
Battleships.exe -b "..\Bots\TheKiller" "..\Bots\MyBot" --clog --pretty --debug -s 2 --nolimit
goto start