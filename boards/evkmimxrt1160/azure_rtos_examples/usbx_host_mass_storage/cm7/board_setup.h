
#ifndef _BOARD_SETUP_H_
#define _BOARD_SETUP_H_

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void board_setup(void);

void usb_host_setup(void);
void usb_host_interrupt_setup(int priority);
ULONG usb_host_base(void);

#endif /* _BOARD_SETUP_H_ */
