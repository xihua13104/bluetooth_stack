/******************************************************************************
  * @file           bt_linknode.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.0.1
  * @date           2020-12-05
  * @brief          bt link header file
******************************************************************************/
#ifndef __BT_LINKNODE_H__
#define __BT_LINKNODE_H__
#include "bt_config.h"

typedef struct _bt_linknode_ {
	struct _bt_linknode_ *next;
} BT_PACK_END bt_linknode_t;

#endif //__BT_LINKNODE_H__


