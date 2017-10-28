/**
 * @file  can_lib.h
 * @brief CAN通信ライブラリ
 */
#ifndef CAN_LIB_H_INCLUDED
#define CAN_LIB_H_INCLUDED

#include <stdio.h>

#include <linux/can.h>

/**
 * @brief CAN受信用構造体
 */
typedef struct rcv_frame {
  int              socket;  ///< ソケット
  struct can_frame frame;   ///< 受信フレーム
} rcv_frame_t;

/**
 * @brief CAN通信初期処理
 */
int can_init(void);

/**
 * @brief データ送信処理
 *
 * 送信フレームを作成し、送信処理を行います。
 *
 * @param[in] sock  ソケット
 * @param[in] id    送信ID
 * @param[in] dlc   送信データ長
 * @param[in] data  データへのポインタ
 */
void can_send(int sock, canid_t id, unsigned char dlc, unsigned char *data);

/**
 * @brief データ受信処理
 *
 * データの受信を行います。10秒間受信できなかった場合、タイムアウトします。
 *
 * @param[in,out] rcv   受信フレームへのポインタ
 * @return        0     受信成功
 * @return        1     タイムアウト
 */
int can_read(rcv_frame_t *rcv);

// =============================================================================
// 関数名 : set_can_filter
// 機能  : 受信フィルタ設定関数
// 引数  : 第1引数 can_filter構造体、第2引数 受信対象id、第3引数 受信idマスク値
// 戻り値 : なし
// =============================================================================
/**
 * @brief 受信フィルタ設定関数
 *
 * 受信フィルタ構造体をセットします。
 *
 * @param[in] filter  フィルタ構造体
 * @param[in] id    受信対象ID
 * @param[in] mask   マスク値
 */
void set_can_filter(struct can_filter *filter, canid_t id, canid_t mask);

#endif
