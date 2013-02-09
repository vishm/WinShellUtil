# WinShellUtil

## Introduction

Allows one to add non Windows file shares to the Windows Library. Typically this is not permitted via Windows Explorer. The caveat of adding a non windows file share to Windows library is that you'll receive downlevel features ie. indexing won't be available for the shared added.

## Usage

    WinShellUtil AddFolder Documents \\mynas\home
    WinShellUtil RemoveFolder Documents \\mynas\home

    WinShellUtil AddFolder Videos \\mynas\movies
    WinShellUtil RemoveFolder Videos \\mynas\movies

Where "Documents", "Videos", "Music" or any custom folder within your Windows library

## Requirements

Microsoft Visual Studio 2012

    msbuild /p:Configuration=Release /t:rebuild WinShellUtil.sln

## Tested Platforms

Microsoft Windows 8

### License
MIT