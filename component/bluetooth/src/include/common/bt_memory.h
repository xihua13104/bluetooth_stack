/******************************************************************************
  * @file           bt_memory.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.0.1
  * @date           2020-12-05
  * @brief          bt memory dynamic pool header file
******************************************************************************/
#ifndef __BT_MEMORY_H__
#define __BT_MEMORY_H__
#include "bt_common.h"
#include "bt_linknode.h"

#define BT_MM_HEADER_SIZE  (sizeof(bt_mm_header_t))
#define BT_MM_FOOTER_SIZE  (4)
#define BT_MM_FOOTER 	   (0xABCDABCD)

#define BT_MM_SIZE_MASK    (0x3FFFFFFF)
#define BT_MM_STATE_MASK   (0xC0000000)
#define BT_MM_SIZE_OFFSET  (0)
#define BT_MM_STATE_OFFSET (30)
#define BT_MM_STATE_FREE   (0U)
#define BT_MM_STATE_USING  (1U)

#define BT_MM_SET_STATE(info, state) ((info) = (((info) & BT_MM_SIZE_MASK) | ((state)<<BT_MM_STATE_OFFSET)))
#define BT_MM_GET_STATE(info)        ((info) >> BT_MM_STATE_OFFSET)
#define BT_MM_SET_SIZE(info, size)	 ((info) = (((info) & BT_MM_STATE_MASK) | ((size) & BT_MM_SIZE_MASK)))
#define BT_MM_GET_SIZE(info)		 ((info) & BT_MM_SIZE_MASK)
#define BT_MM_SET_INFO(info, state, size) ((info) = ((((state) << BT_MM_STATE_OFFSET) & BT_MM_STATE_MASK) | ((size) & BT_MM_SIZE_MASK)))

/**********************
bit31 & bit30 indicats the memory state
00->free
01->using
01->reserve for future
11->reserve for future

bit29-bit0 indicats the memory block size
**********************/
typedef struct {
    uint32_t info;
} bt_mm_header_t;

typedef enum {
    BT_MEMORY_TX = 0,
    BT_MEMORY_RX = 1
} bt_memory_type_t;

typedef struct {
    bt_mm_header_t *start_mm_h[2];
    bt_mm_header_t *search_mm_h[2];
    uint32_t mm_poll_size[2];
    bt_linknode_t rx_queue;
    bt_linknode_t tx_all_queue;
} bt_mm_poll_ctrl_block_t;

void bt_memory_init(bt_memory_type_t type, uint8_t *buf, uint32_t size);
uint8_t *bt_memory_allocate_packet(bt_memory_type_t type, uint32_t size);
void bt_memory_free_packet(bt_memory_type_t type, uint8_t *ptr);



#endif //__BT_MEMORY_H__


