/* dsa.c
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * http://www.wolfssl.com
 */



#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#ifndef NO_DSA

#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/integer.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/logging.h>
#include <wolfssl/wolfcrypt/sha.h>
#include <wolfssl/wolfcrypt/dsa.h>


enum {
    DSA_HALF_SIZE = 20,   /* r and s size  */
    DSA_SIG_SIZE  = 40    /* signature size */
};


#ifndef WOLFSSL_HAVE_MIN
#define WOLFSSL_HAVE_MIN

    static INLINE word32 min(word32 a, word32 b)
    {
        return a > b ? b : a;
    }

#endif /* WOLFSSL_HAVE_MIN */


void wc_InitDsaKey(DsaKey* key)
{
    key->type = -1;  /* haven't decided yet */
    key->heap = NULL;

/* TomsFastMath doesn't use memory allocation */
#ifndef USE_FAST_MATH
    key->p.dp = 0;   /* public  alloc parts */
    key->q.dp = 0;    
    key->g.dp = 0;    
    key->y.dp = 0;    

    key->x.dp = 0;   /* private alloc parts */
#endif
}


int wc_InitDsaKey_h(DsaKey* key, void* h)
{
    wc_InitDsaKey(key);
    key->heap = h;

    return 0;
}


void wc_FreeDsaKey(DsaKey* key)
{
    (void)key;
/* TomsFastMath doesn't use memory allocation */
#ifndef USE_FAST_MATH
    if (key->type == DSA_PRIVATE)
        mp_clear(&key->x);
    mp_clear(&key->y);
    mp_clear(&key->g);
    mp_clear(&key->q);
    mp_clear(&key->p);
#endif
}

#ifdef WOLFSSL_KEY_GEN

int wc_MakeDsaKey(WC_RNG *rng, DsaKey *dsa)
{
    unsigned char *buf;
    int qsize, err;

    if (rng == NULL || dsa == NULL)
        return BAD_FUNC_ARG;

    qsize = mp_unsigned_bin_size(&dsa->q);
    if (qsize == 0)
        return BAD_FUNC_ARG;

    /* allocate ram */
    buf = (unsigned char *)XMALLOC(qsize, dsa->heap,
                                   DYNAMIC_TYPE_TMP_BUFFER);
    if (buf == NULL)
        return MEMORY_E;

    if (mp_init(&dsa->x) != MP_OKAY) {
        XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return MP_INIT_E;
    }

    do {
        /* make a random exponent mod q */
        err = wc_RNG_GenerateBlock(rng, buf, qsize);
        if (err != MP_OKAY) {
            mp_clear(&dsa->x);
            XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
            return err;
        }

        err = mp_read_unsigned_bin(&dsa->x, buf, qsize);
        if (err != MP_OKAY) {
            mp_clear(&dsa->x);
            XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
            return err;
        }
    } while (mp_cmp_d(&dsa->x, 1) != MP_GT);

    XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);

    if (mp_init(&dsa->y) != MP_OKAY) {
        mp_clear(&dsa->x);
        return MP_INIT_E;
    }

    /* public key : y = g^x mod p */
    err = mp_exptmod(&dsa->g, &dsa->x, &dsa->p, &dsa->y);
    if (err != MP_OKAY) {
        mp_clear(&dsa->x);
        mp_clear(&dsa->y);
        return err;
    }

    dsa->type = DSA_PRIVATE;
    
    return MP_OKAY;
}

/* modulus_size in bits */
int wc_MakeDsaParameters(WC_RNG *rng, int modulus_size, DsaKey *dsa)
{
    mp_int  tmp, tmp2;
    int     err, msize, qsize,
            loop_check_prime = 0,
            check_prime = MP_NO;
    unsigned char   *buf;

    if (rng == NULL || dsa == NULL)
        return BAD_FUNC_ARG;

    /* set group size in bytes from modulus size
     * FIPS 186-4 defines valid values (1024, 160) (2048, 256) (3072, 256)
     */
    switch (modulus_size) {
        case 1024:
            qsize = 20;
            break;
        case 2048:
        case 3072:
            qsize = 32;
            break;
        default:
            return BAD_FUNC_ARG;
            break;
    }

    /* modulus size in bytes */
    msize = modulus_size / 8;

    /* allocate ram */
    buf = (unsigned char *)XMALLOC(msize - qsize,
                                   dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
    if (buf == NULL) {
        return MEMORY_E;
    }

    /* make a random string that will be multplied against q */
    err = wc_RNG_GenerateBlock(rng, buf, msize - qsize);
    if (err != MP_OKAY) {
        XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return err;
    }

    /* force magnitude */
    buf[0] |= 0xC0;

    /* force even */
    buf[msize - qsize - 1] &= ~1;

    if (mp_init_multi(&tmp2, &dsa->p, &dsa->q, 0, 0, 0) != MP_OKAY) {
        mp_clear(&dsa->q);
        XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return MP_INIT_E;
    }

    err = mp_read_unsigned_bin(&tmp2, buf, msize - qsize);
    if (err != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp2);
        XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return err;
    }
    XFREE(buf, dsa->heap, DYNAMIC_TYPE_TMP_BUFFER);

    /* make our prime q */
    err = mp_rand_prime(&dsa->q, qsize, rng, NULL);
    if (err != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp2);
        return err;
    }

    /* p = random * q */
    err = mp_mul(&dsa->q, &tmp2, &dsa->p);
    if (err != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp2);
        return err;
    }

    /* p = random * q + 1, so q is a prime divisor of p-1 */
    err = mp_add_d(&dsa->p, 1, &dsa->p);
    if (err != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp2);
        return err;
    }

    if (mp_init(&tmp) != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp2);
        return MP_INIT_E;
    }

    /* tmp = 2q  */
    err = mp_add(&dsa->q, &dsa->q, &tmp);
    if (err != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp);
        mp_clear(&tmp2);
        return err;
    }

    /* loop until p is prime */
    while (check_prime == MP_NO) {
        err = mp_prime_is_prime(&dsa->p, 8, &check_prime);
        if (err != MP_OKAY) {
            mp_clear(&dsa->q);
            mp_clear(&dsa->p);
            mp_clear(&tmp);
            mp_clear(&tmp2);
            return err;
        }

        if (check_prime != MP_YES) {
            /* p += 2q */
            err = mp_add(&tmp, &dsa->p, &dsa->p);
            if (err != MP_OKAY) {
                mp_clear(&dsa->q);
                mp_clear(&dsa->p);
                mp_clear(&tmp);
                mp_clear(&tmp2);
                return err;
            }

            loop_check_prime++;
        }
    }

    /* tmp2 += (2*loop_check_prime)
     * to have p = (q * tmp2) + 1 prime
     */
    if (loop_check_prime) {
        err = mp_add_d(&tmp2, 2*loop_check_prime, &tmp2);
        if (err != MP_OKAY) {
            mp_clear(&dsa->q);
            mp_clear(&dsa->p);
            mp_clear(&tmp);
            mp_clear(&tmp2);
            return err;
        }
    }

    if (mp_init(&dsa->g) != MP_OKAY) {
        mp_clear(&dsa->q);
        mp_clear(&dsa->p);
        mp_clear(&tmp);
        mp_clear(&tmp2);
        return MP_INIT_E;
    }

    /* find a value g for which g^tmp2 != 1 */
    mp_set(&dsa->g, 1);

    do {
        err = mp_add_d(&dsa->g, 1, &dsa->g);
        if (err != MP_OKAY) {
            mp_clear(&dsa->q);
            mp_clear(&dsa->p);
            mp_clear(&dsa->g);
            mp_clear(&tmp);
            mp_clear(&tmp2);
            return err;
        }

        err = mp_exptmod(&dsa->g, &tmp2, &dsa->p, &tmp);
        if (err != MP_OKAY) {
            mp_clear(&dsa->q);
            mp_clear(&dsa->p);
            mp_clear(&dsa->g);
            mp_clear(&tmp);
            mp_clear(&tmp2);
            return err;
        }

    } while (mp_cmp_d(&tmp, 1) == MP_EQ);

    /* at this point tmp generates a group of order q mod p */
    mp_exch(&tmp, &dsa->g);

    mp_clear(&tmp);
    mp_clear(&tmp2);

    return MP_OKAY;
}
#endif /* WOLFSSL_KEY_GEN */


int wc_DsaSign(const byte* digest, byte* out, DsaKey* key, WC_RNG* rng)
{
    mp_int k, kInv, r, s, H;
    int    ret, sz;
    byte   buffer[DSA_HALF_SIZE];

    sz = min((int)sizeof(buffer), mp_unsigned_bin_size(&key->q));
    
    if (mp_init_multi(&k, &kInv, &r, &s, &H, 0) != MP_OKAY)
        return MP_INIT_E;

    do {
        /* generate k */
        ret = wc_RNG_GenerateBlock(rng, buffer, sz);
        if (ret != 0)
            return ret;

        buffer[0] |= 0x0C;

        if (mp_read_unsigned_bin(&k, buffer, sz) != MP_OKAY)
            ret = MP_READ_E;
            
        /* k is a random numnber and it should be less than q
         * if k greater than repeat
         */
    } while (mp_cmp(&k, &key->q) != MP_LT);
    
    if (ret == 0 && mp_cmp_d(&k, 1) != MP_GT)
        ret = MP_CMP_E;    

    /* inverse k mod q */
    if (ret == 0 && mp_invmod(&k, &key->q, &kInv) != MP_OKAY)
        ret = MP_INVMOD_E;

    /* generate r, r = (g exp k mod p) mod q */
    if (ret == 0 && mp_exptmod(&key->g, &k, &key->p, &r) != MP_OKAY)
        ret = MP_EXPTMOD_E;

    if (ret == 0 && mp_mod(&r, &key->q, &r) != MP_OKAY)
        ret = MP_MOD_E;

    /* generate H from sha digest */
    if (ret == 0 && mp_read_unsigned_bin(&H, digest,SHA_DIGEST_SIZE) != MP_OKAY)
        ret = MP_READ_E;

    /* generate s, s = (kInv * (H + x*r)) % q */
    if (ret == 0 && mp_mul(&key->x, &r, &s) != MP_OKAY)
        ret = MP_MUL_E;

    if (ret == 0 && mp_add(&s, &H, &s) != MP_OKAY)
        ret = MP_ADD_E;

    if (ret == 0 && mp_mulmod(&s, &kInv, &key->q, &s) != MP_OKAY)
        ret = MP_MULMOD_E;

    /* write out */
    if (ret == 0)  {
        int rSz = mp_unsigned_bin_size(&r);
        int sSz = mp_unsigned_bin_size(&s);

        if (rSz == DSA_HALF_SIZE - 1) {
            out[0] = 0;
            out++;
        }

        if (mp_to_unsigned_bin(&r, out) != MP_OKAY)
            ret = MP_TO_E;
        else {
            if (sSz == DSA_HALF_SIZE - 1) {
                out[rSz] = 0;
                out++;
            }    
            ret = mp_to_unsigned_bin(&s, out + rSz);
        }
    }

    mp_clear(&H);
    mp_clear(&s);
    mp_clear(&r);
    mp_clear(&kInv);
    mp_clear(&k);

    return ret;
}


int wc_DsaVerify(const byte* digest, const byte* sig, DsaKey* key, int* answer)
{
    mp_int w, u1, u2, v, r, s;
    int    ret = 0;

    if (mp_init_multi(&w, &u1, &u2, &v, &r, &s) != MP_OKAY)
        return MP_INIT_E;

    /* set r and s from signature */
    if (mp_read_unsigned_bin(&r, sig, DSA_HALF_SIZE) != MP_OKAY ||
        mp_read_unsigned_bin(&s, sig + DSA_HALF_SIZE, DSA_HALF_SIZE) != MP_OKAY)
        ret = MP_READ_E;

    /* sanity checks */
    if (ret == 0) {
        if (mp_iszero(&r) == MP_YES || mp_iszero(&s) == MP_YES ||
                mp_cmp(&r, &key->q) != MP_LT || mp_cmp(&s, &key->q) != MP_LT) {
            ret = MP_ZERO_E;
        }
    }

    /* put H into u1 from sha digest */
    if (ret == 0 && mp_read_unsigned_bin(&u1,digest,SHA_DIGEST_SIZE) != MP_OKAY)
        ret = MP_READ_E;

    /* w = s invmod q */
    if (ret == 0 && mp_invmod(&s, &key->q, &w) != MP_OKAY)
        ret = MP_INVMOD_E;

    /* u1 = (H * w) % q */
    if (ret == 0 && mp_mulmod(&u1, &w, &key->q, &u1) != MP_OKAY)
        ret = MP_MULMOD_E;

    /* u2 = (r * w) % q */
    if (ret == 0 && mp_mulmod(&r, &w, &key->q, &u2) != MP_OKAY)
        ret = MP_MULMOD_E;

    /* verify v = ((g^u1 * y^u2) mod p) mod q */
    if (ret == 0 && mp_exptmod(&key->g, &u1, &key->p, &u1) != MP_OKAY)
        ret = MP_EXPTMOD_E;

    if (ret == 0 && mp_exptmod(&key->y, &u2, &key->p, &u2) != MP_OKAY)
        ret = MP_EXPTMOD_E;

    if (ret == 0 && mp_mulmod(&u1, &u2, &key->p, &v) != MP_OKAY)
        ret = MP_MULMOD_E;

    if (ret == 0 && mp_mod(&v, &key->q, &v) != MP_OKAY)
        ret = MP_MULMOD_E;

    /* do they match */
    if (ret == 0 && mp_cmp(&r, &v) == MP_EQ)
        *answer = 1;
    else
        *answer = 0;

    mp_clear(&s);
    mp_clear(&r);
    mp_clear(&u1);
    mp_clear(&u2);
    mp_clear(&w);
    mp_clear(&v);

    return ret;
}


#endif /* NO_DSA */

