/******************************************************************************
  * @file           bt_memory.c
  * @author         Leonard-y.He(1027901566@qq.com)
  * @version        V0.0.1
  * @date           2020-12-05
  * @brief          bt memory dynamic pool source file
******************************************************************************/
#include "bt_memory.h"
#include "stdbool.h"
static uint32_t bt_mm_footer = BT_MM_FOOTER;
static bt_mm_poll_ctrl_block_t bt_mm_cb;
bt_mm_poll_ctrl_block_t *p_bt_mm_cb = &bt_mm_cb;

void bt_memory_check_and_merge(bt_memory_type_t type);
	
void bt_memory_init(bt_memory_type_t type, uint8_t *buf, uint32_t size)
{
    BT_ASSERT("bt_mm_cb is null\r\n", p_bt_mm_cb);
    BT_ASSERT("bt_memory_init, buf size is too small \r\n", !(size % 4) && (size >= (sizeof(bt_mm_header_t) + BT_MM_FOOTER_SIZE)));
    bt_mm_cb.mm_poll_size[type] = size;
    bt_mm_cb.start_mm_h[type] = (bt_mm_header_t *)(buf);
    bt_mm_cb.search_mm_h[type] = bt_mm_cb.start_mm_h[type];
    /* add footer */
    memcpy((void *)(buf + size - BT_MM_FOOTER_SIZE), (void *)&bt_mm_footer, BT_MM_FOOTER_SIZE);
    /*info's length contains the size of footer*/
    BT_MM_SET_INFO(bt_mm_cb.start_mm_h[type]->info, BT_MM_STATE_FREE, size - sizeof(bt_mm_header_t));
}

static bool bt_memory_is_allocatable_packet(bt_memory_type_t type, uint32_t size)
{
    size += BT_MM_FOOTER_SIZE;
    if (bt_mm_cb.search_mm_h[type] != NULL && BT_MM_GET_SIZE(bt_mm_cb.search_mm_h[type]->info) > size) {
        return true;
    } else {
        return false;
    }
}

uint8_t *bt_memory_allocate_packet(bt_memory_type_t type, uint32_t size)
{
    uint8_t *ptr = NULL;
    bt_mm_header_t *new_mm = NULL;
    //bt_mm_header_t *next_mm = NULL;
    uint32_t new_mm_size = 0;
    if (!bt_memory_is_allocatable_packet(type, size)) {
        /*尝试合并内存碎片*/
        bt_memory_check_and_merge(type);
        if (bt_mm_cb.search_mm_h[type] == NULL) {
            BT_ASSERT("bt_memory_allocate_packet:out of memory \r\n", 0);
            return NULL;
        }
    }

    size += BT_MM_FOOTER_SIZE;
    size = MEM_ALIGN_SIZE(size);
    new_mm = bt_mm_cb.search_mm_h[type];
    BT_ASSERT("search_mm_h is invalid \r\n", new_mm != NULL);
    /* update search_mm_h*/
    if ((BT_MM_GET_SIZE(new_mm->info) - size) >= (sizeof(bt_mm_header_t) + BT_MM_FOOTER_SIZE)) {
        bt_mm_cb.search_mm_h[type] = (bt_mm_header_t *)((uint8_t *)new_mm + sizeof(bt_mm_header_t) + size);
        BT_MM_SET_INFO(bt_mm_cb.search_mm_h[type]->info, BT_MM_STATE_FREE, BT_MM_GET_SIZE(new_mm->info) - size - sizeof(bt_mm_header_t));
        new_mm_size = size;
    } else {
        bt_mm_cb.search_mm_h[type] = NULL;
        new_mm_size = BT_MM_GET_SIZE(new_mm->info);
    }

    /*update new_mm*/
    memset((uint8_t *)new_mm + sizeof(bt_mm_header_t), 0, new_mm_size - BT_MM_FOOTER_SIZE);
    /*set header info*/
    BT_MM_SET_INFO(new_mm->info, BT_MM_STATE_USING, new_mm_size);
    /*set footer info*/
    memcpy((uint8_t *)new_mm + sizeof(bt_mm_header_t) + new_mm_size - BT_MM_FOOTER_SIZE, &bt_mm_footer, BT_MM_FOOTER_SIZE);
    ptr = (uint8_t *)new_mm + sizeof(bt_mm_header_t);

    return ptr;
}

void bt_memory_free_packet(bt_memory_type_t type, uint8_t *ptr)
{
    bt_mm_header_t *mm_to_free = NULL;
    mm_to_free = (bt_mm_header_t *)(ptr - sizeof(bt_mm_header_t));
    void *footer = (uint8_t *)mm_to_free + sizeof(bt_mm_header_t) + BT_MM_GET_SIZE(mm_to_free->info) - BT_MM_FOOTER_SIZE;
    BT_ASSERT("bt_memory_free_packet:Footer was broken \r\n", !memcmp(footer, &bt_mm_footer, BT_MM_FOOTER_SIZE));
    BT_ASSERT("bt_memory_free_packet:mm state is invalid\r\n", BT_MM_STATE_USING == BT_MM_GET_STATE(mm_to_free->info));
    BT_ASSERT("bt_memory_free_packet:free ptr is overfolw\r\n", (ptr + BT_MM_GET_SIZE(mm_to_free->info)) <= ((uint8_t *)bt_mm_cb.start_mm_h[type] + bt_mm_cb.mm_poll_size[type]));
    BT_MM_SET_STATE(mm_to_free->info, BT_MM_STATE_FREE);
	bt_memory_check_and_merge(type);
}

/*在内存池中遍历搜寻free buffer，将连续的free buffer合并，且将search ptr指向size最大的free buffer*/
void bt_memory_check_and_merge(bt_memory_type_t type)
{
    bt_mm_header_t *tmp_mm = bt_mm_cb.start_mm_h[type];
    bt_mm_header_t *free_mm = NULL;
    uint8_t mm_state;
    do {
        mm_state = BT_MM_GET_STATE(tmp_mm->info);
        if (mm_state == BT_MM_STATE_FREE) {
            if (free_mm == NULL) {
                free_mm = tmp_mm;
            } else {
                /*找到两块连续的free buffer，合并它们*/
                BT_MM_SET_INFO(free_mm->info, BT_MM_STATE_FREE, BT_MM_GET_SIZE(free_mm->info) + BT_MM_GET_SIZE(tmp_mm->info) + BT_MM_HEADER_SIZE);
            }
            /*将search ptr指向size最大的free buffer*/
            if ((bt_mm_cb.search_mm_h[type] == NULL) || (BT_MM_GET_SIZE(free_mm->info) > BT_MM_GET_SIZE(bt_mm_cb.search_mm_h[type]->info))) {
                bt_mm_cb.search_mm_h[type] = free_mm;
            }
        } else {
            free_mm = NULL;
        }
        tmp_mm = (bt_mm_header_t *)((uint8_t *)tmp_mm + BT_MM_GET_SIZE(tmp_mm->info) + BT_MM_HEADER_SIZE);
    } while ((uint32_t)((uint8_t *)tmp_mm - (uint8_t *)bt_mm_cb.start_mm_h[type]) < (bt_mm_cb.mm_poll_size[type]));
}

