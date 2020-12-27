#include <iostream>
#include <time.h>
#include <string>

#include "serialPort.hpp"

using namespace std;

int main()
{
    serialPort sp;
    std::string testData = "www.singularity-blog.top";
    for( auto i = 0; i<10; i++)
    {
        sp.sendData(testData.data(),testData.size());
        sleep(4);
    }

    return 0;
}
