/*
 * sample-common.c
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include <imp/imp_log.h>
#include <imp/imp_common.h>
#include <imp/imp_system.h>
#include <imp/imp_framesource.h>
#include <imp/imp_encoder.h>
#include <imp/imp_isp.h>
#include <imp/imp_osd.h>

#include "stub.h"

#define SENSOR_FRAME_RATE_NUM       25
#define SENSOR_FRAME_RATE_DEN       1

#define SENSOR_NAME             "gc2083"
#define SENSOR_CUBS_TYPE        TX_SENSOR_CONTROL_INTERFACE_I2C
//#define SENSOR_I2C_ADDR         0x6e
#define SENSOR_I2C_ADDR         0x37
#define SENSOR_WIDTH            1920
#define SENSOR_HEIGHT           1080
#define CHN0_EN                 1
#define CHN1_EN                 0
#define CHN2_EN                 0
#define CHN3_EN                 1
#define CROP_EN                 1

#define SENSOR_WIDTH_SECOND     640
#define SENSOR_HEIGHT_SECOND    360

#define SENSOR_WIDTH_THIRD      1280
#define SENSOR_HEIGHT_THIRD     720

#define BITRATE_720P_Kbs        1000

#define NR_FRAMES_TO_SAVE       200
#define STREAM_BUFFER_SIZE      (1 * 1024 * 1024)

#define ENC_VIDEO_CHANNEL       0
#define ENC_JPEG_CHANNEL        1

#define STREAM_FILE_PATH_PREFIX "/devel"
#define SNAP_FILE_PATH_PREFIX   "/devel"

#define OSD_REGION_WIDTH        16
#define OSD_REGION_HEIGHT       34
#define OSD_REGION_WIDTH_SEC    8
#define OSD_REGION_HEIGHT_SEC   18


#define SLEEP_TIME          1

#define FS_CHN_NUM          4  //MIN 1,MAX 3
#define IVS_CHN_ID          3

#define CH0_INDEX  0
#define CH1_INDEX  1
#define CH2_INDEX  2
#define CH3_INDEX  3
#define CHN_ENABLE 1
#define CHN_DISABLE 0

struct chn_conf{
    unsigned int index;//0 for main channel ,1 for second channel
    unsigned int enable;
  IMPEncoderProfile payloadType;
    IMPFSChnAttr fs_chn_attr;
    IMPCell framesource_chn;
    IMPCell imp_encoder;
};

#define  CHN_NUM  ARRAY_SIZE(chn)

#define TAG "Sample-Common"

struct chn_conf chn[FS_CHN_NUM] = {
    {
        .index = CH0_INDEX,
        .enable = CHN0_EN,
        .payloadType = IMP_ENC_PROFILE_HEVC_MAIN,
        .fs_chn_attr = {
            .pixFmt = PIX_FMT_NV12,
            .outFrmRateNum = SENSOR_FRAME_RATE_NUM,
            .outFrmRateDen = SENSOR_FRAME_RATE_DEN,
            .nrVBs = 2,
            .type = FS_PHY_CHANNEL,

            .crop.enable = CROP_EN,
            .crop.top = 0,
            .crop.left = 0,
            .crop.width = SENSOR_WIDTH,
            .crop.height = SENSOR_HEIGHT,

            .scaler.enable = 0,

            .picWidth = SENSOR_WIDTH,
            .picHeight = SENSOR_HEIGHT,
           },
        .framesource_chn =  { DEV_ID_FS, CH0_INDEX, 0},
        .imp_encoder = { DEV_ID_ENC, CH0_INDEX, 0},
    },
    {
        .index = CH1_INDEX,
        .enable = CHN1_EN,
    .payloadType = IMP_ENC_PROFILE_HEVC_MAIN,
        .fs_chn_attr = {
            .pixFmt = PIX_FMT_NV12,
            .outFrmRateNum = SENSOR_FRAME_RATE_NUM,
            .outFrmRateDen = SENSOR_FRAME_RATE_DEN,
            .nrVBs = 2,
            .type = FS_PHY_CHANNEL,

            .crop.enable = 0,
            .crop.top = 0,
            .crop.left = 0,
            .crop.width = SENSOR_WIDTH,
            .crop.height = SENSOR_HEIGHT,

            .scaler.enable = 1,
            .scaler.outwidth = SENSOR_WIDTH_THIRD,
            .scaler.outheight = SENSOR_HEIGHT_THIRD,

            .picWidth = SENSOR_WIDTH_THIRD,
            .picHeight = SENSOR_HEIGHT_THIRD,
           },
        .framesource_chn =  { DEV_ID_FS, CH1_INDEX, 0},
        .imp_encoder = { DEV_ID_ENC, CH1_INDEX, 0},
    },
    {
        .index = CH2_INDEX,
        .enable = CHN2_EN,
    .payloadType = IMP_ENC_PROFILE_HEVC_MAIN,
        .fs_chn_attr = {
            .pixFmt = PIX_FMT_NV12,
            .outFrmRateNum = SENSOR_FRAME_RATE_NUM,
            .outFrmRateDen = SENSOR_FRAME_RATE_DEN,
            .nrVBs = 2,
            .type = FS_PHY_CHANNEL,

            .crop.enable = 0,
            .crop.top = 0,
            .crop.left = 0,
            .crop.width = SENSOR_WIDTH,
            .crop.height = SENSOR_HEIGHT,

            .scaler.enable = 1,
            .scaler.outwidth = SENSOR_WIDTH_SECOND,
            .scaler.outheight = SENSOR_HEIGHT_SECOND,

            .picWidth = SENSOR_WIDTH_SECOND,
            .picHeight = SENSOR_HEIGHT_SECOND,
           },
        .framesource_chn =  { DEV_ID_FS, CH2_INDEX, 0},
        .imp_encoder = { DEV_ID_ENC, CH2_INDEX, 0},
    },
    {
        .index = CH3_INDEX,
        .enable = CHN3_EN,
    .payloadType = IMP_ENC_PROFILE_HEVC_MAIN,
        .fs_chn_attr = {
            .pixFmt = PIX_FMT_NV12,
            .outFrmRateNum = SENSOR_FRAME_RATE_NUM,
            .outFrmRateDen = SENSOR_FRAME_RATE_DEN,
            .nrVBs = 2,
            .type = FS_EXT_CHANNEL,

            .crop.enable = 0,
            .crop.top = 0,
            .crop.left = 0,
            .crop.width = SENSOR_WIDTH,
            .crop.height = SENSOR_HEIGHT,

            .scaler.enable = 1,
            .scaler.outwidth = SENSOR_WIDTH_SECOND,
            .scaler.outheight = SENSOR_HEIGHT_SECOND,

            .picWidth = SENSOR_WIDTH_SECOND,
            .picHeight = SENSOR_HEIGHT_SECOND,
           },
        .framesource_chn =  { DEV_ID_FS, CH3_INDEX, 0},
        .imp_encoder = { DEV_ID_ENC, CH3_INDEX, 0},
    },
};

extern int IMP_OSD_SetPoolSize(int size);

static IMPSensorInfo sensor_info;

static int save_stream(int fd, IMPEncoderStream *stream)
{
    int ret, i, nr_pack = stream->packCount;

  //IMP_LOG_DBG(TAG, "----------packCount=%d, stream->seq=%u start----------\n", stream->packCount, stream->seq);
    for (i = 0; i < nr_pack; i++) {
    //IMP_LOG_DBG(TAG, "[%d]:%10u,%10lld,%10u,%10u,%10u\n", i, stream->pack[i].length, stream->pack[i].timestamp, stream->pack[i].frameEnd, *((uint32_t *)(&stream->pack[i].nalType)), stream->pack[i].sliceType);
        IMPEncoderPack *pack = &stream->pack[i];
        if(pack->length){
            uint32_t remSize = stream->streamSize - pack->offset;
            if(remSize < pack->length){
                ret = write(fd, (void *)(stream->virAddr + pack->offset), remSize);
                if (ret != remSize) {
                    IMP_LOG_ERR(TAG, "stream write ret(%d) != pack[%d].remSize(%d) error:%s\n", ret, i, remSize, strerror(errno));
                    return -1;
                }
                ret = write(fd, (void *)stream->virAddr, pack->length - remSize);
                if (ret != (pack->length - remSize)) {
                    IMP_LOG_ERR(TAG, "stream write ret(%d) != pack[%d].(length-remSize)(%d) error:%s\n", ret, i, (pack->length - remSize), strerror(errno));
                    return -1;
                }
            }else {
                ret = write(fd, (void *)(stream->virAddr + pack->offset), pack->length);
                if (ret != pack->length) {
                    IMP_LOG_ERR(TAG, "stream write ret(%d) != pack[%d].length(%d) error:%s\n", ret, i, pack->length, strerror(errno));
                    return -1;
                }
            }
        }
    }
  //IMP_LOG_DBG(TAG, "----------packCount=%d, stream->seq=%u end----------\n", stream->packCount, stream->seq);
    return 0;
}

int sample_system_init()
{
    int ret = 0;

   //IMP_OSD_SetPoolSize(512*1024);

    memset(&sensor_info, 0, sizeof(IMPSensorInfo));
    memcpy(sensor_info.name, SENSOR_NAME, sizeof(SENSOR_NAME));
    sensor_info.cbus_type = SENSOR_CUBS_TYPE;
    memcpy(sensor_info.i2c.type, SENSOR_NAME, sizeof(SENSOR_NAME));
    sensor_info.i2c.addr = SENSOR_I2C_ADDR;

printf("Step 1.1\n");

    IMP_LOG_DBG(TAG, "sample_system_init start\n");

    ret = IMP_ISP_Open();
    if(ret < 0){
        IMP_LOG_ERR(TAG, "failed to open ISP\n");
        return -1;
    }

printf("Step 1.2\n");

#if 0

/**
* 摄像头控制总线类型是I2C时，需要配置的参数结构体
*/
typedef struct {
    char type[20];      /**< I2C设备名字，必须与摄像头驱动中struct i2c_device_id中name变量一致 */
    int addr;       /**< I2C地址 */
    int i2c_adapter_id; /**< I2C控制器 */
} IMPI2CInfo;
/**
* 摄像头控制总线类型是SPI时，需要配置的参数结构体
*/
typedef struct {
    char modalias[32];  /**< SPI设备名字，必须与摄像头驱动中struct spi_device_id中name变量一致 */
    int bus_num;        /**< SPI总线地址 */
} IMPSPIInfo;

typedef struct {
    char name[32];                  /**< 摄像头名字 */
    IMPSensorControlBusType cbus_type;  /**< 摄像头控制总线类型 */
    union {
        IMPI2CInfo i2c;             /**< I2C总线信息 */
        IMPSPIInfo spi;             /**< SPI总线信息 */
    };
    unsigned short rst_gpio;        /**< 摄像头reset接口链接的GPIO，注意：现在没有启用该参数 */
    unsigned short pwdn_gpio;       /**< 摄像头power down接口链接的GPIO，注意：现在没有启用该参数 */
    unsigned short power_gpio;      /**< 摄像头power 接口链接的GPIO，注意：现在没有启用该参数 */
} IMPSensorInfo;

#endif

    printf("Sensor Info\n-------------\n");
    printf("Name:%s\n",sensor_info.name);
    printf(" IMPI2CInfo\n");
    printf(" IMPI2CInfo.type:%s\n",sensor_info.i2c.type);
    printf(" IMPI2CInfo.addr:0x%02X\n",sensor_info.i2c.addr);
    printf(" IMPI2CInfo.i2c_adapter_id:%d\n",sensor_info.i2c.i2c_adapter_id);
    printf(" IMPSPIInfo\n");
    printf(" IMPSPIInfo.modalias:%s\n",sensor_info.spi.modalias);
    printf(" IMPSPIInfo.bus_num:%d\n",sensor_info.spi.bus_num);
    printf("rst_gpio:%u\n",sensor_info.rst_gpio);
    printf("pwdn_gpio:%u\n",sensor_info.pwdn_gpio);
    printf("power_gpio:%u\n",sensor_info.power_gpio);
    printf("-------------\n");
    

    ret = IMP_ISP_AddSensor(&sensor_info);
    if(ret < 0){
        IMP_LOG_ERR(TAG, "failed to AddSensor\n");
        printf("here\n");
        return -1;
    }

printf("Step 1.3\n");
    ret = IMP_ISP_EnableSensor();
    if(ret < 0){
        IMP_LOG_ERR(TAG, "failed to EnableSensor\n");
        return -1;
    }

printf("Step 1.4\n");
    ret = IMP_System_Init();
    if(ret < 0){
        IMP_LOG_ERR(TAG, "IMP_System_Init failed\n");
        return -1;
    }

printf("Step 1.5\n");
    /* enable turning, to debug graphics */
    ret = IMP_ISP_EnableTuning();
    if(ret < 0){
        IMP_LOG_ERR(TAG, "IMP_ISP_EnableTuning failed\n");
        return -1;
    }
printf("Step 1.6\n");
    IMP_ISP_Tuning_SetContrast(128);
    IMP_ISP_Tuning_SetSharpness(128);
    IMP_ISP_Tuning_SetSaturation(128);
    IMP_ISP_Tuning_SetBrightness(128);
#if 1
    ret = IMP_ISP_Tuning_SetISPRunningMode(IMPISP_RUNNING_MODE_DAY);
    if (ret < 0){
        IMP_LOG_ERR(TAG, "failed to set running mode\n");
        return -1;
    }
#endif
#if 0
    ret = IMP_ISP_Tuning_SetSensorFPS(SENSOR_FRAME_RATE_NUM, SENSOR_FRAME_RATE_DEN);
    if (ret < 0){
        IMP_LOG_ERR(TAG, "failed to set sensor fps\n");
        return -1;
    }
#endif
    IMP_LOG_DBG(TAG, "ImpSystemInit success\n");

    return 0;
}

int sample_system_exit()
{
    int ret = 0;

    IMP_LOG_DBG(TAG, "sample_system_exit start\n");


    IMP_System_Exit();

    ret = IMP_ISP_DisableSensor();
    if(ret < 0){
        IMP_LOG_ERR(TAG, "failed to EnableSensor\n");
        return -1;
    }

    ret = IMP_ISP_DelSensor(&sensor_info);
    if(ret < 0){
        IMP_LOG_ERR(TAG, "failed to AddSensor\n");
        return -1;
    }

    ret = IMP_ISP_DisableTuning();
    if(ret < 0){
        IMP_LOG_ERR(TAG, "IMP_ISP_DisableTuning failed\n");
        return -1;
    }

    if(IMP_ISP_Close()){
        IMP_LOG_ERR(TAG, "failed to open ISP\n");
        return -1;
    }

    IMP_LOG_DBG(TAG, " sample_system_exit success\n");

    return 0;
}

int sample_framesource_init()
{
    int i, ret;

    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            ret = IMP_FrameSource_CreateChn(chn[i].index, &chn[i].fs_chn_attr);
            if(ret < 0){
                IMP_LOG_ERR(TAG, "IMP_FrameSource_CreateChn(chn%d) error !\n", chn[i].index);
                return -1;
            }

            ret = IMP_FrameSource_SetChnAttr(chn[i].index, &chn[i].fs_chn_attr);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_FrameSource_SetChnAttr(chn%d) error !\n",  chn[i].index);
                return -1;
            }
        }
    }

    return 0;
}

int sample_jpeg_init()
{
    int i, ret;
    IMPEncoderChnAttr channel_attr;
    IMPFSChnAttr *imp_chn_attr_tmp;

    for (i = 0; i <  FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            imp_chn_attr_tmp = &chn[i].fs_chn_attr;
            memset(&channel_attr, 0, sizeof(IMPEncoderChnAttr));
            ret = IMP_Encoder_SetDefaultParam(&channel_attr, IMP_ENC_PROFILE_JPEG, IMP_ENC_RC_MODE_FIXQP,
                    imp_chn_attr_tmp->picWidth, imp_chn_attr_tmp->picHeight,
                    imp_chn_attr_tmp->outFrmRateNum, imp_chn_attr_tmp->outFrmRateDen, 0, 0, 25, 0);

            /* Create Channel */
            ret = IMP_Encoder_CreateChn(4 + chn[i].index, &channel_attr);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_CreateChn(%d) error: %d\n",
                            chn[i].index, ret);
                return -1;
            }

            /* Resigter Channel */
            ret = IMP_Encoder_RegisterChn(i, 4 + chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_RegisterChn(0, %d) error: %d\n",
                            chn[i].index, ret);
                return -1;
            }
        }
    }

    return 0;
}

int sample_framesource_streamon()
{
    int ret = 0, i = 0;
    /* Enable channels */
    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            ret = IMP_FrameSource_EnableChn(chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_FrameSource_EnableChn(%d) error: %d\n", ret, chn[i].index);
                return -1;
            }
        }
    }
    return 0;
}

int sample_get_jpeg_snap()
{
    int i, ret;
    char snap_path[64];

    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            ret = IMP_Encoder_StartRecvPic(4 + chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_StartRecvPic(%d) failed\n", 3 + chn[i].index);
                return -1;
            }

            sprintf(snap_path, "%s/snap-%d.jpg",
                    SNAP_FILE_PATH_PREFIX, chn[i].index);

            IMP_LOG_ERR(TAG, "Open Snap file %s ", snap_path);
            int snap_fd = open(snap_path, O_RDWR | O_CREAT | O_TRUNC, 0777);
            if (snap_fd < 0) {
                IMP_LOG_ERR(TAG, "failed: %s\n", strerror(errno));
                return -1;
            }
            IMP_LOG_DBG(TAG, "OK\n");

            /* Polling JPEG Snap, set timeout as 1000msec */
            ret = IMP_Encoder_PollingStream(4 + chn[i].index, 10000);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "Polling stream timeout\n");
                continue;
            }

            IMPEncoderStream stream;
            /* Get JPEG Snap */
            ret = IMP_Encoder_GetStream(chn[i].index + 4, &stream, 1);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_GetStream() failed\n");
                return -1;
            }

            ret = save_stream(snap_fd, &stream);
            if (ret < 0) {
                close(snap_fd);
                return ret;
            }

            IMP_Encoder_ReleaseStream(4 + chn[i].index, &stream);

            close(snap_fd);

            ret = IMP_Encoder_StopRecvPic(4 + chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_StopRecvPic() failed\n");
                return -1;
            }
        }
    }
    return 0;
}

int sample_framesource_streamoff()
{
    int ret = 0, i = 0;
    /* Enable channels */
    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable){
            ret = IMP_FrameSource_DisableChn(chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_FrameSource_DisableChn(%d) error: %d\n", ret, chn[i].index);
                return -1;
            }
        }
    }
    return 0;
}

int sample_jpeg_exit(void)
{
    int ret = 0, i = 0, chnNum = 0;
    IMPEncoderChnStat chn_stat;

    for (i = 0; i <  FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            chnNum = 4 + chn[i].index;
            memset(&chn_stat, 0, sizeof(IMPEncoderChnStat));
            ret = IMP_Encoder_Query(chnNum, &chn_stat);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_Query(%d) error: %d\n", chnNum, ret);
                return -1;
            }

            if (chn_stat.registered) {
                ret = IMP_Encoder_UnRegisterChn(chnNum);
                if (ret < 0) {
                    IMP_LOG_ERR(TAG, "IMP_Encoder_UnRegisterChn(%d) error: %d\n", chnNum, ret);
                    return -1;
                }

                ret = IMP_Encoder_DestroyChn(chnNum);
                if (ret < 0) {
                    IMP_LOG_ERR(TAG, "IMP_Encoder_DestroyChn(%d) error: %d\n", chnNum, ret);
                    return -1;
                }
            }
        }
    }

    return 0;
}

int sample_framesource_exit()
{
    int ret,i;

    for (i = 0; i <  FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            /*Destroy channel */
            ret = IMP_FrameSource_DestroyChn(chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_FrameSource_DestroyChn(%d) error: %d\n", chn[i].index, ret);
                return -1;
            }
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int i, ret;

printf("Step.1\n");
    /* Step.1 System init */
    ret = sample_system_init();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "IMP_System_Init() failed\n");
        return -1;
    }

printf("Step.2\n");
    /* Step.2 FrameSource init */
    ret = sample_framesource_init();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "FrameSource init failed\n");
        return -1;
    }

    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            ret = IMP_Encoder_CreateGroup(chn[i].index);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "IMP_Encoder_CreateGroup(%d) error !\n", i);
                return -1;
            }
        }
    }

printf("Step.3\n");
    /* Step.3 Encoder init */
    ret = sample_jpeg_init();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "Encoder init failed\n");
        return -1;
    }
    
printf("Step.4\n");
    /* Step.4 Bind */
    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            ret = IMP_System_Bind(&chn[i].framesource_chn, &chn[i].imp_encoder);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "Bind FrameSource channel%d and Encoder failed\n",i);
                return -1;
            }
        }
    }

printf("Step.5\n");
    /* Step.5 Stream On */
    ret = sample_framesource_streamon();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "ImpStreamOn failed\n");
        return -1;
    }

    /* drop several pictures of invalid data */
    sleep(SLEEP_TIME);

printf("Step.6\n");
    /* Step.6 Get Snap */
    ret = sample_get_jpeg_snap();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "Get H264 stream failed\n");
        return -1;
    }

    /* Exit sequence as follow... */
    /* Step.a Stream Off */
    ret = sample_framesource_streamoff();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "FrameSource StreamOff failed\n");
        return -1;
    }

    /* Step.b UnBind */
    for (i = 0; i < FS_CHN_NUM; i++) {
        if (chn[i].enable) {
            ret = IMP_System_UnBind(&chn[i].framesource_chn, &chn[i].imp_encoder);
            if (ret < 0) {
                IMP_LOG_ERR(TAG, "UnBind FrameSource channel%d and Encoder failed\n",i);
                return -1;
            }
        }
    }

    ret = sample_jpeg_exit();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "Encoder jpeg exit failed\n");
        return -1;
    }

    /* Step.d FrameSource exit */
    ret = sample_framesource_exit();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "FrameSource exit failed\n");
        return -1;
    }

    /* Step.e System exit */
    ret = sample_system_exit();
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "sample_system_exit() failed\n");
        return -1;
    }

    return 0;
}
