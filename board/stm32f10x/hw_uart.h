/******************************************************************************
  * @file    hw_usart.h
  * @author  Yu-ZhongJun
  * @version V0.0.1
  * @date    2018-6-26
  * @brief   usart header file
******************************************************************************/
#ifndef HW_UART_H_H_H
#define HW_UART_H_H_H
#include "hw_misc.h"

#define BT_HCI_LOG_CMD      0x01
#define BT_HCI_LOG_ACL_OUT  0x02
#define BT_HCI_LOG_ACL_IN   0x04
#define BT_HCI_LOG_EVT      0x08
typedef uint8_t bt_hci_log_type_t;

uint8_t hw_uart_debug_init(uint32_t baud_rate);
uint8_t hw_uart_hci_log_init(uint32_t baud_rate);
void bt_hci_log(bt_hci_log_type_t type, uint8_t *log, uint16_t log_length);
#endif
