
#ifndef _BOARD_SETUP_H_
#define _BOARD_SETUP_H_

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void board_setup(void);

void usb_device_setup(void);
void usb_device_interrupt_setup(int priority);
ULONG usb_device_base(void);

#endif /* _BOARD_SETUP_H_ */
