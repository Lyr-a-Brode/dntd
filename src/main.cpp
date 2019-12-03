#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "nngpp/nngpp.h"
#include "nngpp/protocol/req0.h"
#include "nngpp/protocol/rep0.h"

#include "options.h"
#include "dntd.h"

int main(int argc, char** argv)
{ 
    try
    {
        auto opts = dntd::Options::parse(argc, argv);

        /*std::ifstream fs("/home/oleg/workspace/datasets/pizza/5.jpg", std::ios::binary);        

        fs.seekg(0, fs.end);
        int length = fs.tellg();
        fs.seekg(0, fs.beg);    
        std::vector<unsigned char> imgData(length);
        fs.read((char*)&imgData[0], length);
        fs.close();*/

        auto sock = nng::rep::open();
	    sock.listen(opts.listenUrl.c_str());

        std::cout << "Listening on port " << opts.listenUrl << "\n";

        while (true) 
        {
            auto buf = sock.recv();

            std::cout << "Request received, size " << buf.size() << "bytes\n";

            auto *start = buf.data<unsigned char>();
            std::vector<unsigned char> imgData(start, start + buf.size());

            if (dntd::is_pizza(imgData))
            {
                sock.send("true");
            }
            else
            {
                sock.send("false");
            }
            
        }        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    

    return 0;
}