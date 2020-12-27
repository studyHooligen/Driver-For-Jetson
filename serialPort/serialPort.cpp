#include "serialPort.hpp"
#include <iostream>

serialPort::serialPort(int comNum = 0,  // 串口号
            BAUDRATE baudR = BR9600,    // 波特率
            PARITY parityB = NONE,  // 校验位
            STOPLENGTH stopBitLen = B1, // 停止位长度
            DATALENGTH dataLenOfFram = DL8) // 每帧数据位个数
        : _COM(comNum), _baudRate(baudR), _parityBit(parityB), _stopBit(stopBitLen), _dataLength(dataLenOfFram)
{
    this -> _serialDevice = open( (std::string("/dev/ttyS")+std::to_string(comNum)).data(),O_RDWR);
    if( -1 == this -> _serialDevice)
    {
        std::cerr<<"open COM failed ! "<<std::endl
            <<"Please check your system HardWare and your permission！"<<std::endl;
        
        #ifdef __USE__EXCEPTION__
            throw std::invalid_argument("COM port");    //抛出异常
        #endif
    }

    struct termios property;
    if(tcgetattr(this->_serialDevice,&property) < 0)    // 获取串口当前参数
    {
        std::cerr<<"read COM property ! "<<std::endl
            <<"Please check your system HardWare and your permission！"<<std::endl;
        
        #ifdef __USE__EXCEPTION__
            throw std::invalid_argument("COM port");    //抛出异常
        #endif
    }

    // 设置波特率
    cfsetispeed(&property,this->_baudRate);
    cfsetospeed(&property,this->_baudRate);

    //设置校验位属性
    switch (this->_parityBit)
    {
    
        // 无校验位
        case NONE:
            property.c_cflag &= ~PARENB;    // 取消校验位使能
            property.c_cflag &= ~INPCK;
            break;

        // 奇校验
        case ODD:
            property.c_cflag |= PARENB;
            property.c_cflag |= PARODD;
            property.c_cflag |= INPCK;
            property.c_cflag |= ISTRIP; // 若设置该位，则有效输入数字被剥离7个字节，否则保留全部8位
            break;

        // 偶校验
        case EVEN:
            property.c_cflag |= PARENB;
            property.c_cflag &= ~PARODD;
            property.c_cflag |= INPCK;
            property.c_cflag |= ISTRIP; // 若设置该位，则有效输入数字被剥离7个字节，否则保留全部8位
            break;

        default:
            std::cerr<< "sorry！ this API still not support MARK and SPACE" << std::endl;

            #ifdef __USE__EXCEPTION__
                throw std::invalid_argument("COM port");    //抛出异常
            #endif

            break;
    }

    // 设置数据位
    property.c_cflag &= ~CSIZE;
    property.c_cflag |= this->_dataLength;

    // 设置停止位
    switch (this->_stopBit)
    {
        case B1:
            property.c_cflag &= ~CSTOPB;
            break;

        case B2:
            property.c_cflag |= CSTOPB;
            break;
        
        default:
                std::cerr<< "sorry！ this API still not support B1P5" << std::endl;

                #ifdef __USE__EXCEPTION__
                    throw std::invalid_argument("COM port");    //抛出异常
                #endif
            break;
    }

    if( tcsetattr(this->_serialDevice,TCSANOW,&property) != 0)
    {
        std::cerr<<"set COM failed ! "<<std::endl
            <<"Please check your system HardWare and your permission！"<<std::endl;
        
        #ifdef __USE__EXCEPTION__
            throw std::invalid_argument("COM port");    //抛出异常
        #endif
    }
}

int serialPort::sendData(const char * dataPtr, int dataLen)
{
    int sendOnce = 0, sendTotal = 0 ;
    while (dataLen > 0)
    {
        if( (sendOnce = write(this->_serialDevice,dataPtr,dataLen)) <= 0)
        {
            if(sendOnce < 0 && errno == EINTR)
                sendOnce = 0;
            else
                break;
        }
        dataLen -= sendOnce;
        dataPtr += sendOnce;
        sendTotal += sendOnce;
    }
    
    return sendOnce;
}

int serialPort::recvData(void * buffPtr,int buffLen)
{
    return read(this->_serialDevice,buffPtr,buffLen);
}
