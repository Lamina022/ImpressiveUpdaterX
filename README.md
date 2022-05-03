# ImpressiveUpdaterX
Very simple auto-updater focusing on backwards compatibility and cross-platform support.
Basically the program compares a local and a remote version file. If they match, it will start the game/program.
If they don't match, it will proceed to download a ZIP file.
![Impressive Updater X](https://i.imgur.com/nz7bEmD.png)


## Features
- you can choose to make a whole re-install of the games client, if a certain file exists in the current working directory (for ex. 'deploy.txt'). 
    
    --> if the file is found, it will download the full client package featuring the complete client

- if the remote version does not match with the local one, it will proceed to download the patch ZIP featuring a patch and/or update

- you can use any host to store your client files on. For example Google Drive or Media Fire.
    
    --> you will need to create static direct links to the ZIP files and the remote version, which will not change.
    
    --> HowTo Google Drive: https://www.youtube.com/watch?v=lebCH3l6ePc


## 'Why'
I am the developer of the software Impressive Launcher, which is a launcher mainly tailored for the KITO community and Impressive Title servers.
But since my current application did not work well with Windows 7 and UNIX-link systems at the time, I needed a simple and reliable solution. So the Impressive Updater X was born with the 'X' indicating the cross-platform and compatiblity aim.

NOTE: Impressive Launcher now fully supports Windows 7


## Software used
- Visual Studio 2022 Community
- CMake (it was not setup proberly and may need modification)
- libcurl ver. 7.59.0 (fetched from NuGet packages)
- openSSL ver 1.0.2 (fetched from NuGet packages)
- Kuba's simple and reliable ZIP port (https://github.com/kuba--/zip)
