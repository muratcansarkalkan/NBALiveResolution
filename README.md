# NBA LIVE 2005-08 Resolution & Widescreen Update

This plugin allows user to play with widescreen resolutions in NBA Live 2005-08 without any external application.

## Installation

- Download the NO-CD fixed executable for the game you would prefer windowed mode. You need the NO-CD fixed executable as the plugin is designed for a certain version of an executable. NO-CD is also advised as original CD's of NBA Live 2005-08 series unfortunately do not run in Windows 10 and further.

- Download [ASI Loader](https://forums.nba-live.com/downloads.php?view=detail&df_id=13583) and follow instructions in Installation section at its Readme file.

- Make sure you have the latest release from Releases.

- Copy "d3d9.dll", "oledlg.dll" and "plugins" folders from the archive to your game's root folder.
Otherwise, this plugin will not work.

- Copy "plugins", "assets" and "movies" folders and "main.ini" file to your game's root folder.

- If you have NBALiveResolution_d.asi in "plugins" folder, remove the file, so that the new plugin can work properly.

- New version of plugin introduces "main.ini" where both windowed mode and menu resolution can be operated from same file. If you installed Windowed Mode plugin before, please re-download latest version.

- Open "main.ini" and you can set RES_X and RES_Y values (width and height) to an integer you want. For example, if you want to play the game in full-screen and 1920x1080 resolution, changed WINDOWED value from 1 to 0, change RES_X value from 1366 to 1920 and RES_Y value from 768 to 1080.

- To enable videos, change INTRO value under BOOTUP to 1. To disable, change to 0.

## Usage

- By default, NBA Live 2005, 06 and 07 has a total of 10 resolutions available. NBA Live 08 has 16 resolutions available. v1.0 of the plugin replaces only these resolutions. Here is the reference table for resolutions.

| Resolution    | 2005         | 2006         | 2007         | 2008         |
|---------------|--------------|--------------|--------------|--------------|
| 640x480x16    | 640x480x32   | 640x480x32   | 640x480x32   | 640x480x32   |
| 640x480x32    | 800x600x32   | 800x600x32   | RES_XxRES_Yx32*   | RES_XxRES_Yx32*   |
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

- To change resolutions at start and menus, you can modify RES_X and RES_Y values.

- For NBA Live 07 and 08, if there aren't any resolutions specified in wndmode.ini, default resolutions are 800x600x32.

- To play with the resolution you prefer, you need to navigate to Options -> Detail Settings and select resolution from left. For example, if you prefer a resolution of 1920x1080x32 in NBA Live 06, select 1600x1200x16 as your resolution.

- There is no extra hex edit needed, as aspect ratio is adjusted accordingly with width and height, rather than a fixed ratio.

- For NBA Live 2005 and 06, there are two sizes of intros that can be created so the plugin can properly resize the videos, until I update the function properly. If you want to create videos 4:3 aspect ratio, you need to create it with a size of 640x480. If you want to create with 16:9 instead, you need to create it with a size of 1920x1088.

- For NBA Live 07 and 08, there are no restrictions of creating video files, as game fits the video to the screen by default.

## Release Notes

- v1.0: Initial release.

- v1.01: Widescreen feature for UI and custom resolution.

- v1.02: Added widescreen adjusted UI components for NBA Live 2005 and NBA Live 06. This update will not change any of the original files and will be only used if you prefer a widescreen resolution.

- v1.03: Fixed scale and position for loadbar, player model at edit zone and movies. Added widescreen videos for 05 and 06. Credits to iceman for his help. Added enabling/disabling video functionality.

## FAQ

- **Q**: When I select resolution at detail settings, I still see old values. When will you fix them?
- **A**: Yes, I am working on this. It will be fixed in the coming updates.

## Building on Windows

- Install [Visual Studio 2017/2022](https://visualstudio.microsoft.com/vs/community/) to your PC. It uses Visual Studio Installer. Community version is free and you can use that one. Check Game development with C++ and Desktop development with C++ options while installing

- Install **C++ Windows XP Support for VS 2017 (v141)** from Individual components tab at Visual Studio Installer.

- Install [Windows 8.1 SDK](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/)

- Check **Developers** section from ASI Loader’s readme. Apply the instructions. You can change output path and library paths, depending on where you place. To do so, you can extract the zip content directly. Extract the zip content (extract all).

- Clone the repository on your local and build the project using Visual Studio.

## Credits

- Dmitri (helping me with coding this, developer of FIFAM ASI Loader)

- wiscard_rush (UI components)

- JuicyShaqMeat (UI components)

- iceman (Widescreen intro videos for 2005 and 06)