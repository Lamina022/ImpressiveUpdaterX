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
// CAUTION: Links must stay static and MAY NOT change !!!
// Links may not have a "/" at the end!
const char* updateVersion = "https://minerva.internet-box.ch/index.php/s/g5R8DfzWoiQd9oz/download/updateVersion.txt"; // Remote version always indicates newest version

// Packages to download
const char* patchFile = "https://minerva.internet-box.ch/index.php/s/FtHXPrbdYWo44RR/download/patch.zip"; // Direct link to patch.zip
const char* fullClientFile = "https://minerva.internet-box.ch/index.php/s/Tn3HkNQnXCJHY2Y/download/full_client.zip"; // Direct link to full_client.zip

// Proxy Settings
#define USE_PROXY FALSE // FALSE = don't use / TRUE = use
#define PROXY_URL "http://my.proxy.net" // replace with your actual proxy
#define PROXY_PORT 8080L // the proxy's port
/////////////////////////////////////////////////////////////////////////