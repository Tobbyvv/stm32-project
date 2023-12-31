/*
 * app.c
 *
 *  Created on: Jul 27, 2023
 *      Author: User
 */

#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "app.h"

static bool exti_flag = false; //버튼 on/off 상태 저장을 위한 변수 선언

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t prev;
  volatile uint32_t curr;
  curr = HAL_GetTick();	//현재 시간 가져옴

  if ((uint32_t)(curr - prev) > 150) { //버튼 동작 시간 간격 0.15초로 중복 실행 방지
    if (GPIO_Pin == GPIO_PIN_13) {
       exti_flag = true;
    }
    prev = curr;	//버튼 누른 시간 저장
  }
}

#define D_TIME_MAX	500	//LED 깜빡거림 시간 간격 0.5초
#define D_LED_MAX	3	//LED 갯수

typedef struct {
  uint32_t prev;	//이전 LED 토글 시간 저장
  void (*cbf)(void *, uint8_t);	//함수 포인터 선언(※ LED Toggle 함수 할당용)
} LED_TOGGLE_T;

LED_TOGGLE_T gLedObj[D_LED_MAX]; //3개의 LED의 토글 동작을 관리할배열 선언


// LD1 LED 토글 함수
void LD1_Toggle(void *pArg, uint8_t op)
{
  LED_TOGGLE_T *p = (LED_TOGGLE_T *)pArg;
  if (op == 1) {
    uint32_t curr = HAL_GetTick();

    if (curr - p->prev > D_TIME_MAX) {
      HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
      p->prev = curr;
      printf("%s\r\n", __func__);
    }
  } else {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
  }
}

// LD2 LED 토글 함수
void LD2_Toggle(void *pArg, uint8_t op)
{
  LED_TOGGLE_T *p = (LED_TOGGLE_T *)pArg;
  if (op == 1) {
    uint32_t curr = HAL_GetTick();

    if (curr - p->prev > D_TIME_MAX) {
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
      p->prev = curr;
      printf("%s\r\n", __func__);
    }
  } else {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
  }
}

// LD3 LED 토글 함수
void LD3_Toggle(void *pArg, uint8_t op)
{
  LED_TOGGLE_T *p = (LED_TOGGLE_T *)pArg;

  if (op == 1) {
    uint32_t curr = HAL_GetTick();
    if (curr - p->prev > D_TIME_MAX) {
      HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
      p->prev = curr;
      printf("%s\r\n", __func__);
    }
  } else {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  }
}

void app_init(void)
{
  // LED 토글 함수 및 구조체 초기화
  gLedObj[0].cbf = LD1_Toggle;
  gLedObj[1].cbf = LD2_Toggle;
  gLedObj[2].cbf = LD3_Toggle;

  for (uint8_t i=0; i<D_LED_MAX; i++) {
    if (gLedObj[i].cbf == NULL) while (1); //TOGGLE 함수 할당이 되었는지 확인
  } //할당 되지 않았을 경우 while문 종료
}

void app(void)
{
  uint8_t state = 0;
  app_init();

  while (1)
  {
     if (exti_flag == true) {
       exti_flag = false;
       state++;
       state %= D_LED_MAX;
       gLedObj[state].cbf((void *)&gLedObj[state], 0);
       //1.gLedObj[state], 증가된 매개변수에 해당하는 함수와
       //2.먼저 0을 전달하여 다른 LED를 종료시킴
     }
     gLedObj[state].cbf((void *)&gLedObj[state], 1);
     //1을 전달하여 해당 LED를 깜빡거리게함
  }
}
