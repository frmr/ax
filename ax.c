#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    AX_UNKNOWN,
    AX_COPY,
    AX_DELETE,
    AX_LIST,
    AX_PASTE
} AxMode;

AxMode determineMode(const int argc, char* argv[]) {
    if (argc == 1) {
        printf("Usage: ax option [files]\n");
        printf("Options: -c (copy)\n");
        printf("         -d (delete)\n");
        printf("         -l (list)\n");
        printf("         -p (paste)\n");
    }
    else {
        if (!strcmp(argv[1], "-c")) {
            return AX_COPY;
        }
        else if (!strcmp(argv[1], "-d")) {
            return AX_DELETE;
        }
        else if (!strcmp(argv[1], "-l")) {
            return AX_LIST;
        }
        else if (!strcmp(argv[1], "-p")) {
            return AX_PASTE;
        }
    }

    return AX_UNKNOWN;
}

FILE* openListFile(char *mode) {
    struct passwd *passwdEnt = getpwuid(getuid());
    char *home = passwdEnt->pw_dir;
    
    char listFilename[AX_LIST_FILENAME_LENGTH];
    sprintf(listFilename, "%s/.ax", home);
    
    FILE *listFile = fopen(listFilename, mode);
    if (listFile == NULL) {
        printf("Could not open list file\n");
    }

    return listFile;
}

int main(int argc, char* argv[]) {

    AxMode mode =  determineMode(argc, argv);
    
    if (mode == AX_COPY) {
        FILE *listFile = openListFile("w");
        if (listFile) {
            char cwd[AX_CWD_LENGTH];
            getcwd(cwd, AX_CWD_LENGTH);
            for (int i = 2; i < argc; ++i) {
                fprintf(listFile, "%s/%s\n", cwd, argv[i]);
            }
            fclose(listFile);
        }
    }
    else if (mode == AX_DELETE) {
        FILE *listFile = openListFile("r");
        if (listFile) {
            char line[AX_LINE_LENGTH];
            char command[AX_COMMAND_LENGTH];
            while (fgets(line, AX_LINE_LENGTH, listFile)) {
                sprintf(command, "rm %s", line);
                system(command);
            }
            fclose(listFile);
        }
        
    }
    else if (mode == AX_LIST) {
        FILE *listFile = openListFile("r");
        if (listFile) {
            char line[AX_LINE_LENGTH];
            while (fgets(line, AX_LINE_LENGTH, listFile)) {
                printf("%s", line);
            }
            fclose(listFile);
        }
    }
    else if (mode == AX_PASTE) {
        FILE *listFile = openListFile("r");
        if (listFile) {
            char line[AX_LINE_LENGTH];
            char command[AX_COMMAND_LENGTH];
            while (fgets(line, AX_LINE_LENGTH, listFile)) {
                sprintf(command, "cp -t . %s", line);
                system(command);
            }
            fclose(listFile);
        }
        
    }
    
    return 0;
}
