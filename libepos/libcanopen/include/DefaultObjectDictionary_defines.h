/**
 * @file DefaultObjectDictionary_defines.h
 * @copyright RT Corp. 2012 All rights reserved.
 */

#ifndef DEFAULT_OBJECT_DICTIONARY_DEFINES_HEADER_INCLUDED
#define DEFAULT_OBJECT_DICTIONARY_DEFINES_HEADER_INCLUDED

// Default Common Object Dictionary 

// Device Type : uint32_t
#define OD_INDEX_DEVICETYPE 0x1000
#define OD_SUBINDEX_DEVICETYPE 0x00

// Error Register : uint8_t
#define OD_INDEX_ERRORREGISTER 0x1001
#define OD_SUBINDEX_ERRORREGISTER 0x00

// Error History : uint8_t 
#define OD_INDEX_NUMBEROFERRORS 0x1003
#define OD_SUBINDEX_NUMBEROFERRORS 0x00


#define OD_INDEX_ERRORHISTORY 0x1003

// COB-ID Sync : uint32_t
#define OD_INDEX_COBIDSYNC 0x1005
#define OD_SUBINDEX_COBIDSYNC 0x00

// Manufacturer Device Type : visible string
#define OD_INDEX_MANUFACTURERDEVICETYPE 0x1008

// Guard Time : uint16_t
#define OD_INDEX_GUARDTIME 0x100C
#define OD_SUBINDEX_GUARDTIME 0x0000

// Store Parameters : uint32_t
#define OD_INDEX_SAVEALLPARAMETERS 0x1010
#define OD_SUBINDEX_SAVEALLPARAMETERS 0x01

// Restore Default Parameters : uint32_t 
#define OD_INDEX_RESTOREDEFAULTPARAMETERS 0x1011
#define OD_SUBINDEX_RESTOREDEFAULTPARAMETERS 0x01

// Restore Default PDO COB-ID : uint32_t
#define OD_INDEX_RESTOREDEFAULTPDOCOBID 0x1011
#define OD_SUBINDEX_RESTOREDEFAULTPDOCOBID 0x05

// COBID Time Stamp Object : uint32_t
#define OD_INDEX_COBIDTIMESTAMPOBJECT 0x1012
#define OD_SUBINDEX_COBIDTIMESTAMPOBJECT 0x00

// High Resolution Time Stamp : uint32_t
#define OD_INDEX_HIGHRESOLUTIONTIMESTAMP 0x1013
#define OD_SUBINDEX_HIGHRESOLUTIONTIMESTAMP 0x00

// COBID EMCY : uint32_t
#define OD_INDEX_COBIDEMCY 0x1014
#define OD_SUBINDEX_COBIDEMCY 0x00

// Consumer Heart Beat Time : uint32_t
#define OD_INDEX_CONSUMERHEARTBEATTIME 0x1016
#define OD_INDEX_CONSUMER1HEARTBEATTIME 0x1016
#define OD_SUBINDEX_CONSUMER1HEARTBEATTIME 0x01
#define OD_INDEX_CONSUMER2HEARTBEATTIME 0x1016
#define OD_SUBINDEX_CONSUMER2HEARTBEATTIME 0x02

// Producer Heart Beat Time : uint16_t
#define OD_INDEX_PRODUCERHEARTBEATTIME 0x1017
#define OD_SUBINDEX_PRODUCERHEARTBEATTIME 0x00

// Vendor ID : uint32_t
#define OD_INDEX_VENDORID 0x1018
#define OD_SUBINDEX_VENDORID 0x01

// Product Code : uint32_t
#define OD_INDEX_PRODUCTCODE 0x1018
#define OD_SUBINDEX_PRODUCTCODE 0x02

// Revision Number : uint32_t
#define OD_INDEX_REVISIONNUMBER 0x1018
#define OD_SUBINDEX_REVISIONNUMBER 0x03

// Serial Number : uint32_t
#define OD_INDEX_SERIALNUMBER 0x1018
#define OD_SUBINDEX_SERIALNUMBER 0x04

// Configuration Date : uint32_t
#define OD_INDEX_CONFIGURATIONDATE 0x1020
#define OD_SUBINDEX_CONFIGURATIONDATE 0x01

// Configuration Time : uint32_t
#define OD_INDEX_CONFIGURATIONTIME 0x1020
#define OD_SUBINDEX_CONFIGURATIONTIME 0x02

// COB-ID SDO Client to Server : uint32_t
#define OD_INDEX_COBIDSDOCLIENTTOSERVER 0x1200
#define OD_SUBINDEX_COBIDSDOCLIENTTOSERVER 0x01

// COB-ID SDO Server to Client : uint32_t
#define OD_INDEX_COBIDSDOSERVERTOCLIENT 0x1200
#define OD_SUBINDEX_COBIDSDOSERVERTOCLIENT 0x02

// Receive COBID PDO 1 Parameter uint32_t
#define OD_INDEX_COBIDRECEIVEPDO1 0x1400
#define OD_SUBINDEX_COBIDRECEIVEPDO1 0x01

// Transmission Type Receive PDO 1 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPERECEIVEPDO1 0x1400
#define OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO1 0x02

// Receive COBID PDO 2 Parameter uint32_t
#define OD_INDEX_COBIDRECEIVEPDO2 0x1401
#define OD_SUBINDEX_COBIDRECEIVEPDO2 0x01

// Transmission Type Receive PDO 2 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPERECEIVEPDO2 0x1401
#define OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO2 0x02

// Receive PDO 3 Parameter uint32_t
#define OD_INDEX_COBIDRECEIVEPDO3 0x1402
#define OD_SUBINDEX_COBIDRECEIVEPDO3 0x01

// Transmission Type Receive PDO 3 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPERECEIVEPDO3 0x1402
#define OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO3 0x02

// Receive PDO 4 Parameter uint32_t
#define OD_INDEX_COBIDRECEIVEPDO4 0x1403
#define OD_SUBINDEX_COBIDRECEIVEPDO4 0x01

// Transmission Type Receive PDO 4 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPERECEIVEPDO4 0x1403
#define OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO4 0x02

// Receive PDO 1 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_RECEIVEPDO1MAPPINGNUMBER 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGNUMBER 0x00

// Receive PDO 1 Mapping Objects : uint32_t
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT1 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT1 0x01
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT2 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT2 0x02
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT3 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT3 0x03
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT4 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT4 0x04
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT5 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT5 0x05
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT6 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT6 0x06
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT7 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT7 0x07
#define OD_INDEX_RECEIVEPDO1MAPPINGOBJECT8 0x1600
#define OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT8 0x08

// Receive PDO 2 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_RECEIVEPDO2MAPPINGNUMBER 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGNUMBER 0x00

// Receive PDO 2 Mapping Objects : uint32_t
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT1 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT1 0x01
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT2 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT2 0x02
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT3 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT3 0x03
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT4 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT4 0x04
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT5 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT5 0x05
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT6 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT6 0x06
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT7 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT7 0x07
#define OD_INDEX_RECEIVEPDO2MAPPINGOBJECT8 0x1601
#define OD_SUBINDEX_RECEIVEPDO2MAPPINGOBJECT8 0x08


// Receive PDO 3 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_RECEIVEPDO3MAPPINGNUMBER 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGNUMBER 0x00

// Receive PDO 3 Mapping Objects : uint32_t
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT1 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT1 0x01
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT2 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT2 0x02
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT3 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT3 0x03
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT4 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT4 0x04
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT5 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT5 0x05
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT6 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT6 0x06
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT7 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT7 0x07
#define OD_INDEX_RECEIVEPDO3MAPPINGOBJECT8 0x1602
#define OD_SUBINDEX_RECEIVEPDO3MAPPINGOBJECT8 0x08

// Receive PDO 4 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_RECEIVEPDO4MAPPINGNUMBER 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGNUMBER 0x00

// Receive PDO 4 Mapping Objects : uint32_t
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT1 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT1 0x01
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT2 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT2 0x02
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT3 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT3 0x03
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT4 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT4 0x04
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT5 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT5 0x05
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT6 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT6 0x06
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT7 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT7 0x07
#define OD_INDEX_RECEIVEPDO4MAPPINGOBJECT8 0x1603
#define OD_SUBINDEX_RECEIVEPDO4MAPPINGOBJECT8 0x08



// Transmit COBID PDO1 Parameter uint32_t
#define OD_INDEX_COBIDTRANSMITPDO1 0x1800
#define OD_SUBINDEX_COBIDTRANSMITPDO1 0x01

// Transmission Type Transmit PDO1 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPETRANSMITPDO1 0x1800
#define OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO1 0x02

// Inhibit Time Transmit PDO1 : uint16_t
#define OD_INDEX_INHIBITTIMETRANSMITPDO1 0x1800
#define OD_SUBINDEX_INHIBITTIMETRANSMITPDO1 0x03

// Transmit COBID PDO2 Parameter uint32_t
#define OD_INDEX_COBIDTRANSMITPDO2 0x1801
#define OD_SUBINDEX_COBIDTRANSMITPDO2 0x01

// Transmission Type Transmit PDO2 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPETRANSMITPDO2 0x1801
#define OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO2 0x02

// Inhibit Time Transmit PDO2 : uint16_t
#define OD_INDEX_INHIBITTIMETRANSMITPDO2 0x1801
#define OD_SUBINDEX_INHIBITTIMETRANSMITPDO2 0x03

// Transmit COBID PDO3 Parameter uint32_t
#define OD_INDEX_COBIDTRANSMITPDO3 0x1802
#define OD_SUBINDEX_COBIDTRANSMITPDO3 0x01

// Transmission Type Transmit PDO3 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPETRANSMITPDO3 0x1802
#define OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO3 0x02

// Inhibit Time Transmit PDO3 : uint16_t
#define OD_INDEX_INHIBITTIMETRANSMITPDO3 0x1802
#define OD_SUBINDEX_INHIBITTIMETRANSMITPDO3 0x03

// Transmit COBID PDO4 Parameter uint32_t
#define OD_INDEX_COBIDTRANSMITPDO4 0x1803
#define OD_SUBINDEX_COBIDTRANSMITPDO4 0x01

// Transmission Type Transmit PDO4 : uint8_t
#define OD_INDEX_TRANSMISSIONTYPETRANSMITPDO4 0x1803
#define OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO4 0x02

// Inhibit Time Transmit PDO4 : uint16_t
#define OD_INDEX_INHIBITTIMETRANSMITPDO4 0x1803
#define OD_SUBINDEX_INHIBITTIMETRANSMITPDO4 0x03

// Transmit PDO 1 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_TRANSMITPDO1MAPPINGNUMBER 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGNUMBER 0x00

// Transmit PDO 1 Mapping Objects : uint32_t
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT1 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT1 0x01
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT2 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT2 0x02
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT3 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT3 0x03
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT4 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT4 0x04
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT5 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT5 0x05
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT6 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT6 0x06
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT7 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT7 0x07
#define OD_INDEX_TRANSMITPDO1MAPPINGOBJECT8 0x1A00
#define OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT8 0x08


// Transmit PDO2 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_TRANSMITPDO2MAPPINGNUMBER 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGNUMBER 0x00

// Transmit PDO2 Mapping Objects : uint32_t
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT1 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT1 0x01
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT2 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT2 0x02
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT3 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT3 0x03
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT4 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT4 0x04
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT5 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT5 0x05
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT6 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT6 0x06
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT7 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT7 0x07
#define OD_INDEX_TRANSMITPDO2MAPPINGOBJECT8 0x1A01
#define OD_SUBINDEX_TRANSMITPDO2MAPPINGOBJECT8 0x08

// Transmit PDO3 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_TRANSMITPDO3MAPPINGNUMBER 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGNUMBER 0x00

// Transmit PDO3 Mapping Objects : uint32_t
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT1 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT1 0x01
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT2 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT2 0x02
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT3 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT3 0x03
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT4 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT4 0x04
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT5 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT5 0x05
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT6 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT6 0x06
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT7 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT7 0x07
#define OD_INDEX_TRANSMITPDO3MAPPINGOBJECT8 0x1A02
#define OD_SUBINDEX_TRANSMITPDO3MAPPINGOBJECT8 0x08

// Transmit PDO4 Mapping Number of Application Objects : uint8_t
#define OD_INDEX_TRANSMITPDO4MAPPINGNUMBER 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGNUMBER 0x00

// Transmit PDO4 Mapping Objects : uint32_t
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT1 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT1 0x01
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT2 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT2 0x02
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT3 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT3 0x03
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT4 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT4 0x04
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT5 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT5 0x05
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT6 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT6 0x06
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT7 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT7 0x07
#define OD_INDEX_TRANSMITPDO4MAPPINGOBJECT8 0x1A03
#define OD_SUBINDEX_TRANSMITPDO4MAPPINGOBJECT8 0x08



#endif // #ifndef DEFAULT_OBJECT_DICTIONARY_DEFINES_HEADER_INCLUDED
