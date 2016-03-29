/**
 * @file nand_list.h
 * @brief this is the information from nandflash
 *
 * This file provides nandflash basic information,so we can
 * search this information in order software fit hardware quickly
 *
 * Copyright (C) 2006 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @author Dengjian
 * @MODIFY  zhaojiahuan
 * @DATE    2006-7-17
 * @MODIFY Yang Yiming
 * @DATE    2012-12-24
 * @version 1.1
 * @
 */

#ifndef _NAND_LIST_H_
#define _NAND_LIST_H_

//bit31 reserved
//bit30 reserved
//bit29��ʾ�Ƿ�ǰ��die��1��ʾ��ǰ��die
//bit28��ʾ�Ƿ���żplane��1��ʾ����żplane
//bit27 reserved
//bit26 reserved
#define FLAG_HIGH_ID    (1 << 25)   //bit25��ʾ�Ƿ���high id,    1��ʾ֧��
#define FLAG_ENHANCE_SLC    (1 << 24)   //bit24��ʾ�Ƿ�֧��Enhanced SLC,    1��ʾ֧��
#define FLAG_READ_CACHE     (1 << 23)   //bit23��ʾ�Ƿ�֧��multi-cache read��1��ʾ֧��     
#define FLAG_PROG_CACHE     (1 << 22)   //bit22��ʾ�Ƿ�֧��multi-cache program��1��ʾ֧��  
#define FLAG_READ_2PCACHE   (1 << 21)   //bit21��ʾ�Ƿ�֧��single-cache read��1��ʾ֧��    
#define FLAG_PROG_2PCACHE   (1 << 20)   //bit20��ʾ�Ƿ�֧��single-cache program��1��ʾ֧�� 
#define FLAG_READ_RETRY     (1 << 19)   //bit19��ʾ�Ƿ�֧��Read-Retry���ܣ�1��ʾ֧��                                                                                            
#define FLAG_STEP_BY_STEP   (1 << 18)   //bit18��ʾ��ͬһ��block���Ƿ���Ҫ�ϸ�1Ϊ����˳��дpage��1��ʾ��Ҫ����Micron MT29F32G08CBACA��MLC�д�Ҫ����ʼpage��һ��Ҫ����0      
#define FLAG_RANDOMIZER     (1 << 17)   //bit17��ʾ�Ƿ���Ҫrandomizer��1��ʾ��Ҫ                                                                                                
#define FLAG_COPY           (1 << 16)   //bit16��ʾ�Ƿ�֧��single-plane copyback��1��ʾ֧��                                                                                     
#define FLAG_COPY_2P        (1 << 15)   //bit15��ʾ�Ƿ�֧��multi-plane copyback��1��ʾ֧��                                                                                
#define FLAG_READ_2P        (1 << 14)   //bit14��ʾ�Ƿ�֧��multi-plane read��1��ʾ֧��        
#define FLAG_ERASE_2P       (1 << 13)   //bit13��ʾ�Ƿ�֧��multi-plane erase��1��ʾ֧��       
#define FLAG_PROG_2P        (1 << 12)   //bit12��ʾ�Ƿ�֧��multi-plane program��1��ʾ֧��     
#define FLAG_LUN_GAP        (1 << 11)   //bit11��ʾplane_blk_num����2^10����������һ��die����ʼblock��ַ ���� group_blk_num * 2                                                                                                                                                 
#define FLAG_BLOCK_GAP      (1 << 10)   //bit10��ʾpage number�Ƿ���Ҫ���Ϲ�����1��ʾ��Ҫ����TLC��192page/block��Ϊ�˶�����һ��block����Ҫ����Ϊ256page/block������                                                                                                             
#define FLAG_SPARE_MASK     (0x3 << 8)  //bit9 ��ʾspare�����С�ĸ�λ                                                                                                                                                                                                          )
                                        //bit8 ��ʾspare�����С�ĸ�λ����λ��256 Bytes����spare_size��ΪT_U8�������Ա�ʾ����nand��400����ֽڵ�spare��С                                                                                                                    
#define FLAG_ECC_POS        (4)         //bit7 ��ʾECC����                                                                                                                 
                                        //bit6 ��ʾECC����                                                                                                                 
                                        //bit5 ��ʾECC����                                                                                                                 
                                        //bit4 ��ʾECC���ͣ�0Ϊ4 bit/512B��1Ϊ8 bit/512B��2Ϊ12 bit/512B��3Ϊ16 bit/512B��4Ϊ24 bit/1024B��5Ϊ32 bit/1024B��6Ϊ40 bit/1024B��7Ϊ44 bit/1024B��8Ϊ60 bit/1024B��9Ϊ72 bit/1024B
#define FLAG_TARGET_ALLOW   (1 << 3)    //bit3 ��ʾ����ж��targetƬѡ�����target��ͬλ�õĿ飨��LUN�ϲ�֮�󣩺ϲ���һ����飻Ϊ'0'�����ϲ���                                                  
#define FLAG_LUN_ALLOW      (1 << 2)    //bit2 ��ʾ����ж��LUN�����LUN��ͬλ�õĿ飨��plane�ϲ�֮�󣩺ϲ���һ����飻Ϊ'0'�����ϲ���                                                          
#define FLAG_PLANE_ALLOW    (1 << 1)    //bit1 ��ʾ���multi plane�����plane��ͬλ�õĿ�ϲ���һ����飻Ϊ'0'�����ϲ���                                                                         

//ע��,Nand�Ĳ���ECC TYPE��AKоƬ��ECC ģʽ,����һ��һһ��Ӧ.
typedef enum 
{
    ECC_TYPE_4BITS = 0,
    ECC_TYPE_8BITS,
    ECC_TYPE_12BITS,
    ECC_TYPE_16BITS,
    ECC_TYPE_24BITS,
    ECC_TYPE_32BITS,
    ECC_TYPE_40BITS,
    ECC_TYPE_44BITS,
    ECC_TYPE_60BITS,
    ECC_TYPE_72BITS 
}E_NAND_TYPE;
#define ECC_TYPE(flag)  ((flag & (0xF << FLAG_ECC_POS)) >> FLAG_ECC_POS)

//currently there are 7 types, more types might be added when new nand come out
//˵����������ǰһ����page��,��һ����page�е�λ��, �����Щλ�ò�Ϊ0xFF���block�ǳ�������
typedef enum
{
    NAND_BAD_FLAG_SAMSUNG = 0,//NAND_TYPE_SAMSUNG:        0x1 СҳSLC([0,1],[517]),   ��ҳSLC([0,1],[2048]),          MLC([127], [2048/4096])
    NAND_BAD_FLAG_HYNIX,//NAND_TYPE_HYNIX:          0x2 СҳSLC([0,1],[517]),   ��ҳSLC([0,1],[2048]),          MLC([125,127], [2048/4096])
    NAND_BAD_FLAG_TOSHIBA,//NAND_TYPE_TOSHIBA:        0x3 СҳSLC([0,1],[0,512]), ��ҳSLC([0,1],[0,2048]),        MLC([127], [0,2048/4096])
    NAND_BAD_FLAG_TOSHIBA_EXT,//NAND_TYPE_TOSHIBA_EXT:    0x4 СҳSLC(),              ��ҳSLC(),                      MLC([0,127/191/255], [0,2048/4096/8192])
    NAND_BAD_FLAG_MICRON,//NAND_TYPE_MICRON:         0x5 СҳSLC([0,1],[512]),   ��ҳSLC([0,1],[2048]),          MLC([0,1], [2048/4096])
    NAND_BAD_FLAG_ST,//NAND_TYPE_ST:             0x6 СҳSLC([0,1],[517]),   ��ҳSLC([0],[2048,2053]),       MLC([127], [0])
    NAND_BAD_FLAG_MICRON_4K//NAND_TYPE_MICRON_4K       0x7 СҳSLC(),              ��ҳSLC(),                      MLC([0], [4096 ~ 4096+218])
}E_NAND_BAD_FLAG;


/**
* @BRIEF    Nandflash info define
* @AUTHOR   zhaojiahuan
* @DATE     2006-7-17
* @MODIFY Yang Yiming
* @DATE    2012-12-24
*/
typedef struct Nand_phy_info{
    T_U32  chip_id;//chip id
    T_U16  page_size; //page size
    T_U16  page_per_blk; //page of one block
    T_U16  blk_num;//total block number
    T_U16  group_blk_num;//the same concept as die, according to nand's struture
    T_U16  plane_blk_num;
    T_U8   spare_size;//spare�����С�ĵ�λ��������255 Byte
    T_U8   col_cycle;//column address cycle
    T_U8   lst_col_mask;//last column  addrress cycle mask bit
    T_U8   row_cycle;//row address cycle
    T_U8   delay_cnt;//Rb delay, unit is 1024 asic clock, default value corresponds to 84MHz
    T_U8   custom_nd;//nand type flag, used to detect the original invilid block
    T_U32  flag;//character bits
    T_U32  cmd_len;//nandflash command length
    T_U32  data_len;//nandflash data length
    T_U8   *des_str;//descriptor string
}T_NAND_PARAM, T_NAND_PHY_INFO;

#define ERROR_CHIP_ID   0//0xFFFFFFFF

#endif

