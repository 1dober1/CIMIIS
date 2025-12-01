rem %1 - Solution dir
rem %2 - Version template file
rem %3 - Version header file

@echo off
if "%1"=="" goto run_error
if "%2"=="" goto run_error
if "%3"=="" goto run_error

rem echo Solution: %1
rem echo Template: %2
rem echo Header:   %3

echo Collecting version information...
subwcrev.exe %1 %2 %3
if errorlevel 10 goto end
if errorlevel 1 goto svn_error
goto end

:run_error
echo Batch file must have 3 parameters!
goto end

:svn_error
echo Cannot create version header file!
goto end

:end
set errorlevel=0