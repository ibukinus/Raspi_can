/**
 * @file  sample.c
 * @brief CAN通信ライブラリの運用例
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if.h>

#include "can_lib.h"

int main(void) {
  int           handle;
  pthread_t     id;
  int           sock;
  rcv_frame_t   rcv;
  int           rcv_status = 0;
  unsigned char data[]     = {0x11, 0x22};

  // CANの初期化
  sock = can_init();

  // フィルタ設定
  struct can_filter rfilter[2];
  set_can_filter(&rfilter[0], 0x123, CAN_SFF_MASK);
  set_can_filter(&rfilter[1], 0x200, 0x700);

  setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

  // データ受信
  rcv.socket = sock;
  handle     = pthread_create(&id, NULL, (void *(*)(void *))can_read, &rcv);
  if (handle) {
    perror("pthread_create");
    return 1;
  }

  // データ送信
  can_send(sock, 0x123, 2, data);
  puts("データ送信完了");

  pthread_join(id, (void *)&rcv_status);
  if (rcv_status) {
    puts("受信タイムアウト");
    return 1;
  }

  for (int i = 0; i < rcv.frame.can_dlc; i++) {
    printf("%d ", rcv.frame.data[i]);
  }
  printf("\n");
  puts("データ受信完了");

  close(sock);

  return 0;
}
