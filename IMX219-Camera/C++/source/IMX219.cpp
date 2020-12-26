#include <string>
#include "IMX219.hpp"

// std::string get_tegra_pipeline(int width, int height, int fps) {
//     return "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) 
//             + ", height=(int)" + std::to_string(height) 
//             + ", format=(string)I420, framerate=(fraction)" + std::to_string(fps) +
//            "/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
// }

std::string get_tegra_pipeline(int width, int height, int fps)
{
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)"+ std::to_string(width)
            +", height=(int)" + std::to_string(height) 
            + ", format=(string)NV12, framerate=(fraction)" + std::to_string(fps) 
            + "/1 ! nvvidconv flip-method=0 ! "
            + "video/x-raw, width=(int)"+ std::to_string(width) 
            + ", height=(int)"+ std::to_string(height) 
            + ", format=(string)BGRx ! "
            + "videoconvert ! "
            + "video/x-raw, format=(string)BGR ! appsink";
}
