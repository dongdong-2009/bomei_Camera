#ifndef _TRANSC_H_
#define _TRANSC_H_

//#include "burn_result.h"
#include "fwl_usb_transc.h"
#define     USB_STATUS_SUCCESS  FHA_SUCCESS
#define     USB_STATUS_FAILE    FHA_FAIL

#define    TRANS_NULL  						0
#define    TRANS_SWITCH_USB					1		//�л�USB�ٶ�ΪHigh Speed
#define    TRANS_TEST_CONNECT				2		//����USbͨѶ
#define    TRANS_SET_MODE					3 		//������¼ģʽ��������¼������������SPI��¼
#define    TRANS_GET_FLASHID				4 		//��ȡnandflash��spi flash id
#define    TRANS_SET_NANDPARAM				5 		//����nandflash�Ĳ���
#define    TRANS_DETECT_NANDPARAM			6 		//��nandflash���������ڲ���nand�б����flashʹ��
#define    TRANS_INIT_SECAREA				7		//��ʼ����ȫ��
#define    TRANS_SET_RESV					8		//���ñ�������С
#define    TRANS_CREATE_PARTITION			9		//��������
#define    TRANS_FORMAT_DRIVER				10		//��ʽ������
#define    TRANS_MOUNT_DRIVER				11		//���ط���
#define    TRANS_DOWNLOAD_BOOT_START		12		//��ʼ����boot
#define    TRANS_DOWNLOAD_BOOT_DATA			13		//����boot����
#define    TRANS_COMPARE_BOOT_START		    14		//��ʼ�Ƚ�boot
#define    TRANS_COMPARE_BOOT_DATA			15		//���ͱȽ�boot����
#define    TRANS_DOWNLOAD_BIN_START			16		//��ʼ����bin�ļ�
#define    TRANS_DOWNLOAD_BIN_DATA			17		//����bin�ļ�����
#define    TRANS_COMPARE_BIN_START			18		//��ʼ�Ƚ�bin�ļ�
#define    TRANS_COMPARE_BIN_DATA			19		//���ͱȽ�bin�ļ�����
#define    TRANS_DOWNLOAD_IMG_START			20		//��ʼ����IMAGE�ļ�
#define    TRANS_DOWNLOAD_IMG_DATA			21		//����IMAGE����
#define    TRANS_COMPARE_IMG_START			22		//��ʼ�Ƚ�IMAGE�ļ�
#define    TRANS_COMPARE_IMG_DATA			23		//���ͱȽ�IMAGE����
#define    TRANS_DOWNLOAD_FILE_START		24		//��ʼ�����ļ�ϵͳ�ļ�
#define    TRANS_DOWNLOAD_FILE_DATA			25		//�����ļ�ϵͳ�ļ�����
#define    TRANS_COMPARE_FILE_START		    26		//��ʼ�Ƚ��ļ�ϵͳ�ļ�
#define    TRANS_COMPARE_FILE_DATA			27		//���ͱȽ��ļ�ϵͳ�ļ�����
#define    TRANS_UPLOAD_BIN_START			28	    //��ʼ�ϴ�BIN�ļ�
#define    TRANS_UPLOAD_BIN_DATA			29	    //�ϴ�BIN�ļ�
#define    TRANS_UPLOAD_FILE_START			30      //��ʼ�ϴ��ļ�ϵͳ�ļ�
#define    TRANS_UPLOAD_FILE_DATA			31		//�ϴ��ļ�ϵͳ�ļ�����
#define    TRANS_SET_GPIO					32 		//����GPIO
#define    TRANS_RESET						33		//�����豸��
#define    TRANS_CLOSE						34		//Close
#define    TRANS_SET_REG					35
#define    TRANS_DOWNLOAD_PRODUCER_START	36
#define    TRANS_DOWNLOAD_PRODUCER_DATA		37
#define    TRANS_RUN_PRODUCER				38
#define    TRANS_GET_DISK_INFO		        39		//��ȡ������Ϣ
#define    TRANS_UPDATESELF_BIN_START		40		//��ʼ��������������
#define    TRANS_UPDATESELF_BIN_DATA		41		//������������
#define    TRANS_UPLOAD_BIN_LEN				43	    //�ϴ�BIN����
#define	   TRANS_WRITE_ASA_FILE				44	    //д��ȫ���ļ�

#define    TRANS_DOWNLOAD_CLIENT_BOOT_START			45		//��ʼ���ؿͻ�BOOT�ļ�
#define    TRANS_DOWNLOAD_CLIENT_BOOT_DATA			46		//���Ϳͻ�BOOT�ļ�����
#define    TRANS_COMPARE_CLIENT_BOOT_START			47		//��ʼ�ȽϿͻ�BOOT�ļ�
#define    TRANS_COMPARE_CLIENT_BOOT_DATA			48		//���ͱȽ�
#define    TRANS_SET_SPIPARAM          			    49		//����PSI����
#define    TRANS_GET_FLASH_HIGH_ID                   50      //��ȡnandflash��high id

#define    TRANS_UPLOAD_SPIDATA_START			    53	    //��ʼ�ϴ�SPIDATA
#define    TRANS_UPLOAD_SPIDATA_DATA			    54	    //��ʼ�ϴ�SPIDATA

#define    TRANS_WRITE_OTP_SERIAL			        55	    //дotp���к�
#define    TRANS_READ_OTP_SERIAL			        56	    //��otp���к�




#define	   TRANS_GET_CHANNEL_ID						100
#define	   TRANS_GET_RAM_VALUE						101	    //
#define	   TRANS_GET_SCSI_STATUS				    102	    //
#define	   TRANS_SET_CHANNEL_ID						103
#define    TRANS_SET_BURNEDPARAM                    104     // �·���¼��ɲ���
#define    TRANS_SET_NANDFLASH_CHIP_SEL             105     // NAND Ƭѡ����
#define	   TRANS_READ_ASA_FILE				        106	    //д��ȫ���ļ�

#define	   TRANS_UPLOAD_BOOT_START				    107	    //��ȡboot�ļ���ʼ
#define	   TRANS_UPLOAD_BOOT_DATA				    108	    //��ȡboot�ļ�������
#define	   TRANS_UPLOAD_BOOT_LEN				    109	    //��ȡboot�ļ��ĳ��

#define	   TRANS_SET_ERASE_NAND_MODE			    111	    //����nand�Ĳ�block��ģʽ
#define	   TRANS_SET_BIN_RESV_SIZE			        112	    //����BIN����ʣ��ռ��С.





static T_BOOL Transc_SwitchUsbHighSpeed(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_TestConnection(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_SetMode(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetFlashID(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_SetNandParam(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_DetectNandParam(T_U8 buf[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_InitSecArea(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_SetResvAreaSize(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_StartDLBin(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_DLBin(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetDiskInfo(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_CreatePartion(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_FormatDriver(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_MountDriver(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_StartDLImg(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_DLImg(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_StartDLBoot(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_DLBoot(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_StartDLFile(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_DLFile(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_Reset(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_Close(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetBinStart(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetBinLength(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetBinData(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_WriteAsaFile(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_SetSPIParam(T_U8 data[], T_U32 len, T_CMD_RESULT *result);

static T_BOOL Transc_ReadAsaFile(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetChannel_ID(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_SetChannel_ID(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetBootStart(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetBootData(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetBootLen(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_SetErase_Mode(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_Set_Bin_Resv_Size(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_ReadAsaFile(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetSpiDataStart(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetSpiData(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_GetFlashHighID(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_write_otp_serial(T_U8 data[], T_U32 len, T_CMD_RESULT *result);
static T_BOOL Transc_read_otp_serial(T_U8 data[], T_U32 len, T_CMD_RESULT *result);





#endif
