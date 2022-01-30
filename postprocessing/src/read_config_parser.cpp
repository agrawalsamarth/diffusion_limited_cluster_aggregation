#include <post.h>

namespace post_p
{

void postprocessing::read_config_parser(char *config_filename)
{

    std::ifstream parser(config_filename, std::ifstream::in);
    int count = 0;

    std::string str;
    std::vector<std::string> results;

    std::vector<std::string> xi_names;
    std::string assigned_name    = "assignedSeedStatus";
    std::string current_name     = "currentSeedStatus";
    std::string diameter_name    = "diameter";
    std::string attachments_name = "attachments";
    std::vector<std::string> att_names;

    std::vector<int> xi_idx(dim());
    int assigned_idx;
    int current_idx;
    int diameter_idx;
    int attachments_idx;
    std::vector<int> att_idx(maxAttachments());

    for (int axis = 0; axis < dim(); axis++)
        xi_names.push_back("x"+std::to_string(axis));

    for (int att = 1; att <= maxAttachments(); att++)
        att_names.push_back("att_"+std::to_string(att));

    while (count < headers_){
        getline(parser, str);
        count++;
    }

    getline(parser,str);

    results = split_string_by_delimiter(str, ',');

    for (int i = 0; i < results.size(); i++){

        if (results[i] == assigned_name)
            assigned_idx = i;

        if (results[i] == current_name)
            current_idx = i;

        if (results[i] == diameter_name)
            diameter_idx = i;

        if (results[i] == attachments_name)
            attachments_idx = i;

        for (int axis = 0; axis < dim(); axis++){
            if (results[i] == xi_names[axis])
                xi_idx[axis] = i;
        }

        for (int att = 0; att < maxAttachments(); att++){
            if (results[i] == att_names[att])
                att_idx[att] = i;
        }

    }

    count=0;

    for (count=0; count < numParticles(); count++){

        getline(parser, str);
        results = split_string_by_delimiter(str, ',');

        current_seed(count)   = stoi(results[current_idx]);
        original_seed(count)  = stoi(results[assigned_idx]);
        diameter(count)       = stod(results[diameter_idx]);
        numAttachments(count) = stoi(results[attachments_idx]);

        for (int axis = 0; axis < dim(); axis++)
            pos(count,axis) = stod(results[xi_idx[axis]]);

        for (int att = 0; att < maxAttachments(); att++){
            if (results[att_idx[att]] == "NaN")
                attachment(count,att) = numParticles();
            else
                attachment(count,att) = stoi(results[att_idx[att]]);
                
        }

    }

    parser.close();

    for (int i = 0; i < numParticles(); i++){
        radius(i) = 0.5 * diameter(i);
    }

    generator.seed(0);
    dis.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));

    for (int i = 0; i < dim(); i++){
        load_bearing_paths_[i] = 0;
    }

}


}