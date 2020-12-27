#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string>

#define __USE__EXCEPTION__

class serialPort{

public:

    // 波特率
    enum BAUDRATE{
        BR1200 = B1200,
        BR2400 = B2400,
        BR4800 = B4800,
        BR9600 = B9600,
        BR19200 = B19200,
        BR38400 = B38400,
        BR57600 = B57600,
        BR115200 = B115200
        // 先定义这么多吧
    };

    // 数据位长度
    enum DATALENGTH{
        DL5 = CS5,  // 5位数据长度
        DL6 = CS6,  // 6位
        DL7 = CS7,  // 7位
        DL8 = CS8   // 8位
    };

    // 校验位
    enum PARITY{    // 校验位
        NONE,   // 无校验位
        EVEN,   // 偶校验
        ODD,    // 奇校验
        MARK,   // 校验位总是1
        SPACE   // 校验位总是0
    };

    // 停止位
    enum STOPLENGTH{  // 停止位
        B1,         // 1位
        B1P5,   // 1.5位
        B2          // 2位
    };

private:

    BAUDRATE _baudRate;         // 波特率
    DATALENGTH _dataLength;     // 数据位个数
    PARITY _parityBit;      // 校验位属性
    STOPLENGTH _stopBit;    // 停止位长度

    int _COM;       // 串口号
    int _serialDevice;  // 串口设备句柄

public:
    /***************************************
     * 串口对象构造函数
     * 输入参数
     *      comNum（串口号）                        ： int类型，默认值0
     *      baudR（波特率）                              ：  BAUDRATE枚举类型，默认波特率9600
     *      parityB（校验位）                            ：  PARITY枚举类型，默认无校验位
     *      stopBitLen（停止位长度）            ：  STOPLENGTH枚举类型，默认一位长度
     *      dataLenOfFram（帧数据位个数）   ：  DATALENGTH枚举类型，默认8个数据位
     * 返回值
     *      serialPort  ： 这是一个构造函数，所以返回的就是该串口对象
     **************************************/
    serialPort(int comNum = 0,
                BAUDRATE baudR = BR9600,
                PARITY parityB = NONE, 
                STOPLENGTH stopBitLen = B1,
                DATALENGTH dataLenOfFram = DL8);

    /**************************************
     * 数据发送API
     * 输入参数
     *      dataPtr（数据包头指针）     ：任意指针类型
     *      dataLen（数据长度）             :   数据单位为字节（Byte、8Bits）
     * 返回值
     *      int类型     ：  成功发送的数据长度，若返回-1代表出错
     *************************************/
    int sendData(const char * dataPtr, int dataLen);

    /**************************************
     * 数据接收API
     * 输入参数
     *      buffPtr（数据缓冲池指针）   ：  任意指针类型
     *      buffLen（数据缓冲池大小）   ： 数据单位为字节（Byte、8Bits）
     * 返回值
     *      int类型     ：  成功接收到的数据长度，若返回-1代表出错
     **************************************/
    int recvData(void * buffPtr,int buffLen);
};