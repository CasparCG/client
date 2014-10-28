@echo off

:: Fail early if environment not set
if not defined BUILD_QMAKE exit /b 1
if not defined BUILD_MINGW_BIN exit /b 1
if not defined BUILD_ARCHIVE_NAME exit /b 1
if not defined BUILD_PARALLEL_THREADS exit /b 1
if not defined BUILD_7ZIP exit /b 1

:: Clean and enter shadow build folder
echo Cleaning...
if exist ..\build rmdir ..\build /s /q || goto :error
mkdir ..\build || goto :error
cd ..\build || goto :error

:: Run qmake specified by BUILD_QMAKE
echo Running qmake...
"%BUILD_QMAKE%" ..\src\Solution.pro -r -spec win32-g++ "CONFIG+=release" || goto :error

:: Run make using the number of hardware threads in BUILD_PARALLEL_THREADS
set PATH=%PATH%;%BUILD_MINGW_BIN%
set MAKE_COMMAND=mingw32-make -j%BUILD_PARALLEL_THREADS%
echo Building...
mingw32-make || goto :error

:: Create client folder to later zip
set CLIENT_FOLDER=CasparCG Client
if exist "%CLIENT_FOLDER%" rmdir "%CLIENT_FOLDER%" /s /q || goto :error
mkdir "%CLIENT_FOLDER%\%CLIENT_FOLDER%" || goto :error

:: Copy compiled binaries
echo Copying binaries...

for /r %%x in (*.dll) do (
    if not exist "%CLIENT_FOLDER%\%CLIENT_FOLDER%\%%~nx.dll" copy "%%x" "%CLIENT_FOLDER%\%CLIENT_FOLDER%" /Y || goto :error
)

copy Shell\release\shell.exe "%CLIENT_FOLDER%\%CLIENT_FOLDER%\CasparCG Client.exe" || goto :error

:: Copy binary dependencies
echo Copying binary dependencies...
copy ..\deploy\win32\* "%CLIENT_FOLDER%\%CLIENT_FOLDER%" || goto :error
xcopy ..\deploy\win32\sqldrivers "%CLIENT_FOLDER%\%CLIENT_FOLDER%\sqldrivers" /E /I /Y || goto :error
xcopy ..\deploy\win32\plugins "%CLIENT_FOLDER%\%CLIENT_FOLDER%\plugins" /E /I /Y || goto :error

:: Copy documentation
echo Copying documentation...
copy ..\CHANGES.TXT "%CLIENT_FOLDER%" || goto :error
copy ..\LICENSE.TXT "%CLIENT_FOLDER%" || goto :error

:: Create zip file
echo Creating zip...
"%BUILD_7ZIP%" a "%BUILD_ARCHIVE_NAME%.zip" "%CLIENT_FOLDER%" || goto :error

:: Skip exiting with failure
goto :EOF

:error
exit /b %errorlevel%
