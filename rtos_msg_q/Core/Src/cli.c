/*
 * cli.c
 *
 *  Created on: Jul 28, 2023
 *      Author: User
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "uart.h"
#include "led.h"
#include "pwm.h"
#include "cli.h"


typedef struct {
  char *cmd;
  uint8_t no;
  int (*cbf)(int, char **); // argc, char *argv[]
  char *remark;
} CMD_LIST_T;

static int cli_help(int argc, char *argv[]);
static int cli_echo(int argc, char *argv[]);
static int cli_led(int argc, char *argv[]);
static int cli_pwm(int argc, char *argv[]);
static int cli_bt(int argc, char *argv[]);

const CMD_LIST_T gCmdListObj[] = {
  { "help", 	1, cli_help, 	"help" 								},
  { "echo", 	2, cli_echo, 	"echo [echo data]"		},
  { "led",		3, cli_led,		"led [2/3] [on/off]"	},
  { "pwm",		3, cli_pwm,		"pmw [0] [0~100]"			},
  { "bt",			2, cli_bt,		"bt [n/v/] [name]"		},
  { NULL, 		0, NULL, 			NULL									}
};

static int cli_bt(int argc, char *argv[])
{
	return 0;
}

static int cli_pwm(int argc, char *argv[])
{
  if (argc < 3) printf("Need more arguments\r\n");

  long no = strtol(argv[1], NULL, 10);
  long duty = strtol(argv[2], NULL, 10);

  if (duty > 100) duty = 100;
  if (duty < 0) duty = 0;

  pwm_dimming((uint8_t)no, (uint8_t)duty);

  return 0;
}


static int cli_led(int argc, char *argv[])
{
  if (argc < 3) printf("Need more arguments\r\n");

  long no = strtol(argv[1], NULL, 10);
  int onoff = strcmp(argv[2], "off");

  if (onoff != 0) onoff = 1;

  //printf("led %ld %s\r\n", no, argv[2]);
  led_onoff((uint8_t)no, (uint8_t)onoff);

  return 0;
}

static int cli_echo(int argc, char *argv[])
{
  if (argc < 2) printf("Need more arguments\r\n");
  printf("%s\r\n", argv[1]);

  return 0;
}

static int cli_help(int argc, char *argv[])
{
  for (int i=0; gCmdListObj[i].cmd != NULL; i++) {
    printf("%s\r\n", gCmdListObj[i].remark);
  }

  return 0;
}

#define D_DELIMITER	" ,\r\n"

bool cli_parser(uint8_t *buf)
{
#if 1
  int argc = 0;
  char *argv[10];
  char *ptr;

  ptr = strtok((char *)buf, D_DELIMITER);
  if (ptr == NULL) return false;

  while(ptr != NULL) {
    //printf("%s\r\n", ptr);
    argv[argc] = ptr;
    argc++;
    ptr = strtok(NULL, D_DELIMITER);
  }

  for (int i=0; gCmdListObj[i].cmd != NULL; i++) {
    if (strcmp(gCmdListObj[i].cmd, argv[0]) == 0) {
      gCmdListObj[i].cbf(argc, argv);
      return true;
    }
  }

  printf("Unsupported command..\r\n");


#else
  char *ptr = strtok(buf, " ");    //첫번째 strtok 사용.
  while (ptr != NULL)              //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
  {
      printf("%s\n", ptr);         //자른 문자 출력
      ptr = strtok(NULL, " ");     //자른 문자 다음부터 구분자 또 찾기
  }

  uart_regcbf(cli_parser);
#endif
  return true;
}

void cli_init(void)
{
  uart_regcbf(0, cli_parser);
}

