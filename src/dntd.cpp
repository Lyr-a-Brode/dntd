#include "dntd.h"

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/dnn.hpp"

using namespace cv;
using namespace dnn;

namespace dntd
{
    float confThreshold = 0.3; // Confidence threshold
    float nmsThreshold = 0.4;  // Non-maximum suppression threshold
    int inpWidth = 416;  // Width of network's input image
    int inpHeight = 416; // Height of network's input image

    constexpr auto modelConfiguration = "/home/oleg/workspace/donatello/dntd/dnn/yolov3.cfg";
    constexpr auto modelWeights = "/home/oleg/workspace/donatello/dntd/dnn/yolov3.weights";

    std::vector<String> getOutputsNames(const Net& net)
    {
        static std::vector<String> names;
        if(names.empty())
        {
            //Get the indices of the output layers, i.e. the layers with unconnected outputs
            std::vector<int> outLayers = net.getUnconnectedOutLayers();
            
            //get the names of all the layers in the network
            std::vector<String> layersNames = net.getLayerNames();
            
            // Get the names of the output layers in names
            names.resize(outLayers.size());
            for (size_t i = 0; i < outLayers.size(); ++i){
                names[i] = layersNames[outLayers[i] - 1];
            }
        }
    
        return names;
    }

    void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
    {
        //Draw a rectangle displaying the bounding box
        rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 255, 255), 1);

        circle(frame, Point(left + cvRound((right - left)/2), top + cvRound(bottom - top)/2), 10, Scalar(255, 255, 255));
        
        //Get the label for the class name and its confidence
        std::string label = format("%.2f", conf);
        
        //Display the label at the top of the bounding box
        int baseLine;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        top = max(top, labelSize.height);
        rectangle(frame, Point(left, top - labelSize.height), Point(left + labelSize.width, top + baseLine), Scalar(255, 255, 255), FILLED);
        putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0),1,LINE_AA);
    }

    bool postProcess(Mat& frame, const std::vector<Mat>& outs)
    {
        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<Rect> boxes;
        
        for (size_t i = 0; i < outs.size(); ++i)
        {
            // Scan through all the bounding boxes output from the network and keep only the
            // ones with high confidence scores. Assign the box's class label as the class
            // with the highest score for the box.
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double confidence;
                // Get the value and location of the maximum score
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;
                    
                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }
        
        // Perform non maximum suppression to eliminate redundant overlapping boxes with
        // lower confidences
        std::vector<int> indices;
        NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
        for (size_t i = 0; i < indices.size(); ++i)
        {
            int idx = indices[i];
            /*Rect box = boxes[idx];
            drawPred(classIds[idx], confidences[idx], box.x, box.y,
                    box.x + box.width, box.y + box.height, frame);*/

            if (classIds[idx] == 53) 
            {
                std::cout << "Found pizza " << classIds[idx] << "\n";

                return true;
            }
        }

        return false;
    }

    bool is_pizza(std::vector<unsigned char> &imgData)
    {        
        Mat img = imdecode(imgData, IMREAD_UNCHANGED);
        

        Net net = readNetFromDarknet(modelConfiguration, modelWeights);
        net.setPreferableBackend(DNN_BACKEND_OPENCV);
        net.setPreferableTarget(DNN_TARGET_CPU);

        std::vector<Mat> outs;
        Mat blob;
        blobFromImage(img, blob, 1/255.0, Size(inpWidth, inpHeight), Scalar(0,0,0), true, false);
        
        net.setInput(blob);
        net.forward(outs, getOutputsNames(net));        

        /*imshow("Pizza", gimg);
        waitKey(0);*/

        return postProcess(img, outs);
    }
}