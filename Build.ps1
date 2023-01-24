param([string]$artifactsRootDirectory)


$projectName = "LibMicroNetwork"
$projectDirectory = $PSScriptRoot
$buildDirectory = "$projectDirectory/build"

if([String]::IsNullOrWhiteSpace($artifactsRootDirectory)){
    Write-Host "Using default artifacts directory"
    $artifactsRootDirectory = "$PSScriptRoot/.artifacts"
}

if (Test-Path -Path $artifactsRootDirectory) {
    
}else{
    New-Item -Path $artifactsRootDirectory -ItemType Directory
}

if (Test-Path -Path $artifactsRootDirectory) {
    $artifactsOutputDirectory = "$artifactsRootDirectory/$projectName/Linux/x86_64"

    if (!(Test-Path -Path $artifactsRootDirectory)) {
        New-Item -Path $artifactsOutputDirectory -ItemType Directory
    }
}

if (Test-Path -Path $buildDirectory) {
    Remove-Item $buildDirectory -Recurse
}

New-Item -Path $buildDirectory -ItemType Directory


pushd $buildDirectory
cmake -D CMAKE_C_COMPILER=/usr/local/gcc-12.2.0/bin/x86_64-linux-gnu-gcc -D CMAKE_CXX_COMPILER=/usr/local/gcc-12.2.0/bin/x86_64-linux-gnu-g++ ..
cmake --build . --target MicroNetwork --config Release

Copy-Item "./libMicroNetwork.so" -Destination "$artifactsOutputDirectory"
popd


