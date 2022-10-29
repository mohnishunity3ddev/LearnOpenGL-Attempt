. $PSScriptRoot\_variables.ps1

# Required arguments
$projNameArg = $args[0]
$chapterNumberArg = $args[1]

if($null -eq $projNameArg) {
    Write-Warning "no project name given. Creating template 'tempproj'"
    $projNameArg = "tempproj"
}

Set-Location $PROJECT_ROOT_DIR_PATH

mkdir .\$projNameArg
Set-Location .\$projNameArg

mkdir bin
mkdir src
touch .\src\main.cpp
touch CMakeLists.txt

$chapterNumber = ""
if($null -eq $chapterNumberArg) {
    $chapterNumber = "##"
} else {
    $chapterNumber = $chapterNumberArg
}
# Add content
Add-Content -Path .\CMakeLists.txt -Value "cmake_minimum_required(VERSION 3.22.0)`n`nproject($projNameArg)`n`ninclude(../../../cmake_macros/prac.cmake)`n`nSETUP_APP($projNameArg `"Chapter$chapterNumber`")"
Add-Content -Path .\src\main.cpp -Value "#include <iostream>`n`nint main() {`n    std::cout << `"Hello, World! from $projNameArg\n`";`n    return 0;`n}"
Add-Content -Path $PROJECT_ROOT_DIR_PATH\CMakeLists.txt -Value "`nadd_subdirectory(src/Chapter$chapterNumber/$projNameArg)"

if($null -ne $chapterNumberArg) {
    Set-Location ..
    $projectPath = "$PROJECT_ROOT_DIR_PATH\src\Chapter$chapterNumber"
    if((Test-Path -Path $projectPath) -eq $false) {
        mkdir $projectPath
    }
    Move-Item .\$projNameArg $projectPath
}