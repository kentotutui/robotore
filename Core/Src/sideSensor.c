/*
 * sideSensor.c
 *
 *  Created on: 2023/07/22
 *      Author: tutui
 */

#include "sideSensor.h"

static uint16_t goal_timer;
static bool side_sensor_l, side_sensor_r;
static bool goal_flag = false;
static bool goal_judge_flag = false;
static uint8_t start_goal_line_cnt;

//white <= 1700 black >= 1700

void updateSideSensorStatus(){
	goal_timer++;

	if(goal_timer >= 10000){
		goal_timer = 10000;
	}

	if(side_sensorL <= 2500){
		side_sensor_l = true;
	}
	else{
		side_sensor_l = false;
	}

	if(side_sensorR <= 2500){
		side_sensor_r = true;
	}
	else{
		side_sensor_r = false;
	}
}

void running(void)
{
	uint16_t pattern = 0;
	startLineTrace();

	while(goal_flag == false){
		switch(pattern){
				  case 0:

					  if(getSideSensorStatusR() == true){
						  start_goal_line_cnt++;
						  goal_timer = 0;
						  pattern = 5;
					  }
					  break;

				  case 5:
					  if(getSideSensorStatusR() == false) pattern = 10;

				  case 10:
					  if(getSideSensorStatusL() == true){ //Leght side line detect
						  goal_judge_flag = false;
						  goal_timer = 0;
					  }

					  if(goal_judge_flag == false && getSideSensorStatusR() == true && goal_timer >= 60){
						  goal_judge_flag = true;
						  goal_timer = 0;
					  }

					  else if(goal_judge_flag == true && goal_timer >= 60){
						  start_goal_line_cnt++;
						  goal_judge_flag = false;
						  goal_timer = 0;
					  }

					  if(start_goal_line_cnt >= 2){
						  pattern = 20;
					  }

					  break;

				  case 20:
					  //setLED('R');
					  //stopLineTrace();

					  //running_flag = false;
					  goal_flag = true;

					  break;
		}
	}
}

bool getSideSensorStatusL()
{
	return side_sensor_l;
}

bool getSideSensorStatusR()
{
	return side_sensor_r;
}

bool getgoalStatus()
{
	return goal_flag;
}
