#include "options.h"

#include <sstream>

#include "cxxopts.hpp"

dntd::Options dntd::Options::parse(int argc, char** argv)
{
    
    cxxopts::Options options("dntd", "a daemon for evaluating pizza quality");
    options.add_options()
        ("h,host", "Host to listen", cxxopts::value<std::string>()->default_value("localhost"))
        ("p,port", "Port to listen", cxxopts::value<int>()->default_value("8000"));

    try {
        auto result = options.parse(argc, argv);

        std::stringstream s;
        s << "tcp://" << result["host"].as<std::string>() << ":" << result["port"].as<int>();
        return Options{s.str()};        
    } catch (const std::exception &e) {
        throw ParseError(e.what());
    } 
}