:: By downloading or using this file, the user agrees to be bound by the terms of the license 
:: agreement located at http://ocaalliance.com/EULA as an original contracting party.
@echo off

:: TODO write the BUILD_VERSION in the version file on bases of the commit.

:: First extract the version from the version file.
for /f "tokens=2,3" %%a in ('type Aes70CompliancyTestTool\VersionDefs.h') do (
    IF %%a == MAJOR_VERSION (set MAJOR_VERSION=%%b)
    IF %%a == MINOR_VERSION (set MINOR_VERSION=%%b)
    IF %%a == BUILD_VERSION (set BUILD_VERSION=%%b)
)
echo "Building with version %MAJOR_VERSION%.%MINOR_VERSION%.%BUILD_VERSION%"

:: This requires doxygen to be installed
echo "Generate documentation"
mkdir out
doxygen --version
doxygen Aes70CompliancyTestToolConsoleDocumentation.doxygen
echo "Done.."

:: This required the HTML Help workshop to be installed.
echo "Compile the documentation"
"C:\Program Files (x86)\HTML Help Workshop\hhc.exe" Out\Aes70CompliancyTestToolDocumentation\html\index.hhp
echo "Done.."

echo "Start build Compliancy Test Tool"
msbuild Aes70CompliancyTestTool.sln /property:Configuration=Debug /property:Platform=x86
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 (
    echo "Failed to build Debug/x86"
    GOTO End
)
 
msbuild Aes70CompliancyTestTool.sln /property:Configuration=Release /property:Platform=x86
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 (
    echo "Failed to build Release/x86"
    GOTO End
)

msbuild Installer\Aes70CompliancyTestTool.sln /property:Configuration=Release /property:Platform=x86 /p:ProductVersion="%MAJOR_VERSION%.%MINOR_VERSION%.%BUILD_VERSION%"
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 (
    echo "Failed to build Release/x86 installer"
    GOTO End
)

msbuild Aes70CompliancyTestTool.sln /property:Configuration=Debug /property:Platform=x64
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 (
    echo "Failed to build Debug/x64"
    GOTO End
)

msbuild Aes70CompliancyTestTool.sln /property:Configuration=Release /property:Platform=x64
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 (
    echo "Failed to build Release/x64"
    GOTO End
)

msbuild Installer\Aes70CompliancyTestTool.sln /property:Configuration=Release /property:Platform=x64 /p:ProductVersion="%MAJOR_VERSION%.%MINOR_VERSION%.%BUILD_VERSION%"
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 (
    echo "Failed to build Release/x86 installer"
    GOTO End
)

echo "Done building"

:End