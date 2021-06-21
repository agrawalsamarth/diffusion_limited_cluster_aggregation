#include <post.h>

namespace post_p
{

void postprocessing::name_postp_dir(char *config_filename)
{

    char parent_folder[] = "./postprocessing_results/";


    std::vector<std::string> results;
    results = split_string_by_delimiter(config_filename,'.');
    results = split_string_by_delimiter(results[(results.size()-2)], '/');

    std::string user_folder = results[1];

    for (int i = 2; i < results.size(); i++){
        user_folder += "_";
        user_folder += results[i];
    }

    //std::cout<<user_folder<<std::endl; 

    folder_name_ = (char*)malloc(strlen(parent_folder)+user_folder.length()+strlen("/")+1);
    strcpy(folder_name_,parent_folder);
    strcat(folder_name_,user_folder.c_str());
    strcat(folder_name_,"/");

    //std::cout<<folder_name_<<std::endl;


}



}