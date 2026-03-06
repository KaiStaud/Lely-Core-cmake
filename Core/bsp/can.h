#ifndef BSP_CAN_H_
#define BSP_CAN_H_

#include <lely/can/msg.h>

#ifdef __cplusplus
extern "C" {
#endif

void can_init(int bitrate);
void can_fini(void);
int bsp_can_init(void);

size_t can_recv(struct can_msg *ptr, size_t n);
int can_send(const struct can_msg *msg, size_t n);

#ifdef __cplusplus
}
#endif

#endif // !BSP_CAN_H_
