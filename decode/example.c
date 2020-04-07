#include "decode.h"

#pragma pack(1)
//imu输出数据结构体
typedef struct imu_data
{
    u8      head[2];    //数据头55 AA
    u32     sow;        //GPS Sow, s
    u32     pps;        //PPS Count
    u32     count;      //IMU Count
    float   gyro_x;     //X陀螺数据(弧度)
	float   gyro_y;     //Y陀螺数据(弧度)
	float   gyro_z;     //Z陀螺数据(弧度)
	float   accl_x;     //X加表数据
	float   accl_y;     //Y加表数据
	float   accl_z;     //Z加表数据
    u16     odo;        //ODO count
    u16     can;        //CAN BUS DATA,m/s
    u8      check;      //Checksum, unsigned char adding from Byte2~Byte41
}IMU_DATA_TYPE;
#pragma pack()

//数据队列定义example
u8 imu_dec_buf[65535] = {0};//解码缓冲区
DCD_QUE_TYPE imu_dcd_que = {imu_dec_buf, sizeof(imu_dec_buf), 0, 0, 0, 0, 0};//解码队列

//imu数据解码校验函数，校验imu数据包是否正确
int ImuDataCheck(void *dat)
{
    IMU_DATA_TYPE *imu_data = dat;
    if(imu_data->check != CheckSum8((u8 *)imu_data + 2, sizeof(IMU_DATA_TYPE) - 3)){//sum8校验
        return -1;
    }
    return 0;
}

//自检函数
static u8 ImuCheckSelf(DCD_QUE_TYPE *que)
{
    DECODE_BIN_TYPE decode[1] = {{{0x55, 0xaa}, 2, sizeof(IMU_DATA_TYPE), ImuDataCheck}};//解码结构体
    IMU_DATA_TYPE imu_data;//解码成功后的数据
    float array[6][200];
    int kind = 0;
    u8 ret = 0xff;

    kind = DcdGetData(que, decode, 1, (void *)&imu_data);
    if(kind >= 0){
        //获取到数据,kind的值对应获取到的第几中数据，example只有一种数据，成功获取到数据，kind为0
    }else{
        //未获取到数据
    }
        

    return ret;
}