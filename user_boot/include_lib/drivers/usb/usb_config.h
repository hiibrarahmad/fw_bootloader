#ifndef  __USB_CONFIG_H__
#define  __USB_CONFIG_H__

#include "typedef.h"
#include "usb.h"
#include "usb/usb_stack.h"

void usb_host_config(usb_dev usb_id);
void usb_host_free(usb_dev usb_id);
void *usb_h_get_ep_buffer(const usb_dev usb_id, u32 ep);
void usb_h_isr_reg(const usb_dev usb_id, u8 priority, u8 cpu_id);
void usb_g_isr_reg(const usb_dev usb_id, u8 priority, u8 cpu_id);



void usb_sof_isr_reg(const usb_dev usb_id, u8 priority, u8 cpu_id);
void *usb_alloc_ep_dmabuffer(const usb_dev usb_id, u32 ep, u32 dma_size);
u32 usb_config(const usb_dev usb_id);
u32 usb_release(const usb_dev usb_id);
void usb_otg_sof_check_init(const usb_dev id);
void usb_memory_init();

#endif  /*USB_CONFIG_H*/
