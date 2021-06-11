#include "post.h"

namespace post_p
{
void postprocessing::create_dir(char *folder)
{

    int check;
    struct stat sb;
    char mkdir_command[] = "mkdir ";
    char *command;

    if (stat(folder, &sb) == -1) {
            command = (char*)malloc(strlen(mkdir_command) + strlen(folder) + 1);
            strcpy(command, mkdir_command);
            strcat(command, folder);
            check = system(command);

            free(command);

    }

    else {

        if ((sb.st_mode & S_IFMT) != S_IFDIR){
            command = (char*)malloc(strlen(mkdir_command) + strlen(folder) + 1);
            strcpy(command, mkdir_command);
            strcat(command, folder);
            check = system(command);

            free(command);
        }


    }


}

}