/*
 * Encoder.c
 *
 *  Created on: May 9, 2023
 *      Author: tutui
 */

#include "Encoder.h"

#define MAX_ENCODER_CNT 65535
#define CNT_OFFSET 10000 //32768

#define WHEEL_RADIUS 10.5 //[mm] ホイール半径
#define PI 3.1415926535
#define ENCODER_RESOLUTION 4096

#define REDUCTION_RATIO 0.4 //Gear reduction ratio 減速比

#define DISTANCE_PER_CNT (2 * PI * WHEEL_RADIUS * REDUCTION_RATIO / ENCODER_RESOLUTION) //[mm per cnt]

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

static int16_t enc_l_cnt, enc_r_cnt, enc_l_total, enc_r_total;

static float distance_1ms;
static float distance_10mm;
static float total_distance;
static float goal_judge_distance;
static float side_line_judge_distance;
static float distance_cross_line_ignore;
static float distance_side_line_ignore;

void initEncoder(void)
{
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);	//encoder start
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);	//encoder start
	TIM3 -> CNT = CNT_OFFSET;
	TIM4 -> CNT = CNT_OFFSET;
}

void updateEncoderCnt(void)
{
	enc_l_cnt = TIM4 -> CNT - CNT_OFFSET;
	enc_r_cnt = CNT_OFFSET - TIM3 -> CNT;

	enc_l_total += enc_l_cnt;
	enc_r_total += enc_r_cnt;
	//enc_total = (enc_l_total + enc_r_total) / 2;

	distance_1ms = DISTANCE_PER_CNT * (enc_l_cnt + enc_r_cnt) / 2;
	distance_10mm += distance_1ms;
	total_distance += distance_1ms;
	goal_judge_distance += distance_1ms;
	side_line_judge_distance += distance_1ms;
	distance_cross_line_ignore += distance_1ms;
	distance_side_line_ignore += distance_1ms;
	//15.73カウントで1ｍｍ
}

void resetEncoderCnt(void)
{
	TIM3 -> CNT = CNT_OFFSET;
	TIM4 -> CNT = CNT_OFFSET;
}
