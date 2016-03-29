/**
* @FILENAME nand_feature.h
* @BRIEF    nand���Բ�����ȡ
* Copyright (C) 2011 Anyka (Guangzhou) Micro Eletronic Technology Co., LTD
* @AUTHOR   YiRuoxiang
* @DATE     2011-03-01
* @VERSION  1.0
* @REF      �Ժ�nand�����ԣ���Ҫ��װΪ��ģ����ɴ��ļ��ṩ
* @NOTE     1��20110301����unique id����
*/

/*************************************************************
The following is an example to use lcd driver APIs

T_VOID nand_read_unique_ID(T_VOID)
{
    ...
    
    nand_id = nand_read_chipID(0);

    cmd_seq_detail.base_cmd_addr = (T_U32 *)FLASH_CTRL_REG0;
    cmd_seq_detail.info_cfg_bit = 11;
    cmd_seq_detail.cmd_cycle_cfg = COMMAND_CYCLES_CONF;
    cmd_seq_detail.addr_cycle_cfg = ADDRESS_CYCLES_CONF;

    reg_addr = nand_config_unique_ID_cmd(nand_id, cmd_seq_detail);

    REG32(reg_addr) = (s_delay_cnt << 11) | DELAY_CNT_CONF | LAST_CMD_FLAG;// delay for tR
    cmd_go(...);
    while (!check_cmd_done());

    ...
    
    //read unique id from nandflash, id length according to different nandflash
    
    ...

}

*************************************************************/

#ifndef __NAND_FEATURE_H__
#define __NAND_FEATURE_H__

//nand������ʹ������nand���еķ�ʽ��������nandflash�������Ҫ�������ṩ����������ص���Ϣ
typedef struct SNandCmdSeqStru
{
    T_U32 *base_cmd_addr;             //�������мĴ����Ļ���ַ����0x2002, A100
    T_U32 cmd_cycle_cfg;              //�������мĴ����У��������ڵ����ã���COMMAND_CYCLES_CONF
    T_U32 addr_cycle_cfg;             //�������мĴ����У���ַ���ڵ����ã���ADDRESS_CYCLES_CONF
    T_U32 info_cfg_bit;               //�������мĴ����У���ʶ��Ϣ���õ�bit����AK980xӦ����Ϊ11λ
    T_U32 BufStart[1];                //��pNF_Add���ƣ�����bufferָ��ΪԤ����չ��
}T_NAND_CMD_SEQ_STRU, *T_PNAND_CMD_SEQ_STRU;

/**
 * @brief config a series of command sequences for reading unique ID
 *
 * @author YiRuoxiang
 * @date 2011-03-01
 * @param[in] nand_id nandflash id
 * @param[in] cmd_seq_detail details of command sequence, refer to defintion of T_NAND_CMD_SEQ_STRU
 * @return T_U32 *
 * @retval newest command register address, user can use this address to add new command sequence.
 *  attention: LAST_CMD_FLAG should be excuted after calling nand_config_unique_ID_cmd
 */
T_U32 *nand_config_unique_ID_cmd(T_U32 nand_id, T_NAND_CMD_SEQ_STRU cmd_seq_detail);

#endif
