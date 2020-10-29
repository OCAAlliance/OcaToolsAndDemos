#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);      

void ETHERNET_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // __ETHERNETIF_H__
