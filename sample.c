// sample.c
#include <stdio.h>

#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if.h>

#include "can_lib.h"

int main(void) {
  int           sock;
  rcv_frame_t   rcv;
  int           i;
  unsigned char data[] = {0x11, 0x22};

  /* CANの初期化 */
  sock = can_init();

  /* フィルタ設定 */
  struct can_filter rfilter[2];
  set_can_filter(&rfilter[0], 0x123, CAN_SFF_MASK);
  set_can_filter(&rfilter[1], 0x200, 0x700);

  setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

  /* データ送信 */
  rcv.socket = sock;
  can_send(sock, 0x123, 2, data);
  puts("データ送信完了");

  can_read(&rcv);
  for (i = 0; i < rcv.frame.can_dlc; i++) {
    printf("%d ", rcv.frame.data[i]);
  }
  printf("\n");
  puts("データ受信完了");

  return 0;
}
