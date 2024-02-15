# NBA LIVE 2005-08 Resolution & Widescreen Update

This plugin allows user to play with widescreen resolutions in NBA Live 2005-08 without any external application.

## Installation

- Download the fixed executable for the game you would prefer windowed mode from [here](https://forums.nba-live.com/downloads.php?cat=263).

- Download [ASI Loader](https://forums.nba-live.com/downloads.php?view=detail&df_id=13583) and follow instructions in Installation section at its Readme file.

- Make sure you have the latest release from Releases.

- Copy "d3d9.dll", "oledlg.dll" and "plugins" folders from the archive to your game's root folder.
Otherwise, this plugin will not work.

- Copy "plugins" folder to your game's root folder.

## Usage

- By default, NBA Live 2005, 06 and 07 has a total of 10 resolutions available. NBA Live 08 has 16 resolutions available. v1.0 of the plugin replaces only these resolutions. Here is the reference table for resolutions.

| Resolution    | 2005         | 2006         | 2007         | 2008         |
|---------------|--------------|--------------|--------------|--------------|
| 640x480x16    | 640x480x32   | 640x480x32   | 640x480x32   | 640x480x32   |
| 640x480x32    | 800x600x32   | 800x600x32   | 800x600x32   | 800x600x32   |
| 800x600x16    | 1024x768x32  | 1024x768x32  | 1024x768x32  | 1024x768x32  |
| 800x600x32    | 1280x720x32  | 1280x720x32  | 1280x720x32  | 1280x720x32  |
| 1024x768x16   | 1280x1024x32 | 1280x1024x32 | 1280x1024x32 | 1280x1024x32 |
| 1024x768x32   | 1366x768x32  | 1366x768x32  | 1366x768x32  | 1366x768x32  |
| 1280x720x16   | -            | -            | -            | 1440x900x32  |
| 1280x720x32   | -            | -            | -            | 1600x900x32  |
| 1280x1024x16  | 1440x900x32  | 1440x900x32  | 1440x900x32  | 1600x1200x32 |
| 1280x1024x32  | 1600x900x32  | 1600x900x32  | 1600x900x32  | 1680x1050x32 |
| 1440x900x16   | -            | -            | -            | 1920x1080x32 |
| 1440x900x32   | -            | -            | -            | 2560x1440x32 |
| 1600x1200x16  | 1920x1080x32 | 1920x1080x32 | 1920x1080x32 | 3440x1440x32 |
| 1600x1200x32  | 2560x1440x32 | 2560x1440x32 | 2560x1440x32 | 3840x1080x32 |
| 1680x1050x16  | -            | -            | -            | 3840x1200x32 |
| 1680x1050x32  | -            | -            | -            | 3840x1600x32 |

- To play with the resolution you prefer, you need to navigate to Options -> Detail Settings and select resolution from left. For example, if you prefer a resolution of 1920x1080x32 in NBA Live 06, select 1600x1200x16 as your resolution.

- There is no extra hex edit needed, as aspect ratio is adjusted accordingly with width and height, rather than a fixed ratio.

## Release Notes

- v1.0: Initial release.

## Building on Windows

- Install [Visual Studio 2017/2022](https://visualstudio.microsoft.com/vs/community/) to your PC. It uses Visual Studio Installer. Community version is free and you can use that one. Check Game development with C++ and Desktop development with C++ options while installing

- Install **C++ Windows XP Support for VS 2017 (v141)** from Individual components tab at Visual Studio Installer.

- Install [Windows 8.1 SDK](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/)

- Check **Developers** section from ASI Loader’s readme. Apply the instructions. You can change output path and library paths, depending on where you place. To do so, you can extract the zip content directly. Extract the zip content (extract all).

- Clone the repository on your local and build the project using Visual Studio.

## Credits

- Dmitri (helping me with coding this, developer of FIFAM ASI Loader)