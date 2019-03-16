
#include "Config.hpp"

Config::Config(std::string path)
{
    read_config_file(path);
}



std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

void Config::read_config_file(std::string path)
{
    std::ifstream input(path);
    if (!input.is_open())
        throw -3;
    std::string name, value;
    name.resize(256);
    value.resize(256);
    while(!input.eof())
    {
        input.getline(&name[0],256,':');
        input.getline(&value[0],256,'\n');
        name = trim(name);
        value = trim(value);
        if (!name.compare(0,9,"Page size"))
        {
            page_size = std::stol(value);
            continue;
        }
        if (!name.compare(0,11,"Memory size"))
        {
            memory_size = std::stol(value);
            continue;
        }
        if (!name.compare(0,18,"Register file size"))
        {
            num_regs = std::stol(value);
            continue;
        }
        if (!name.compare(0, 19, "Pipeline stages num"))
        {
            pipeline_stages = std::stol(value);
            continue;
        }
        if (!name.compare(0,9,"Whole log"))
        {
            if (!value.compare(0,4,"true") || !value.compare(0,4,"True")
                || !value.compare(0,4,"TRUE"))
            {
                marlin_log = true;
                continue;
            }
            if (!value.compare(0,5,"False") || !value.compare(0,5,"False")
                || !value.compare(0,5,"FALSE"))
            {
                marlin_log = false;
                continue;
            }
            throw -1;
        }
        if (!name.compare(0,8,"Log path"))
        {
            path_to_log = value;
            continue;
        }
        
    }
}