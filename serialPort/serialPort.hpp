#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

class serialPort{
private:
    int baudRate;
    bool parityBit;
    bool stopBit;

    int serialDevice;

public:
};