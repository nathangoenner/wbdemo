/**
******************************************************************************
* @file    config_client.c
* @author  BLE Mesh Team
* @version V1.11.002
* @date    27-09-2019
* @brief   Config model Client middleware file
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Initial BLE-Mesh is built over Motorola�s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "config_client.h"
#include "common.h"
#include "models_if.h"
#include <string.h>
#include "compiler.h"
#include "appli_config_client.h"
#include "ble_mesh.h"
#include "appli_mesh.h"

/** @addtogroup MODEL_CONFIG
*  @{
*/

/** @addtogroup Config_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* ALIGN(4) */
__attribute__((aligned(4))) Composition_Data_Page0_t NodeCompositionPage0; /* Storage of the Node Page0 */

/* ALIGN(4) */
__attribute__((aligned(4)))Elements_Page0_t aNodeElements[MAX_ELEMENTS_PER_NODE];

/* ALIGN(4)*/
__attribute__((aligned(4)))NodeInfo_t NodeInfo;

const MODEL_OpcodeTableParam_t Config_Client_Opcodes_Table[] = {
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_CONFIG_MODEL_CLIENT
  {OPCODE_CONFIG_COMPOSITION_DATA_STATUS,  MOBLE_FALSE,  10, 100, 0x8FFF , 0, 0},
  {OPCODE_CONFIG_APPKEY_STATUS,            MOBLE_FALSE,  4, 4, 0x8FFF , 0, 0},
  {OPCODE_CONFIG_SUBSCRIPTION_STATUS,      MOBLE_FALSE,  7, 9, 0x8FFF , 0, 0},
  {OPCODE_CONFIG_MODEL_PUBLI_STATUS,       MOBLE_FALSE,  12, 14, 0x8FFF , 0, 0},
  {OPCODE_CONFIG_MODEL_APP_STATUS,         MOBLE_FALSE,  7, 9, 0x8FFF , 0, 0},
#endif
  {0}
};

/* Private function prototypes -----------------------------------------------*/
void PackNetkeyAppkeyInto3Bytes (MOBLEUINT16 netKeyIndex, 
                                 MOBLEUINT16 appKeyIndex,
                                 MOBLEUINT8* keysArray3B);
void NetkeyAppkeyUnpack (MOBLEUINT16 *pnetKeyIndex, 
                                MOBLEUINT16 *pappKeyIndex,
                                MOBLEUINT8* keysArray3B);
MOBLE_RESULT ConfigClient_AppKeyAdd (MOBLEUINT16 netKeyIndex, 
                                     MOBLEUINT16 appKeyIndex, 
                                     MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyStatus(MOBLEUINT8 const *pSrcAppKeyStatus, 
                                                        MOBLEUINT32 length); 
MOBLE_RESULT ConfigClient_AppKeyUpdate (MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyDelete (MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyGet (MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyList (MOBLEUINT8* appkey);
MOBLE_RESULT _ConfigClient_SubscriptionAdd (configClientModelSubscriptionAdd_t *modelSubscription);
MOBLE_RESULT ConfigClient_SubscriptionDelete (void);
MOBLE_RESULT ConfigClient_SubscriptionDeleteAll (void);
MOBLE_RESULT ConfigClient_SubscriptionOverwrite (void);
MOBLE_RESULT ConfigClient_SubscriptionGet (void);
MOBLE_RESULT ConfigClient_SubscriptionList (void);
MOBLE_RESULT ConfigClient_ModelAppUnbind (void);
MOBLEUINT16 CopyU8LittleEndienArrayToU16word (MOBLEUINT8* pArray);
MOBLEUINT32 CopyU8LittleEndienArrayToU32word (MOBLEUINT8* pArray);
WEAK_FUNCTION (MOBLEUINT8* GetNewProvNodeDevKey(void));

/* Private functions ---------------------------------------------------------*/

/**
* @brief  ConfigClient_CompositionDataGet: This function is called to read the 
          composition data of the node 
* @param  None: No parameter for this function
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_CompositionDataGet(void) 
{
  
  /* 4.3.2.4 Config Composition Data Get
  The Config Composition Data Get is an acknowledged message used to read one 
  page of the Composition Data (see Section 4.2.1).
  The response to a Config Composition Data Get message is a 
  Config Composition Data Status message
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLEUINT32 dataLength;
  MOBLE_ADDRESS dst_peer;
    
  configClientGetCompositionMsg_t ccGetCompositionMsg;
  
  TRACE_M(TF_CONFIG_CLIENT,"Config CompositionDataGet Message \r\n");  
  ccGetCompositionMsg.Opcode = OPCODE_CONFIG_COMPOSITION_DATA_GET;
  ccGetCompositionMsg.page = COMPOSITION_PAGE0;

  
  if(result)
  {
    TRACE_M(TF_CONFIG_CLIENT,"Get Composition Data Error  \r\n");
  }  
  
  msg_opcode = OPCODE_CONFIG_COMPOSITION_DATA_GET;
  pConfigData = (MOBLEUINT8*) &(ccGetCompositionMsg.page);
  dataLength = sizeof(ccGetCompositionMsg.page);
  dst_peer = NodeInfo.nodePrimaryAddress; 

  ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);
  
  return result;
}


/**
* @brief  ConfigClient_CompositionDataStatusResponse: This function is a call
           back when the response is received for Composition
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_CompositionDataStatusResponse(MOBLEUINT8 const *pSrcComposition, 
                                                        MOBLEUINT32 length)  
{
  MOBLEUINT8 *pSrcElements;
  MOBLEUINT8 elementIndex;
  MOBLEUINT8 numNodeSIGmodels;
  MOBLEUINT8 numNodeVendormodels;
  MOBLEUINT8 varModels;
  MOBLEUINT8 indexModels;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
       
  TRACE_M(TF_CONFIG_CLIENT,"Composition Status Cb \r\n");  

  /* Copy the header of the Composition page */ 
  NodeCompositionPage0.sComposition_Data_Page0.sheader.DataPage = *pSrcComposition; 
  NodeCompositionPage0.sComposition_Data_Page0.sheader.NodeCID = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+1));
  NodeCompositionPage0.sComposition_Data_Page0.sheader.NodePID = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+3));
  NodeCompositionPage0.sComposition_Data_Page0.sheader.NodeVID = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+5));
  NodeCompositionPage0.sComposition_Data_Page0.sheader.NodeCRPL = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+7));
  NodeCompositionPage0.sComposition_Data_Page0.sheader.NodeFeatures = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+9));
  
  /* Point to the Start of Elements data from source  */
  /* Point after the Header and Loc , NumS, NumV */
  pSrcElements = (MOBLEUINT8*)(pSrcComposition+11);
  
  for (elementIndex =0; elementIndex < MAX_ELEMENTS_PER_NODE; elementIndex++ )
  { 
    /* Point to the destination address in Global Variable */
    /* Copy Loc, NumSIGmodels, NumVendorModels in Composition page */
    aNodeElements[elementIndex].Loc = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)pSrcElements);
    
    pSrcElements += 2;
    aNodeElements[elementIndex].NumSIGmodels = *(pSrcElements);
    
    pSrcElements++;
    aNodeElements[elementIndex].NumVendorModels = *(pSrcElements);

    pSrcElements++;

    /******************* Copy the SIG Models **********************************/    
    /* Prepare the variables to find the number of SIG Models, SInce header is already copied,
       it can use used directly for the comparision */
    /* This is to be used for running the loop for data copy */
    numNodeSIGmodels = aNodeElements[elementIndex].NumSIGmodels;
    varModels = numNodeSIGmodels;
    
    /* Point to the Elements array for the SIG Models  */

    if (numNodeSIGmodels > MAX_SIG_MODELS_PER_ELEMENT)
    { /* Number of models of Node is more than storage capacity */
      varModels = MAX_SIG_MODELS_PER_ELEMENT;
    }
 
    for (indexModels=0; indexModels< varModels; indexModels++)
    {
      aNodeElements[elementIndex].aSIGModels[indexModels] = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)pSrcElements);
      pSrcElements +=2;  /* Increment by 2 Bytes for next Model */
    }
       
    /* If the source has more SIG Model IDs, then keep reading them till next address */
    if (numNodeSIGmodels > MAX_SIG_MODELS_PER_ELEMENT)
    {
      for (; indexModels< numNodeSIGmodels; indexModels++)
      {
        /* Increase the Source address pointer ONLY */
        pSrcElements +=2;
      }
    }
    
    /******************* Copy the Vendor Models *******************************/
    numNodeVendormodels = aNodeElements[elementIndex].NumVendorModels;
    varModels = numNodeVendormodels;
    
    /* Point to the destination address in Global Variable */
    if (numNodeVendormodels > MAX_VENDOR_MODELS_PER_ELEMENT)
    {
      varModels = MAX_VENDOR_MODELS_PER_ELEMENT;
    }

    /* Start copying the Vendor Models */
    for (indexModels=0; indexModels < varModels; indexModels++)
    {
      aNodeElements[elementIndex].aVendorModels[indexModels] = CopyU8LittleEndienArrayToU32word(pSrcElements);
      pSrcElements +=4;
    }
       
    /* If the source has more Vendor Model IDs, then keep reading them till next Element */
    if (numNodeVendormodels > MAX_VENDOR_MODELS_PER_ELEMENT)
    {
      for (; indexModels< numNodeVendormodels; indexModels++)
      {
        /* Increase the Source address pointer only */
        pSrcElements +=4;
      }
    }
      
  } /*   for (elementIndex =0; elementIndex < MAX_ELEMENTS_PER_NODE; elementIndex++ ) */
  
  
  NodeInfo.NbOfelements = elementIndex;   /* Save number of elements available in node for later use */
                                     /* Element index is already incremented by 1 after 'for' loop */ 
  Appli_CompositionDataStatusCb(result);
  return result;
  
}


/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetNodeElementAddress(void)
{
  return NodeInfo.nodePrimaryAddress; 
}

/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 ConfigClient_GetNodeElements(void)
{
  return NodeInfo.NbOfelements;  
}

/**
* @brief  GetSIGModelFromCompositionData: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetSIGModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG)
{
  MOBLEUINT16 model;

  model = aNodeElements[elementIdx].aSIGModels[idxSIG+2]; 
  return model;
}


/**
* @brief  SetSigModelsNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
void SetSIGModelCountToConfigure(MOBLEUINT8 count)
{
  MOBLEUINT8 sigModelsCount;
  
  sigModelsCount = GetTotalSIGModelsCount(0);
  if (count > sigModelsCount)
  { /* if count required by application is more than Element's SIG Models, 
       keep the low value  */
    count = sigModelsCount;
  }
 
  NodeInfo.NbOfSIGModelsToConfigure = count;
}

/**
* @brief  SetVendorModelsNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
void SetVendorModelCountToConfigure(MOBLEUINT8 count)
{
  MOBLEUINT8 vendorModelsCount;
  
  vendorModelsCount = GetTotalVendorModelsCount(0);
  if (count > vendorModelsCount)
  { /* if count required by application is more than Element's SIG Models, 
       keep the low value  */
    count = vendorModelsCount;
  }
  
  NodeInfo.NbOfVendorModelsToConfigure = count;
}

/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT32 GetVendorModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxVendor)
{
  MOBLEUINT32 model;

  model = aNodeElements[elementIdx].aVendorModels[idxVendor]; 
  /* Maybe little endien conversion may be needed */
  
  return model;
}

/**
* @brief  GetTotalSIGModelsCount: This function gets the Total number of 
           SIG Models available in the node 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetTotalSIGModelsCount(MOBLEUINT8 elementIdx)
{
  MOBLEUINT8 sigModelsCount;

  sigModelsCount = aNodeElements[elementIdx].NumSIGmodels; 
  return sigModelsCount;
}

/**
* @brief  GetTotalSIGModelsCount: This function gets the Total number of 
           SIG Models available in the node 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetTotalVendorModelsCount(MOBLEUINT8 elementIdx)
{
  return aNodeElements[elementIdx].NumVendorModels ; 
}

/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetNumberofSIGModels(MOBLEUINT8 elementIdx)
{
    return NodeInfo.NbOfSIGModelsToConfigure;
}

/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetNumberofVendorModels(MOBLEUINT8 elementIdx)
{
  return NodeInfo.NbOfVendorModelsToConfigure; 
}
  
/**
* @brief  PackNetkeyAppkeyInto3Bytes: This function is called to pack the 
          2 key Index into 3Bytes
* @param  netKeyIndex: keyIndex to be packed
* @param  appKeyIndex: keyIndex to be packed
* @retval MOBLE_RESULT
*/ 
void PackNetkeyAppkeyInto3Bytes (MOBLEUINT16 netKeyIndex, 
                                 MOBLEUINT16 appKeyIndex,
                                 MOBLEUINT8* keysArray3B)
{
  /*
  4.3.1.1 Key indexes
  Global key indexes are 12 bits long. Some messages include 
    one, two or multiple key indexes. 
      To enable efficient packing, two key indexes are packed into three octets.  
  */
  
  /* 
  To pack two key indexes into three octets... 
  8 LSbs of first key index value are packed into the first octet
    placing the remaining 4 MSbs into 4 LSbs of the second octet. 
  The first 4 LSbs of the second 12-bit key index are packed into 
  the 4 MSbs of the second octet with the remaining 8 MSbs into the third octet.
  */
  keysArray3B[0] = (MOBLEUINT8) (netKeyIndex & 0x00ff);
  keysArray3B[1] = (MOBLEUINT8) ((netKeyIndex & 0x0f00) >> 8); /* Take 4bit Nibble to 4LSb nibble */
  keysArray3B[1] |= (MOBLEUINT8) ((appKeyIndex & 0x000f) << 4); /* Take 4LSb to upper Nibble */
  keysArray3B[2] = (MOBLEUINT8) ((appKeyIndex >>4) & 0xff);  /* Take 8MSb to a byte */
  
}

/**
* @brief  PackNetkeyAppkeyInto3Bytes: This function is called to pack the 
          2 key Index into 3Bytes
* @param  netKeyIndex: keyIndex to be packed
* @param  appKeyIndex: keyIndex to be packed
* @retval MOBLE_RESULT
*/ 
void NetkeyAppkeyUnpack (MOBLEUINT16 *pnetKeyIndex, 
                                MOBLEUINT16 *pappKeyIndex,
                                MOBLEUINT8* keysArray3B)
{
  /*
  4.3.1.1 Key indexes
  Global key indexes are 12 bits long. Some messages include 
    one, two or multiple key indexes. 
      To enable efficient packing, two key indexes are packed into three octets.  
  */
  MOBLEUINT16 netKeyIndex; 
  MOBLEUINT16 appKeyIndex;
  /* 
  To pack two key indexes into three octets... 
  8 LSbs of first key index value are packed into the first octet
    placing the remaining 4 MSbs into 4 LSbs of the second octet. 
  The first 4 LSbs of the second 12-bit key index are packed into 
  the 4 MSbs of the second octet with the remaining 8 MSbs into the third octet.
  */
   netKeyIndex = keysArray3B[1] & 0x0f;  /* Take 4MSb from 2nd octet */
   netKeyIndex <<= 8;
   netKeyIndex |= keysArray3B[0];
   
   appKeyIndex = keysArray3B[1] & 0xf0;  /* Take LSb from 2nd octet */
   appKeyIndex >>= 4;
   appKeyIndex |= (keysArray3B[2] << 0x04);
   
   *pnetKeyIndex = netKeyIndex;
   *pappKeyIndex = appKeyIndex;
}


/**
* @brief  ConfigClient_AppKeyAdd: This function is called to 
          add the default AppKeys and net keys to a node under configuration
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyAdd (MOBLEUINT16 netKeyIndex, 
                                     MOBLEUINT16 appKeyIndex, 
                                     MOBLEUINT8* appkey)
{
  /*
4.3.2.37 Config AppKey Add
The Config AppKey Add is an acknowledged message used to add an AppKey to 
the AppKey List on a node and bind it to the NetKey identified by NetKeyIndex. 
The added AppKey can be used by the node only as a pair with the specified NetKey. The AppKey is used to authenticate and decrypt messages it receives, as well as authenticate and encrypt messages it sends.
The response to a Config AppKey Add message is a Config AppKey Status message.

NetKeyIndexAndAppKeyIndex: 3B : Index of the NetKey and index of the AppKey
AppKey 16B : AppKey value
  */
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  configClientAppKeyAdd_t configClientAppKeyAdd;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLEUINT32 dataLength;
  MOBLE_ADDRESS dst_peer;
  
  configClientAppKeyAdd.netKeyIndex = netKeyIndex; 
  configClientAppKeyAdd.appKeyIndex = appKeyIndex;
  memcpy (configClientAppKeyAdd.a_Appkeybuffer, appkey, APPKEY_SIZE );

  msg_opcode = OPCODE_CONFIG_APPKEY_ADD;
  pConfigData = (MOBLEUINT8*) &(configClientAppKeyAdd);
  dataLength = sizeof(configClientAppKeyAdd_t);
  dst_peer = NodeInfo.nodePrimaryAddress; 
  
  TRACE_M(TF_CONFIG_CLIENT,"Config Client App Key Add  \r\n");  
  ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);

  return result;
}

/**
* @brief  ConfigClient_AppKeyStatus: This function is a call
           back when the response is received for AppKey Add
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyStatus(MOBLEUINT8 const *pSrcAppKeyStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
  4.3.2.40 Config AppKey Status
  The Config AppKey Status is an unacknowledged message used to report a status 
  for the requesting message, based on the 
     NetKey Index identifying the NetKey on the NetKey List and on the 
     AppKey Index identifying the AppKey on the AppKey List.
  */
  
  /*
Status : 1B : Status Code for the requesting message
NetKeyIndexAndAppKeyIndex : 3B : Index of the NetKey and index of the AppKey
  */
  configClientAppKeyStatus_t configClientAppKeyStatus;
  MOBLEUINT16 netKeyIndex; 
  MOBLEUINT16 appKeyIndex;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* Config AppKey Status 0x80 0x03*/
  configClientAppKeyStatus.Status = pSrcAppKeyStatus[0];  /* Ignoring the OpCode */
  
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientAppKeyStatus.Status)
  {
    /* Status returned is an error */
  }
  
  NetkeyAppkeyUnpack (&netKeyIndex, &appKeyIndex, (MOBLEUINT8*) (pSrcAppKeyStatus+1));
  configClientAppKeyStatus.appKeyIndex = appKeyIndex;
  configClientAppKeyStatus.netKeyIndex = netKeyIndex;
  Appli_AppKeyStatusCb(configClientAppKeyStatus.Status);
  /* The Netkey and AppKey can be compared with what was issued */

  return result;
}

/**
* @brief  ConfigClient_AppKeyUpdate: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyUpdate (MOBLEUINT8* appkey)
{
  /*
4.3.2.38 Config AppKey Update
The Config AppKey Update is an acknowledged message used to update an AppKey value on the AppKey List on a node. The updated AppKey is used by the node to authenticate and decrypt messages it receives, as well as authenticate and encrypt messages it sends, as defined by the Key Refresh procedure (see Section 3.10.4).
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}


/**
* @brief  ConfigClient_AppKeyDelete: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyDelete (MOBLEUINT8* appkey)
{
/*
4.3.2.39 Config AppKey Delete
The Config AppKey Delete is an acknowledged message used to delete an AppKey from the AppKey List on a node.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}


/**
* @brief  ConfigClient_AppKeyGet: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyGet (MOBLEUINT8* appkey)
{
/*
4.3.2.41 Config AppKey Get
The AppKey Get is an acknowledged message used to report all AppKeys bound to the NetKey.
The response to a Config AppKey Get message is a Config AppKey List message.
*/
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}

/**
* @brief  ConfigClient_AppKeyList: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyList (MOBLEUINT8* appkey)
{
/*
4.3.2.42 Config AppKey List
The Config AppKey List is an unacknowledged message reporting all AppKeys that are bound to the NetKey.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}


/**
* @brief  ConfigClient_PublicationSet: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_PublicationSet (MOBLEUINT16 elementAddress,
                                          MOBLEUINT16 publishAddress,
                                          MOBLEUINT16 appKeyIndex,
                                          MOBLEUINT8 credentialFlag,
                                          MOBLEUINT8 publishTTL,
                                          MOBLEUINT8 publishPeriod,
                                          MOBLEUINT8 publishRetransmitCount,
                                          MOBLEUINT8 publishRetransmitIntervalSteps,
                                          MOBLEUINT32 modelIdentifier)
{
  /* 
4.3.2.16 Config Model Publication Set
The Config Model Publication Set is an acknowledged message used to set the Model
Publication state (see Section 4.2.2) of an outgoing message that originates 
from a model.

The response to a Config Model Publication Set message is a Config Model 
Publication Status message.
The Config Model Publication Set message uses a single octet opcode to 
maximize the size of a payload.

  ElementAddress : 16b : Address of the element
  PublishAddress : 16b : Value of the publish address
  AppKeyIndex : 12b : Index of the application key
  CredentialFlag : 1b : Value of the Friendship Credential Flag
  RFU : 3b : Reserved for Future Use
  PublishTTL : 8b : Default TTL value for the outgoing messages
  PublishPeriod : 8b : Period for periodic status publishing
  PublishRetransmitCount : 3b : Number of retransmissions for each published message
  PublishRetransmitIntervalSteps : 5b: Number of 50-millisecond steps between retransmissions
  ModelIdentifier: 16 or 32b: SIG Model ID or Vendor Model ID
  */
  
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLEUINT32 dataLength;
  MOBLE_ADDRESS dst_peer;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  configClientModelPublication_t configClientModelPublication;

  TRACE_M(TF_CONFIG_CLIENT,"Config Client Publication Add Message  \r\n");  
  dataLength = sizeof(configClientModelPublication_t);

  
  if ( (ADDRESS_IS_GROUP(elementAddress)) || (ADDRESS_IS_UNASSIGNED(elementAddress)) )
  {
    /* The ElementAddress field is the unicast address of the element, 
       all other address types are Prohibited. */
    result = MOBLE_RESULT_INVALIDARG;
  }
  else{
    
  configClientModelPublication.elementAddr = elementAddress;
  configClientModelPublication.publishAddr = publishAddress;
  configClientModelPublication.appKeyIndex = appKeyIndex;
  configClientModelPublication.credentialFlag = credentialFlag;
  configClientModelPublication.rfu = 0;
  configClientModelPublication.publishTTL=publishTTL;
  configClientModelPublication.publishPeriod=publishPeriod;
  configClientModelPublication.publishRetransmitCount=publishRetransmitCount;
  configClientModelPublication.publishRetransmitIntervalSteps=publishRetransmitIntervalSteps;
  configClientModelPublication.modelIdentifier=modelIdentifier;
  
    
    if(CHKSIGMODEL(modelIdentifier))
    {
      /* if upper 16b are 0, then it's a SIG Model */
      dataLength -= 2;
    }
  }
  

  msg_opcode = OPCODE_CONFIG_MODEL_PUBLI_SET;
  pConfigData = (MOBLEUINT8*) &(configClientModelPublication);
  dst_peer = NodeInfo.nodePrimaryAddress; 
  
  TRACE_M(TF_CONFIG_CLIENT,"Config Client Publication Add  \r\n");  
  TRACE_M(TF_CONFIG_CLIENT,"elementAddr = [%04x]\r\n", elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT,"publishAddress = [%04x]\r\n", publishAddress); 
  TRACE_M(TF_CONFIG_CLIENT,"modelIdentifier = [%08x]\r\n", modelIdentifier);
  
  TRACE_I(TF_CONFIG_CLIENT,"Publication Set buffer ");
  
  for (MOBLEUINT8 count=0 ; count<dataLength; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT,"%.2x ", pConfigData[count]);
  } 
  
  if (elementAddress == CONFIG_CLIENT_UNICAST_ADDR)
  {
    /* Provisioner needs to be configured */
    ConfigModel_SelfPublishConfig(CONFIG_CLIENT_UNICAST_ADDR,
                                         msg_opcode,pConfigData,dataLength);
  }
  else
  {
    /* Node address to be configured */
    ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);
  }

  return result;
}


/**
* @brief  ConfigClient_PublicationStatus: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_PublicationStatus(MOBLEUINT8 const *pPublicationStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
  4.3.2.18 Config Model Publication Status
  The Config Model Publication Status is an unacknowledged message used to report 
  the model Publication state (see Section 4.2.2) of an outgoing message that is 
  published by the model.
 */
  
  /*
     
  Status : 8b : Status Code for the requesting message
  ElementAddress : 16b : Address of the element
  PublishAddress : 16b : Value of the publish address
  AppKeyIndex : 12b : Index of the application key
  CredentialFlag : 1b : Value of the Friendship Credential Flag
  RFU : 3b : Reserved for Future Use
  PublishTTL : 8b : Default TTL value for the outgoing messages
  PublishPeriod : 8b : Period for periodic status publishing
  PublishRetransmitCount : 3b : Number of retransmissions for each published message
  PublishRetransmitIntervalSteps : 5b: Number of 50-millisecond steps between retransmissions
  ModelIdentifier: 16 or 32b: SIG Model ID or Vendor Model ID
  */
  configClientPublicationStatus_t configClientPublicationStatus;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  MOBLEUINT8 temp_var=0;
  MOBLEUINT16 u16temp_var=0;
  MOBLEUINT32 modelIdentifier=0;
  MOBLEUINT8* pSrc;
  
  pSrc = (MOBLEUINT8*)pPublicationStatus;
  configClientPublicationStatus.Status = *pPublicationStatus; 
  configClientPublicationStatus.elementAddr = CopyU8LittleEndienArrayToU16word(pSrc+1); 
  configClientPublicationStatus.publishAddr = CopyU8LittleEndienArrayToU16word(pSrc+3); 
  u16temp_var = CopyU8LittleEndienArrayToU16word(pSrc+5); 
  u16temp_var &= 0x0FFF;  /* Take 12b only*/
  configClientPublicationStatus.appKeyIndex = u16temp_var; 
  
  temp_var = *(pSrc+6) & 0x10;
  configClientPublicationStatus.credentialFlag = temp_var >> 4; 
  configClientPublicationStatus.publishTTL = *(pSrc+7); 
  configClientPublicationStatus.publishPeriod = *(pSrc+8); 
  configClientPublicationStatus.publishRetransmitCount = *(pSrc+9) & 0x07; 
  temp_var = *(pSrc+9) & 0xf8; 
  temp_var>>= 3;            
  configClientPublicationStatus.publishRetransmitIntervalSteps = temp_var; 

  if (length == 12 )
  {
    modelIdentifier = CopyU8LittleEndienArrayToU16word(pSrc+10);
  }
  else
  {
    modelIdentifier = CopyU8LittleEndienArrayToU32word(pSrc+10);
  }
    
  configClientPublicationStatus.modelIdentifier = modelIdentifier;   
    
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientPublicationStatus.Status)
  {
    /* Status returned is an error */
  }
  
  TRACE_M(TF_CONFIG_CLIENT,"\r\n Config Client Publication Status Recd \r\n");  
  TRACE_I(TF_CONFIG_CLIENT,"Publication Status buffer: ");
  for (MOBLEUINT8 count=0 ; count<length; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT,"%.2x ", pPublicationStatus[count]);
  }  
  TRACE_M(TF_CONFIG_CLIENT,"elementAddr = [%04x]\r\n", configClientPublicationStatus.elementAddr);  
  TRACE_M(TF_CONFIG_CLIENT,"publishAddress = [%04x]\r\n", configClientPublicationStatus.publishAddr); 
  TRACE_M(TF_CONFIG_CLIENT,"modelIdentifier = [%08x]\r\n", configClientPublicationStatus.modelIdentifier);
  TRACE_M(TF_CONFIG_CLIENT,"status = [%02x]\r\n", configClientPublicationStatus.Status);  

  Appli_PublicationStatusCb(configClientPublicationStatus.Status);

  return result;
}



/**
* @brief  ConfigClient_SubscriptionAdd: This function is called for issuing
         add subscribe message to an element(node) for Models and AppKey binding
* @param  elementAddress: Element address of node for model binding
* @param  appKeyIndex: Index of App key 
* @param  MOBLEUINT32: Model to be subscribed 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionAdd (MOBLEUINT16 elementAddress,
                                           MOBLEUINT16 address, 
                                           MOBLEUINT32 modelIdentifier)
{
/*
4.3.2.19 Config Model Subscription Add
The Config Model Subscription Add is an acknowledged message used to add an 
address to a Subscription List of a model (see Section 4.2.3).

The response to a Config Model Subscription Add message is a 
Config Model Subscription Status message.

  ElementAddress  : 2B       : Address of the element
  address         : 2B       : Value of the address
  ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID
*/

  MOBLEUINT32 dataLength;
  MOBLE_ADDRESS dst_peer;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  configClientModelSubscriptionAdd_t modelSubscription;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
   
  TRACE_M(TF_CONFIG_CLIENT,"Config Client Subscription Add Message  \r\n");  
  
  /* The ElementAddress field is the unicast address of the element, 
     all other address types are Prohibited.
     The Address field shall contain the new address to be added to the 
     Subscription List. 
     The value of the Address field shall not be an unassigned address, 
     unicast address, all-nodes address or virtual address.
   */

  if ( (ADDRESS_IS_GROUP(elementAddress)) || (ADDRESS_IS_UNASSIGNED(elementAddress)) )
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    modelSubscription.elementAddress = elementAddress;    
    modelSubscription.address = address;
    modelSubscription.modelIdentifier = modelIdentifier;
    
    /*  
         The ModelIdentifier field is either a SIG Model ID or a Vendor Model ID 
     that shall identify the model within the element.
    */
    
    if(CHKSIGMODEL(modelIdentifier))
    {
      /* if upper 16b are 0, then it's a SIG Model */
     dataLength = sizeof(configClientModelSubscriptionAdd_t) - 2;
    }
    else
    {
      dataLength = sizeof(configClientModelSubscriptionAdd_t);
    }
  } /* else: address is valid */
  
   msg_opcode = OPCODE_CONFIG_MODEL_SUBSCR_ADD;
   pConfigData = (MOBLEUINT8*) &(modelSubscription);
   dst_peer = NodeInfo.nodePrimaryAddress; 
  
  TRACE_I(TF_CONFIG_CLIENT,"Subscription Set buffer ");
  
  for (MOBLEUINT8 count=0 ; count<dataLength; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT,"%.2x ", pConfigData[count]);
  } 
   
  TRACE_M(TF_CONFIG_CLIENT,"elementAddr = [%04x]\r\n", elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT,"SubscriptionAddress = [%04x]\r\n", address); 
  TRACE_M(TF_CONFIG_CLIENT,"modelIdentifier = [%08x]\r\n", modelIdentifier);
   
  if (elementAddress == CONFIG_CLIENT_UNICAST_ADDR)
  {
    /* Provisioner needs to be configured */
    ConfigModel_SelfSubscriptionConfig(CONFIG_CLIENT_UNICAST_ADDR,
                                              msg_opcode,pConfigData,dataLength);
  }
  else
  {
    ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);  
  }
  
  return result;
  
}

/**
* @brief  ConfigClient_SubscriptionDelete: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionDelete (void)
{
/*
4.3.2.21 Config Model Subscription Delete
The Config Model Subscription Delete is an acknowledged message used to delete a subscription address from the Subscription List of a model (see Section 4.2.3).
The response to a Config Model Subscription Delete message is a Config Model Subscription Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  return result;
}

/**
* @brief  ConfigClient_SubscriptionDeleteAll: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionDeleteAll (void)
{
/*
4.3.2.25 Config Model Subscription Delete All
The Config Model Subscription Delete All is an acknowledged message used to discard the Subscription List of a model (see Section 4.2.3).
The response to a Config Model Subscription Delete All message is a Config Model Subscription Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  return result;

}

/**
* @brief  ConfigClient_SubscriptionOverwrite: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionOverwrite (void)
{
  /*
4.3.2.23 Config Model Subscription Overwrite
The Config Model Subscription Overwrite is an acknowledged message used to discard the Subscription List and add an address to the cleared Subscription List of a model (see Section 4.2.3).
The response to a Config Model Subscription Overwrite message is a Config Model Subscription Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  return result;
  
}


/**
* @brief  ConfigClient_SubscriptionStatus: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionStatus(MOBLEUINT8 const *pSrcSubscriptionStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
  4.3.2.26 Config Model Subscription Status
  The Config Model Subscription Status is an unacknowledged message used to report
  a status of the operation on the Subscription List (see Section 4.2.3).
  */
  
  /*
     Status : 1B : Status Code for the requesting message
     ElementAddress : 2B : Address of the element
     Address        : 2B : Value of the address
     ModelIdentifier : 2B or 4B: SIG Model ID or Vendor Model ID
  */
  configClientSubscriptionStatus_t configClientSubscriptionStatus;
  MOBLEUINT8 *pSrc;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 modelIdentifier=0;
    
  pSrc = (MOBLEUINT8*) pSrcSubscriptionStatus;
  configClientSubscriptionStatus.Status = *pSrc;
  configClientSubscriptionStatus.elementAddress = CopyU8LittleEndienArrayToU16word(pSrc+1); 
  configClientSubscriptionStatus.address = CopyU8LittleEndienArrayToU16word(pSrc+3); 
    
  if (length == 7 )
  {
    modelIdentifier = CopyU8LittleEndienArrayToU16word(pSrc+5);
  }
  else
  {
    modelIdentifier = CopyU8LittleEndienArrayToU32word(pSrc+5);
  }
  
  configClientSubscriptionStatus.modelIdentifier = modelIdentifier;   
    
  
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientSubscriptionStatus.Status)
  {
    /* Status returned is an error */
  }
  
  TRACE_M(TF_CONFIG_CLIENT,"ConfigClient_SubscriptionStatus  \r\n");  
  TRACE_I(TF_CONFIG_CLIENT,"SubscriptionStatus buffer ");
  
  for (MOBLEUINT8 count=0 ; count<length; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT,"%.2x ", pSrcSubscriptionStatus[count]);
  }
  
  TRACE_M(TF_CONFIG_CLIENT,"elementAddr = [%04x]\r\n", configClientSubscriptionStatus.elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT,"SubscriptionAddress = [%04x]\r\n", configClientSubscriptionStatus.address); 
  TRACE_M(TF_CONFIG_CLIENT,"modelIdentifier = [%08x]\r\n", configClientSubscriptionStatus.modelIdentifier);
  TRACE_M(TF_CONFIG_CLIENT,"subscription status = [%02x]\r\n", configClientSubscriptionStatus.Status);
  
  Appli_SubscriptionAddStatusCb(configClientSubscriptionStatus.Status);

  return result;


}


/**
* @brief  ConfigClient_SubscriptionGet: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionGet (void)
{
/*
4.3.2.27 Config SIG Model Subscription Get
The Config SIG Model Subscription Get is an acknowledged message used to get the list of subscription addresses of a model within the element. This message is only for SIG Models.
The response to a Config SIG Model Subscription Get message is a Config SIG Model Subscription List message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  return result;
}


/**
* @brief  ConfigClient_SubscriptionList: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionList (void)
{
/*
4.3.2.28 Config SIG Model Subscription List
The Config SIG Model Subscription List is an unacknowledged message used to 
report all addresses from the Subscription List of the model (see Section 4.2.3). This message is only for SIG Models.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  return result;
  
}


/**
* @brief  ConfigClient_ModelAppBind: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_ModelAppBind (MOBLEUINT16 elementAddress,
                                        MOBLEUINT16 appKeyIndex,
                                        MOBLEUINT32 modelIdentifier)
{
/*
4.3.2.46 Config Model App Bind
The Config Model App Bind is an acknowledged message used to bind an AppKey to a model.
The response to a Config Model App Bind message is a Config Model App Status message.

  ElementAddress : 2B : Address of the element
  AppKeyIndex : 2B : Index of the AppKey
  ModelIdentifier : 2 or 4: SIG Model ID or Vendor Model ID
*/

  MOBLEUINT32 dataLength;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLE_ADDRESS dst_peer;
  configClientModelAppBind_t modelAppBind;
  
  modelAppBind.appKeyIndex = appKeyIndex;
  modelAppBind.elementAddress = elementAddress; /* Will be converted to address inside lib */
  modelAppBind.modelIdentifier = modelIdentifier;
  dataLength = sizeof(configClientModelAppBind_t);

  if(CHKSIGMODEL(modelIdentifier))
  {
    /* if upper 16b are 0, then it's a SIG Model */
    dataLength -= 2;  /* reduce 2 bytes when it's SIG Model */
  }
  
  msg_opcode = OPCODE_CONFIG_MODEL_APP_BIND;
  pConfigData = (MOBLEUINT8*) &(modelAppBind);

  dst_peer = NodeInfo.nodePrimaryAddress; 
  
  TRACE_M(TF_CONFIG_CLIENT,"Config Client App Key Bind message  \r\n");   
  TRACE_M(TF_CONFIG_CLIENT,"Model = 0x%8x \r\n", modelIdentifier );
  
  if (elementAddress == CONFIG_CLIENT_UNICAST_ADDR)
  {
    /* Provisioner needs to be configured */
    ConfigClient_SelfModelAppBindConfig(CONFIG_CLIENT_UNICAST_ADDR,
                                        msg_opcode,pConfigData,dataLength);
  }
  else
  {
    /* Node address to be configured */
    ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);
  }
  
  return result;
}


/**
* @brief  ConfigClient_ModelAppUnbind: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_ModelAppUnbind (void)
{
/*
4.3.2.47 Config Model App Unbind
The Config Model App Unbind is an acknowledged message used to remove the binding between an AppKey and a model.
The response to a Config Model App Unbind message is a Config Model App Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  return result;
}



/**
* @brief  ConfigClient_ModelAppStatus: This function is a call
           back when the response is received for AppKey Add
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_ModelAppStatus(MOBLEUINT8 const *pSrcModelAppStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
    4.3.2.48 Config Model App Status
    The Config Model App Status is an unacknowledged message used to report a status
    for the requesting message, based on the element address, 
    the AppKeyIndex identifying the AppKey on the AppKey List, 
                                                       and the ModelIdentifier.
  */
  
  /*
     Status : 1B : Status Code for the requesting message
     ElementAddress : 2B : Address of the element
     AppKeyIndex : 2B : Index of the AppKey
     ModelIdentifier : 2B or 4B: SIG Model ID or Vendor Model ID
  */
  configClientModelAppStatus_t configClientModelAppStatus;
  MOBLEUINT8* pSrcAppStatus;
  MOBLEUINT32 modelIdentifier;
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  pSrcAppStatus = (MOBLEUINT8*)pSrcModelAppStatus;
  
  configClientModelAppStatus.Status = *pSrcAppStatus; 
  configClientModelAppStatus.elementAddress = CopyU8LittleEndienArrayToU16word(pSrcAppStatus+1);
  configClientModelAppStatus.appKeyIndex = CopyU8LittleEndienArrayToU16word(pSrcAppStatus+3);

  if (length == 7 )
  {
    modelIdentifier = CopyU8LittleEndienArrayToU16word(pSrcAppStatus+5);
  }
  else
  {
    modelIdentifier = CopyU8LittleEndienArrayToU32word(pSrcAppStatus+5);
  }
  
  configClientModelAppStatus.modelIdentifier = modelIdentifier;  
    
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientModelAppStatus.Status)
  {
    /* Status returned is an error */
  }
  
  TRACE_M(TF_CONFIG_CLIENT,"ConfigClient_ModelAppStatus = %d \r\n", configClientModelAppStatus.Status);    
  Appli_AppBindModelStatusCb(configClientModelAppStatus.Status);

  return result;
}


void CopyU8LittleEndienArray_fromU16word (MOBLEUINT8* pArray, MOBLEUINT16 inputWord)
{
  *(pArray+1) = (MOBLEUINT8)(inputWord & 0x00ff);  /* Copy the LSB first */
  *pArray = (MOBLEUINT8)((inputWord & 0xff00) >> 0x08); /* Copy the MSB later */
}

MOBLEUINT16 CopyU8LittleEndienArrayToU16word (MOBLEUINT8* pArray) 
{
  MOBLEUINT16 u16Word=0;
  MOBLEUINT8 lsb_byte=0;
  MOBLEUINT8 msb_byte=0;

  lsb_byte = *pArray;
  pArray++;
  msb_byte = *pArray;
  u16Word = (msb_byte<<8);
  u16Word &= 0xFF00;
  u16Word |= lsb_byte;

  return u16Word;
}

MOBLEUINT32 CopyU8LittleEndienArrayToU32word (MOBLEUINT8* pArray) 
{
  MOBLEUINT32 u32Word=0;

  u32Word = *(pArray+3); 
  u32Word <<= 8;     
  u32Word |= *(pArray+2); 
  u32Word <<= 8;     
  u32Word |= *(pArray+1); 
  u32Word <<= 8;     
  u32Word |= *pArray;
  return u32Word;
}

void CopyU8LittleEndienArray_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord)
{
  *pArray = (MOBLEUINT8)(inputWord & 0x000000ff);  /* Copy the LSB first */
  *(pArray+1) = (MOBLEUINT8)((inputWord & 0x0000ff00) >> 8); /* Copy the MSB later */
  *(pArray+2) = (MOBLEUINT8)((inputWord & 0x00ff0000) >> 16); /* Copy the MSB later */
  *(pArray+3) = (MOBLEUINT8)((inputWord & 0xff000000) >> 24); /* Copy the MSB later */
}

void CopyU8LittleEndienArray_2B_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord)
{
  *pArray = (MOBLEUINT8)(inputWord & 0x000000ff);  /* Copy the LSB first */
  *(pArray+1) = (MOBLEUINT8)((inputWord & 0x0000ff00) >> 8); /* Copy the MSB later */
}

/**
* @brief   GenericModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_SendMessage(MOBLE_ADDRESS dst_peer ,
                                     MOBLEUINT16 opcode, 
                                     MOBLEUINT8 *pData,
                                     MOBLEUINT32 dataLength)
{
  MOBLE_ADDRESS peer_addr;  
  peer_addr = 0; //inside the library, it is taken as index
  MOBLEUINT8 *pTargetDevKey;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  
    pTargetDevKey = GetNewProvNodeDevKey();
    
    ConfigModel_SendMessage(peer_addr, dst_peer, opcode, 
                            pData, dataLength, pTargetDevKey); 
  return result;
}

/**
* @brief   ApplicationGetConfigServerDeviceKey: This function is call-back 
*          from the library to Get the Device Keys from the application
* @param  MOBLE_ADDRESS src: Source address of the Config Server for which 
           device key is required
* @param  pkeyTbUse: Pointer to the Device key to be updated
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ApplicationGetConfigServerDeviceKey(MOBLE_ADDRESS src, 
                                                 const MOBLEUINT8 **ppkeyTbUse)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  *ppkeyTbUse= GetNewProvNodeDevKey();
  
  return result;  
}



/**
* @brief   ConfigClient_SaveMsgSendingTime: This function is used by application to save 
           the Initial time of message issue
* @param  None
* @retval None
*/ 
void ConfigClient_SaveMsgSendingTime (void)
{
   NodeInfo.Initial_time = Clock_Time();  /* Get the current time to see the 
                                                Timeout later */
}

/**
* @brief  ConfigClient_ChkRetrialState: This function is used by application 
          to check if there is a timeout of the Config Client Message sending
* @param  None
* @retval None
*/ 
MOBLEUINT8 ConfigClient_ChkRetrialState (eServerRespRecdState_t* peRespRecdState)
{
  MOBLEUINT8 retry_state = CLIENT_TX_INPROGRESS;
  MOBLEUINT32 nowClockTime;
  
  nowClockTime = Clock_Time();
  if(( (nowClockTime - NodeInfo.Initial_time) >= CONFIGCLIENT_RESPONSE_TIMEOUT))
  {
    /* Timeout occured, Do retry or enter the error state  */
    NodeInfo.numberOfAttemptsTx++;
    
    if (NodeInfo.numberOfAttemptsTx >= CONFIGCLIENT_MAX_TRIALS)
    {
      NodeInfo.numberOfAttemptsTx = 0;
      retry_state = CLIENT_TX_RETRY_ENDS; /* re-trial cycle ends, no response */
      *peRespRecdState = NodeNoResponse_State;
      ConfigClient_ErrorState();
    }
    else //(NodeInfo.numberOfAttemptsTx >= CONFIGCLIENT_RE_TRIALS)
    {
       retry_state = CLIENT_TX_TIMEOUT;   
       *peRespRecdState = NodeIdle_State;    /* Run next re-trial cycle again */
       TRACE_M(TF_CONFIG_CLIENT,"Retry started \n\r");       
    }
    
    ConfigClient_SaveMsgSendingTime(); /* Save the time again for next loop */
  }

  return retry_state;
}

/**
* @brief   ConfigClient_ErrorState: This function is used by application to save 
           the Initial time of message issue
* @param  None
* @retval None
*/ 
void ConfigClient_ErrorState (void)
{
   /* No Response from the Node under provisioning after trials */
  TRACE_M(TF_CONFIG_CLIENT,"No response from Node \n\r"); 
}

/**
* @brief   ConfigClient_ResetTrials: This function is used by application to 
            Reset the Number of attempts of transmissions
* @param  None
* @retval None
*/ 
void ConfigClient_ResetTrials (void)
{
  NodeInfo.numberOfAttemptsTx = 0;
}
             

/**
* @brief   ConfigClientModel_GetOpcodeTableCb: This function is call-back 
*          from the library to process the Config Client Status messages 
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Config_Client_Opcodes_Table;
  *length = sizeof(Config_Client_Opcodes_Table)/sizeof(Config_Client_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  ConfigClientModel_GetStatusRequestCb : This function is call-back 
from the library to send response to the message from peer
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response)

{
  TRACE_M(TF_CONFIG_CLIENT,"Response **Should Never enter here  \n\r");
  switch(opcode)
  {
 
  case 0:
    {     
      break;
    }

  default:
    {
      break;
    }
  }
  return MOBLE_RESULT_SUCCESS;    
}

/**
* @brief  GenericModelServer_ProcessMessageCb: This is a callback function from
the library whenever a Generic Model message is received
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  tClockTime delay_t = Clock_Time();
  
  TRACE_M(TF_CONFIG_CLIENT,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X ,response= %.2X \r\n  ",
                                                      dst_peer, peer_addr, opcode , response);

  switch(opcode)
  {
    
  case OPCODE_CONFIG_COMPOSITION_DATA_STATUS: 
    {     
      ConfigClient_CompositionDataStatusResponse(pRxData, dataLength); 
      if(result == MOBLE_RESULT_SUCCESS)
      {
        /*
        when device is working as proxy and is a part of node
        delay will be included in the toggelinf of led.
        */         
      }
      
      break;
    }
    
  case OPCODE_CONFIG_APPKEY_STATUS:
    {
      ConfigClient_AppKeyStatus(pRxData, dataLength); 
      break;
    }

  case OPCODE_CONFIG_SUBSCRIPTION_STATUS:
    {
      ConfigClient_SubscriptionStatus(pRxData, dataLength); 
        
      break;
    }
    
   case OPCODE_CONFIG_MODEL_PUBLI_STATUS:
    {
      ConfigClient_PublicationStatus(pRxData, dataLength); 
      break;
    }

   case OPCODE_CONFIG_MODEL_APP_STATUS:
    {
      ConfigClient_ModelAppStatus(pRxData, dataLength); 
      break;
    }

  default:
    {
      break;
    }          
  } /* Switch ends */
  
    
  return MOBLE_RESULT_SUCCESS;
}

WEAK_FUNCTION (MOBLEUINT8* GetNewProvNodeDevKey(void))
{
  return 0;
}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

