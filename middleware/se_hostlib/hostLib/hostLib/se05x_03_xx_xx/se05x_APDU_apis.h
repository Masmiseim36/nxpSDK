/* Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef SE050X_APDU_H_INC
#define SE050X_APDU_H_INC

#include "se05x_enums.h"

/** Se05x_API_CreateSession
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] authObjectID auth [1:kSE05x_TAG_1]
 * @param[out] sessionId  [0:kSE05x_TAG_1]
 * @param[in,out] psessionIdLen Length for sessionId
 */
smStatus_t Se05x_API_CreateSession(
    pSe05xSession_t session_ctx, uint32_t authObjectID, uint8_t *sessionId, size_t *psessionIdLen);

/** Se05x_API_ExchangeSessionData
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy Check pdf [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_ExchangeSessionData(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy);

/** Se05x_API_RefreshSession
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 */
smStatus_t Se05x_API_RefreshSession(pSe05xSession_t session_ctx, pSe05xPolicy_t policy);

/** Se05x_API_CloseSession
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 */
smStatus_t Se05x_API_CloseSession(pSe05xSession_t session_ctx);

/** Se05x_API_VerifySessionUserID
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] userId userId [1:kSE05x_TAG_1]
 * @param[in] userIdLen Length of userId
 */
smStatus_t Se05x_API_VerifySessionUserID(pSe05xSession_t session_ctx, const uint8_t *userId, size_t userIdLen);

/** Se05x_API_SetLockState
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] lockIndicator lock indicator [1:kSE05x_TAG_1]
 * @param[in] lockState lock state [2:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_SetLockState(pSe05xSession_t session_ctx, uint8_t lockIndicator, uint8_t lockState);

/** Se05x_API_SetPlatformSCPRequest
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] platformSCPRequest platf scp req [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_SetPlatformSCPRequest(pSe05xSession_t session_ctx, SE05x_PlatformSCPRequest_t platformSCPRequest);

/** Se05x_API_SetAppletFeatures
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] variant variant [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_SetAppletFeatures(pSe05xSession_t session_ctx, SE05x_Variant_t variant);

/** Se05x_API_WriteECKey
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] maxAttempt maxAttempt [2:kSE05x_TAG_MAX_ATTEMPTS]
 * @param[in] objectID object id [3:kSE05x_TAG_1]
 * @param[in] curveID curveID [4:kSE05x_TAG_2]
 * @param[in] privKey privKey [5:kSE05x_TAG_3]
 * @param[in] privKeyLen Length of privKey
 * @param[in] pubKey pubKey [6:kSE05x_TAG_4]
 * @param[in] pubKeyLen Length of pubKey
 */
smStatus_t Se05x_API_WriteECKey(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    SE05x_MaxAttemps_t maxAttempt,
    uint32_t objectID,
    SE05x_ECCurve_t curveID,
    const uint8_t *privKey,
    size_t privKeyLen,
    const uint8_t *pubKey,
    size_t pubKeyLen,
    const SE05x_TransientType_t transient_type,
    const SE05x_KeyPart_t key_part);

/** Se05x_API_WriteRSAKey
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy To be Checked(last 3 not pdf) [1:kSE05x_TAG_POLICY]
 * @param[in] objectID object id [2:kSE05x_TAG_1]
 * @param[in] size size in bits [3:kSE05x_TAG_2]
 * @param[in] p p [4:kSE05x_TAG_3]
 * @param[in] pLen Length of p
 * @param[in] q q [5:kSE05x_TAG_4]
 * @param[in] qLen Length of q
 * @param[in] dp dp [6:kSE05x_TAG_5]
 * @param[in] dpLen Length of dp
 * @param[in] dq dq [7:kSE05x_TAG_6]
 * @param[in] dqLen Length of dq
 * @param[in] qInv qnv [8:kSE05x_TAG_7]
 * @param[in] qInvLen Length of qInv
 * @param[in] pubExp public exp [9:kSE05x_TAG_8]
 * @param[in] pubExpLen Length of pubExp
 * @param[in] priv priv [10:kSE05x_TAG_9]
 * @param[in] privLen Length of priv
 * @param[in] pubMod public mod [11:kSE05x_TAG_10]
 * @param[in] pubModLen Length of pubMod
 */
smStatus_t Se05x_API_WriteRSAKey(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    uint32_t objectID,
    uint16_t size,
    const uint8_t *p,
    size_t pLen,
    const uint8_t *q,
    size_t qLen,
    const uint8_t *dp,
    size_t dpLen,
    const uint8_t *dq,
    size_t dqLen,
    const uint8_t *qInv,
    size_t qInvLen,
    const uint8_t *pubExp,
    size_t pubExpLen,
    const uint8_t *priv,
    size_t privLen,
    const uint8_t *pubMod,
    size_t pubModLen,
    const SE05x_TransientType_t transient_type,
    const SE05x_KeyPart_t key_part,
    const SE05x_RSAKeyFormat_t rsa_format);

/** Se05x_API_WriteSymmKey
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] maxAttempt maxAttempt [2:kSE05x_TAG_MAX_ATTEMPTS]
 * @param[in] objectID object id [3:kSE05x_TAG_1]
 * @param[in] kekID KEK id [4:kSE05x_TAG_2]
 * @param[in] keyValue key value [5:kSE05x_TAG_3]
 * @param[in] keyValueLen Length of keyValue
 */
smStatus_t Se05x_API_WriteSymmKey(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    SE05x_MaxAttemps_t maxAttempt,
    uint32_t objectID,
    SE05x_KeyID_t kekID,
    const uint8_t *keyValue,
    size_t keyValueLen,
    const SE05x_TransientType_t transient_type,
    const SE05x_SymmKeyType_t type);

/** Se05x_API_WriteBinary
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] objectID object id [2:kSE05x_TAG_1]
 * @param[in] offset offset [3:kSE05x_TAG_2]
 * @param[in] length length [4:kSE05x_TAG_3]
 * @param[in] inputData input data [5:kSE05x_TAG_4]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_WriteBinary(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    uint32_t objectID,
    uint16_t offset,
    uint16_t length,
    const uint8_t *inputData,
    size_t inputDataLen);

/** Se05x_API_WriteUserID
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] maxAttempt maxAttempt [2:kSE05x_TAG_MAX_ATTEMPTS]
 * @param[in] objectID object id [3:kSE05x_TAG_1]
 * @param[in] userId userId [4:kSE05x_TAG_2]
 * @param[in] userIdLen Length of userId
 */
smStatus_t Se05x_API_WriteUserID(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    SE05x_MaxAttemps_t maxAttempt,
    uint32_t objectID,
    const uint8_t *userId,
    size_t userIdLen,
    const SE05x_AttestationType_t attestation_type);

/** Se05x_API_WriteCounter
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] objectID object id [2:kSE05x_TAG_1]
 * @param[in] size size [3:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_WriteCounter(pSe05xSession_t session_ctx, pSe05xPolicy_t policy, uint32_t objectID, uint16_t size);

/** Se05x_API_WritePCR
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] pcrID object id [2:kSE05x_TAG_1]
 * @param[in] initialValue initialValue [3:kSE05x_TAG_2]
 * @param[in] initialValueLen Length of initialValue
 * @param[in] inputData inputData [4:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_WritePCR(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    uint32_t pcrID,
    const uint8_t *initialValue,
    size_t initialValueLen,
    const uint8_t *inputData,
    size_t inputDataLen);

/** Se05x_API_ImportObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] rsaKeyComp rsaKeyComp [2:kSE05x_TAG_2]
 * @param[in] serializedObject serializedObject [3:kSE05x_TAG_3]
 * @param[in] serializedObjectLen Length of serializedObject
 */
smStatus_t Se05x_API_ImportObject(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_RSAKeyComponent_t rsaKeyComp,
    const uint8_t *serializedObject,
    size_t serializedObjectLen);

/** Se05x_API_ImportExternalObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] fastSCPdata fastSCPdata [1:kSE05x_TAG_2]
 * @param[in] fastSCPdataLen Length of fastSCPdata
 * @param[in] serializedObject serializedObject [2:kSE05x_TAG_3]
 * @param[in] serializedObjectLen Length of serializedObject
 */
smStatus_t Se05x_API_ImportExternalObject(pSe05xSession_t session_ctx,
    const uint8_t *fastSCPdata,
    size_t fastSCPdataLen,
    const uint8_t *serializedObject,
    size_t serializedObjectLen);

/** Se05x_API_ReadObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] offset offset [2:kSE05x_TAG_2]
 * @param[in] length length [3:kSE05x_TAG_3]
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 */
smStatus_t Se05x_API_ReadObject(
    pSe05xSession_t session_ctx, uint32_t objectID, uint16_t offset, uint16_t length, uint8_t *data, size_t *pdataLen);

/** Se05x_API_ReadObject_W_Attst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] offset offset [2:kSE05x_TAG_2]
 * @param[in] length length [3:kSE05x_TAG_3]
 * @param[in] attestID attestID [4:kSE05x_TAG_5]
 * @param[in] attestAlgo attestAlgo [5:kSE05x_TAG_6]
 * @param[in] random random [6:kSE05x_TAG_7]
 * @param[in] randomLen Length of random
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 * @param[out] timeStamp  [1:kSE05x_TAG_2]
 * @param[out] outrandom  [2:kSE05x_TAG_3]
 * @param[in,out] poutrandomLen Length for outrandom
 * @param[out] chipId  [3:kSE05x_TAG_4]
 * @param[in,out] pchipIdLen Length for chipId
 * @param[out] signature  [4:kSE05x_TAG_5]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_ReadObject_W_Attst(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint16_t offset,
    uint16_t length,
    uint32_t attestID,
    SE05x_AttestationAlgo_t attestAlgo,
    const uint8_t *random,
    size_t randomLen,
    uint8_t *data,
    size_t *pdataLen,
    uint8_t *attribute,
    size_t *pattributeLen,
    SE05x_TimeStamp_t *ptimeStamp,
    uint8_t *outrandom,
    size_t *poutrandomLen,
    uint8_t *chipId,
    size_t *pchipIdLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_ReadRSA
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] offset offset [2:kSE05x_TAG_2]
 * @param[in] length length [3:kSE05x_TAG_3]
 * @param[in] rsa_key_comp rsa_key_comp [4:kSE05x_TAG_4]
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 */
smStatus_t Se05x_API_ReadRSA(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint16_t offset,
    uint16_t length,
    SE05x_RSAPubKeyComp_t rsa_key_comp,
    uint8_t *data,
    size_t *pdataLen);

/** Se05x_API_ReadRSA_W_Attst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] offset offset [2:kSE05x_TAG_2]
 * @param[in] length length [3:kSE05x_TAG_3]
 * @param[in] rsa_key_comp rsa_key_comp [4:kSE05x_TAG_4]
 * @param[in] attestID attestID [5:kSE05x_TAG_5]
 * @param[in] attestAlgo attestAlgo [6:kSE05x_TAG_6]
 * @param[in] random random [7:kSE05x_TAG_7]
 * @param[in] randomLen Length of random
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 * @param[out] timeStamp  [1:kSE05x_TAG_2]
 * @param[out] outrandom  [2:kSE05x_TAG_3]
 * @param[in,out] poutrandomLen Length for outrandom
 * @param[out] chipId  [3:kSE05x_TAG_4]
 * @param[in,out] pchipIdLen Length for chipId
 * @param[out] signature  [4:kSE05x_TAG_5]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_ReadRSA_W_Attst(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint16_t offset,
    uint16_t length,
    SE05x_RSAPubKeyComp_t rsa_key_comp,
    uint32_t attestID,
    SE05x_AttestationAlgo_t attestAlgo,
    const uint8_t *random,
    size_t randomLen,
    uint8_t *data,
    size_t *pdataLen,
    uint8_t *attribute,
    size_t *pattributeLen,
    SE05x_TimeStamp_t *ptimeStamp,
    uint8_t *outrandom,
    size_t *poutrandomLen,
    uint8_t *chipId,
    size_t *pchipIdLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_ReadObjectAttributes
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] attestID attestID [2:kSE05x_TAG_5]
 * @param[in] attestAlgo attestAlgo [3:kSE05x_TAG_6]
 * @param[in] random random [4:kSE05x_TAG_7]
 * @param[in] randomLen Length of random
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 * @param[out] timeStamp  [1:kSE05x_TAG_2]
 * @param[out] outrandom  [2:kSE05x_TAG_3]
 * @param[in,out] poutrandomLen Length for outrandom
 * @param[out] chipId  [3:kSE05x_TAG_4]
 * @param[in,out] pchipIdLen Length for chipId
 * @param[out] signature  [4:kSE05x_TAG_5]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_ReadObjectAttributes(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint32_t attestID,
    SE05x_AttestationAlgo_t attestAlgo,
    const uint8_t *random,
    size_t randomLen,
    uint8_t *data,
    size_t *pdataLen,
    SE05x_TimeStamp_t *ptimeStamp,
    uint8_t *outrandom,
    size_t *poutrandomLen,
    uint8_t *chipId,
    size_t *pchipIdLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_ExportObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] rsaKeyComp rsaKeyComp [2:kSE05x_TAG_2]
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 */
smStatus_t Se05x_API_ExportObject(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_RSAKeyComponent_t rsaKeyComp,
    uint8_t *data,
    size_t *pdataLen);

/** Se05x_API_ReadType
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[out] type  [0:kSE05x_TAG_1]
 * @param[out] isTransient  [1:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_ReadType(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_SecureObjectType_t *ptype,
    uint8_t *pisTransient,
    const SE05x_AttestationType_t attestation_type);

/** Se05x_API_ReadSize
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[out] size  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_ReadSize(pSe05xSession_t session_ctx, uint32_t objectID, uint16_t *psize);

/** Se05x_API_ReadIDList
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] outputOffset output offset [1:kSE05x_TAG_1]
 * @param[in] filter filter [2:kSE05x_TAG_2]
 * @param[out] more If more ids are present [0:kSE05x_TAG_1]
 * @param[out] idlist Byte array containing 4-byte identifiers [1:kSE05x_TAG_2]
 * @param[in,out] pidlistLen Length for idlist
 */
smStatus_t Se05x_API_ReadIDList(pSe05xSession_t session_ctx,
    uint16_t outputOffset,
    uint8_t filter,
    uint8_t *pmore,
    uint8_t *idlist,
    size_t *pidlistLen);

/** Se05x_API_CheckObjectExists
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[out] result  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_CheckObjectExists(pSe05xSession_t session_ctx, uint32_t objectID, SE05x_Result_t *presult);

/** Se05x_API_DeleteSecureObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_DeleteSecureObject(pSe05xSession_t session_ctx, uint32_t objectID);

/** Se05x_API_CreateECCurve
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] curveID curve id [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_CreateECCurve(pSe05xSession_t session_ctx, SE05x_ECCurve_t curveID);

/** Se05x_API_SetECCurveParam
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] curveID curve id [1:kSE05x_TAG_1]
 * @param[in] ecCurveParam ecCurveParam [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_SetECCurveParam(pSe05xSession_t session_ctx,
    SE05x_ECCurve_t curveID,
    SE05x_ECCurveParam_t ecCurveParam,
    const uint8_t *inputData,
    size_t inputDataLen);

/** Se05x_API_GetECCurveId
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[out] pcurveId  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_GetECCurveId(pSe05xSession_t session_ctx, uint32_t objectID, uint8_t *pcurveId);

/** Se05x_API_ReadECCurveList
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[out] curveList  [0:kSE05x_TAG_1]
 * @param[in,out] pcurveListLen Length for curveList
 */
smStatus_t Se05x_API_ReadECCurveList(pSe05xSession_t session_ctx, uint8_t *curveList, size_t *pcurveListLen);

/** Se05x_API_DeleteECCurve
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] curveID curve id [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_DeleteECCurve(pSe05xSession_t session_ctx, SE05x_ECCurve_t curveID);

/** Se05x_API_CreateCryptoObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_1]
 * @param[in] cryptoContext cryptoContext [2:kSE05x_TAG_2]
 * @param[in] subtype 1-byte Crypto Object subtype, either from DigestMode, CipherMode or MACAlgo (depending on TAG_2). [3:kSE05x_TAG_3]
 */
smStatus_t Se05x_API_CreateCryptoObject(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    SE05x_CryptoContext_t cryptoContext,
    SE05x_CryptoModeSubType_t subtype);

/** Se05x_API_ReadCryptoObjectList
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[out] idlist If more ids are present [0:kSE05x_TAG_1]
 * @param[in,out] pidlistLen Length for idlist
 */
smStatus_t Se05x_API_ReadCryptoObjectList(pSe05xSession_t session_ctx, uint8_t *idlist, size_t *pidlistLen);

/** Se05x_API_DeleteCryptoObject
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_DeleteCryptoObject(pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID);

/** Se05x_API_ECDSASign
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] ecSignAlgo ecSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] signature  [0:kSE05x_TAG_1]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_ECDSASign(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECSignatureAlgo_t ecSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_EdDSASign
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] edSignAlgo edSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] signature  [0:kSE05x_TAG_1]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_EdDSASign(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_EDSignatureAlgo_t edSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_ECDAASign
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] ecdaaSignAlgo ecdaaSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] randomData randomData [4:kSE05x_TAG_4]
 * @param[in] randomDataLen Length of randomData
 * @param[out] signature  [0:kSE05x_TAG_1]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_ECDAASign(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECDAASignatureAlgo_t ecdaaSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *randomData,
    size_t randomDataLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_ECDSAVerify
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] ecSignAlgo ecSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] signature signature [4:kSE05x_TAG_5]
 * @param[in] signatureLen Length of signature
 * @param[out] result  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_ECDSAVerify(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECSignatureAlgo_t ecSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *signature,
    size_t signatureLen,
    SE05x_Result_t *presult);

/** Se05x_API_EdDSAVerify
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] edSignAlgo edSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] signature signature [4:kSE05x_TAG_5]
 * @param[in] signatureLen Length of signature
 * @param[out] result  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_EdDSAVerify(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_EDSignatureAlgo_t edSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *signature,
    size_t signatureLen,
    SE05x_Result_t *presult);

/** Se05x_API_ECDHGenerateSharedSecret
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] pubKey pubKey [2:kSE05x_TAG_2]
 * @param[in] pubKeyLen Length of pubKey
 * @param[out] sharedSecret  [0:kSE05x_TAG_1]
 * @param[in,out] psharedSecretLen Length for sharedSecret
 */
smStatus_t Se05x_API_ECDHGenerateSharedSecret(pSe05xSession_t session_ctx,
    uint32_t objectID,
    const uint8_t *pubKey,
    size_t pubKeyLen,
    uint8_t *sharedSecret,
    size_t *psharedSecretLen);

/** Se05x_API_RSASign
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] rsaSigningAlgo rsaSigningAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] signature  [0:kSE05x_TAG_1]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_RSASign(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_RSASignatureAlgo_t rsaSigningAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_RSAVerify
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] rsaSigningAlgo rsaSigningAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] signature signature [4:kSE05x_TAG_5]
 * @param[in] signatureLen Length of signature
 * @param[out] result  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_RSAVerify(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_RSASignatureAlgo_t rsaSigningAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *signature,
    size_t signatureLen,
    SE05x_Result_t *presult);

/** Se05x_API_RSAEncrypt
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] rsaEncryptionAlgo rsaEncryptionAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] encryptedData  [0:kSE05x_TAG_1]
 * @param[in,out] pencryptedDataLen Length for encryptedData
 */
smStatus_t Se05x_API_RSAEncrypt(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_RSAEncryptionAlgo_t rsaEncryptionAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *encryptedData,
    size_t *pencryptedDataLen);

/** Se05x_API_RSADecrypt
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] rsaEncryptionAlgo rsaEncryptionAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] decryptedData  [0:kSE05x_TAG_1]
 * @param[in,out] pdecryptedDataLen Length for decryptedData
 */
smStatus_t Se05x_API_RSADecrypt(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_RSAEncryptionAlgo_t rsaEncryptionAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *decryptedData,
    size_t *pdecryptedDataLen);

/** Se05x_API_CipherInit
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] cryptoObjectID cryptoObjectID [2:kSE05x_TAG_2]
 * @param[in] IV IV [3:kSE05x_TAG_4]
 * @param[in] IVLen Length of IV
 */
smStatus_t Se05x_API_CipherInit(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *IV,
    size_t IVLen,
    const SE05x_Cipher_Oper_t operation);

/** Se05x_API_CipherUpdate
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 * @param[in] inputData inputData [2:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_CipherUpdate(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *outputData,
    size_t *poutputDataLen);

/** Se05x_API_CipherFinal
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 * @param[in] inputData inputData [2:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_CipherFinal(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *outputData,
    size_t *poutputDataLen);

/** Se05x_API_CipherOneShot
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] cipherMode cipherMode [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] IV IV [4:kSE05x_TAG_4]
 * @param[in] IVLen Length of IV
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_CipherOneShot(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_CipherMode_t cipherMode,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *IV,
    size_t IVLen,
    uint8_t *outputData,
    size_t *poutputDataLen,
    const SE05x_Cipher_Oper_OneShot_t operation);

/** Se05x_API_MACInit
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] cryptoObjectID cryptoObjectID [2:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_MACInit(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const SE05x_Mac_Oper_t mac_oper);

/** Se05x_API_MACUpdate
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] inputData inputData [1:kSE05x_TAG_1]
 * @param[in] inputDataLen Length of inputData
 * @param[in] cryptoObjectID cryptoObjectID [2:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_MACUpdate(
    pSe05xSession_t session_ctx, const uint8_t *inputData, size_t inputDataLen, SE05x_CryptoObjectID_t cryptoObjectID);

/** Se05x_API_MACFinal
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] inputData inputData [1:kSE05x_TAG_1]
 * @param[in] inputDataLen Length of inputData
 * @param[in] cryptoObjectID cryptoObjectID [2:kSE05x_TAG_2]
 * @param[in] macValidateData macValidateData [3:kSE05x_TAG_3]
 * @param[in] macValidateDataLen Length of macValidateData
 * @param[out] macValue  [0:kSE05x_TAG_1]
 * @param[in,out] pmacValueLen Length for macValue
 */
smStatus_t Se05x_API_MACFinal(pSe05xSession_t session_ctx,
    const uint8_t *inputData,
    size_t inputDataLen,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *macValidateData,
    size_t macValidateDataLen,
    uint8_t *macValue,
    size_t *pmacValueLen);

/** Se05x_API_MACOneShot_G
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] macOperation macOperation [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] macValue  [0:kSE05x_TAG_1]
 * @param[in,out] pmacValueLen Length for macValue
 */
smStatus_t Se05x_API_MACOneShot_G(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint8_t macOperation,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *macValue,
    size_t *pmacValueLen);

/** Se05x_API_MACOneShot_V
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] macOperation macOperation [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] MAC MAC to verify (when P2=P2_VALIDATE_ONESHOT) [4:kSE05x_TAG_5]
 * @param[in] MACLen Length of MAC
 * @param[out] macValue  [0:kSE05x_TAG_1]
 * @param[in,out] pmacValueLen Length for macValue
 */
smStatus_t Se05x_API_MACOneShot_V(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint8_t macOperation,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *MAC,
    size_t MACLen,
    uint8_t *macValue,
    size_t *pmacValueLen);

/** Se05x_API_HKDF
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] hmacID hmacID [1:kSE05x_TAG_1]
 * @param[in] digestMode digestMode [2:kSE05x_TAG_2]
 * @param[in] salt salt [3:kSE05x_TAG_3]
 * @param[in] saltLen Length of salt
 * @param[in] info info [4:kSE05x_TAG_4]
 * @param[in] infoLen Length of info
 * @param[in] deriveDataLen 2-byte requested length (L) [5:kSE05x_TAG_5]
 * @param[out] hkdfOuput  [0:kSE05x_TAG_1]
 * @param[in,out] phkdfOuputLen Length for hkdfOuput
 */
smStatus_t Se05x_API_HKDF(pSe05xSession_t session_ctx,
    uint32_t hmacID,
    SE05x_DigestMode_t digestMode,
    const uint8_t *salt,
    size_t saltLen,
    const uint8_t *info,
    size_t infoLen,
    uint16_t deriveDataLen,
    uint8_t *hkdfOuput,
    size_t *phkdfOuputLen);

/** Se05x_API_PBKDF2
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID 4-byte password identifier (object type must be HMACKey) [1:kSE05x_TAG_1]
 * @param[in] salt salt [2:kSE05x_TAG_2]
 * @param[in] saltLen Length of salt
 * @param[in] count count [3:kSE05x_TAG_3]
 * @param[in] requestedLen requestedLen [4:kSE05x_TAG_4]
 * @param[out] derivedSessionKey  [0:kSE05x_TAG_1]
 * @param[in,out] pderivedSessionKeyLen Length for derivedSessionKey
 */
smStatus_t Se05x_API_PBKDF2(pSe05xSession_t session_ctx,
    uint32_t objectID,
    const uint8_t *salt,
    size_t saltLen,
    uint16_t count,
    uint16_t requestedLen,
    uint8_t *derivedSessionKey,
    size_t *pderivedSessionKeyLen);

/** Se05x_API_DFDiversifyKey
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] masterKeyID masterKeyID [1:kSE05x_TAG_1]
 * @param[in] diversifiedKeyID diversifiedKeyID [2:kSE05x_TAG_2]
 * @param[in] divInputData divInputData [3:kSE05x_TAG_3]
 * @param[in] divInputDataLen Length of divInputData
 */
smStatus_t Se05x_API_DFDiversifyKey(pSe05xSession_t session_ctx,
    uint32_t masterKeyID,
    uint32_t diversifiedKeyID,
    const uint8_t *divInputData,
    size_t divInputDataLen);

/** Se05x_API_DFAuthenticateFirstPart1
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] inputData inputData [2:kSE05x_TAG_2]
 * @param[in] inputDataLen Length of inputData
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_DFAuthenticateFirstPart1(pSe05xSession_t session_ctx,
    uint32_t objectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *outputData,
    size_t *poutputDataLen);

/** Se05x_API_DFAuthenticateNonFirstPart1
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] inputData inputData [2:kSE05x_TAG_2]
 * @param[in] inputDataLen Length of inputData
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_DFAuthenticateNonFirstPart1(pSe05xSession_t session_ctx,
    uint32_t objectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *outputData,
    size_t *poutputDataLen);

/** Se05x_API_DFAuthenticateFirstPart2
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] inputData inputData [1:kSE05x_TAG_1]
 * @param[in] inputDataLen Length of inputData
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_DFAuthenticateFirstPart2(pSe05xSession_t session_ctx,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *outputData,
    size_t *poutputDataLen);

/** Se05x_API_DFAuthenticateNonFirstPart2
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] inputData inputData [1:kSE05x_TAG_1]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_DFAuthenticateNonFirstPart2(
    pSe05xSession_t session_ctx, const uint8_t *inputData, size_t inputDataLen);

/** Se05x_API_DFDumpSessionKeys
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[out] sessionData 38 bytes: KeyID.SesAuthENCKey || KeyID.SesAuthMACKey || TI || Cmd-Ctr [0:kSE05x_TAG_1]
 * @param[in,out] psessionDataLen Length for sessionData
 */
smStatus_t Se05x_API_DFDumpSessionKeys(pSe05xSession_t session_ctx, uint8_t *sessionData, size_t *psessionDataLen);

/** Se05x_API_DFChangeKeyPart1
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] oldObjectID oldObjectID [1:kSE05x_TAG_1]
 * @param[in] newObjectID newObjectID [2:kSE05x_TAG_2]
 * @param[in] keySetNr keySetNr [3:kSE05x_TAG_3]
 * @param[in] keyNoDESFire keyNoDESFire [4:kSE05x_TAG_4]
 * @param[in] keyVer keyVer [5:kSE05x_TAG_5]
 * @param[out] KeyData  [0:kSE05x_TAG_1]
 * @param[in,out] pKeyDataLen Length for KeyData
 */
smStatus_t Se05x_API_DFChangeKeyPart1(pSe05xSession_t session_ctx,
    uint32_t oldObjectID,
    uint32_t newObjectID,
    uint8_t keySetNr,
    uint8_t keyNoDESFire,
    uint8_t keyVer,
    uint8_t *KeyData,
    size_t *pKeyDataLen);

/** Se05x_API_DFChangeKeyPart2
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] MAC MAC [1:kSE05x_TAG_1]
 * @param[in] MACLen Length of MAC
 * @param[out] result  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_DFChangeKeyPart2(pSe05xSession_t session_ctx, const uint8_t *MAC, size_t MACLen, uint8_t *presult);

/** Se05x_API_DFKillAuthentication
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 */
smStatus_t Se05x_API_DFKillAuthentication(pSe05xSession_t session_ctx);

/** Se05x_API_TLSGenerateRandom
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[out] randomValue  [0:kSE05x_TAG_1]
 * @param[in,out] prandomValueLen Length for randomValue
 */
smStatus_t Se05x_API_TLSGenerateRandom(pSe05xSession_t session_ctx, uint8_t *randomValue, size_t *prandomValueLen);

/** Se05x_API_TLSCalculatePreMasterSecret
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] keyPairId keyPairId [1:kSE05x_TAG_1]
 * @param[in] pskId pskId [2:kSE05x_TAG_2]
 * @param[in] hmacKeyId hmacKeyId [3:kSE05x_TAG_3]
 * @param[in] inputData inputData [4:kSE05x_TAG_4]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_TLSCalculatePreMasterSecret(pSe05xSession_t session_ctx,
    uint32_t keyPairId,
    uint32_t pskId,
    uint32_t hmacKeyId,
    const uint8_t *inputData,
    size_t inputDataLen);

/** Se05x_API_TLSPerformPRF
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] digestAlgo digestAlgo [2:kSE05x_TAG_2]
 * @param[in] label Label (1 to 64 bytes) [3:kSE05x_TAG_3]
 * @param[in] labelLen Length of label
 * @param[in] random 32-byte random [4:kSE05x_TAG_4]
 * @param[in] randomLen Length of random
 * @param[in] reqLen 2-byte requested length [5:kSE05x_TAG_5]
 * @param[out] outputData  [0:kSE05x_TAG_1]
 * @param[in,out] poutputDataLen Length for outputData
 */
smStatus_t Se05x_API_TLSPerformPRF(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint8_t digestAlgo,
    const uint8_t *label,
    size_t labelLen,
    const uint8_t *random,
    size_t randomLen,
    uint16_t reqLen,
    uint8_t *outputData,
    size_t *poutputDataLen,
    const SE05x_TLSPerformPRFType_t tlsprf);

/** Se05x_API_I2CM_ExecuteCommandSet
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] inputData inputData [1:kSE05x_TAG_1]
 * @param[in] inputDataLen Length of inputData
 * @param[in] attestationID attestationID [2:kSE05x_TAG_2]
 * @param[in] attestationAlgo attestationAlgo [3:kSE05x_TAG_3]
 * @param[out] response  [0:kSE05x_TAG_1]
 * @param[in,out] presponseLen Length for response
 * @param[out] timeStamp  [1:kSE05x_TAG_2]
 * @param[out] freshness  [2:kSE05x_TAG_3]
 * @param[in,out] pfreshnessLen Length for freshness
 * @param[out] chipId  [3:kSE05x_TAG_4]
 * @param[in,out] pchipIdLen Length for chipId
 * @param[out] signature  [4:kSE05x_TAG_5]
 * @param[in,out] psignatureLen Length for signature
 * @param[in] random random [4:kSE05x_TAG_7]
 * @param[in] randomLen Length of random
 */
smStatus_t Se05x_API_I2CM_ExecuteCommandSet(pSe05xSession_t session_ctx,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint32_t attestationID,
    uint8_t attestationAlgo,
    uint8_t *response,
    size_t *presponseLen,
    SE05x_TimeStamp_t *ptimeStamp,
    uint8_t *freshness,
    size_t *pfreshnessLen,
    uint8_t *chipId,
    size_t *pchipIdLen,
    uint8_t *signature,
    size_t *psignatureLen,
    uint8_t *randomAttst,
    size_t randomAttstLen);

/** Se05x_API_DigestInit
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_DigestInit(pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID);

/** Se05x_API_DigestUpdate
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 * @param[in] inputData inputData [2:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_DigestUpdate(
    pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID, const uint8_t *inputData, size_t inputDataLen);

/** Se05x_API_DigestFinal
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 * @param[in] inputData inputData [2:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] cmacValue  [0:kSE05x_TAG_1]
 * @param[in,out] pcmacValueLen Length for cmacValue
 */
smStatus_t Se05x_API_DigestFinal(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *cmacValue,
    size_t *pcmacValueLen);

/** Se05x_API_DigestOneShot
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] digestMode digestMode [1:kSE05x_TAG_1]
 * @param[in] inputData inputData [2:kSE05x_TAG_2]
 * @param[in] inputDataLen Length of inputData
 * @param[out] hashValue  [0:kSE05x_TAG_1]
 * @param[in,out] phashValueLen Length for hashValue
 */
smStatus_t Se05x_API_DigestOneShot(pSe05xSession_t session_ctx,
    uint8_t digestMode,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *hashValue,
    size_t *phashValueLen);

/** Se05x_API_GetVersion
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[out] appletVersion  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_GetVersion(pSe05xSession_t session_ctx, uint8_t *pappletVersion, size_t * appletVersionLen);

/** Se05x_API_GetTimestamp
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[out] timeStamp  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_GetTimestamp(pSe05xSession_t session_ctx, SE05x_TimeStamp_t *ptimeStamp);

/** Se05x_API_GetFreeMemory
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] memoryType memoryType [1:kSE05x_TAG_1]
 * @param[out] freeMem  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_GetFreeMemory(pSe05xSession_t session_ctx, SE05x_MemoryType_t memoryType, uint16_t *pfreeMem);

/** Se05x_API_GetRandom
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] size size [1:kSE05x_TAG_1]
 * @param[out] randomData  [0:kSE05x_TAG_1]
 * @param[in,out] prandomDataLen Length for randomData
 */
smStatus_t Se05x_API_GetRandom(pSe05xSession_t session_ctx, uint16_t size, uint8_t *randomData, size_t *prandomDataLen);

/** Se05x_API_DeleteAll
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 */
smStatus_t Se05x_API_DeleteAll(pSe05xSession_t session_ctx);

#endif /* SE050X_APDU_H_INC */
