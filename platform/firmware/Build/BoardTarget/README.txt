/*******************************************************************************
����˵���ļ���
    ���ܱ�Ŀ¼�µ�perl�ļ���bat�ļ���command_XXX.txt�����б�������ĺ�����÷���
*******************************************************************************/

1. Ŀ¼�ļ�����
    (1). *.pl�ļ��� ��������makefile�ļ���
    (2). *.bat�ļ��������������ļ���
    (3). command_XXX.txt�ļ�����������ļ���

2. ʹ�ò���
    (1). ��װ��perl��cygwin�������ȷ���û���������������롢��¼�ĵ�����
    (2). ѡ����Ҫ��perl�ļ���˫������������ִ�У�����makefile�ļ���   
    (3). ѡ����Ҫ�ı�������ļ�command_XXX.txt������һ�ݲ�������Ϊcommand.txt��
    (4). ѡ��Build.bat�ļ���˫������������ִ�У�
    (5). ������̿�ʼ�������Ҫ5���Ӽ������ɿ�ִ���ļ�Sword37.bin��ͨ����¼���ߣ�����Դ�ļ���Sword37.binһ��д��ϵͳ���������С�

3. ��command_XXX.txt�ļ����
    (1). command_CI3760_svt.txt��AK3760C svtӲ�������� ��320*240�ߴ�lcd��8M SDRAM��SPIBOOT�ı�������ļ� 
    (2). command_CI3750.txt    ��AK3750C Ӳ��������, 320*240�ߴ�lcd��8M SDRAM��SPIBOOT�ı�������ļ���
ע��command_CI3760_svt.txt �����ڲ�ʹ�ã������ö�Ӧ�Ŀ����岻�ⷢ��    
    

4. �������˵��
    ����command_XXX.txt�ļ��еı���������û����Ը�����Ҫ��������,ѡY��N����������Ⱥ��ұ�ѡY����ʾ֧�֣�ѡN��ʾ��֧�֡�
   
ע�⣺1��ÿ���������ռһ�У���Ҫ��д�޹ص����ַ��š�
      2����ĳ�б��������ʱ����Ҫ��������������һ��Ӣ�ĵķֺ�; ����ע�͵����������Ͳ��������롣

�����������ǣ�

    CHIP=AK3750 ��    	�������ͺ�ѡ��ʹ��AK3750C������
    PLATFORM=CI37XX :   ѡ��CI37XX Ӳ��ƽ̨��
		
    KEYPAD_TYPE��       ����ѡ��, GPIO����: 0, AD����: 1,
    
    PLATFORM_DEBUG_VER=N :  Y: ���԰汾(�򿪲��ֵ��Թ���)��N: �����汾

    BOOT_MODE=SPI :    SPI:SPI����ģʽ , ����ģʽ�ݲ�֧��

    LCD=MPU_R61580 : LCD TYPE
    LCDWIDTH=480 LCDHEIGHT=272��lcd480��272�ģ�
    LCDWIDTH=320 LCDHEIGHT=240��lcd320��240�ģ�

    RAM_SIZE=8 : RAM�Ĵ�С��8M
    RAM_SIZE=16 : RAM�Ĵ�С��16M

    ENABLE_COMPRESS_BIN=Y: ֧��ѹ��
    
    SUPPORT_TSCR=Y ��       �Ƿ�֧�ִ���������ѡ�Ŀǰֻ��ѡN��

    TVOUT_TYPE=0: ��ʹ��TV OUT����
    TVOUT_TYPE=1: ʹ������TV OUT������ܣ�
    TVOUT_TYPE=2: ʹ��TV OUT���оƬ����tv�����
    TVOUT_TYPE=3: ʹ��TV OUT���оƬ����tv���,����7026��TV���Ϊ720��480��
    
    SUPPORT_GESHADE=Y ��    ֧��GE��Ч����ѡ��,16M���ϰ汾��֧��

    SUPPORT_EXTERN_RTC=Y �� ֧�����RTC����ѡ�
    SUPPORT_DEBUG_OUTPUT=Y :֧�ִ��ڵ���, ��ѡ��;   
    SUPPORT_DEBUG_OUTPUT_USB=Y: ֧��USBģʽ������Ϣ, ��ѡ����SUPPORT_DEBUG_OUTPUT=Y����Ч 
    SUPPORT_LCDPWM=Y

    SUPPORT_AUDIOPLAYER=Y ��֧����Ƶ���ţ���ѡ�
    SUPPORT_CAMERA=Y ��     ֧�����������ѡ�
    SUPPORT_AUDIOREC=Y ��   ֧��¼������ѡ�
    SUPPORT_VIDEOPLAYER=Y ��֧����Ƶ���ţ���ѡ�
    SUPPORT_IMG_BROWSE=Y �� ֧��ͼƬ�������ѡ�
    
    SUPPORT_UVC=Y ��	    ֧��pc camera����ѡ����SUPPORT_CAMERA=Y����Ч
    SUPPORT_HOST=Y ��       ֧��host����ģʽ����ѡ�
    SUPPORT_EXPLORER=Y :    ֧����Դ����������ѡ�
    SUPPORT_SYS_SET=Y :     ֧��ϵͳ���ã���ѡ�

    SUPPORT_NETWORK=Y  :    ֧������Ӧ�ã� ��ѡ�
    SUPPORT_AUTOTEST=N��֧���Զ������Կ�ѡ�
    
5. SD BOOT��ʹ�÷�����
   �ݲ�֧��SD BOOT

6. SPI BOOT��ʹ�÷�����
   1). SPI�汾֧��ѹ��,Ҫ�ѱ������Ϊ"Y":ENABLE_COMPRESS_BIN=Y;
   2). ������Դ��ִ�С�build_37xx_qvga_spi.cmd��;
   3). ��¼������ʹ�������ļ�"config.txt��;
   4). ����Ҫ����bios

7������
   ��������û�н��ܵ��ļ������������δ������ɰ汾����anyka�ڲ��з�ʹ�ã��ͻ�����ѡ����ܻ��������Ԥ֪����������Ը���
