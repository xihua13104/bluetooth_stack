/******************************************************************************
  * @file           bt_pbuf.h
  * @author         Yu-ZhongJun(124756828@qq.com)
  * @Taobao link    https://shop220811498.taobao.com/
  * @version        V0.0.1
  * @date           2020-4-13
  * @brief          bt pbuf header file
******************************************************************************/

#ifndef BT_PBUF_H_H_H
#define BT_PBUF_H_H_H

#include "bt_common.h"

#define BT_PBUF_TRANSPORT_H2 0x01  /** Bluetooth H2(USB D+/D-) transport */
#define BT_PBUF_TRANSPORT_H4 0x02  /** Bluetooth H4(UART TX/RX/CTS/RTS) transport */
#define BT_PBUF_TRANSPORT_H5 0x03  /** Bluetooth H5(UART TX/RX) transport */
#define BT_PBUF_TRANSPORT_BCSP 0x04/** Bluetooth BCSP(UART TX/RX) transport */
#define BT_PBUF_RAW 0x05           /** Bluetooth RAW packet */


typedef enum
{
    BT_PBUF_RAM,
    BT_PBUF_POOL
} bt_pbuf_type_e;


/** pbuf�ӿڽṹ�嶨�� */
struct bt_pbuf_t
{
    /** �������е���һ��pbuf�ڵ� */
    struct bt_pbuf_t *next;

    /** pbuf�ڵ�payload����ָ�� */
    void *payload;

    /** pbuf�������б��ڵ��Լ������ڵ�������ܺ� */
    uint16_t tot_len;

    /** ��pbuf�ڵ��payload���ݳ��� */
    uint16_t len;

    /** pbuf���� */
    uint8_t  type;

    /** pbuf��־ */
    uint8_t flags;

    /** pbuf���ô��� */
    uint16_t ref;
};

struct bt_pbuf_t *bt_pbuf_alloc(uint8_t layer, uint16_t length, bt_pbuf_type_e type);
void bt_pbuf_realloc(struct bt_pbuf_t *p, uint16_t size);
uint8_t bt_pbuf_header(struct bt_pbuf_t *p, int16_t header_size);
void bt_pbuf_ref(struct bt_pbuf_t *p);
uint8_t bt_pbuf_free(struct bt_pbuf_t *p);
void bt_pbuf_cat(struct bt_pbuf_t *head, struct bt_pbuf_t *tail);
void bt_pbuf_chain(struct bt_pbuf_t *head, struct bt_pbuf_t *tail);
struct bt_pbuf_t *bt_pbuf_dechain(struct bt_pbuf_t *p);
err_t bt_pbuf_copy(struct bt_pbuf_t *p_to, const struct bt_pbuf_t *p_from);
uint16_t bt_pbuf_copy_partial(const struct bt_pbuf_t *p, void *dataptr, uint16_t len, uint16_t offset);
err_t bt_pbuf_take(struct bt_pbuf_t *buf, const void *dataptr, uint16_t len);
err_t bt_pbuf_take_at(struct bt_pbuf_t *buf, const void *dataptr, uint16_t len, uint16_t offset);
struct bt_pbuf_t *bt_pbuf_skip(struct bt_pbuf_t* in, uint16_t in_offset, uint16_t* out_offset);



#endif
