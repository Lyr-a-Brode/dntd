#ifndef DNTD_OPTIONS_H
#define DNTD_OPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>

namespace dntd
{
    struct Options
    {
        std::string listenUrl;

        static Options parse(int argc, char** argv); // throws ParseError

        private:
            Options(std::string listenUrlIn)
                : listenUrl(listenUrlIn) {};                    

    };

    class ParseError : public std::runtime_error
    {
        public:
            ParseError(std::string msg)
                : runtime_error(msg)
            {}
    };
}

#endif