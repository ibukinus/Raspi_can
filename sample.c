// sample.c
#include <stdio.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_lib.h"

int main(void) {
    int sock;
	struct can_frame frame;
    int i;
    unsigned char data[] = {0x11, 0x22};

    /* CANの初期化 */
    sock = can_init();

    /* フィルタ設定 */
    struct can_filter rfilter[2];
    rfilter[0] = set_can_filter(0x123, CAN_SFF_MASK);
    rfilter[1] = set_can_filter(0x200, 0x700);

    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    /* データ送信 */
    can_send(sock, 0x123, 2, data);
    puts("データ送信完了");

    frame = can_read(sock);
    for(i = 0; i < frame.can_dlc; i++){
        printf("%d ", frame.data[i]);
    }
    printf("\n");
    puts("データ受信完了");

    return 0;
}
