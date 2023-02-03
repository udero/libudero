#pragma once

extern double g_foldOutAngle[6];
extern double g_foldInAngle[6];
extern double g_foldingSpeed;
extern double g_gear_ratio[7];

#ifdef INCLUDE_WITH_DEFINE

double g_foldOutAngle[6] = {
  0.01,
  0.78,
  1.57,
  0.01,
  -0.78,
  0.01
};

double g_workAngle[6] = {
  0.78,
  0.78,
  1.57,
  -0.9,
  -1.3,
  0.5
};

double g_foldInAngle[6] = {
  0.0,
  // -1.3,
  -1.5,
  //  2.8,
  3.1,
  0.0,
  0.0,
  0.0
};

double g_foldingSpeed = 0.2;

double g_gear_ratio[7] = {
  100,
  //  202, // J2 
  2020.0/7,
  //  400, // J2 Version 2.0
  100,
  // 50, // J4 (J3.5)
  50, // J4 (J3.5) Version 2.0
  729/25.0*4,
  729/25.0*4,
  34.392,
};


#endif

static double base_gear_ratio = 100 * 40;
static double base_encoder_res = 512;
static double base_angle_min = -1000;
static double base_angle_max = 0.0;

