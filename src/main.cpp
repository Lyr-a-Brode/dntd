#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "nngpp/nngpp.h"
#include "nngpp/protocol/req0.h"
#include "nngpp/protocol/rep0.h"

#include "options.h"

int main(int argc, char** argv)
{
    /*cv::Mat img = cv::imread("/home/oleg/workspace/datasets/pizza/1.jpg");

    cv::imshow("Pizza", img);
    cv::waitKey(0);*/    

    try
    {
        auto opts = dntd::Options::parse(argc, argv);

        std::cout << opts.listenUrl << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    

    return 0;
}