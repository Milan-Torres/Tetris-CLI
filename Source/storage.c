#include "storage.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else

#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

#define PATH_MAX 4096 // Linux
#endif

char *get_app_dir_parent_on_linux() {
	char *app_dir_parent = getenv("XDG_DATA_HOME");

	if (app_dir_parent == NULL) {
		char *home_dir = getenv("HOME");

		if (home_dir == NULL) {
			struct passwd *pw = getpwuid(getuid());
			if (pw == NULL) {
				// handle error
				return NULL;
			}
			home_dir = pw->pw_dir;
		}
		app_dir_parent = home_dir;
	}

	return app_dir_parent;
}

char *get_data_dir() {
	char *data_dir;
#ifdef _WIN32
	TCHAR szPath[PATH_MAX];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
		data_dir = strdup(szPath);
	} else {
		// handle error
		return NULL;
	}
#else
	char *app_dir_parent = get_app_dir_parent_on_linux();

	data_dir = malloc(PATH_MAX);
	snprintf(data_dir, PATH_MAX, "%s/%s", app_dir_parent, APP_DIR);
#endif
	return data_dir;
}

void save_highscore_on_disk(unsigned long long int value) {
	char *data_dir = get_data_dir();
	if (data_dir == NULL) {
		// handle error
		return;
	}
	char filepath[PATH_MAX];
	snprintf(filepath, PATH_MAX, "%s/%s", data_dir, HIGHSCORE_FILE);

	// Check if the directory exists, if not, create it
#ifdef _WIN32
	if (_access(data_dir, 0) != 0) {  // Check if the directory exists
#else
	if (access(data_dir, F_OK) != 0) {  // Check if the directory exists
#endif
		// Directory doesn't exist, create it
#ifdef _WIN32
		if (_mkdir(data_dir) != 0) {
#else
		if (mkdir(data_dir, 0777) != 0) {
#endif
			// handle error
			perror("Error creating directory");
			free(data_dir);
			return;
		}
	}

	FILE *file = fopen(filepath, "wb");  // Use "wb" mode to overwrite the file if it exists
	if (file == NULL) {
		// handle error
		perror("Error opening file");
		free(data_dir);
		return;
	}
	fwrite(&value, sizeof(long long), 1, file);
	fclose(file);
	free(data_dir);
}

unsigned long long int retrieve_highscore_from_disk() {
	char *data_dir = get_data_dir();
	if (data_dir == NULL) {
		return 0;
	}
	char filepath[PATH_MAX];
	snprintf(filepath, PATH_MAX, "%s/%s", data_dir, HIGHSCORE_FILE);
	FILE *file = fopen(filepath, "rb");
	if (file == NULL) {
		free(data_dir);
		return 0;
	}
	long long value;
	size_t items_read = fread(&value, sizeof(long long), 1, file);
	if (items_read != 1) {
		fprintf(stderr, "Error: Unable to read value from file '%s'\n", filepath);
		fclose(file);
		free(data_dir);
		return -1;
	}
	fclose(file);
	free(data_dir);
	return value;
}
