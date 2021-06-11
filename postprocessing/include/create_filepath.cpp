#include "post.h"

namespace post_p
{

void postprocessing::create_filepath(char *folder, char *filename)
{

    filepath_ = (char*)malloc(strlen(folder) + strlen(filename) + 1);
    strcpy(filepath_,folder);
    strcat(filepath_,filename);
}


}