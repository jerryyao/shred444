/**************************************************************************************************
  Filename:       devinfoservice.c
  Revised:        $Date $
  Revision:       $Revision $

  Description:    This file contains the Device Information service.

**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"

#include "devinfoservice.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Device information service
CONST uint8 devInfoServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_SERV_UUID), HI_UINT16(DEVINFO_SERV_UUID)
};

// System ID
CONST uint8 devInfoSystemIdUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_SYSTEM_ID_UUID), HI_UINT16(DEVINFO_SYSTEM_ID_UUID)
};

// Model Number String
CONST uint8 devInfoModelNumberUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_MODEL_NUMBER_UUID), HI_UINT16(DEVINFO_MODEL_NUMBER_UUID)
};

// Serial Number String
CONST uint8 devInfoSerialNumberUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_SERIAL_NUMBER_UUID), HI_UINT16(DEVINFO_SERIAL_NUMBER_UUID)
};

// Firmware Revision String
CONST uint8 devInfoFirmwareRevUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_FIRMWARE_REV_UUID), HI_UINT16(DEVINFO_FIRMWARE_REV_UUID)
};

// Hardware Revision String
CONST uint8 devInfoHardwareRevUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_HARDWARE_REV_UUID), HI_UINT16(DEVINFO_HARDWARE_REV_UUID)
};

// Software Revision String
CONST uint8 devInfoSoftwareRevUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_SOFTWARE_REV_UUID), HI_UINT16(DEVINFO_SOFTWARE_REV_UUID)
};

// Manufacturer Name String
CONST uint8 devInfoMfrNameUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_MANUFACTURER_NAME_UUID), HI_UINT16(DEVINFO_MANUFACTURER_NAME_UUID)
};

// IEEE 11073-20601 Regulatory Certification Data List
CONST uint8 devInfo11073CertUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(DEVINFO_11073_CERT_DATA_UUID), HI_UINT16(DEVINFO_11073_CERT_DATA_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * Profile Attributes - variables
 */

// Device Information Service attribute
static CONST gattAttrType_t devInfoService = { ATT_BT_UUID_SIZE, devInfoServUUID };

// System ID characteristic
static uint8 devInfoSystemIdProps = GATT_PROP_READ;
static uint8 devInfoSystemId[DEVINFO_SYSTEM_ID_LEN] = {0, 0, 0, 0, 0, 0, 0, 0};

// Model Number String characteristic
static uint8 devInfoModelNumberProps = GATT_PROP_READ;
static const uint8 devInfoModelNumber[] = "Model Number";

// Serial Number String characteristic
static uint8 devInfoSerialNumberProps = GATT_PROP_READ;
static const uint8 devInfoSerialNumber[] = "Serial Number";

// Firmware Revision String characteristic
static uint8 devInfoFirmwareRevProps = GATT_PROP_READ;
static const uint8 devInfoFirmwareRev[] = "Firmware Revision";

// Hardware Revision String characteristic
static uint8 devInfoHardwareRevProps = GATT_PROP_READ;
static const uint8 devInfoHardwareRev[] = "Hardware Revision";

// Software Revision String characteristic
static uint8 devInfoSoftwareRevProps = GATT_PROP_READ;
static const uint8 devInfoSoftwareRev[] = "Software Revision";

// Manufacturer Name String characteristic
static uint8 devInfoMfrNameProps = GATT_PROP_READ;
static const uint8 devInfoMfrName[] = "Manufacturer Name";

// IEEE 11073-20601 Regulatory Certification Data List characteristic
static uint8 devInfo11073CertProps = GATT_PROP_READ;
static const uint8 devInfo11073Cert[] = 
{
  DEVINFO_11073_BODY_EXP,      // authoritative body type
  0x00,                       // authoritative body structure type
                              // authoritative body data follows below:
  'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l'
};

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t devInfoAttrTbl[] = 
{
  // Device Information Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&devInfoService                /* pValue */
  },

    // System ID Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoSystemIdProps 
    },

      // System ID Value
      { 
        { ATT_BT_UUID_SIZE, devInfoSystemIdUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoSystemId 
      },

    // Model Number String Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoModelNumberProps 
    },

      // Model Number Value
      { 
        { ATT_BT_UUID_SIZE, devInfoModelNumberUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoModelNumber 
      },

    // Serial Number String Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoSerialNumberProps 
    },

      // Serial Number Value
      { 
        { ATT_BT_UUID_SIZE, devInfoSerialNumberUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoSerialNumber
      },

    // Firmware Revision String Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoFirmwareRevProps 
    },

      // Firmware Revision Value
      { 
        { ATT_BT_UUID_SIZE, devInfoFirmwareRevUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoFirmwareRev
      },

    // Hardware Revision String Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoHardwareRevProps 
    },

      // Hardware Revision Value
      { 
        { ATT_BT_UUID_SIZE, devInfoHardwareRevUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoHardwareRev
      },

    // Software Revision String Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoSoftwareRevProps 
    },

      // Software Revision Value
      { 
        { ATT_BT_UUID_SIZE, devInfoSoftwareRevUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoSoftwareRev
      },

    // Manufacturer Name String Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfoMfrNameProps 
    },

      // Manufacturer Name Value
      { 
        { ATT_BT_UUID_SIZE, devInfoMfrNameUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfoMfrName 
      },

    // IEEE 11073-20601 Regulatory Certification Data List Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &devInfo11073CertProps 
    },

      // IEEE 11073-20601 Regulatory Certification Data List Value
      { 
        { ATT_BT_UUID_SIZE, devInfo11073CertUUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *) devInfo11073Cert
      }
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 devInfo_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      DevInfo_AddService
 *
 * @brief   Initializes the Device Information service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t DevInfo_AddService( void )
{
  // Register GATT attribute list and CBs with GATT Server App
  return GATTServApp_RegisterService( devInfoAttrTbl, GATT_NUM_ATTRS( devInfoAttrTbl ),
                                        devInfo_ReadAttrCB, NULL, NULL );
}

/*********************************************************************
 * @fn      DevInfo_SetParameter
 *
 * @brief   Set a Device Information parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t DevInfo_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
     case DEVINFO_SYSTEM_ID:
      memcpy(devInfoSystemId, value, len);
      break;      

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      DevInfo_GetParameter
 *
 * @brief   Get a Device Information parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to get.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t DevInfo_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case DEVINFO_SYSTEM_ID:      
      memcpy(value, devInfoSystemId, sizeof(devInfoSystemId));
      break;
      
    case DEVINFO_MODEL_NUMBER:   
      memcpy(value, devInfoModelNumber, sizeof(devInfoModelNumber));
      break;
    case DEVINFO_SERIAL_NUMBER:  
      memcpy(value, devInfoSystemId, sizeof(devInfoSystemId));
      break;
      
    case DEVINFO_FIRMWARE_REV:   
      memcpy(value, devInfoFirmwareRev, sizeof(devInfoFirmwareRev));
      break;
      
    case DEVINFO_HARDWARE_REV:   
      memcpy(value, devInfoHardwareRev, sizeof(devInfoHardwareRev));
      break;
      
    case DEVINFO_SOFTWARE_REV:   
      memcpy(value, devInfoSoftwareRev, sizeof(devInfoSoftwareRev));
      break;
      
    case DEVINFO_MANUFACTURER_NAME:
      memcpy(value, devInfoMfrName, sizeof(devInfoMfrName));
      break;
      
    case DEVINFO_11073_CERT_DATA:
      memcpy(value, devInfo11073Cert, sizeof(devInfo11073Cert));
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          devInfo_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 devInfo_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

 
  uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);

  switch (uuid)
  {
    case DEVINFO_SYSTEM_ID_UUID:            
      // verify offset
      if (offset >= sizeof(devInfoSystemId))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoSystemId) - offset));
        
        // copy data
        memcpy(pValue, &devInfoSystemId[offset], *pLen);
      }
      break;
      
    case DEVINFO_MODEL_NUMBER_UUID:         
      // verify offset
      if (offset >= sizeof(devInfoModelNumber))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoModelNumber) - offset));
        
        // copy data
        memcpy(pValue, &devInfoModelNumber[offset], *pLen);
      }
      break;

    case DEVINFO_SERIAL_NUMBER_UUID:        
      // verify offset
      if (offset >= sizeof(devInfoSerialNumber))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoSerialNumber) - offset));
        
        // copy data
        memcpy(pValue, &devInfoSerialNumber[offset], *pLen);
      }
      break;

    case DEVINFO_FIRMWARE_REV_UUID:         
      // verify offset
      if (offset >= sizeof(devInfoFirmwareRev))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoFirmwareRev) - offset));
        
        // copy data
        memcpy(pValue, &devInfoFirmwareRev[offset], *pLen);
      }
      break;

    case DEVINFO_HARDWARE_REV_UUID:         
      // verify offset
      if (offset >= sizeof(devInfoHardwareRev))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoHardwareRev) - offset));
        
        // copy data
        memcpy(pValue, &devInfoHardwareRev[offset], *pLen);
      }
      break;

    case DEVINFO_SOFTWARE_REV_UUID:         
      // verify offset
      if (offset >= sizeof(devInfoSoftwareRev))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoSoftwareRev) - offset));
        
        // copy data
        memcpy(pValue, &devInfoSoftwareRev[offset], *pLen);
      }
      break;

    case DEVINFO_MANUFACTURER_NAME_UUID:    
      // verify offset
      if (offset >= sizeof(devInfoMfrName))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfoMfrName) - offset));
        
        // copy data
        memcpy(pValue, &devInfoMfrName[offset], *pLen);
      }
      break;

    case DEVINFO_11073_CERT_DATA_UUID:      
      // verify offset
      if (offset >= sizeof(devInfo11073Cert))
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        // determine read length
        *pLen = MIN(maxLen, (sizeof(devInfo11073Cert) - offset));
        
        // copy data
        memcpy(pValue, &devInfo11073Cert[offset], *pLen);
      }
      break;

    default:
      break;
  }
  
  return ( status );
}


/*********************************************************************
*********************************************************************/
