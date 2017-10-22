// sample.c
#include <stdio.h>
#include <sys/socket.h>
#include <linux/if.h>
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
    set_can_filter(&rfilter[0], 0x123, CAN_SFF_MASK);
    set_can_filter(&rfilter[1], 0x200, 0x700);

    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    /* データ送信 */
    can_send(sock, 0x123, 2, data);
    puts("データ送信完了");

    can_read(sock, &frame);
    for(i = 0; i < frame.can_dlc; i++){
        printf("%d ", frame.data[i]);
    }
    printf("\n");
    puts("データ受信完了");

    return 0;
}
