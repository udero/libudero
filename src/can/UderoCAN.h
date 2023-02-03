
/**
* UderoCAN.h
*
* @author Yuki Suga
* @copyright Techno Tool, 2015
*/

#pragma once

#include "udero/Udero.h"
#include "CANopen.h"
#include "SocketCAN.h"


LIBUDERO_API reharo::IUdero* createCANUdero(const reharo::UderoConnectionProfile& profile);

canopen::ICANInterface* getCAN(reharo::IUdero* udero);
canopen::CANopen* getCANopen(reharo::IUdero* udero);
