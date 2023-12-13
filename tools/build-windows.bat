@echo off

:: Fail early if environment not set
if not defined BUILD_QT_PATH set BUILD_QT_PATH=C:\Qt\6.5.3
if not defined BUILD_ARCHIVE_NAME set BUILD_ARCHIVE_NAME=casparcg_client
if not defined BUILD_PARALLEL_THREADS set BUILD_PARALLEL_THREADS=8
if not defined BUILD_7ZIP set BUILD_7ZIP=C:\Program Files\7-Zip\7z.exe

set BUILD_VCVARSALL=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
set BUILD_7ZIP=C:\Program Files\7-Zip\7z.exe

:: Github Actions has Enterprise available
if DEFINED CI set BUILD_VCVARSALL=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat

:: Clean and enter shadow build folder
echo Cleaning...
if exist ..\build rmdir ..\build /s /q || goto :error
mkdir ..\build || goto :error
cd ..\build || goto :error

:: Setup VC++ environment
echo Setting up VC++...
call "%BUILD_VCVARSALL%" amd64 || goto :error

:: Run cmake
echo Running cmake...
cmake -G "Visual Studio 17 2022" -A x64 -DQt6_ROOT="%BUILD_QT_PATH%\msvc2019_64" ..\src || goto :error

:: Build with MSBuild
echo Building...
msbuild "Solution.sln" /t:Clean /p:Configuration=Release || goto :error
msbuild "Solution.sln" /p:Configuration=Release /m:%BUILD_PARALLEL_THREADS% || goto :error

:: Create client folder to later zip
set CLIENT_FOLDER=%BUILD_ARCHIVE_NAME%
if exist "%CLIENT_FOLDER%" rmdir "%CLIENT_FOLDER%" /s /q || goto :error
mkdir "%CLIENT_FOLDER%\%CLIENT_FOLDER%" || goto :error

:: Copy compiled binaries
echo Copying binaries...
xcopy Release\* "%CLIENT_FOLDER%\%CLIENT_FOLDER%\"  /E /I /Y
call "%BUILD_QT_PATH%\msvc2019_64\bin\windeployqt.exe" --dir "%CLIENT_FOLDER%\%CLIENT_FOLDER%" Release\casparcg-client.exe

:: Copy documentation
echo Copying documentation...
copy ..\CHANGELOG "%CLIENT_FOLDER%" || goto :error
copy ..\LICENSE "%CLIENT_FOLDER%" || goto :error

:: Create zip file
echo Creating zip...
"%BUILD_7ZIP%" a "%BUILD_ARCHIVE_NAME%.zip" "%CLIENT_FOLDER%" || goto :error

:: Skip exiting with failure
goto :EOF

:error
exit /b %errorlevel%
