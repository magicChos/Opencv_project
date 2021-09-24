#include <iostream>
#include "ini_reader.h"
#include <memory>

int main(int argc, char **argv)
{
    std::string config_file = "/home/han/project/test_code/ini/obstacle_detection.ini";
    std::shared_ptr<ace::common::INIReader> reader = std::make_shared<ace::common::INIReader>(config_file);
    if (reader->ParseError() < 0)
    {
        std::cerr << "Can't load obstacle detection params file" << std::endl;
        exit(0);
    }

    float resolution = reader->GetReal("params", "resolution", 10.0);
    bool detectObjects = reader->GetBoolean("params", "detectObjects", true);
    std::string name = reader->Get("params", "name", "hscoder");

    std::cout << "resolution: " << resolution << std::endl;
    std::cout << "detectObjects: " << detectObjects << std::endl;
    std::cout << "name: " << name << std::endl;
    return 1;
}