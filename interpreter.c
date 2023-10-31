#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
void* current_library = NULL; // Pointer to store the current library handle

void load_library(const char* lib_name) {
    current_library = dlopen(lib_name, RTLD_LAZY);
    if (!current_library) {
        const char* dlsym_error = dlerror();
        if (strstr(dlsym_error, "file too short") != NULL) {
            printf("Error: The file '%s' is not a valid shared library.\n", lib_name);
        } else {
            printf("Error loading library: %s\n", dlsym_error);
        }
        return;
    }
}
