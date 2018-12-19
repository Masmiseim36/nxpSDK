
#ifndef _FSL_SSS_CONFIG_H_
#define _FSL_SSS_CONFIG_H_

/* clang-format off */
#define SSS_SESSION_MAX_CONTEXT_SIZE        (16)
#define SSS_KEY_STORE_MAX_CONTEXT_SIZE      (16 + 80)
#define SSS_KEY_OBJECT_MAX_CONTEXT_SIZE     (16)
#define SSS_SYMMETRIC_MAX_CONTEXT_SIZE      (16 + 80)
#define SSS_AEAD_MAX_CONTEXT_SIZE           (16)
#define SSS_DIGEST_MAX_CONTEXT_SIZE         (16 + 80)
#define SSS_MAC_MAX_CONTEXT_SIZE            (16 + 16)
#define SSS_ASYMMETRIC_MAX_CONTEXT_SIZE     (16)
#define SSS_TUNNEL_MAX_CONTEXT_SIZE         (16)
#define SSS_DERIVE_KEY_MAX_CONTEXT_SIZE     (16)
/* clang-format on */

#endif /* _FSL_SSS_CONFIG_H_ */
