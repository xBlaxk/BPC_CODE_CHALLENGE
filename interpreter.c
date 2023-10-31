#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024

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

void call_function(const char* func_name) {
    if (!current_library) {
        printf("No library loaded.\n");
        return;
    }
    
    void (*func)() = dlsym(current_library, func_name);
    const char* dlsym_error = dlerror();

    if (dlsym_error) {
        printf("Error calling function: %s\n", dlsym_error);
        return;
    }

    func(); // Call the function
}

char* remove_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    return str;
}

bool handle_command(const char* command) {
    // Process the command using your interpreter logic.
    // If you detect an "exit" or "quit" command, return false.
    // Otherwise, return true.

    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
        return false;
    }

    if (strncmp(command, "use ", 4) == 0) {
        load_library(command + 4); // Skip the "use " part
    } else if (strncmp(command, "call ", 5) == 0) {
        call_function(command + 5); // Skip the "call " part
    } else {
        fprintf(stderr, "Syntax error: Unrecognized command or line format: %s\n", command);
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Entering interactive mode. Type 'exit' or 'quit' to leave.\n");

        char command[MAX_INPUT_SIZE];
        bool running = true;

        while (running) {
            printf("> ");  // prompt
            if (fgets(command, MAX_INPUT_SIZE, stdin) == NULL) {
                break;  // exit on EOF
            }

            // Remove newline character
            char* modified_command = remove_newline(command);
            
            // Check for empty line or comment line
            if (strlen(modified_command) <= 1 || strncmp(modified_command, "#", 1) == 0) {
                continue; // Skip this line and go to the next one
            }

            running = handle_command(command);
        }
    } else {
        // Open the script file
        FILE* script_file = fopen(argv[1], "r");
        if (!script_file) {
            perror("Error opening script file");
            return 1;
        }

        // Assuming a maximum line length of 256
        char line[256];
        
        // Loops through the whole file, one line per loop
        while (fgets(line, sizeof(line), script_file)) {

            // Remove newline character
            char* modified_command = remove_newline(line);
            // Removing newline character
           
            // Check for empty line or comment line
            if (strlen(modified_command) <= 1 || strncmp(line, "#", 1) == 0) {
                continue; // Skip this line and go to the next one
            }
                
            handle_command(line);
        }

        fclose(script_file);
        return 0;
    }
}
