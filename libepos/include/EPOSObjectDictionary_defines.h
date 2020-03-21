/**
 * @file EPOSObjectDictionary_defines.h
 */

#ifndef EPOS_OBJECT_DICTIONARY_DEFINES_HEADER_INCLUDED
#define EPOS_OBJECT_DICTIONARY_DEFINES_HEADER_INCLUDED


// Error Register
enum EPOS_ERRORCODE {
	EPOS_ERROR_NOERROR = 0x0000,
	EPOS_ERROR_GENERIC = 0x1000,
	EPOS_ERROR_OVERCURRENT = 0x2310,
	EPOS_ERROR_OVERVOLTAGE = 0x3210,
	EPOS_ERROR_UNDERVOLTAGE = 0x3220,
	EPOS_ERROR_OVERTEMPERATURE = 0x4210,
	EPOS_ERROR_LOGICSUPPLYVOLTAGETOOLOW = 0x5113,
	EPOS_ERROR_SUPPLYVOLTAGEOUTPUTSTAGETOOLOW = 0x5114,
	EPOS_ERROR_INTERNALSOFTWARE = 0x6100,
	EPOS_ERROR_SOFTWAREPARAMETER = 0x6320,
	EPOS_ERROR_POSITIONSENSOR = 0x7320,
	EPOS_ERROR_CANOVERRRUNOBJECTLOST = 0x8110,
	EPOS_ERROR_CANOVERRUN = 0x8111,
	EPOS_ERROR_CANPASSIVEMODE = 0x8120,
	EPOS_ERROR_CANLIFEGUARD = 0x8130,
	EPOS_ERROR_CANTRANSMITCOBIDCOLLISION = 0x8150,
	EPOS_ERROR_CANBUSOFF = 0x81FD,
	EPOS_ERROR_CANRXQUEUEOVERFLOW = 0x81FE,
	EPOS_ERROR_CANTXQUEUEOVERFLOW = 0x81FF,
	EPOS_ERROR_CANPDOLENGTH = 0x8210,
	EPOS_ERROR_FOLLOWING = 0x8611,
	EPOS_ERROR_HALLSENSOR = 0xFF01,
	EPOS_ERROR_INDEXPROCESSING = 0xFF02,
	EPOS_ERROR_ENCODERRESOLUTION = 0xFF03,
	EPOS_ERROR_HALLSENSORNOTFOUND = 0xFF04,
	EPOS_ERROR_NEGATIVELIMITSWITCH = 0xFF06,
	EPOS_ERROR_POSITIVELIMITSWITCH = 0xFF07,
	EPOS_ERROR_HALLANGLEDETECTION  = 0xFF08,
	EPOS_ERROR_SOFTWAREPOSITIONLIMIT = 0xFF09,
	EPOS_ERROR_POSITIONSENSORBREACH = 0xFF0A,
	EPOS_ERROR_SYSTEMOVERLOADED = 0xFF0B,
	EPOS_ERROR_INTERPOLATEDPOSITIONMODE = 0xFF0C,
	EPOS_ERROR_AUTOTUNINGINDENTIFICATION = 0xFF0D,
	EPOS_ERROR_GEARSCALINGFACTOR = 0xFF0F,
	EPOS_ERROR_CONTROLLERGAIN = 0xFF10,
	EPOS_ERROR_MAINSENSORDIRECTION = 0xFF11,
	EPOS_ERROR_AUXILIARYSENSORDIRECTION = 0xFF12,
};


// Node ID : uint8_t 
#define OD_INDEX_NODEID 0x2000
#define OD_SUBINDEX_NODEID 0x00


// CAN Bit Rate : uint16_t
#define OD_INDEX_CANBITRATE 0x2001
#define OD_SUBINDEX_CANBITRATE 0x00

#define EPOS_CANBITRATE_1M   0
#define EPOS_CANBITRATE_800K 1
#define EPOS_CANBITRATE_500K 2
#define EPOS_CANBITRATE_250K 3
#define EPOS_CANBITRATE_125K 4
#define EPOS_CANBITRATE_50K  6
#define EPOS_CANBITRATE_20K  7
#define EPOS_CANBITRATE_AUTO 9

// RS232 Bit Rate : uint16_t
#define OD_INDEX_RS232BITRATE 0x2002
#define OD_SUBINDEX_RS232BITRATE 0x00

#define EPOS_RS232BITRATE9600   0
#define EPOS_RS232BITRATE14400  1
#define EPOS_RS232BITRATE19200  2
#define EPOS_RS232BITRATE38400  3
#define EPOS_RS232BITRATE57600  4
#define EPOS_RS232BITRATE115200 5

// Software Version : uint16_t
#define OD_INDEX_SOFTWAREVERSION 0x2003
#define OD_SUBINDEX_SOFTWAREVERSION 0x01

// Hardware Version : uint16_t
#define OD_INDEX_HARDWAREVERSION 0x2003
#define OD_SUBINDEX_HARDWAREVERSION 0x02

// Application Number : uint16_t
#define OD_INDEX_APPLICATIONNUMBER 0x2003
#define OD_SUBINDEX_APPLICATIONNUMBER 0x03

// Application Version : uint16_t
#define OD_INDEX_APPLICATIONVERSION 0x2003
#define OD_SUBINDEX_APPLICATIONVERSION 0x04

// Internal Object : uint16_t
#define OD_INDEX_INTERNALOBJECT 0x2003
#define OD_SUBINDEX_INTERNALOBJECT 0x04

// Serial Number : uint64_t
#define OD_INDEX_SERIALNUMBER64 0x2004
#define OD_SUBINDEX_SERIALNUMBER64 0x00

// RS232 RRAME TIMEOUT : uint16_t
#define OD_INDEX_RS232FRAMETIMEOUT 0x2005
#define OD_SUBINDEX_RS232FRAMETIMEOUT 0x00

// USB RRAME TIMEOUT : uint16_t
#define OD_INDEX_USBFRAMETIMEOUT 0x2006
#define OD_SUBINDEX_USBFRAMETIMEOUT 0x00

// Miscellaneous Configuration : uint16_t
#define OD_INDEX_MISCELLANEOUSCONFIGURATION 0x2008
#define OD_SUBINDEX_MISCELLANEOUSCONFIGURATION 0x00

#define EPOS_MISC_CONFIG_NORMALPOLARITY  0x0000 //<If demanded velocity or position is positive, direction of rotation is CCW
#define EPOS_MISC_CONFIG_INVERSEPOLARITY 0x0100 //<If demanded velocity or position is positive, direction of rotation is CW
#define EPOS_MISC_CONFIG_MEASUREAUX      0x0010 //<Measure auxiliary position sensor’s motor speed exacting by detecting encoder pulse time (only available for sensor type Hall, Incremental Encoder 1 and Incremental Encoder 2)
#define EPOS_MISC_CONFIG_MEASUREMAIN     0x0008 //<Measure main position sensor’s motor speed exacting by detecting encoder pulse time (only available for sensor type Hall, Incremental Encoder 1 and Incremental Encoder 2)
#define EPOS_MISC_CONFIG_MEASURERESISTFIRST 0x0000 //<Measure (DC) motor resistance at first change to enable
#define EPOS_MISC_CONFIG_MEASURERESISTEVERY 0x0004 //<Measure (DC) motor resistance at every change to enable (used for position sensor supervision by software)
#define EPOS_MISC_CONFIG_DISABLEHARDWARESENSE 0x0002 //<Disable sensor supervision by hardware
#define EPOS_MISC_CONFIG_DISABLESOFTWARESENSE 0x0001 //<Disable sensor supervision by software


// CAN Bit Rate Display : uint16_t
#define OD_INDEX_CANBITRATEDISPLAY 0x200A
#define OD_SUBINDEX_CANBITRATEDISPLAY 0x00

// Custom Persistent Memory : uint32_t
#define OD_INDEX_CUSTOMPERSISTENTMEMORY1    0x200C
#define OD_SUBINDEX_CUSTOMPERSISTENTMEMORY1 0x01
#define OD_INDEX_CUSTOMPERSISTENTMEMORY2    0x200C
#define OD_SUBINDEX_CUSTOMPERSISTENTMEMORY2 0x02
#define OD_INDEX_CUSTOMPERSISTENTMEMORY3    0x200C
#define OD_SUBINDEX_CUSTOMPERSISTENTMEMORY3 0x03
#define OD_INDEX_CUSTOMPERSISTENTMEMORY4    0x200C
#define OD_SUBINDEX_CUSTOMPERSISTENTMEMORY4 0x04

// Incremental Encoder 1 Counter : uint32_t
#define OD_INDEX_INCREMENTALENCODER1COUNTER    0x2020
#define OD_SUBINDEX_INCREMENTALENCODER1COUNTER 0x00

// Incremental Encoder 1 Counter At Index Pulse : uint32_t
#define OD_INDEX_INCREMENTALENCODER1COUNTERATINDEX    0x2021
#define OD_SUBINDEX_INCREMENTALENCODER1COUNTERATINDEX 0x00

// Hall Sensor Pattern : uint16_t
#define OD_INDEX_HALLSENSORPATTERN    0x2022
#define OD_SUBINDEX_HALLSENSORPATTERN 0x00

#define EPOS_HALLSENSOR1BIT 0
#define EPOS_HALLSENSOR2BIT 1
#define EPOS_HALLSENSOR3BIT 2

// Current Actual Averaged : int16_t
#define OD_INDEX_CURRENTACTUALVALUEAVERAGED    0x2027
#define OD_SUBINDEX_CURRENTACTUALVALUEAVERAGED 0x00

// Velocity Actual Averaged : int32_t
#define OD_INDEX_VELOCITYACTUALVALUEAVERAGED    0x2028
#define OD_SUBINDEX_VELOCITYACTUALVALUEAVERAGED 0x00


// Current Mode Setting Value : int16_t
#define OD_INDEX_CURRENTMODESETTINGVALUE    0x2030
#define OD_SUBINDEX_CURRENTMODESETTINGVALUE 0x00

// Current Demand Value : int32_t
#define OD_INDEX_CURRENTDEMANDGVALUE    0x2031
#define OD_SUBINDEX_CURRENTDEMANDVALUE  0x00

// Position Mode Setting Value : int32_t
#define OD_INDEX_POSITIONMODESETTINGVALUE    0x2062
#define OD_SUBINDEX_POSITIONMODESETTINGVALUE 0x00

// Aux Velocity Sensor Actual Value : int32_t
#define OD_INDEX_AUXVELOCITYSENSORACTUALVALUE 0x2069
#define OD_SUBINDEX_AUXVELOCITYSENSORACTUALVALUE 0x00

// Velocity Mode Setting Value : int32_t
#define OD_INDEX_VELOCITYMODESETTINGVALUE    0x206B
#define OD_SUBINDEX_VELOCITYMODESETTINGVALUE 0x00

// Aux Velocity Actual Value : int32_t
#define OD_INDEX_AUXVELOCITYACTUALVALUE 0x206C
#define OD_SUBINDEX_AUXVELOCITYACTUALVALUE 0x00

// Configuration of Dital Input : uint16_t
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT1    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT1 0x01
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT2    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT2 0x02
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT3    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT3 0x03
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT4    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT4 0x04
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT5    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT5 0x05
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT6    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT6 0x06
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT7    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT7 0x07
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT8    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT8 0x08
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT9    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT9 0x09
#define OD_INDEX_CONFIGURATIONOFDIGITALINPUT10    0x2070
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALINPUT10 0x0A

#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEA     15
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEB     14
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEC     13
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSED     12
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEE     11
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEF     10
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEG      9
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEH      8
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEI      7
#define EPOS_DIGITALINPUTCONFIG_GENERALPURPOSEJ      6
#define EPOS_DIGITALINPUTCONFIG_QUICKSTOP            5
#define EPOS_DIGITALINPUTCONFIG_DEVICEENABLE         4
#define EPOS_DIGITALINPUTCONFIG_POSITIONMARKER       3
#define EPOS_DIGITALINPUTCONFIG_HOMESWITCH           2
#define EPOS_DIGITALINPUTCONFIG_POSITIVELIMITSWITCH  1
#define EPOS_DIGITALINPUTCONFIG_NEGATIVELIMITSWITCH  0

// Digital Input Functionarities : uint16_t
#define OD_INDEX_DIGITALINPUTFUNCTIONARITIES    0x2071
#define OD_SUBINDEX_DIGITALINPUTFUNCTIONARITIES 0x01

#define EPOS_DIGITALINPUT_GENERALPURPOSEA     0x8000
#define EPOS_DIGITALINPUT_GENERALPURPOSEB     0x4000
#define EPOS_DIGITALINPUT_GENERALPURPOSEC     0x2000
#define EPOS_DIGITALINPUT_GENERALPURPOSED     0x1000
#define EPOS_DIGITALINPUT_GENERALPURPOSEE     0x0800
#define EPOS_DIGITALINPUT_GENERALPURPOSEF     0x0400
#define EPOS_DIGITALINPUT_GENERALPURPOSEG     0x0200
#define EPOS_DIGITALINPUT_GENERALPURPOSEH     0x0100
#define EPOS_DIGITALINPUT_GENERALPURPOSEI     0x0080
#define EPOS_DIGITALINPUT_GENERALPURPOSEJ     0x0040
#define EPOS_DIGITALINPUT_QUICKSTOP           0x0020
#define EPOS_DIGITALINPUT_DEVICEENABLE        0x0010
#define EPOS_DIGITALINPUT_POSITIONMARKER      0x0008
#define EPOS_DIGITALINPUT_HOMESWITCH          0x0004
#define EPOS_DIGITALINPUT_POSITIVELIMITSWITCH 0x0002
#define EPOS_DIGITALINPUT_NEGATIVELIMITSWITCH 0x0001

// Digital Input Functionarities Mask : uint16_t
#define OD_INDEX_DIGITALINPUTFUNCTIONARITIESMASK    0x2071
#define OD_SUBINDEX_DIGITALINPUTFUNCTIONARITIESMASK 0x02

// Digital Input Functionarities Polarity : uint16_t
// If each flag is '1', low active.
#define OD_INDEX_DIGITALINPUTFUNCTIONARITIESPOLARITY    0x2071
#define OD_SUBINDEX_DIGITALINPUTFUNCTIONARITIESPOLARITY 0x03

// Digital Input Functionarities Mask : uint16_t
#define OD_INDEX_DIGITALINPUTFUNCTIONARITIESEXECUTIONMASK    0x2071
#define OD_SUBINDEX_DIGITALINPUTFUNCTIONARITIESEXECUTIONMASK 0x04

// Position Marker : uint32_t
#define OD_INDEX_POSITIONMARKER    0x2074
#define OD_SUBINDEX_POSITIONMARKER 0x01

// Position Marker Edge Type : uint8_t
#define OD_INDEX_POSITIONMARKEREDGETYPE    0x2074
#define OD_SUBINDEX_POSITIONMARKEREDGETYPE 0x02

#define EPOS_POSITIONMARKEREDGE_BOTH    0
#define EPOS_POSITIONMARKEREDGE_RISING  1
#define EPOS_POSITIONMARKEREDGE_FALLING 2

// Position Marker Mode : uint8_t
#define OD_INDEX_POSITIONMARKERMODE    0x2074
#define OD_SUBINDEX_POSITIONMARKERMODE 0x03

#define EPOS_POSITIONMARKERMODE_CONTINUOUS   0
#define EPOS_POSITIONMARKERMODE_SINGLE       1
#define EPOS_POSITIONMARKERMODE_MULTIPLE     2
#define EPOS_POSITIONMARKERMODE_SINGLEANDSTOP 3

// Position Marker Counter : uint16_t
#define OD_INDEX_POSITIONMARKERCOUNTER    0x2074
#define OD_SUBINDEX_POSITIONMARKERCOUNTER 0x04

// Position Marker History : uint32_t
#define OD_INDEX_POSITIONMARKERHISTORY1    0x2074
#define OD_SUBINDEX_POSITIONMARKERHISTORY1 0x05
#define OD_INDEX_POSITIONMARKERHISTORY2    0x2074
#define OD_SUBINDEX_POSITIONMARKERHISTORY2 0x06

// Digital Output Functionarities State : uint16_t
#define OD_INDEX_DIGITALOUTPUTFUNCTIONARITIESSTATE 0x2078
#define OD_SUBINDEX_DIGITALOUTPUTFUNCTIONARITIESSTATE 0x01

#define EPOS_DIGITALOUTPUT_GENERALPURPOSEA     0x8000
#define EPOS_DIGITALOUTPUT_GENERALPURPOSEB     0x4000
#define EPOS_DIGITALOUTPUT_GENERALPURPOSEC     0x2000
#define EPOS_DIGITALOUTPUT_GENERALPURPOSED     0x1000
#define EPOS_DIGITALOUTPUT_GENERALPURPOSEE     0x0800
#define EPOS_DIGITALOUTPUT_GENERALPURPOSEF     0x0400
#define EPOS_DIGITALOUTPUT_GENERALPURPOSEG     0x0200
#define EPOS_DIGITALOUTPUT_GENERALPURPOSEH     0x0100
#define EPOS_DIGITALOUTPUT_HOLDINGBRAKE        0x0004
#define EPOS_DIGITALOUTPUT_POSITIONCOMPARE     0x0002
#define EPOS_DIGITALOUTPUT_READYFAULT          0x0001

// Digital Output Functionarities Mask : uint16_t
#define OD_INDEX_DIGITALOUTPUTFUNCTIONARITIESMASK 0x2078
#define OD_SUBINDEX_DIGITALOUTPUTFUNCTIONARITIESMASK 0x02

// Digital Output Functionarities Polarity : uint16_t
// If flag is 1, inverted output will be done.
#define OD_INDEX_DIGITALOUTPUTFUNCTIONARITIESPOLARITY 0x2078
#define OD_SUBINDEX_DIGITALOUTPUTFUNCTIONARITIESPOLARITY 0x03

// Configuration of Digital Outputs : uint16_t
#define OD_INDEX_CONFIGURATIONOFDIGITALOUTPUTS1 0x2079
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALOUTPUTS1 0x01
#define OD_INDEX_CONFIGURATIONOFDIGITALOUTPUTS2 0x2079
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALOUTPUTS2 0x02
#define OD_INDEX_CONFIGURATIONOFDIGITALOUTPUTS3 0x2079
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALOUTPUTS3 0x03
#define OD_INDEX_CONFIGURATIONOFDIGITALOUTPUTS4 0x2079
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALOUTPUTS4 0x04
#define OD_INDEX_CONFIGURATIONOFDIGITALOUTPUTS5 0x2079
#define OD_SUBINDEX_CONFIGURATIONOFDIGITALOUTPUTS5 0x05

#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEA     15
#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEB     14
#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEC     13
#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSED     12
#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEE     11
//#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEF     10
//#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEG      9
//#define EPOS_DIGITALOUTPUTCONFIG_GENERALPURPOSEH      8
#define EPOS_DIGITALOUTPUTCONFIG_HOLDINGBRAKE         2
#define EPOS_DIGITALOUTPUTCONFIG_POSITIONCOMPARE      1
#define EPOS_DIGITALOUTPUTCONFIG_READYFAULT           0

// Position Compare Configuration : uint16_t
#define OD_INDEX_POSITIONCOMPARECONFIGURATION    0x207A
#define OD_SUBINDEX_POSITIONCOMPARECONFIGURATION 0x01

#define EPOS_POSITIONCOMPARE_ENABLE           0x0001
#define EPOS_POSITIONCOMPARE_DISABLE          0x0000
#define EPOS_POSITIONCOMPARE_SINGLE           0x0000
#define EPOS_POSITIONCOMPARE_SEQUENCE         0x0002
#define EPOS_POSITIONCOMPARE_NEGATIVEINTERVAL 0x0000
#define EPOS_POSITIONCOMPARE_POSITIVEINTERVAL 0x0008
#define EPOS_POSITIONCOMPARE_BOTHINTERVAL     0x0010
#define EPOS_POSITIONCOMPARE_COMPARENEGATIVE  0x0000
#define EPOS_POSITIONCOMPARE_COMPAREPOSITIVE  0x0020
#define EPOS_POSITIONCOMPARE_COMPAREBOTH      0x0040

// Position Compare Reference Position : int32_t
#define OD_INDEX_POSITIONCOMPAREREFERENCEPOSITION    0x207A
#define OD_SUBINDEX_POSITIONCOMPAREREFERENCEPOSITION 0x02

// Position Compare Interval Width : int32_t
#define OD_INDEX_POSITIONCOMPAREINTERVALWIDTH    0x207A
#define OD_SUBINDEX_POSITIONCOMPAREINTERVALWIDTH 0x03

// Position Compare Interval Repetition : uint16_t
#define OD_INDEX_POSITIONCOMPAREINTERVALREPETITION    0x207A
#define OD_SUBINDEX_POSITIONCOMPAREINTERVALREPETITION 0x04

// Position Compare Pulse Width : int32_t
#define OD_INDEX_POSITIONCOMPAREPULSEWIDTH    0x207A
#define OD_SUBINDEX_POSITIONCOMPAREPULSEWIDTH 0x05

// Configuration of Analog Input 1 : uint16_t
#define OD_INDEX_CONFIGURATIONOFANALOGINPUT1    0x207B
#define OD_SUBINDEX_CONFIGURATIONOFANALOGINPUT1 0x01

// Configuration of Analog Input 2 : uint16_t
#define OD_INDEX_CONFIGURATIONOFANALOGINPUT2    0x207B
#define OD_SUBINDEX_CONFIGURATIONOFANALOGINPUT2 0x02

// Analog Input 1 : int16_t
#define OD_INDEX_ANALOGINPUT1    0x207C
#define OD_SUBINDEX_ANALOGINPUT1 0x01

// Analog Input 2 : int16_t
#define OD_INDEX_ANALOGINPUT2    0x207C
#define OD_SUBINDEX_ANALOGINPUT2 0x02

// Analog Input Functionarities Execution Mask : uint16_t
#define OD_INDEX_ANALOGINPUTFUNCTIONARITIESEXECUTIONMASK    0x207D
#define OD_SUBINDEX_ANALOGINPUTFUNCTIONARITIESEXECUTIONMASK 0x00

#define EPOS_ANALOGINPUTFUNCTIONARITIESEXECUTIONMASK_CURRENT 0x01
#define EPOS_ANALOGINPUTFUNCTIONARITIESEXECUTIONMASK_VELOCITY 0x02
#define EPOS_ANALOGINPUTFUNCTIONARITIESEXECUTIONMASK_POSITION 0x04

// Analog Output 1 : uint16_t
#define OD_INDEX_ANALOGOUTPUT1    0x207E
#define OD_SUBINDEX_ANALOGOUTPUT1 0x00

// Current Threshold for Homing Mode : uint16_t
#define OD_INDEX_CURRENTTHRESHOLDFORHOMINGMODE 0x2080
#define OD_SUBINDEX_CURRENTTHRESHOLDFORHOMINGMODE 0x00

// Home Position : int32_t
#define OD_INDEX_HOMEPOSITION 0x2081
#define OD_SUBINDEX_HOMEPOSITION 0x00

// Home Position Displacement : int32_t
#define OD_INDEX_HOMEPOSITIONDISPLACEMENT    0x2082
#define OD_SUBINDEX_HOMEPOSITIONDISPLACEMENT 0x00

// Interpolation Data Record
#define OD_INDEX_INTERPOLATIONDATARECORD    0x20C1
#define OD_SUBINDEX_INTERPOLATIONDATARECORD 0x00

// Interpolation Buffer Status : uint16_t
#define OD_INDEX_INTERPOLATIONBUFFERSTATUS    0x20C4
#define OD_SUBINDEX_INTERPOLATIONBUFFERSTATUS 0x01

// Interpolation Buffer Underflow Warning : uint16_t
#define OD_INDEX_INTERPOLATIONBUFFERUNDERFLOWWARNING    0x20C4
#define OD_SUBINDEX_INTERPOLATIONBUFFERUNDERFLOWWARNING 0x02

// Interpolation Buffer Overflow Warning : uint16_t
#define OD_INDEX_INTERPOLATIONBUFFEROVERFLOWWARNING    0x20C4
#define OD_SUBINDEX_INTERPOLATIONBUFFEROVERFLOWWARNING 0x03

// Following Error Actual Value : int16_t
#define OD_INDEX_FOLLOWINGERRORACTUALVALUE    0x20F4
#define OD_SUBINDEX_FOLLOWINGERRORACTUALVALUE 0x00

// Brake Reaction Rising Time : uint16_t
#define OD_INDEX_BRAKEREACTIONRISINGTIME 0x2100
#define OD_SUBINDEX_BRAKEREACTIONRISINGTIME 0x01

// Brake Reaction Falling Time : uint16_t
#define OD_INDEX_BRAKEREACTIONFALLINGTIME 0x2100
#define OD_SUBINDEX_BRAKEREACTIONFALLINGTIME 0x02

// Brake TimeOut : uint16_t
#define OD_INDEX_BRAKETIMEOUT 0x2100
#define OD_SUBINDEX_BRAKETIMEOUT 0x03


// Analog Current Setpoint Scaling : int16_t
#define OD_INDEX_ANALOGCURRENTSETPOINTSCALING 0x2301
#define OD_SUBINDEX_ANALOGCURRENTSETPOINTSCALING 0x01





// Control Word : uint16_t
#define OD_INDEX_CONTROLWORD 0x6040
#define OD_SUBINDEX_CONTROLWORD 0x00

enum CONTROLWORD {
	CONTROLWORD_SWITCHON             = 0x001,
	CONTROLWORD_ENABLEVOLTAGE        = 0x002,
	CONTROLWORD_QUICKSTOP            = 0x004,
	CONTROLWORD_ENABLEOPERATION      = 0x008,
	CONTROLWORD_HOMINGOPERATIONSTART = 0x010,
	CONTROLWORD_NEWSETPOINT          = 0x010,
	CONTROLWORD_CHANGESETIMMEDIATELY = 0x020,
	CONTROLWORD_ABSREL               = 0x040,
	CONTROLWORD_FAULTRESET           = 0x080,
	CONTROLWORD_HALT                 = 0x100,
};

// Status Word : uint16_t
#define OD_INDEX_STATUSWORD 0x6041
#define OD_SUBINDEX_STATUSWORD 0x00

enum STATUSWORD {
	STATUSWORD_OPERATIONENABLE = 0x0004,
	STATUSWORD_FAULT           = 0x0008,
	STATUSWORD_VOLTAGEENABLED  = 0x0010,
	STATUSWORD_WARNING         = 0x0080,
	STATUSWORD_HOMINGERROR     = 0x2000,
	STATUSWORD_HOMINGATTAINED  = 0x1000,
	STATUSWORD_TARGETREACHED = 0x0400,
	STATUSWORD_POSITIONREFERENCEDTOHOMEPOSITION = 0x8000,
};

// Shutdown option code : int16_t
#define OD_INDEX_SHUTDOWNOPTIONCODE 0x605B
#define OD_SUBINDEX_SHUTDOWNOPTIONCODE 0x00

#define EPOS_SHUTDOWNOPTION_DISABLEDRIVE 0x00
#define EPOS_SHUTDOWNOPTION_SLOWDOWN     0x01

// Disable Operation option code : int16_t
#define OD_INDEX_DISABLEOPERATIONOPTIONCODE 0x605C
#define OD_SUBINDEX_DISABLEOPERATIONOPTIONCODE 0x00

#define EPOS_DISABLEOPERATIONOPTION_DISABLEDRIVE 0x00
#define EPOS_DISABLEOPERATIONOPTION_SLOWDOWN     0x01

// Fault reaction option code : int16_t
#define OD_INDEX_FAULTREACTIONOPTIONCODE 0x605E
#define OD_SUBINDEX_FAULTREACTIONOPTIONCODE 0x00

#define EPOS_FAULTREACTIONOPTION_SIGNALONLY    -1
#define EPOS_FAULTREACTIONOPTION_DISABLEDRIVE  0
#define EPOS_FAULTREACTIONOPTION_SLOWDOWN      1
#define EPOS_FAULTREACTIONOPTION_SLOWDOWNQUICK 2

// Modes Of Operation uint8
#define OD_INDEX_MODESOFOPERATION 0x6060
#define OD_SUBINDEX_MODESOFOPERATION 0x00

/**
 * @brief Object Dictionary - Modes Of Operation
 */
enum MODESOFOPERATION {
	MODESOFOPERATION_INTERPOLATEDPOSITIONMODE = 7,
	MODESOFOPERATION_HOMINGMODE = 6,
	MODESOFOPERATION_PROFILEVELOCITYMODE = 3,
	MODESOFOPERATION_PROFILEPOSITIONMODE = 1,
	MODESOFOPERATION_POSITIONMODE = -1,
	MODESOFOPERATION_VELOCITYMODE = -2,
	MODESOFOPERATION_CURRENTMODE = -3,
	MODESOFOPERATION_DIAGNOSTICMODE = -4,
	MODESOFOPERATION_MASTERENCODERMODE = -5,
	MODESOFOPERATION_STEPDIRECTIONMODE = -6,
};

// Modes of Operation Display
#define OD_INDEX_MODESOFOPERATIONDISPLAY 0x6061
#define OD_SUBINDEX_MODESOFOPERATIONDISPLAY 0x00

// Position Actual Value
#define OD_INDEX_POSITIONACTUALVALUE 0x6064
#define OD_SUBINDEX_POSITIONACTUALVALUE 0x00

#define OD_INDEX_MAXIMALFOLLOWINGERROR 0x6065
#define OD_SUBINDEX_MAXIMALFOLLOWINGERROR 0x00


// Velocity Actual Value int32_t
#define OD_INDEX_VELOCITYACTUALVALUE 0x606C
#define OD_SUBINDEX_VELOCITYACTUALVALUE 0x00


// Target Position
#define OD_INDEX_TARGETPOSITION 0x607A
#define OD_SUBINDEX_TARGETPOSITION 0x00

// Homing Offset : int32_t
#define OD_INDEX_HOMINGOFFSET 0x607C
#define OD_SUBINDEX_HOMINGOFFSET 0x00

// Minimum Position Limit : int32_t
#define OD_INDEX_MINIMUMPOSITIONLIMIT 0x607D
#define OD_SUBINDEX_MINIMUMPOSITIONLIMIT 0x01

// Maximum Position Limit : int32_t
#define OD_INDEX_MAXIMUMPOSITIONLIMIT 0x607D
#define OD_SUBINDEX_MAXIMUMPOSITIONLIMIT 0x02

// Maximum Profile Velocity : uint32_t
#define OD_INDEX_MAXIMUMPROFILEVELOCITY 0x607F
#define OD_SUBINDEX_MAXIMUMPROFILEVELOCITY 0x00

// Profile Velocity : uint32_t
#define OD_INDEX_PROFILEVELOCITY 0x6081
#define OD_SUBINDEX_PROFILEVELOCITY 0x00


// Profile Acceleration : uint32_t
#define OD_INDEX_PROFILEACCELERATION 0x6083
#define OD_SUBINDEX_PROFILEACCELERATION 0x00

// Profile Deceleration : uint32_t
#define OD_INDEX_PROFILEDECELERATION 0x6084
#define OD_SUBINDEX_PROFILEDECELERATION 0x00

enum FACTORGROUP_DIMENSION {
	REV_PER_SEC = 0xA3,
	REV_PER_MIN = 0xA4,
	STEPS = 0xAC,
	STEPS_PER_REV = 0xAD,
};

enum FACTORGROUP_NOTATION {
	MEGA = 6,
	KILO = 3,
	HECTO = 2,
	DECA = 1,
	NONE = 0,
	DECI = -1,
	CENTI = -2,
	MILLI = -3,
	MICRO = -6,
};

#define OD_INDEX_VELOCITYNOTATIONINDEX 0x608B
#define OD_SUBINDEX_VELOCITYNOTATIONINDEX 0x00
#define OD_INDEX_VELOCITYDIMENSIONINDEX 0x608C
#define OD_SUBINDEX_VELOCITYDIMENSIONINDEX 0x00


// Target Velocity : int32_t
#define OD_INDEX_TARGETVELOCITY 0x60FF
#define OD_SUBINDEX_TARGETVELOCITY 0x00


// Homing Method : int8_t
#define OD_INDEX_HOMINGMETHOD 0x6098
#define OD_SUBINDEX_HOMINGMETHOD 0x00

enum HOMINGMETHOD {
	HOMINGMETHOD_ACTUALPOSITION = 35,
	HOMINGMETHOD_INDEXNEGATIVEPOSITIVESPEED = 34,
	HOMINGMETHOD_INDEXNEGATIVEPOSITIVESPEED2 = 33,
	HOMINGMETHOD_HOMESWITCHNEGATIVESPEED = 27,
	HOMINGMETHOD_HOMESWITCHPOSITIVESPEED = 23,
	HOMINGMETHOD_POSITIVELIMITSWITCH = 18,
	HOMINGMETHOD_NEGATIVELIMITSWITCH = 17,
	HOMINGMETHOD_HOMESWITCHNEGATIVESPEEDANDINDEX = 11,
	HOMINGMETHOD_HOMESWITCHPOSITIVESPEEDANDINDEX = 7,
	HOMINGMETHOD_POSITIVELIMITSWITCHANDINDEX = 2,
	HOMINGMETHOD_NEGATIVELIMITSWITCHANDINDEX = 1,
	HOMINGMETHOD_NOHOMINGMETHODREQUIRED = 0,
	HOMINGMETHOD_CURRENTTHRESHOLDPOSITIVESPEEDANDINDEX = -1,
	HOMINGMETHOD_CURRENTTHRESHOLDNEGATIVESPEEDANDINDEX = -2,
	HOMINGMETHOD_CURRENTTHRESHOLDPOSITIVESPEED = -3,
	HOMINGMETHOD_CURRENTTHRESHOLDNEGATIVESPEED = -4,
};

// Current Threshold for Homing Mode : uint16_t , default 500
//#define OD_INDEX_CURRENTTHRESHOLDFORHOMINGMODE 0x2080
//#define OD_SUBINDEX_CURRENTTHRESHOLDFORHOMINGMODE 0

// Home Offset : int32_t
#define OD_INDEX_HOMEOFFSET 0x607C
#define OD_SUBINDEX_HOMEOFFSET 0

//// Home Position : int32_t
//#define OD_INDEX_HOMEPOSITION 0x2081
//#define OD_SUBINDEX_HOMEPOSITION 0


// Homing Speed: uint32
#define OD_INDEX_HOMINGSPEEDFORSWITCHSEARCH 0x6099
#define OD_SUBINDEX_HOMINGSPEEDFORSWITCHSEARCH 0x01

// Homing Speed: uint32
#define OD_INDEX_HOMINGSPEEDFORZEROSEARCH 0x6099
#define OD_SUBINDEX_HOMINGSPEEDFORZEROSEARCH 0x02

// Homing Accel: uint32
#define OD_INDEX_HOMINGACCELERATION 0x609A
#define OD_SUBINDEX_HOMINGACCELERATION 0x00

/// OBJECTID IS CONSTRUCTED BY                                                                                                                                                                                                                
/// 16bits (OD_INDEX) 8bits (OD_SUBINDEX) 8bits (SIZE [0x08=8bit, 0x10=16bit, 0x20=32bit, 0x40=64ibt])                                                                                                                                        
#define OD_PDOMAPPINGOBJECT_HIGHRESOLUTIONTIMESTAMP 0x10130020 // High Resolution Time Stamp                                                                                                                                                  
#define OD_PDOMAPPINGOBJECT_INCREMENTALENCODER1COUNTER 0x20200010 //Incremental Encoder 1 Counter                                                                                                                                             
#define OD_PDOMAPPINGOBJECT_INCREMENTALENCODER1COUNTERATINDEXPULSE 0x20210010 // Incremental Encoder 1 Counter at Index Pulse                                                                                                                 
#define OD_PDOMAPPINGOBJECT_HALLSENSORPATTERN 0x20220010 // Hall Sensor Pattern                                                                                                                                                               
#define OD_PDOMAPPINGOBJECT_CURRENTACTUALVALUEAVERAGED 0x20270010 // Current Actual Value Averaged                                                                                                                                            
#define OD_PDOMAPPINGOBJECT_VELOCITYACTUALVALUEAVERAGED 0x20280020 // Velocity Actual Value Averaged                                                                                                                                          
#define OD_PDOMAPPINGOBJECT_CURRENTMODESETTINGVALUE 0x20300010 // Current Mode Setting Value                                                                                                                                                  
#define OD_PDOMAPPINGOBJECT_CURRENTDEMANDVALUE 0x20310020 // Current Demand Value                                                                                                                                                             
#define OD_PDOMAPPINGOBJECT_POSITIONMODESETTINGVALUE 0x20620020 // Position Mode Setting Value                                                                                                                                                
#define OD_PDOMAPPINGOBJECT_VELOCITYMODESETTINGVALUE 0x206B0020 // Velocity Mode Setting Value                                                                                                                                                
#define OD_PDOMAPPINGOBJECT_DIGITALINPUTFUNCTIONALITIES 0x20710110 // Digital Input Functionalities                                                                                                                                           
#define OD_PDOMAPPINGOBJECT_POSITIONMARKERCAPTUREDPOSITION 0x20740120 // Position Marker captured Position(Position Marker)                                                                                                                   
#define OD_PDOMAPPINGOBJECT_POSITIONMARKERCOUNTER 0x20740410 // Position Marker Counter(Position Marker)                                                                                                                                      
#define OD_PDOMAPPINGOBJECT_DIGITALOUTPUTFUNCTIONALITIES 0x20780110 // Digital Output Functionalities                                                                                                                                         
#define OD_PDOMAPPINGOBJECT_POSITIONCOMPARECONFIGURATION 0x207A0110 // Position Compare Configuration(Position Compare)                                                                                                                       
#define OD_PDOMAPPINGOBJECT_POSITIONCOMPAREREFERENCEPOSITION 0x207A0220 // Position Compare Reference Position(Position Compare)                                                                                                              
#define OD_PDOMAPPINGOBJECT_ANALOGINPUT1 0x207C0110 // Analog Input 1 (Analog Inputs)                                                                                                                                                         
#define OD_PDOMAPPINGOBJECT_ANALOGINPUT2 0x207C0210 // Analog Input 2 (Analog Inputs)                                                                                                                                                         
#define OD_PDOMAPPINGOBJECT_ANALOGOUTPUT1 0x207E0010 // Analog Output 1                                                                                                                                                                       
#define OD_PDOMAPPINGOBJECT_CURRENTTHRESHOLDFORHOMINGMODE 0x20800010 // Current Threshold for Homing Mode                                                                                                                                     
#define OD_PDOMAPPINGOBJECT_HOMEPOSITION 0x20810020 // Home Position                                                                                                                                                                          
#define OD_PDOMAPPINGOBJECT_INTERPOLATIONDATARECORD 0x20C10040 // Interpolation Data Record                                                                                                                                                   
#define OD_PDOMAPPINGOBJECT_INTERPOLATIONBUFFERSTATUS 0x20C40110 // Interpolation Buffer Status(Interpolation Buffer)                                                                                                                         
#define OD_PDOMAPPINGOBJECT_FOLLOWINGERRORACTUALVALUE 0x20F40010 // Following Error Actual Value                                                                                                                                              
#define OD_PDOMAPPINGOBJECT_CONTROLWORD 0x60400010 // Controlword                                                                                                                                                                             
#define OD_PDOMAPPINGOBJECT_STATUSWORD 0x60410010 // Statusword                                                                                                                                                                               
#define OD_PDOMAPPINGOBJECT_MODESOFOPERATION 0x60600008 // Modes of Operation                                                                                                                                                                 
#define OD_PDOMAPPINGOBJECT_MODESOFOPERATIONDISPLAY 0x60610008 // Modes of Operation Display                                                                                                                                                  
#define OD_PDOMAPPINGOBJECT_POSITIONDEMANDVALUE 0x60620020 // Position Demand Value                                                                                                                                                           
#define OD_PDOMAPPINGOBJECT_POSITIONACTUALVALUE 0x60640020 // Position Actual Value                                                                                                                                                           
#define OD_PDOMAPPINGOBJECT_MAXIMALFOLLOWINGERROR 0x60650020 // Maximal Following Error                                                                                                                                                       
#define OD_PDOMAPPINGOBJECT_VELOCITYSENSORACTUALVALUE 0x60690020 // Velocity Sensor Actual Value                                                                                                                                              
#define OD_PDOMAPPINGOBJECT_VELOCITYDEMANDVALUE 0x606B0020 // Velocity Demand Value    
#define OD_PDOMAPPINGOBJECT_VELOCITYACTUALVALUE 0x606C0020 // Velocity Actual Value                                                                                                                                                           
#define OD_PDOMAPPINGOBJECT_CURRENTACTUALVALUE 0x60780010 // Current Actual Value                                                                                                                                                             
#define OD_PDOMAPPINGOBJECT_TARGETPOSITION 0x607A0020 // Target Position                                                                                                                                                                      
#define OD_PDOMAPPINGOBJECT_HOMEOFFSET 0x607C0020 // Home Offset                                                                                                                                                                              
#define OD_PDOMAPPINGOBJECT_PROFILEVELOCITY 0x60810020 // Profile Velocity                                                                                                                                                                    
#define OD_PDOMAPPINGOBJECT_PROFILEACCELERATION 0x60830020 // Profile Acceleration                                                                                                                                                            
#define OD_PDOMAPPINGOBJECT_PROFILEDECELERATION 0x60840020 // Profile Deceleration                                                                                                                                                            
#define OD_PDOMAPPINGOBJECT_QUICKSTOPDECELERATION 0x60850020 // Quickstop Deceleration                                                                                                                                                        
#define OD_PDOMAPPINGOBJECT_MOTIONPROFILETYPE 0x60860010 // Motion Profile Type                                                                                                                                                               
#define OD_PDOMAPPINGOBJECT_HOMINGMETHOD 0x60980008 // Homing Method                                                                                                                                                                          
#define OD_PDOMAPPINGOBJECT_SPEEDSFORSWITCHSEARCH 0x60990120 // Speeds for Switch Search(Homing Speeds)      

#endif // #ifndef EPOS_OBJECT_DICTIONARY_DEFINES_HEADER_INCLUDED
