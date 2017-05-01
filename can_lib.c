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

/* CAN初期化関数 */
int can_init(void)
{
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
void can_send(int sock, canid_t id, size_t dlc, char *data)
{
    struct can_frame frame;
	frame.can_id  = id;
	frame.can_dlc = dlc;
    for (size_t i = 0; i < dlc; i++) {
        frame.data[i] = data[i];
    }
    write(sock, &frame, sizeof(struct can_frame));
}

/* CANデータ受信関数 */
void can_read(int sock, struct can_frame* frame)
{
    int nbytes;

    nbytes =read(sock, frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("can raw socket read");
        exit(1);
    }

    if (nbytes < (signed)sizeof(struct can_frame)) {
        fprintf(stderr, "read: incomplete CAN frame\n");
        exit(1);
    }
}

/* フィルタ設定関数 */
struct can_filter set_can_filter(canid_t id, canid_t mask)
{
    struct can_filter temp;
    temp.can_id = id;
    temp.can_mask = mask;

    return temp;
}

int main(void)
{
    int sock;
	struct can_frame frame;
    int i;
    char data[] = {0x11, 0x22};

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

    can_read(sock, &frame);
    for(i = 0; i < frame.can_dlc; i++){
        printf("%d ", frame.data[i]);
    }
    printf("\n");
    puts("データ受信完了");

    return 0;
}
