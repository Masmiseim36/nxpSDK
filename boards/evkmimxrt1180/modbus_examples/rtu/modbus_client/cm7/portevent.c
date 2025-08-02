/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

/* ----------------------- Variables ----------------------------------------*/
static uint32_t eQueuedEvent;
static BOOL     xEventInQueue;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBMasterPortEventInit( void )
{
    xEventInQueue = FALSE;
    return TRUE;
}

BOOL
xMBMasterPortEventPost( eMBMasterEventType eEvent )
{
    xEventInQueue = TRUE;
    eQueuedEvent |= eEvent;
    return TRUE;
}

/*
 * in os the event get should be block when no event
 * in on_on the event just get the varibale and use the poll way to check the variable always.
 */
BOOL
xMBMasterPortEventGet( eMBMasterEventType * eEvent )
{
    BOOL            xEventHappened = FALSE;

    if( xEventInQueue )
    {
      if(eQueuedEvent&EV_MASTER_READY){
        *eEvent=EV_MASTER_READY;
        eQueuedEvent&=~EV_MASTER_READY;
      }else if(eQueuedEvent&EV_MASTER_FRAME_RECEIVED){
         *eEvent=EV_MASTER_FRAME_RECEIVED;
         eQueuedEvent&=~EV_MASTER_FRAME_RECEIVED;
      }else if(eQueuedEvent&EV_MASTER_EXECUTE){
        *eEvent=EV_MASTER_EXECUTE;
        eQueuedEvent&=~EV_MASTER_EXECUTE;
      }else if(eQueuedEvent&EV_MASTER_FRAME_SENT){
        *eEvent=EV_MASTER_FRAME_SENT;
        eQueuedEvent&=~EV_MASTER_FRAME_SENT;
      }else if(eQueuedEvent&EV_MASTER_ERROR_PROCESS){
        *eEvent=EV_MASTER_ERROR_PROCESS;
        eQueuedEvent&=~EV_MASTER_ERROR_PROCESS;
      }else if(eQueuedEvent&EV_MASTER_PROCESS_SUCESS){
        *eEvent=EV_MASTER_PROCESS_SUCESS;
        eQueuedEvent&=~EV_MASTER_PROCESS_SUCESS;
      }else{
        //no need event
        xEventHappened=FALSE;
        return xEventHappened;
      }
      if(eQueuedEvent==0)
          xEventInQueue = FALSE;
      xEventHappened=TRUE; 
    }
    return xEventHappened;
}

/**
 * This function is initialize the OS resource for modbus master.
 * Note:The resource is define by OS.If you not use OS this function can be empty.
 *
 */
void vMBMasterOsResInit( void )
{
    
}

/**
 * This function is take Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be just return TRUE.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource taked result
 */
BOOL xMBMasterRunResTake( LONG lTimeOut )
{
    
    return TRUE;
}

/**
 * This function is release Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMBMasterRunResRelease( void )
{
    /* release resource */
    
}

/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(UCHAR ucDestAddress, const UCHAR* pucPDUData,
    USHORT ucPDULength) {

    xMBMasterPortEventPost(EV_MASTER_ERROR_RESPOND_TIMEOUT);
}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBReceiveData(UCHAR ucDestAddress, const UCHAR* pucPDUData,
    USHORT ucPDULength) {

    xMBMasterPortEventPost(EV_MASTER_ERROR_RECEIVE_DATA);
}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction(UCHAR ucDestAddress, const UCHAR* pucPDUData,
    USHORT ucPDULength) {

    xMBMasterPortEventPost(EV_MASTER_ERROR_EXECUTE_FUNCTION);
}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 */
void vMBMasterCBRequestScuuess( void ) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    //rt_event_send(&xMasterOsEvent, EV_MASTER_PROCESS_SUCESS);

    /* You can add your code under here. */
    xMBMasterPortEventPost(EV_MASTER_PROCESS_SUCESS);
}

/**
 * This function is wait for modbus master request finish and return result.
 * Waiting result include request process success, request respond timeout,
 * receive data error and execute function error.You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run
 * much user custom delay for waiting.
 *
 * @return request error code
 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish(void) {
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
    eMBMasterEventType event;
    while(1){
      if(xEventInQueue){
        if((eQueuedEvent&EV_MASTER_PROCESS_SUCESS)||(eQueuedEvent&EV_MASTER_ERROR_RESPOND_TIMEOUT)
           ||(eQueuedEvent&EV_MASTER_ERROR_RECEIVE_DATA)||(eQueuedEvent&EV_MASTER_ERROR_EXECUTE_FUNCTION))
        {
          //get the event
          xMBMasterPortEventGet(&event);
          break;
        }
      }
    }
    switch (event)
    {
    case EV_MASTER_PROCESS_SUCESS:
        break;
    case EV_MASTER_ERROR_RESPOND_TIMEOUT:
    {
        eErrStatus = MB_MRE_TIMEDOUT;
        break;
    }
    case EV_MASTER_ERROR_RECEIVE_DATA:
    {
        eErrStatus = MB_MRE_REV_DATA;
        break;
    }
    case EV_MASTER_ERROR_EXECUTE_FUNCTION:
    {
        eErrStatus = MB_MRE_EXE_FUN;
        break;
    }
    default:
        break;
    }
    return eErrStatus;
}


