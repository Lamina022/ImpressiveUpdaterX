#pragma once

/////////////////////////////////////////////////////////////////////////
/////////////////////// CONFIGURATION ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////


// The name of the application/game to start if everything's up-to-date
#define APP_NAME "game.exe"


// Name of the local version text file
#define LOCAL_VERSION_FILE "version.txt"


// If this file exists, it will download the full_client.zip
#define FULL_INSTALL "deploy.txt"



// Fully Qualified Destination Path (FQDN) to files on server
// CAUTION NR.1: Links must stay static and MAY NOT change !!!
// CAUTION NR.2: If your link DOES NOT end with the filename (including its extension), you need to manually specify the filename in a separate variable
// Links may not have a "/" at the end!

#define UPDATE_VER_LINK "https://minerva.internet-box.ch/index.php/s/g5R8DfzWoiQd9oz/download/updateVersion.txt" // Remote version always indicates newest version


// Packages to download
#define PATCH_LINK "https://minerva.internet-box.ch/index.php/s/FtHXPrbdYWo44RR/download/patch.zip" // Direct link to patch.zip
#define FULL_CLIENT_LINK "https://minerva.internet-box.ch/index.php/s/Tn3HkNQnXCJHY2Y/download/full_client.zip" // Direct link to full_client.zip



// Use explicit filenames
// Set USING_EXPLICIT_FILENAMES to TRUE if you are using obfuscated direct links like ones from Google Drive
// Then set the filenames accordingly
#define USING_EXPLICIT_FILENAMES FALSE // FALSE = don't use / TRUE = use
#define UPDATE_VER_FILENAME "updateVersion.txt"	// Filename of the remote version, which always indicates newest version
#define PATCH_FILENAME "patch.zip" // Filename of the patch ZIP
#define FULL_CLIENT_FILENAME "full_client.zip" // Filename of the ZIP storing the whole client/software



// Proxy Settings
#define USE_PROXY FALSE // FALSE = don't use / TRUE = use
#define PROXY_URL "http://my.proxy.net" // replace with your actual proxy
#define PROXY_PORT 8080L // the proxy's port
/////////////////////////////////////////////////////////////////////////