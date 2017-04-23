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
void can_send(int sock, struct can_frame frame)
{
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

/* フレーム設定関数 */
struct can_filter set_can_frame(int id, int dlc, char *data);

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

    /* CANの初期化 */
    sock = can_init();

    /* フィルタ設定 */
    struct can_filter rfilter[2];
    rfilter[0] = set_can_filter(0x123, CAN_SFF_MASK);
    rfilter[1] = set_can_filter(0x200, 0x700);

    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    /* データ送信 */
	frame.can_id  = 0x123;
	frame.can_dlc = 2;
	frame.data[0] = 0x11;
	frame.data[1] = 0x22;

    /* can_send(sock, frame); */
    can_read(sock, &frame);
    for(i = 0; i < frame.can_dlc; i++){
        printf("%d ", frame.data[i]);
    }
    printf("\n");

    return 0;
}
