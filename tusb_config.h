#ifndef _TUSB_CONFIG_H
#define _TUSB_CONFIG_H

//#define TUSB_CDC

// Enable Device stack
#define CFG_TUD_ENABLED       1

#define CFG_TUD_ENDPOINT0_SIZE    64

//------------- CLASS -------------//
#define CFG_TUD_CDC               2

// CDC FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE   (64)
#define CFG_TUD_CDC_TX_BUFSIZE   (64)

// CDC Endpoint transfer buffer size, more is faster
#define CFG_TUD_CDC_EP_BUFSIZE   (64)

#endif //_TUSB_CONFIG_H
