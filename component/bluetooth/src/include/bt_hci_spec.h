/******************************************************************************
  * @file           bt_hci_spec.h
  * @author         Leonard-y.He(1027901566@qq.com)
  * @version        V0.0.1
  * @date           2020-12-03
  * @brief          bt hci spec header file
******************************************************************************/

#ifndef __BT_HCI_SPEC_H__
#define __BT_HCI_SPEC_H__

#include "bt_common.h"
#include "bt_pbuf.h"
#include "bt_vendor_manager.h"

typedef struct {
	uint16_t opcode;
	uint16_t length;
	uint8_t data[1];
}BT_PACK_END bt_hci_packet_cmd_t;

typedef struct {
	uint8_t indicator;
	union {
		bt_hci_cmd_t cmd;
		bt_hci_evt_t evt;
		bt_hci_acl_t acl;
	}
}BT_PACK_END bt_hci_packet_t;


#endif //__BT_HCI_SPEC_H__


