#pragma once

extern double g_foldOutAngle[6];
extern double g_foldInAngle[6];
extern double g_foldingSpeed;
extern double g_gear_ratio[7];

extern double g_angle_min_max[7][2];
#ifdef INCLUDE_WITH_DEFINE



#endif

static double base_gear_ratio = 100 * 40;
static double base_encoder_res = 512;
static double base_angle_min = -1000;
static double base_angle_max = 0.0;

