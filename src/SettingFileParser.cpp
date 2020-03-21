#include <iostream>
#include <stdlib.h>
#include <float.h>
#include "udero/UderoLogger.h"
#include "SettingFileParser.h"

#include "param_loader.h"
#include "global_parameter.h"

void parseSettingFile(const std::string& filename) {
  UINFO("Setting File (%s)", filename.c_str());
  //  std::cout << "Setting filename is " << filename << std::endl;

  ParamLoader loader(filename.c_str());
  
  g_foldOutAngle[0] = atof(loader.get("FOLDOUT_ANGLE_J1", g_foldOutAngle[0]).c_str());
  g_foldOutAngle[1] = atof(loader.get("FOLDOUT_ANGLE_J2", g_foldOutAngle[1]).c_str());
  g_foldOutAngle[2] = atof(loader.get("FOLDOUT_ANGLE_J3", g_foldOutAngle[2]).c_str());
  g_foldOutAngle[3] = atof(loader.get("FOLDOUT_ANGLE_J4", g_foldOutAngle[3]).c_str());
  g_foldOutAngle[4] = atof(loader.get("FOLDOUT_ANGLE_J5", g_foldOutAngle[4]).c_str());
  g_foldOutAngle[5] = atof(loader.get("FOLDOUT_ANGLE_J6", g_foldOutAngle[5]).c_str());

  g_foldInAngle[0] = atof(loader.get("FOLDIN_ANGLE_J1", g_foldInAngle[0]).c_str());
  g_foldInAngle[1] = atof(loader.get("FOLDIN_ANGLE_J2", g_foldInAngle[1]).c_str());
  g_foldInAngle[2] = atof(loader.get("FOLDIN_ANGLE_J3", g_foldInAngle[2]).c_str());
  g_foldInAngle[3] = atof(loader.get("FOLDIN_ANGLE_J4", g_foldInAngle[3]).c_str());
  g_foldInAngle[4] = atof(loader.get("FOLDIN_ANGLE_J5", g_foldInAngle[4]).c_str());
  g_foldInAngle[5] = atof(loader.get("FOLDIN_ANGLE_J6", g_foldInAngle[5]).c_str());

  g_foldingSpeed = atof(loader.get("FOLDING_SPEED", "0.2").c_str());

  g_gear_ratio[0] = atof(loader.get("GEAR_RATIO_J1", "100").c_str());
  g_gear_ratio[1] = atof(loader.get("GEAR_RATIO_J2", "288.57142857").c_str());
  g_gear_ratio[2] = atof(loader.get("GEAR_RATIO_J3", "100").c_str());
  g_gear_ratio[3] = atof(loader.get("GEAR_RATIO_J4", "50").c_str());
  g_gear_ratio[4] = atof(loader.get("GEAR_RATIO_J5", "116.64").c_str());
  g_gear_ratio[5] = atof(loader.get("GEAR_RATIO_J6", "116.64").c_str());
  g_gear_ratio[6] = atof(loader.get("GEAR_RATIO_J7", "34.392").c_str());

  /*
  for(int i = 0;i < 6;i++) {
    std::cout << "FOLDOUT_ANGLE_J" << i+1 << " = " << g_foldOutAngle[i] << std::endl;
  }

  for(int i = 0;i < 6;i++) {
    std::cout << "FOLDIN_ANGLE_J" << i+1 << " = " << g_foldInAngle[i] << std::endl;
  }

  for(int i = 0;i < 7;i++) {
    std::cout << "GEAR_RATIO_J" << i+1 << " = " << g_gear_ratio[i] << std::endl;
  }
  */
}
