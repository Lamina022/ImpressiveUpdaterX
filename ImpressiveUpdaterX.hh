// ImpressiveUpdaterX.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif