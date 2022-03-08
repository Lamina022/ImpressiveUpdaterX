/// ImpressiveUpdaterX.cpp : Defines the entry point for the application.
///
/// Very simple auto-updater focusing on backwards compatibility and cross-platform support.
/// Basically the program compares a local and a remote version file. If they match, it will start the game/program.
/// If they don't match, it will proceed to download a ZIP file.
/// - if the file deploy.txt exists in the current working directory, it will download the full_client.zip featuring the complete client
/// - if the deploy.txt does not exist, it will proceed to download the patch.zip featuring a patch and/or update
///
///
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
/// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
/// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
/// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/// OTHER DEALINGS IN THE SOFTWARE.
///
/// Lamina022, lamina dot anju at outlook dot com

#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")


#include "ImpressiveUpdaterX.hh"
#include "dirent.h"
#include "zip.h"
#include "LauncherConfig.h"

// Function copies
std::string hex_to_string(const std::string& in);
char* parse_url(const char* path);
size_t write_callback(const void* ptr, size_t size, size_t nmemb, FILE* stream);
int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);
CURLcode download_file(const char* urlIn, bool showProgress);
bool write_file(const char* filePath, const char* str);
std::string read_file(const char* filePath);
bool find_file(const char* file);
int on_extract_entry(const char* filename, void* arg);


///////////////////// PROGRAM START	///////////////////////////////
int main(int argc, char* argv[])
{

	// returns CURLcode
	// 0 = success
	// 60 = SSL Cert Error
	// 13 = Timeout
	// --> see curl.h for more info
	puts("Checking for new version, please wait...");
	const CURLcode result = download_file(UPDATE_VER_LINK, false);
	
	// Get current working directory (cwd)
	char cwd[FILENAME_MAX];
	GetCurrentDir(cwd, FILENAME_MAX);
	printf("Current working dir: %s\n", cwd);

	try
	{
		if (result == CURLE_OK)
		{
			puts("Fetched newest version, comparing...\n");

#if USING_EXPLICIT_FILENAMES
			const char* ver_name = UPDATE_VER_FILENAME;
			const char* patch_name = PATCH_FILENAME;
			const char* full_client_name = FULL_CLIENT_FILENAME;
#else
			const char* ver_name = parse_url(UPDATE_VER_LINK);
			const char* patch_name = parse_url(PATCH_LINK);
			const char* full_client_name = parse_url(FULL_CLIENT_LINK);
#endif

			// prepare variables for later use
			const std::string rver = read_file(ver_name);
			const std::string lver = read_file(LOCAL_VERSION_FILE);
			
			std::string command(cwd); command += APP_NAME;
			bool isDeploy;

			// check if isDeploy(true) --> download full client, else download the patch
			if (find_file(FULL_INSTALL))
			{
				printf("File '%s' FOUND.\n Installing full client\n", FULL_INSTALL);
				isDeploy = true;
			}
			else
			{
				printf("File '%s' NOT FOUND.\n Installing patch\n", FULL_INSTALL);
				isDeploy = false;
			}

			if (rver.c_str() != nullptr && lver.c_str() != nullptr || isDeploy)
			{
				printf("Newest Version: %s\n", rver.c_str());
				printf("Installed Version: %s\n", lver.c_str());

				// Versions match
				if (rver == lver)
				{
					puts("======================"); 
					puts("   NO UPDATE FOUND    ");
					puts("======================");

					// clean ups
#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
					remove(ver_name);
#ifdef _WIN64
					if (ShellExecute(NULL, reinterpret_cast<LPCWSTR>("open"), reinterpret_cast<LPCWSTR>(command.c_str()), NULL, NULL, SW_SHOWNORMAL) == reinterpret_cast<HINSTANCE>(ERROR_FILE_NOT_FOUND))
#else
					if (system(APP_NAME))
#endif
					{
						fprintf(stderr, "ERROR: Couldn't find '%s' with error code (%d): %s\n", APP_NAME, errno, strerror(errno));
						system("pause");
					}
#else
					if (unlink(ver_name) == -1) { fprintf(stderr, "File '%s' unlink error (%d): %s\n", filename, errno, strerror(errno)); }
					// try to execute a child process of the game
					exec(command);
#endif
				}

				// Versions do not match
				else
				{
					puts("======================");
					puts("     UPDATE FOUND     ");
					puts("======================");
					
					puts("Downloading files, please do NOT close this window!\n");
					printf("Fetching file from: %s\n", isDeploy ? FULL_CLIENT_LINK : PATCH_LINK);

					// if isDeploy(true), download full client, else download the patch
					const CURLcode res = download_file(isDeploy ? FULL_CLIENT_LINK : PATCH_LINK, true);

					if (res == CURLE_OK)
					{
						// extract files
						int arg = 2;
						const int result_zip = zip_extract(isDeploy ? full_client_name : patch_name, cwd, on_extract_entry, &arg);

						// was extract successful?
						if (result_zip != NULL)
						{
							printf("Error while extracting: %s", zip_strerror(result_zip));
						}
						else
						{
							puts("======================");
							puts(" SUCCESSFULLY UPDATED ");
							puts("======================");


							// clean ups
							write_file(LOCAL_VERSION_FILE, rver.c_str());

#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
							remove(ver_name);
							remove(isDeploy ? full_client_name : patch_name);
							if (isDeploy) { remove(FULL_INSTALL); }
#ifdef _WIN64
							if (ShellExecute(NULL, reinterpret_cast<LPCWSTR>("open"), reinterpret_cast<LPCWSTR>(command.c_str()), NULL, NULL, SW_SHOWNORMAL) == reinterpret_cast<HINSTANCE>(ERROR_FILE_NOT_FOUND))
#else
							if (system(APP_NAME))
#endif
							{
								fprintf(stderr, "ERROR: Couldn't find '%s' with error code (%d): %s\n", APP_NAME, errno, strerror(errno));
								throw;
							}
#else
							if unlink(isDeploy ? full_client_name : patch_name) && unlink(ver_name) == -1)
							{
								fprintf(stderr, "File '%s' unlink error (%d): %s\n", filename, errno, strerror(errno));
							}
							if (isDeploy)
							{
								if (unlink(FULL_INSTALL) == -1)
								{
									fprintf(stderr, "File '%s' unlink error (%d): %s\n", filename, errno, strerror(errno));
								}
							}

							// try to execute a child process of the game
							exec(command);
#endif
						}
					}
				}
			}
		}

		// CURLcode returned NOT 0
		else
		{
			fprintf(stderr, "ERROR: %s\n", curl_easy_strerror(result));
#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
			system("pause");
#endif
			return -1;
		}
	}

	// catch any error occurring
	catch (...)
	{
		fprintf(stderr, "ERROR: %s", strerror(errno));
#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
		system("pause");
#endif
		return -1;
	}
	return 0;
}
///////////////////// PROGRAM START	END ///////////////////////////////


//////////////////// HELPER FUNCTIONS ///////////////////////////
std::string hex_to_string(const std::string& in)
{
	std::string output;

	if ((in.length() % 2) != 0) {
		throw std::runtime_error("String is of invalid length ...");
	}

	const size_t cnt = in.length() / 2;

	for (size_t i = 0; cnt > i; ++i) {
		uint32_t s = 0;
		std::stringstream ss;
		ss << std::hex << in.substr(i * 2, 2);
		ss >> s;

		output.push_back(static_cast<char>(s));
	}

	return output;
}

char* parse_url(const char* path)
{
	char* part1 = static_cast<char*>(malloc(strlen(path)));
	char* part2 = nullptr;
	if (part1 != nullptr)
	{
		strcpy(part1, path);
		char* pos = strrchr(part1, '/');
		*pos = '\0';
		part2 = strdup(pos + 1);
	}
	return part2;
}

size_t write_callback(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	const size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}



int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	double last_progress = 0;

#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
	if (last_progress != round(dlnow / dltotal * 100))
	{
		constexpr int nb_bar = 25;
		const double progress_bar_adv = round(dlnow / dltotal * nb_bar);

		std::cout << "\r ";
		SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), 160);
		std::cout << " Download Progress: [ ";

		if (round(dlnow / dltotal * 100) < 10)
		{
			std::cout << "0" << round(dlnow / dltotal * 100) << " %]";
		}
		else
		{
			std::cout << round(dlnow / dltotal * 100) << " %] ";
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		std::cout << " [";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		for (int i = 0; i <= progress_bar_adv; i++)
		{
			std::cout << "#";
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		for (int i = 0; i < nb_bar - progress_bar_adv; i++)
		{
			std::cout << ".";
		}

		std::cout << "]";
		last_progress = round(dlnow / dltotal * 100);
	}
#endif
	return 0; /* all is good */
}

CURLcode download_file(const char* urlIn, bool showProgress)
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURLcode result = CURLE_FAILED_INIT;
	char errorBuffer[CURL_ERROR_SIZE] = { 0 };
	const std::string url(urlIn);
	const char* filename = parse_url(urlIn);

	CURL* curl_handle = curl_easy_init();
	if (curl_handle)
	{
		FILE* fp = fopen(filename, "wb");
		curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, false);

#if USE_PROXY
		curl_easy_setopt(curl_handle, CURLOPT_PROXY, PROXY_URL);
		curl_easy_setopt(curl_handle, CURLOPT_PROXYPORT, PROXY_PORT);
#endif

		// Write file chunks
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);

		if (showProgress == true)
		{
			// Show some progress
			curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, false);
			curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
		}

		// Error handling
		curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errorBuffer);
		//curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);


		result = curl_easy_perform(curl_handle);
		curl_easy_cleanup(curl_handle);
		fclose(fp);
	}
	return result;
}

bool write_file(const char* filePath, const char* str)
{
	std::ofstream file;
	const std::string readBuffer(str);
	file.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

	if (file.good())
	{
		file.write(readBuffer.c_str(), readBuffer.length());
		file.close();
		return true;
	}

	file.close();
	return false;
}

std::string read_file(const char* filePath)
{
	std::ifstream file;
	std::string readBuffer;
	file.open(filePath, std::ios::in | std::ios::binary);

	if (file.good())
	{
		std::getline(file, readBuffer);
	}

	file.close();
	return readBuffer;
}

bool find_file(const char* file)
{
	DIR* dir;
	dirent* dp;

	char cwd[FILENAME_MAX];
	GetCurrentDir(cwd, FILENAME_MAX);

	if ((dir = opendir(cwd)) == nullptr) {
		throw std::runtime_error("Unable to open directory.\n");
	}
	while ((dp = readdir(dir)) != nullptr) {
		//printf("%s\n", dp->d_name);
		if (strcmp(dp->d_name, file) == 0) { return true; }
	}
	closedir(dir);
	return false;
}

int on_extract_entry(const char* filename, void* arg)
{
	static int i = 0;
	const int n = *static_cast<int*>(arg);
	printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

	return 0;
}