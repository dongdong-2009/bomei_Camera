/**
 * @filename usb_host_drv.c
 * @brief AK880X frameworks of usb driver.
 *
 * This file describe udriver of usb in host mode.
 * Copyright (C) 2006 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @author  liao_zhijun
 * @date    2010-07-20
 * @version 1.0
 * @ref
 */
#ifdef OS_ANYKA

#include    "usb_host_drv.h"
#include    "hal_usb_h_std.h"
#include    "hal_usb_std.h"
#include    "interrupt.h"
#include    "l2.h"
#include    "drv_api.h"
#include    "sysctl.h"
#include    "usb.h"


//********************************************************************
/****** DEVCTL bit MASK  *******/
#define M_DEVCTL_SESSION          0x01
#define M_DEVCTL_HR               0x02
#define M_DEVCTL_HM               0X04
#define M_DEVCTL_PDCON            0x08
#define M_DEVCTL_PUCON            0x10
#define M_DEVCTL_LSDEV            0x20
#define M_DEVCTL_FSDEV            0x40
#define M_DEVCTL_BDEVICE          0x80

#define USB_PKTYPE_SETUP          0x1
#define USB_PKTYPE_EP0IN          0x2
#define USB_PKTYPE_EP0OUT         0x3
#define USB_PKTYPE_EP0IN_STAT     0x4
#define USB_PKTYPE_EP0OUT_STAT    0x5
#define USB_PKTYPE_EPIN           0x6
#define USB_PKTYPE_EPOUT          0x7
#define USB_PKTYPE_EPIN_STAT      0x8
#define USB_PKTYPE_EPOUT_STAT     0x9

#define USB_TXCSR1_H_NAKTIMEOUT   0x80

#define UHOST_IN_INDEX   EP1_INDEX
#define UHOST_OUT_INDEX   EP2_INDEX

//********************************************************************

typedef enum
{
    CTRL_STAGE_IDLE = 0,    ///< idle stage
    CTRL_STAGE_SETUP,       ///< setup stage
    CTRL_STAGE_DATA_IN,     ///< data in stage
    CTRL_STAGE_DATA_OUT,    ///< data out stage
    CTRL_STAGE_STATUS       ///< status stage
}
E_CTRL_TRANS_STAGE;

typedef enum
{
    UHOST_TRANS_IDLE = 0,
    UHOST_TRANS_BULK_IN,
    UHOST_TRANS_BULK_OUT
}
E_USB_HOST_TRANS;

typedef struct tagCONTROL_TRANS
{
    E_CTRL_TRANS_STAGE stage;   ///< stage
    T_UsbDevReq dev_req;        ///< request
    T_U8 *buffer;               ///< buffer
    T_U32 buf_len;              ///< buffer length
    T_U32 data_len;             ///< data length
    T_U32 trans_len;             ///< data length
    T_fUHOST_TRANS_CALLBACK cbk_func;
}
T_CONTROL_TRANS;

typedef struct tagBULK_TRANS
{
    T_U8 stage;
    T_U8 *buffer;    
    T_U8  TxL2Bufid; 
    T_U8  RxL2Bufid;
    T_U32 buf_len;
    T_U32 data_len;
    T_U32 trans_len;
    T_BOOL dma_start;
    T_fUHOST_TRANS_CALLBACK cbk_func;
}
T_USB_HOST_TRANS;

typedef struct
{
    T_fUHOST_COMMON_INTR_CALLBACK fcbk_connect;
    T_fUHOST_COMMON_INTR_CALLBACK fcbk_disconnect;
}
T_USB_HOST_COMMON_INTR_CALLBACK;

//to record the device bulk out/in ep index 
T_U8 g_UsbBulkinIndex;
T_U8 g_UsbBulkoutIndex;
//********************************************************************
static T_VOID usb_print_host_info(T_VOID);
static T_BOOL usb_host_intr_handler(T_VOID);
static T_VOID usb_host_read_int_reg(T_U8* usb_int, T_U32* usb_dma_int_tem);
static T_VOID usb_host_read_ep_int_reg(T_U8* usb_ep_int_tx, T_U8* usb_ep_int_rx);
static T_U32 usb_host_data_in(T_U8 EP_index, T_U8 *data);
static T_U32 usb_host_data_out(T_U8 EP_index, T_U8 *data, T_U32 count);

static T_U8 usb_bulk_send_id;
static T_U8 usb_bulk_receive_id;

T_U32 usb_mode = USB_MODE_20;

static volatile T_U32 m_max_ep0_size = 64;

static volatile T_CONTROL_TRANS m_uhost_ctrl = {0};
static volatile T_USB_HOST_TRANS m_uhost_trans = {0};
static T_USB_HOST_COMMON_INTR_CALLBACK m_uhost_cbk = {0};

//********************************************************************

/**
 * @brief   enable usb host driver.
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param  mode [in] T_U32  full speed or high speed
 * @return  T_VOID
 */
T_VOID usb_host_device_enable(T_U32 mode)
{
    T_U8 usb_ep_int_tx,usb_ep_int_rx;
    T_U32 regvalue;

    // enable usb clock
    sysctl_clock(CLOCK_USB_ENABLE);
    //reset usb module
    sysctl_reset(RESET_USB_OTG);    
    //enable the otg controller and reset the otg phy
    REG32(USB_CONTROL_REG) |= 0x7;  
    REG32(USB_CONTROL_REG) &= (~0x1); 

    // set bypass USB VBUS
    REG32(RTC_USB_CTRL_REG) &= ~(1UL<<31);
    REG32(RTC_USB_CTRL_REG) |= (1<<30);
    REG32(RTC_USB_CTRL_REG) |= (1<<29);
    REG32(RTC_USB_CTRL_REG) |= (1<<28);
    REG32(RTC_USB_CTRL_REG) |= (1<<27);
    REG32(RTC_USB_CTRL_REG) |= (1<<26);

    //config usb phy
    regvalue = REG32(USB_NEW_CFG_REG);

    //enable otg
    regvalue |= (0x1<<8); 
    
    //use USB2
    regvalue |= (0x1<<17); 

    REG32(USB_NEW_CFG_REG) = regvalue;

    //set usb connector type is A type
    REG32(USB_CONTROL_REG2) &= ~(1<<19);
    REG32(USB_CONTROL_REG2) |= (1<<18);

    akprintf(C2, M_DRVSYS, "USB_REG_DEVCTL0 = %x\n", REG8(USB_REG_DEVCTL));

    //start session
    REG8(USB_REG_DEVCTL) |= 0x1;

    //select mode
    if(mode == USB_MODE_11) //full speed
    {
        REG8(USB_REG_POWER) = 0x0;
        usb_mode = USB_MODE_11;
    }
    else                    //high speed
    {
        REG8(USB_REG_POWER) = 0x21;
        usb_mode = USB_MODE_20;
    }

    akprintf(C3, M_DRVSYS, "USB_REG_DEVCTL2 = %x\n", REG8(USB_REG_DEVCTL));

    //disable the sof interrupt
    REG8(USB_REG_INTRUSBE) = 0xFF & (~USB_INTR_SOF);

    //???
    mini_delay(200);

    //close test mode
    REG8(USB_REG_TESEMODE) = 0x0;

    usb_print_host_info();

    REG8(USB_REG_INDEX) = USB_EP0_INDEX;

    //set the nak count to max
    REG8(USB_REG_NAKLIMIT0) = 16;

    //disable the ping protocol
    REG8(USB_REG_CSR02) = 8;

    //alloc l2 buffer
    usb_bulk_receive_id = l2_alloc(ADDR_USB_EP1);
    usb_bulk_send_id = l2_alloc(ADDR_USB_EP2);
    if((BUF_NULL==usb_bulk_receive_id) || (BUF_NULL==usb_bulk_send_id))
    {
        akprintf(C1, M_DRVSYS, "usb_host_device_enable: malloc L2 buffer id error\n");
        return;
    }

    akprintf(C2, M_DRVSYS, "receive buf id %x\n", usb_bulk_receive_id);
    akprintf(C2, M_DRVSYS, "send buf id %x\n", usb_bulk_send_id);

    //clear usb interrupt status
    usb_host_read_ep_int_reg(&usb_ep_int_tx, &usb_ep_int_rx);
    REG8(USB_REG_INDEX) = USB_EP1_INDEX;
    REG8(USB_REG_RXCSR1)=0;
    REG8(USB_REG_INDEX) = USB_EP2_INDEX;
    REG8(USB_REG_TXCSR1)=0;
    REG8(USB_REG_INDEX) = USB_EP0_INDEX;

    int_register_irq(INT_VECTOR_USB, usb_host_intr_handler);
    int_register_irq(INT_VECTOR_USB_DMA, usb_host_intr_handler);

    //init global variable
    m_uhost_ctrl.stage = CTRL_STAGE_IDLE;
    m_uhost_trans.stage = UHOST_TRANS_IDLE;

    m_max_ep0_size = 64;
}


//********************************************************************
/**
 * @brief   disable usb host driver.
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @return  T_VOID
 */
T_VOID usb_host_device_disable(T_VOID)
{
    T_U32 tmp;

    //disable usb irq
    INTR_DISABLE(IRQ_MASK_USB_BIT);

    //set valid ID from external pin
    REG32(USB_CONTROL_REG2) &= ~(1<<18);

    //free l2 buffer alloced in usb_device_enable 
    l2_free(ADDR_USB_EP2);
    l2_free(ADDR_USB_EP1);

    REG8(USB_REG_POWER) = 0;

    //reset controller 
    sysctl_reset(RESET_USB_OTG);

    //close USB clock
    sysctl_clock(~CLOCK_USB_ENABLE);

    //reset transcieve
    REG32(USB_CONTROL_REG) |= 1;

    //disable  usb phy
    REG32(USB_NEW_CFG_REG) &= ~(1<<8);

    //disable usb transcieve
    REG32(USB_CONTROL_REG) &= (~0x7); 
    memset(&m_uhost_ctrl, 0, sizeof(T_CONTROL_TRANS));
    memset(&m_uhost_trans, 0, sizeof(T_USB_HOST_TRANS));
    memset(&m_uhost_cbk, 0, sizeof(T_USB_HOST_COMMON_INTR_CALLBACK));

}

/**
 * @brief   reset data toggle.
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @return  T_VOID
 */
T_VOID usb_host_clear_data_toggle(T_U8 EP_index)
{
    REG8(USB_REG_INDEX) = EP_index;
    if (UHOST_IN_INDEX == EP_index)
    {
        REG8(USB_REG_RXCSR1) |= USB_RXCSR1_CLRDATATOG;
    }
    else if (UHOST_OUT_INDEX == EP_index)
    {
        REG8(USB_REG_TXCSR1) |= USB_TXCSR1_CLRDATATOG;
    }
    else
    {
        return;
    }
}
T_VOID usb_host_flush_fifo(T_U8 EP_index)
{
    REG8(USB_REG_INDEX) = EP_index;
  
    if (UHOST_IN_INDEX == EP_index)
    {
        REG8(USB_REG_RXCSR1) |= USB_RXCSR1_FLUSHFIFO;
    }
    else if (UHOST_OUT_INDEX == EP_index)
    {
        REG8(USB_REG_TXCSR1) |= USB_TXCSR1_FLUSHFIFO|USB_TXCSR1_TXPKTRDY;

    }
    else
    {
        return;
    }
    m_uhost_trans.stage = UHOST_TRANS_IDLE;
}

/**
 * @brief   set callback func for common interrupt
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param intr_type interrupt type
 * @param callback callback function
 * @return  T_VOID
 */
T_VOID usb_host_set_common_intr_callback(E_USB_HOST_COMMON_INTR intr_type, T_fUHOST_COMMON_INTR_CALLBACK callback)
{
    if(USB_HOST_CONNECT == intr_type)
    {
        m_uhost_cbk.fcbk_connect = callback;
    }
    else if(USB_HOST_DISCONNECT == intr_type)
    {
        m_uhost_cbk.fcbk_disconnect = callback;
    }
}

/**
 * @brief   set callback func for transfer
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param ctrl_cbk callback function for control transfer
 * @param trans_cbk callback function for other transfer
 * @return  T_VOID
 */
T_VOID usb_host_set_trans_callback(T_fUHOST_TRANS_CALLBACK ctrl_cbk, T_fUHOST_TRANS_CALLBACK trans_cbk)
{
    m_uhost_ctrl.cbk_func = ctrl_cbk;
    m_uhost_trans.cbk_func = trans_cbk;
}

//********************************************************************
static T_VOID usb_print_host_info(T_VOID)
{
    T_U8  devctl_v;

    devctl_v = REG8(USB_REG_DEVCTL);
    
    akprintf(C2, M_DRVSYS, "USB_REG_DEVCTL = %x\r\n", devctl_v);
    if(0 != (devctl_v & M_DEVCTL_BDEVICE))
    {
        akprintf(C2, M_DRVSYS, "operate as B device!\r\n");
    }
    else
    {
        akprintf(C2, M_DRVSYS, "operate as A device!\r\n");
    }
}

static T_VOID usb_host_request_data(T_U8 EP_index)
{
    T_U32 pkt_num,byte_num,usb_bulk_in_maxsize;
  
    irq_mask();
    REG8(USB_REG_INDEX) = EP_index; 
    usb_bulk_in_maxsize = REG16(USB_REG_RXMAXP1);
    irq_unmask();
    
    //usb dma
    if((512 == usb_bulk_in_maxsize) && (m_uhost_trans.data_len >= usb_bulk_in_maxsize) && (m_uhost_trans.trans_len == 0))
    {
        byte_num = m_uhost_trans.data_len;
        pkt_num = byte_num / usb_bulk_in_maxsize;
        byte_num -= (byte_num % usb_bulk_in_maxsize);

        irq_mask();
        REG8(USB_REG_INDEX) = EP_index; 
        REG8(USB_REG_RXCSR2) |= (USB_RXCSR2_AUTOCLEAR | USB_RXCSR2_AUTOREQ | USB_RXCSR2_DMAENAB | USB_RXCSR2_DMAMODE);
        REG16(USB_REG_REQPKTCNT1) = pkt_num;
        irq_unmask();
        
        l2_clr_status(usb_bulk_receive_id);
        l2_combuf_dma((T_U32)m_uhost_trans.buffer, usb_bulk_receive_id, byte_num, BUF2MEM, AK_FALSE);

        irq_mask();
        REG32(USB_DMA_ADDR_2) = 0x70000000;
        REG32(USB_DMA_COUNT_2) = byte_num;
        REG32(USB_DMA_CNTL_2) = (USB_ENABLE_DMA|USB_DIRECTION_RX|USB_DMA_MODE1|USB_DMA_INT_ENABLE|(EP_index<<4)|USB_DMA_BUS_MODE3);
        irq_unmask();
        m_uhost_trans.dma_start = AK_TRUE;
    }
    
    irq_mask();
    REG8(USB_REG_INDEX) = EP_index; 
    REG8(USB_REG_RXCSR1) |= USB_RXCSR1_H_REQPKT;
    irq_unmask();
}

static T_VOID usb_host_irq_ctrl(T_BOOL enable)
{
    if(enable)
    {
        INTR_ENABLE(IRQ_MASK_USB_BIT|IRQ_MASK_USBDMA_BIT);
    }
    else
    {
        INTR_DISABLE(IRQ_MASK_USB_BIT|IRQ_MASK_USBDMA_BIT);
    }
}

T_VOID usb_host_set_max_ep0_size(T_U32 size)
{
    if(size > 64)
        m_max_ep0_size = 64;
    else 
        m_max_ep0_size = size;
}

//********************************************************************
static T_U32 usb_host_data_out(T_U8 EP_index, T_U8 *data, T_U32 count)
{
    T_U32 i, j;
    T_U32 send_count= 0;
    T_U32 usb_bulk_out_maxsize;
/*
    REG8(USB_REG_INDEX) = EP_index;
    usb_bulk_out_maxsize = REG16(USB_REG_TXMAXP0);
    if (count >= usb_bulk_out_maxsize)
    {
        send_count = usb_bulk_out_maxsize;
    }
    else
    {
        send_count = count;
    }
    if(0 != send_count)
    {  
        for (i = 0; i < send_count; i++)
        {
            REG8(USB_FIFO_EP0 + (EP_index << 2)) = *(data + i);
        }
    }
    //set tx count
    REG32(USB_EP2_TX_COUNT) = send_count;
    //set TXPKTRDY
    REG8(USB_REG_TXCSR1) |= USB_TXCSR1_TXPKTRDY;
    return send_count;

*/
    //check EP_index
    if(EP0_INDEX == EP_index || EP4_INDEX == EP_index)
    {
        akprintf(C1, M_DRVSYS, "usb_host_data_out: error ep number: %d\n", EP_index);
        return 0;        
    }

    irq_mask();
    REG8(USB_REG_INDEX) = EP_index;
    send_count = count;
    usb_bulk_out_maxsize = REG16(USB_REG_TXMAXP0);
    irq_unmask();
    
    if((count >= 512) && (512 ==usb_bulk_out_maxsize))
    {
        send_count -= (count % 512);       
        
        //set autoset/DMAReqEnable/DMAReqMode
        irq_mask();
        REG8(USB_REG_INDEX) = EP_index;
        REG8(USB_REG_TXCSR2) |= (USB_TXCSR2_DMAMODE|USB_TXCSR2_DMAENAB|USB_TXCSR2_MODE|USB_TXCSR2_AUTOSET);
        irq_unmask();
        
        //send data to l2
        l2_clr_status(usb_bulk_send_id);
        l2_combuf_dma((T_U32)data, usb_bulk_send_id, send_count, MEM2BUF, AK_FALSE);
        
        irq_mask();
        REG32(USB_DMA_ADDR_1) = 0x71000000;
        REG32(USB_DMA_COUNT_1) = send_count;       
        REG32(USB_DMA_CNTL_1) = (USB_ENABLE_DMA | USB_DIRECTION_TX| USB_DMA_MODE1 | USB_DMA_INT_ENABLE| (EP_index<<4) | USB_DMA_BUS_MODE3);
        irq_unmask();
    }
    else
    {
        irq_mask();
        send_count = (count > usb_bulk_out_maxsize) ? usb_bulk_out_maxsize : count;
        REG8(USB_REG_INDEX) = EP_index;
        for (i = 0; i < send_count; i++)
        {
            REG8(USB_FIFO_EP0 + (EP_index << 2)) = *(data + i);
        }
        //set tx count
        REG32(USB_EP2_TX_COUNT) = send_count;
        //set TXPKTRDY
        REG8(USB_REG_TXCSR1) |= USB_TXCSR1_TXPKTRDY;
        irq_unmask();
    }
    return send_count;
}

//********************************************************************
static T_U32 usb_host_data_in(T_U8 EP_index, T_U8 *data)
{
    T_U32 i,fifo_count = 0;

    fifo_count = REG16(USB_REG_RXCOUNT1);
    //unload data from usb fifo
    for (i = 0; i < fifo_count; i++)
    {
        *(data + i) = REG8(USB_FIFO_EP0 + (EP_index << 2));
    }
    //clear RXPKTRDY
    REG8(USB_REG_RXCSR1) &= (~USB_RXCSR1_RXPKTRDY);

    return fifo_count;
}

//********************************************************************
static T_VOID usb_host_read_int_reg(T_U8* usb_int, T_U32* usb_dma_int_tem)
{
    *usb_int = REG8(USB_REG_INTRUSB);
    *usb_dma_int_tem = REG32(USB_DMA_INTR);
}
//********************************************************************
static T_VOID usb_host_read_ep_int_reg(T_U8* usb_ep_int_tx, T_U8* usb_ep_int_rx)
{
    *usb_ep_int_tx = REG8(USB_REG_INTRTX1);
    *usb_ep_int_rx = REG8(USB_REG_INTRRX1);
}

//********************************************************************
/**
 * @brief   set faddr to the new address send to device.
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param address  [in]  usb device address.
 * @return  T_VOID
 */
T_VOID usb_host_set_address(T_U8 address)
{
    REG8(USB_REG_FADDR) = address;
}


/**
 * @brief   config usb host endpoint through device ep descriptor.
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param  ep [in]  ep description.
 * @return  T_VOID
 */
T_VOID usb_host_set_ep(T_USB_ENDPOINT_DESCRIPTOR ep)
{
    T_U8 ep_num; 

    ep_num = ep.bEndpointAddress & 0xF;
    if (ENDPOINT_TYPE_BULK == ep.bmAttributes)
    {
        if(ep.bEndpointAddress & ENDPOINT_DIR_IN)
        {
            g_UsbBulkinIndex = ep_num;
            REG8(USB_REG_INDEX) = USB_HOST_IN_INDEX;      
            //set ep type, ep num, max packet size
            REG8(USB_REG_RXTYPE) = ((ep.bmAttributes << 4) | ep_num);
            REG16(USB_REG_RXMAXP1) = ep.wMaxPacketSize;

            //clear toggle
            REG8(USB_REG_RXCSR1) = USB_RXCSR1_CLRDATATOG;

            //open interrupt
            REG8(USB_REG_INTRRX1E) |= (1 << USB_HOST_IN_INDEX);
        }
        else
        {
            g_UsbBulkoutIndex = ep_num;
            REG8(USB_REG_INDEX) = USB_HOST_OUT_INDEX;
            //set ep type, ep num, max packet size
            REG8(USB_REG_TXTYPE) = ((ep.bmAttributes << 4) | ep_num);
            REG16(USB_REG_TXMAXP0) = ep.wMaxPacketSize;

            //clear toggle, set to tx mode
            REG8(USB_REG_TXCSR1) = USB_TXCSR1_CLRDATATOG;
            REG8(USB_REG_TXCSR2) = USB_TXCSR2_MODE;
            
            //open interrupt
            REG8(USB_REG_INTRTX1E) |= (1 << USB_HOST_OUT_INDEX);
        }
    }
}


/**
 * @brief   open or close sof interrupt
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param  enable [in]  open sof interrupt or not.
 * @return  T_VOID
 */
T_VOID usb_host_sof_intr(T_BOOL enable)
{
    if(enable)
        REG8(USB_REG_INTRUSBE) |= USB_INTR_SOF_ENA;
    else
        REG8(USB_REG_INTRUSBE) &= ~USB_INTR_SOF_ENA;
}

//********************************************************************
/**
 * @brief   send reset signal to device.
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @return  T_VOID
 */
T_VOID usb_host_reset(T_VOID)
{
    REG8(USB_REG_POWER) |= USB_POWER_RESET;     
    mini_delay(20);
    REG8(USB_REG_POWER) &= (~USB_POWER_RESET);
}


/**
   @brief   sent suspend signal
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @return  T_VOID
 */
T_VOID usb_host_suspend(T_VOID)
{
    REG8(USB_REG_POWER) |= USB_POWER_SUSPENDM;     
}

/**
   @brief   sent resume signal
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @return  T_VOID
 */
T_VOID usb_host_resume(T_VOID)
{
    REG8(USB_REG_POWER) &= ~USB_POWER_SUSPENDM;     
    REG8(USB_REG_POWER) |= USB_POWER_RESUME;     
    mini_delay(20);
    REG8(USB_REG_POWER) &= (~USB_POWER_RESUME);
}

static T_U32 usb_host_ctrl_in(T_U8 *data)
{
    T_U32 fifo_count;
    T_U32 i, j;
    T_U32 tmp_val;

    //read data count
    fifo_count = REG8(USB_REG_COUNT0);
    if(0 == fifo_count)
        return 0;

    //receive data from l2
    for (i = 0; i < fifo_count; i++)
    {
        *(data + i) = REG8(USB_FIFO_EP0);
    }
    //clear RXPKTRDY
    REG8(USB_REG_CSR0) &= (~USB_CSR0_RXPKTRDY);
    
    return fifo_count;
}

//send one packet
static T_U32 usb_host_ctrl_out(T_U8 *data, T_U8 len, T_BOOL bSetup)
{
    T_U32 i;
    T_U32 count;

    //set ep index    
    REG8(USB_REG_INDEX) = EP0_INDEX;
    
    //clear csr
    REG8(USB_REG_CSR0) = 0;

    count = len;
    if(count == 0)          //zero packet
    {
        REG32(USB_EP0_TX_COUNT) = count;            
        REG8(USB_REG_CSR0) = USB_CSR0_TXPKTRDY;
        return 0;
    }
    else if(count > m_max_ep0_size)
    {
        count = m_max_ep0_size;
    }
    //load data to ep0 fifo
    for( i = 0; i < count; i++ )
    {
        REG8( USB_FIFO_EP0 ) = data[i];
    }   
    //SET THE TX COUNT and START PRE READ
    REG32(USB_EP0_TX_COUNT) = count;            

    if(bSetup)
        REG8(USB_REG_CSR0) |= USB_CSR0_H_SETUPPKT | USB_CSR0_TXPKTRDY;
    else
        REG8(USB_REG_CSR0) |= USB_CSR0_TXPKTRDY;

    return count;
}

/**
   @brief   start control tranfer
 *
 * @author  liao_zhijun
 * @date    2010-06-30
 * @param dev_req [in] device request
 * @param data [in/out] data buffer
 * @param len [in] buffer length
 * @return  T_BOOL
 */
T_BOOL usb_host_ctrl_tranfer(T_UsbDevReq dev_req, T_U8 *data, T_U32 len)
{

    //check if current stage is idle or not
    if(m_uhost_ctrl.stage != CTRL_STAGE_IDLE)
    {
        return AK_FALSE;
    }

    //save param into m_uhost_ctrl
    memcpy(&m_uhost_ctrl.dev_req, &dev_req, sizeof(dev_req));

    m_uhost_ctrl.buffer = data;
    m_uhost_ctrl.buf_len = len;
    m_uhost_ctrl.data_len = len;
    m_uhost_ctrl.trans_len = 0;

    //send setup packet
    m_uhost_ctrl.stage = CTRL_STAGE_SETUP;
    usb_host_ctrl_out((T_U8 *)&m_uhost_ctrl.dev_req, sizeof(T_UsbDevReq), AK_TRUE);

    return AK_TRUE;
}

/**
   * @brief   start bulk in tranfer
   *
   * @author  liao_zhijun
   * @date    2010-06-30
   * @param  EP_index [in]  usb end point.
   * @param  data [out]  usb data buffer.
   * @param  len [in]  length
   * @return  T_U32 acctual read bytes
 */
T_BOOL usb_host_bulk_in(T_U8 *data, T_U32 len)
{
    //return if not in idle stage
    if(m_uhost_trans.stage != UHOST_TRANS_IDLE)
    {
        return AK_FALSE;
    }

    usb_host_irq_ctrl(AK_FALSE);

    //set global variable
    m_uhost_trans.buffer = data;
    m_uhost_trans.data_len = len;
    m_uhost_trans.trans_len = 0;
    m_uhost_trans.stage = UHOST_TRANS_BULK_IN;

    usb_host_irq_ctrl(AK_TRUE);

    //request data packet
    usb_host_request_data(UHOST_IN_INDEX);

    return AK_TRUE;
}

/**
   * @brief   start bulk out tranfer
   *
   * @author  liao_zhijun
   * @date    2010-06-30
   * @param  EP_index [in]  usb end point.
   * @param  data [in]  usb data buffer.
   * @param  len [in] len length
   * @return  T_U32 acctual read bytes
 */
T_BOOL usb_host_bulk_out(T_U8 *data, T_U32 len)
{
    T_U32 trans_len = 0;

    //return if not in idle stage
    if(m_uhost_trans.stage != UHOST_TRANS_IDLE)
    {
        return AK_FALSE;
    }

    usb_host_irq_ctrl(AK_FALSE);

    //set global variable
    m_uhost_trans.buffer = data;
    m_uhost_trans.data_len = len;
    m_uhost_trans.trans_len = 0;
    m_uhost_trans.stage = UHOST_TRANS_BULK_OUT;

    //start trans
    trans_len = usb_host_data_out(UHOST_OUT_INDEX, data, len);
    m_uhost_trans.trans_len += trans_len;

    usb_host_irq_ctrl(AK_TRUE);

    return AK_TRUE;
}

//********************************************************************
T_VOID usb_set_mode(T_U32 mode)
{
    REG8(USB_REG_POWER) = mode;
    
    mini_delay(50);
    
    REG8(USB_REG_POWER) |= 0x08;
    mini_delay(30);
    REG8(USB_REG_POWER) &= ~0x08;
}

//********************************************************************

volatile T_U8 usb_host_connect_flg = AK_FALSE;

static T_VOID usb_host_common_intr_handler(T_U8 usb_int)
{
    if(0 != (usb_int & USB_INTR_CONNECT))                   //connect
    {
		usb_host_connect_flg = AK_TRUE;
		if(m_uhost_cbk.fcbk_connect != AK_NULL)
        {
			m_uhost_cbk.fcbk_connect();
        }

        return;
    }

    if(0 != (usb_int & USB_INTR_SUSPEND))                   //suspend
    {
        return;
    }

    if(0 != (usb_int & USB_INTR_RESUME))                    //resume
    {
        return;
    }

    if(0 != (usb_int & USB_INTR_DISCONNECT))                //disconnect
    {        
        //INTR_DISABLE(IRQ_MASK_USB_BIT);
        m_uhost_ctrl.stage = CTRL_STAGE_IDLE;
        m_uhost_trans.stage = UHOST_TRANS_IDLE;
    	usb_host_connect_flg = AK_FALSE;
        if(m_uhost_cbk.fcbk_disconnect != AK_NULL)
        {
			m_uhost_cbk.fcbk_disconnect();
        }
    }
    
    if(0 != (usb_int & USB_INTR_SOF))
    {
        return;
    }
    
    if((0 != (usb_int & 0x80)) || (0 != (usb_int & 0x40)))
    {
        akprintf(C3, M_DRVSYS, "usb_int = %x\n", usb_int);
        akprintf(C3, M_DRVSYS, "usb otg = %x\n", REG8(USB_REG_DEVCTL));
        if((REG8(USB_REG_DEVCTL) & 0x80) != 0)
        {
            usb_host_device_disable();
            usb_host_device_enable(usb_mode);
        }
    }
}

static T_VOID usb_host_ep0_rx_handler()
{
    T_U32 trans_len;
    
    switch(m_uhost_ctrl.stage)
    {
        case CTRL_STAGE_DATA_IN:
            trans_len = usb_host_ctrl_in(m_uhost_ctrl.buffer + m_uhost_ctrl.trans_len);
            m_uhost_ctrl.trans_len += trans_len;

            if((trans_len < m_max_ep0_size) || (m_uhost_ctrl.trans_len >= m_uhost_ctrl.dev_req.wLength))
            {            
                //last packet, set to status stage
                REG8(USB_REG_CSR0) |= USB_CSR0_TXPKTRDY | USB_CSR0_H_STATUSPKT;
                m_uhost_ctrl.stage = CTRL_STAGE_STATUS;
            }
            else
            {
                //continue request packet
                REG8(USB_REG_CSR0) |= USB_CSR0_H_REQPKT;
            }
            
            break;
            
        case CTRL_STAGE_STATUS:
            //transaction finish
            REG8(USB_REG_CSR0) &= (~USB_CSR0_RXPKTRDY);

            m_uhost_ctrl.stage = CTRL_STAGE_IDLE;
        
            if(m_uhost_ctrl.cbk_func != AK_NULL)
                m_uhost_ctrl.cbk_func(USB_HOST_TRANS_COMPLETE, m_uhost_ctrl.trans_len);
            
            break;

        default:
            //must be error in this case
            akprintf(C1, M_DRVSYS, "error stage %d in ep0 rx\n", m_uhost_ctrl.stage);
            break;
    }
}

static T_VOID usb_host_ep0_tx_handler()
{
    T_U32 trans_len, data_len;

    switch(m_uhost_ctrl.stage)
    {
    case CTRL_STAGE_SETUP:
        if(0 == m_uhost_ctrl.dev_req.wLength)               //no data stage
        {
            REG8(USB_REG_CSR0) |= USB_CSR0_H_REQPKT | USB_CSR0_H_STATUSPKT;
            m_uhost_ctrl.stage = CTRL_STAGE_STATUS;
        }
        else
        {
            if(m_uhost_ctrl.dev_req.bmRequestType & (1<<7)) //data in
            {
                //request packet
                REG8(USB_REG_CSR0) = USB_CSR0_H_REQPKT;

                //set stage to data in
                m_uhost_ctrl.stage = CTRL_STAGE_DATA_IN;
            }
            else                                            //data out
            {
                //start send
                data_len = m_uhost_ctrl.data_len;
                trans_len = usb_host_ctrl_out(m_uhost_ctrl.buffer, data_len, AK_FALSE);

                m_uhost_ctrl.trans_len += trans_len;

                //set stage to data out
                m_uhost_ctrl.stage = CTRL_STAGE_DATA_OUT;                
            }
        }
        break;

    case CTRL_STAGE_DATA_OUT:
        if(m_uhost_ctrl.trans_len < m_uhost_ctrl.data_len)
        {
            data_len = m_uhost_ctrl.data_len - m_uhost_ctrl.trans_len;
            trans_len = usb_host_ctrl_out(m_uhost_ctrl.buffer+m_uhost_ctrl.trans_len, data_len, AK_FALSE);

            m_uhost_ctrl.trans_len += trans_len;
        }
        else
        {
            //data out finish, enter status stage
            REG8(USB_REG_CSR0) |= USB_CSR0_H_REQPKT | USB_CSR0_H_STATUSPKT;
            m_uhost_ctrl.stage = CTRL_STAGE_STATUS;
        }
        break;

    case CTRL_STAGE_STATUS:
        //transaction finish
        m_uhost_ctrl.stage = CTRL_STAGE_IDLE;

        if(m_uhost_ctrl.cbk_func != AK_NULL)
            m_uhost_ctrl.cbk_func(USB_HOST_TRANS_COMPLETE, m_uhost_ctrl.trans_len);
        
        break;

    default:
        //must be error in this case
        akprintf(C1, M_DRVSYS, "error stage %d in ep0 tx\n", m_uhost_ctrl.stage);
        break;
    }
}

static T_VOID usb_host_ep0_intr_handler()
{
    T_U8 usb_ep_csr;

    //read control and status reg0
    REG8(USB_REG_INDEX) = EP0_INDEX;
    usb_ep_csr = REG8(USB_REG_CSR0);

    if(usb_ep_csr & USB_CSR0_H_RXSTALL)                 //stall
    {
        //clear stall
        REG8(USB_REG_CSR0) &= ~USB_CSR0_H_RXSTALL;
        goto CTRL_TRANS_ERROR;
    }

    if(usb_ep_csr & USB_CSR0_H_ERROR)                   //error
    {
        //clear error
        REG8(USB_REG_CSR0) &= ~USB_CSR0_H_ERROR;

        goto CTRL_TRANS_ERROR;
    }

    if(usb_ep_csr & USB_CSR0_H_NAKTIMEOUT)             //nak timeout
    {
        //clear nak timeout
        REG8(USB_REG_CSR0) &= ~USB_CSR0_H_NAKTIMEOUT;

        goto CTRL_TRANS_ERROR;
    }

    if(usb_ep_csr & USB_CSR0_RXPKTRDY)                  //a packet is received
    {
        usb_host_ep0_rx_handler();
    }
    else if(!(usb_ep_csr & USB_CSR0_TXPKTRDY))
    {
        usb_host_ep0_tx_handler();
    }
    else
    {
        akprintf(C1, M_DRVSYS, "undefined ep0 interrupt: %x\n", usb_ep_csr);
    }

    return;

CTRL_TRANS_ERROR:

    m_uhost_ctrl.stage = CTRL_STAGE_IDLE;

    if(m_uhost_ctrl.cbk_func != AK_NULL)
    {
        m_uhost_ctrl.cbk_func(USB_HOST_TRANS_ERROR, m_uhost_ctrl.data_len);
    }
}

static T_VOID usb_host_tx_handler()
{
    T_U8 usb_ep_csr;
    T_U32 trans_len = 0;

    REG8(USB_REG_INDEX) = EP2_INDEX;
    usb_ep_csr = REG8(USB_REG_TXCSR1);

    if (0 != (usb_ep_csr & USB_TXCSR1_H_RXSTALL))           //stall
    {
        //clear stall
        REG8(USB_REG_TXCSR1) &= (~USB_TXCSR1_H_RXSTALL);

        goto TX_ERROR;
    }

    if(0 != (usb_ep_csr & USB_TXCSR1_H_ERROR))              //error
    {
        //clear error
        REG8(USB_REG_TXCSR1) &= (~USB_TXCSR1_H_ERROR);

        goto TX_ERROR;
    }
    
    if(0 != (usb_ep_csr & USB_TXCSR1_H_NAKTIMEOUT))         //nak timeout
    {
        //clear nak timeout
        REG8(USB_REG_TXCSR1) &= (~USB_TXCSR1_H_NAKTIMEOUT);

        goto TX_ERROR;
    }
    
    if(0 == (usb_ep_csr & USB_TXCSR1_TXPKTRDY))             //one packet send successfully
    {
        if(m_uhost_trans.stage == UHOST_TRANS_BULK_OUT)
        {
            if(m_uhost_trans.trans_len < m_uhost_trans.data_len)
            {
                trans_len = usb_host_data_out(UHOST_OUT_INDEX, m_uhost_trans.buffer + m_uhost_trans.trans_len, m_uhost_trans.data_len - m_uhost_trans.trans_len);
                m_uhost_trans.trans_len += trans_len;
            }
            else
            {
                //trans finish
                m_uhost_trans.stage = UHOST_TRANS_IDLE;

                if(m_uhost_trans.cbk_func != AK_NULL)
                {
                    m_uhost_trans.cbk_func(USB_HOST_TRANS_COMPLETE, m_uhost_trans.trans_len);
                }
            }
        }
    }

    return;

TX_ERROR:

    m_uhost_trans.stage = UHOST_TRANS_IDLE;

    if(m_uhost_trans.cbk_func != AK_NULL)
    {
        m_uhost_trans.cbk_func(USB_HOST_TRANS_ERROR, m_uhost_trans.trans_len);
    }
}

static T_VOID usb_host_rx_handler()
{
    T_U8 usb_ep_csr;
    T_U32 trans_len = 0;
    
    REG8(USB_REG_INDEX) = UHOST_IN_INDEX;
    usb_ep_csr = REG16(USB_REG_RXCSR1);
    
    if (0 != (usb_ep_csr & USB_RXCSR1_H_RXSTALL))       //stall
    {
        //clear stall
        //akprintf(C1, M_DRVSYS, "S");
        REG8(USB_REG_RXCSR1) &= (~USB_REG_RXCSR1_RXSTALL);
    
        goto RX_ERROR;
    }

    if(0 != (usb_ep_csr & USB_RXCSR1_H_ERROR))         //error
    {
        //clear error
        
        //akprintf(C1, M_DRVSYS, "e");
        REG8(USB_REG_RXCSR1) &= (~USB_RXCSR1_H_ERROR);

        goto RX_ERROR;
    }

    if(0 != (usb_ep_csr & USB_RXCSR1_NAKTIMEOUT))      //nak timeout
    {
        //clear nak timeout
        
        //akprintf(C1, M_DRVSYS, "t");
        REG8(USB_REG_RXCSR1) &= (~USB_RXCSR1_NAKTIMEOUT);

        goto RX_ERROR;
    }

    if (0 != (usb_ep_csr & USB_RXCSR1_RXPKTRDY))       //a packet is received
    {
        if(m_uhost_trans.stage == UHOST_TRANS_BULK_IN)
        {
            //a short pkt is reveived while usb dma is working
            if (m_uhost_trans.dma_start)
            {
                m_uhost_trans.trans_len += REG32(USB_DMA_ADDR_2) - 0x70000000;
            }
            trans_len = usb_host_data_in(UHOST_IN_INDEX, m_uhost_trans.buffer + m_uhost_trans.trans_len);
            m_uhost_trans.trans_len += trans_len;

            //trans finish
            if (trans_len < REG16(USB_REG_RXMAXP1) ||
               m_uhost_trans.trans_len >=  m_uhost_trans.data_len)
            {
                //a short pkt is reveived while usb dma is working
                if (m_uhost_trans.dma_start)
                {
                    //rxcsr2 is not allowed to modified before RxPktRdy is cleared,otherwise an ep3 int will occur
                    REG8(USB_REG_RXCSR2) = 0;
                    REG32(USB_DMA_CNTL_2) = 0;
                    REG16(USB_REG_REQPKTCNT1) = 0;
                    l2_combuf_stop_dma(usb_bulk_receive_id);
                    m_uhost_trans.dma_start  = AK_FALSE;
                }
                m_uhost_trans.stage = UHOST_TRANS_IDLE;

                //call back
                if(m_uhost_trans.cbk_func != AK_NULL)
                {
                    m_uhost_trans.cbk_func(USB_HOST_TRANS_COMPLETE, m_uhost_trans.trans_len);
                }
            }
            else
            {
                usb_host_request_data(EP1_INDEX);
            }
        }
        else
        {
            //discard this packet
            l2_clr_status(usb_bulk_receive_id);
            REG8(USB_REG_RXCSR1) &= (~USB_RXCSR1_RXPKTRDY);
        }
    }

    return;

RX_ERROR:
    m_uhost_trans.stage = UHOST_TRANS_IDLE;

    if(m_uhost_trans.cbk_func != AK_NULL)
    {
        m_uhost_trans.cbk_func(USB_HOST_TRANS_ERROR, m_uhost_trans.trans_len);
    }
}

static T_VOID usb_host_dma_handler(T_U32 dma_int)
{
    T_U32 i,short_pkt_len;
    
    if( (dma_int & DMA_CHANNEL1_INT)  == DMA_CHANNEL1_INT)
    {
        REG8(USB_REG_INDEX) = UHOST_OUT_INDEX;
        REG8(USB_REG_TXCSR2) = USB_TXCSR_MODE1;
        REG32(USB_DMA_CNTL_1) = 0;
        l2_combuf_wait_dma_finish(usb_bulk_send_id);
        
        short_pkt_len = m_uhost_trans.data_len - m_uhost_trans.trans_len;
        
        if (0 != short_pkt_len)
        {
            for (i = 0; i < short_pkt_len; i++)
            {
                REG8(USB_FIFO_EP0 + (UHOST_OUT_INDEX << 2)) = *(m_uhost_trans.buffer +  m_uhost_trans.trans_len + i);
            }
            //set tx count
            REG32(USB_EP2_TX_COUNT) = short_pkt_len;
            //set TXPKTRDY
            REG8(USB_REG_TXCSR1) |= USB_TXCSR1_TXPKTRDY;
        }
        else
        {
            //trans finish
            m_uhost_trans.stage = UHOST_TRANS_IDLE;

            if(m_uhost_trans.cbk_func != AK_NULL)
            {
                m_uhost_trans.cbk_func(USB_HOST_TRANS_COMPLETE, m_uhost_trans.trans_len);
            }
        }
   
    }

    if( (dma_int & DMA_CHANNEL2_INT)  == DMA_CHANNEL2_INT)
    {        
        REG8(USB_REG_INDEX) = UHOST_IN_INDEX;
        REG8(USB_REG_RXCSR2) = 0;
        REG32(USB_DMA_CNTL_2) = 0;
        l2_combuf_wait_dma_finish(usb_bulk_receive_id);
        m_uhost_trans.trans_len += REG32(USB_DMA_ADDR_2) - 0x70000000;
        m_uhost_trans.dma_start = AK_FALSE;
        if (m_uhost_trans.trans_len < m_uhost_trans.data_len)
        {
            REG8(USB_REG_RXCSR1) |= USB_RXCSR1_H_REQPKT;
        }
        else
        {
            m_uhost_trans.stage = UHOST_TRANS_IDLE;
            //call back
            if(m_uhost_trans.cbk_func != AK_NULL)
            {
                m_uhost_trans.cbk_func(USB_HOST_TRANS_COMPLETE, m_uhost_trans.trans_len);
            }
        }
    }
}

//********************************************************************
static T_BOOL usb_host_intr_handler(T_VOID)
{
    T_U8 usb_int;
    T_U8 usb_ep_int_tx;
    T_U8 usb_ep_int_rx;
    T_U32 usb_dma_int;

    T_BOOL usb_other_interrupt = AK_TRUE;

    usb_host_read_int_reg(&usb_int, &usb_dma_int);
    usb_host_read_ep_int_reg(&usb_ep_int_tx, &usb_ep_int_rx);
    
    if (usb_dma_int != 0)
    {
        usb_host_dma_handler(usb_dma_int);
        usb_other_interrupt = AK_FALSE;
    }

    if(0 != usb_int)
    {
        usb_host_common_intr_handler(usb_int);
        usb_other_interrupt = AK_FALSE;
    }

    if(USB_INTR_EP0 == (usb_ep_int_tx & USB_INTR_EP0))
    {
        usb_host_ep0_intr_handler();
        usb_other_interrupt = AK_FALSE;
    }
    
    if(USB_INTR_EP1 == (usb_ep_int_rx & USB_INTR_EP1))
    {
        usb_other_interrupt = AK_FALSE;

        usb_host_rx_handler();
    }
    
    if(USB_INTR_EP3 == (usb_ep_int_rx & USB_INTR_EP3))
    {
        usb_other_interrupt = AK_FALSE;

        usb_host_rx_handler();
    }
    
    if(USB_INTR_EP2 == (usb_ep_int_tx & USB_INTR_EP2))
    {
        usb_other_interrupt = AK_FALSE;
        
        usb_host_tx_handler();
    }    

    if(usb_other_interrupt)
    {
        akprintf(C3, M_DRVSYS, "usb_int = %x\n", usb_int);
        akprintf(C3, M_DRVSYS, "usb_t_int = %x\n", usb_ep_int_tx);
        akprintf(C3, M_DRVSYS, "usb_r_int = %x\n\n", usb_ep_int_rx);
        akprintf(C3, M_DRVSYS, "not designed !\n" );
    }
    
    return AK_TRUE;
    
}

//********************************************************************

#endif

