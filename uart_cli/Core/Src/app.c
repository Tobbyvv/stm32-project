/*
 * app.c
 *
 *  Created on: Jul 28, 2023
 *      Author: User
 *
 */


#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "led.h"
#include "pwm.h"
#include "uart.h"
#include "cli.h"
#include "app.h"

typedef struct {
  uint32_t counter, period;
  void (*cbf)(void *);
  bool flag;
} PROC_T;

//typedef __packed struct {
typedef struct {
  uint8_t a, b;
  uint16_t xx;
} XX_T;

static void proc_10ms(void *arg);
static void proc_50ms(void *arg);
static void proc_100ms(void *arg);
static void proc_1s(void *arg);

PROC_T gProcListObj[] = {
    { 0, 	1000, 	proc_10ms, 	false },
    { 0, 	5000, 	proc_50ms, 	false },
    { 0, 	10000, 	proc_100ms, 	false },
    { 0, 	100000, 	proc_1s, 	false },
    { 0,	0,	NULL,		false }
};

void app_init(void)
{
  printf("System start....\r\n");
}


void app(void)
{
  app_init();
  led_init();
  pwm_init();
  uart_init();
  cli_init();
}

void proc_10ms(void *arg)
{
  printf("%s\r\n", __func__);
}

void proc_50ms(void *arg)
{
  printf("%s\r\n", __func__);
}

void proc_100ms(void *arg)
{
  printf("%s\r\n", __func__);
}

void proc_1s(void *arg)
{
  printf("%s\r\n", __func__);
}

void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
  {
//    for (int i=0; gProcListObj[i].cbf != NULL; i++) {
//      gProcListObj[i].counter++;
//      gProcListObj[i].counter %= gProcListObj[i].period;
//      if (gProcListObj[i].counter == 0) gProcListObj[i].flag = true;
//    }
  }
}


