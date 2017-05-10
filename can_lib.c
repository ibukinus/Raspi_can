/* CAN通信ライブラリ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_lib.h"

/* CAN初期化関数 */
int can_init(void) {
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;

    const char *ifname = "can0";

	if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0){
		perror("ソケット作成に失敗");
        exit(1);
	}

	strcpy(ifr.ifr_name, ifname);
	ioctl(sock, SIOCGIFINDEX, &ifr);

	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("ソケットのバインドに失敗");
        exit(1);
	}

    return sock;
}

/* CANデータ送信関数 */
void can_send(int sock, canid_t id, size_t dlc, char *data) {
    struct can_frame frame;
    int nbytes;
	frame.can_id  = id;
	frame.can_dlc = dlc;
    for (size_t i = 0; i < dlc; i++) {
        frame.data[i] = data[i];
    }
    nbytes = write(sock, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("送信失敗");
        exit(1);
    }

    if (nbytes < (signed)sizeof(struct can_frame)) {
        fprintf(stderr, "送信未完了\n");
        exit(1);
    }

}

/* CANデータ受信関数 */
struct can_frame can_read(int sock) {
    struct can_frame frame;
    int nbytes;

    nbytes = read(sock, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("can raw socket read");
        exit(1);
    }

    if (nbytes < (signed)sizeof(struct can_frame)) {
        fprintf(stderr, "read: incomplete CAN frame\n");
        exit(1);
    }
    return frame;
}

/* フィルタ設定関数 */
struct can_filter set_can_filter(canid_t id, canid_t mask) {
    struct can_filter temp;
    temp.can_id = id;
    temp.can_mask = mask;

    return temp;
}

// 整数型からバイト列への変換関数
void itob(int src, char *byte) {
    memcpy(byte, &src, sizeof(src));
}
void ltob(long src, char *byte) {
    memcpy(byte, &src, sizeof(src));
}

// バイト列から整数型への変換関数
int btoi(char *src, size_t size) {
    int data = 0x00;
    memcpy(&data, src, size);
    return data;
}

long btol(char *src, size_t size) {
    int data = 0x00;
    memcpy(&data, src, size);
    return data;
}
