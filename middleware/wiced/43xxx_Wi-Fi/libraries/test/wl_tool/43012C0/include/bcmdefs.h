/*
 * Misc system wide definitions
 *
 * $ Copyright Open Cypress Semiconductor $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: bcmdefs.h 659962 2016-09-16 18:46:17Z jund $
 */

#ifndef	_bcmdefs_h_
#define	_bcmdefs_h_

/*
 * One doesn't need to include this file explicitly, gets included automatically if
 * typedefs.h is included.
 */

/* Use BCM_REFERENCE to suppress warnings about intentionally-unused function
 * arguments or local variables.
 */
#define BCM_REFERENCE(data)	((void)(data))

/* Allow for suppressing unused variable warnings. */
#ifdef __GNUC__
#define UNUSED_VAR     __attribute__ ((unused))
#else
#define UNUSED_VAR
#endif

#ifdef EFI
#define STATIC_ASSERT(expr)	ASSERT((expr))
#else
/* Compile-time assert can be used in place of ASSERT if the expression evaluates
 * to a constant at compile time.
 */
#define STATIC_ASSERT(expr) { \
	/* Make sure the expression is constant. */ \
	typedef enum { _STATIC_ASSERT_NOT_CONSTANT = (expr) } _static_assert_e UNUSED_VAR; \
	/* Make sure the expression is true. */ \
	typedef char STATIC_ASSERT_FAIL[(expr) ? 1 : -1] UNUSED_VAR; \
}
#endif /* #ifdef EFI */

/* Reclaiming text and data :
 * The following macros specify special linker sections that can be reclaimed
 * after a system is considered 'up'.
 * BCMATTACHFN is also used for detach functions (it's not worth having a BCMDETACHFN,
 * as in most cases, the attach function calls the detach function to clean up on error).
 */
#if defined(BCM_RECLAIM)

extern bool bcm_reclaimed;
extern bool bcm_attach_part_reclaimed;
extern bool bcm_preattach_part_reclaimed;
extern bool bcm_postattach_part_reclaimed;

#define RECLAIMED()			(bcm_reclaimed)
#define ATTACH_PART_RECLAIMED()		(bcm_attach_part_reclaimed)
#define PREATTACH_PART_RECLAIMED()	(bcm_preattach_part_reclaimed)
#define POSTATTACH_PART_RECLAIMED()	(bcm_postattach_part_reclaimed)

#if defined(BCM_RECLAIM_ATTACH_FN_DATA)
#define BCMATTACHDATA(_data)	__attribute__ ((__section__ (".dataini2." #_data))) _data
#define BCMATTACHFN(_fn)	__attribute__ ((__section__ (".textini2." #_fn), noinline)) _fn

/* Relocate attach symbols to save-restore region to increase pre-reclaim heap size. */
#define BCM_SRM_ATTACH_DATA(_data)    __attribute__ ((__section__ (".datasrm." #_data))) _data
#define BCM_SRM_ATTACH_FN(_fn)        __attribute__ ((__section__ (".textsrm." #_fn), noinline)) _fn

#ifndef PREATTACH_NORECLAIM
#define BCMPREATTACHDATA(_data)	__attribute__ ((__section__ (".dataini3." #_data))) _data
#define BCMPREATTACHFN(_fn)	__attribute__ ((__section__ (".textini3." #_fn), noinline)) _fn
#else
#define BCMPREATTACHDATA(_data)	__attribute__ ((__section__ (".dataini2." #_data))) _data
#define BCMPREATTACHFN(_fn)	__attribute__ ((__section__ (".textini2." #_fn), noinline)) _fn
#endif /* PREATTACH_NORECLAIM  */
#define BCMPOSTATTACHDATA(_data)	__attribute__ ((__section__ (".dataini5." #_data))) _data
#define BCMPOSTATTACHFN(_fn)	__attribute__ ((__section__ (".textini5." #_fn), noinline)) _fn
#else  /* BCM_RECLAIM_ATTACH_FN_DATA  */
#define BCMATTACHDATA(_data)	_data
#define BCMATTACHFN(_fn)	_fn
#define BCMPREATTACHDATA(_data)	_data
#define BCMPREATTACHFN(_fn)	_fn
#define BCMPOSTATTACHDATA(_data)	_data
#define BCMPOSTATTACHFN(_fn)	_fn
#endif /* BCM_RECLAIM_ATTACH_FN_DATA  */

#ifdef BCMDBG_SR
/*
 * Don't reclaim so we can compare SR ASM
 */
#define BCMPREATTACHDATASR(_data)	_data
#define BCMPREATTACHFNSR(_fn)		_fn
#define BCMATTACHDATASR(_data)		_data
#define BCMATTACHFNSR(_fn)		_fn
#else
#define BCMPREATTACHDATASR(_data)	BCMPREATTACHDATA(_data)
#define BCMPREATTACHFNSR(_fn)		BCMPREATTACHFN(_fn)
#define BCMATTACHDATASR(_data)		BCMATTACHDATA(_data)
#define BCMATTACHFNSR(_fn)		BCMATTACHFN(_fn)
#endif

#if defined(BCM_RECLAIM_INIT_FN_DATA)
#define BCMINITDATA(_data)	__attribute__ ((__section__ (".dataini1." #_data))) _data
#define BCMINITFN(_fn)		__attribute__ ((__section__ (".textini1." #_fn), noinline)) _fn
#define CONST
#else /* BCM_RECLAIM_INIT_FN_DATA  */
#define BCMINITDATA(_data)	_data
#define BCMINITFN(_fn)		_fn
#ifndef CONST
#define CONST	const
#endif
#endif /* BCM_RECLAIM_INIT_FN_DATA  */

/* Non-manufacture or internal attach function/dat */
#if !(defined(WLTEST) || defined(BCMINTERNAL))
#define	BCMNMIATTACHFN(_fn)	BCMATTACHFN(_fn)
#define	BCMNMIATTACHDATA(_data)	BCMATTACHDATA(_data)
#else
#define	BCMNMIATTACHFN(_fn)	_fn
#define	BCMNMIATTACHDATA(_data)	_data
#endif	/* WLTEST || BCMINTERNAL */

/* SROM with OTP support */
#if defined(BCMOTPSROM)
#define	BCMSROMATTACHFN(_fn)		_fn
#define	BCMSROMATTACHDATA(_data)	_data
#else
#define	BCMSROMATTACHFN(_fn)		BCMNMIATTACHFN(_fn)
#define	BCMSROMATTACHDATA(_data)	BCMNMIATTACHFN(_data)
#endif	/* BCMOTPSROM */

#ifdef BCMNODOWN
#define BCMUNINITFN(_fn)	BCMINITFN(_fn)
#else
#define BCMUNINITFN(_fn)	_fn
#endif

#else /* BCM_RECLAIM */

#define bcm_reclaimed			(1)
#define bcm_attach_part_reclaimed	(1)
#define bcm_preattach_part_reclaimed	(1)
#define bcm_postattach_part_reclaimed	(1)
#define BCMATTACHDATA(_data)		_data
#define BCMATTACHFN(_fn)		_fn
#define BCM_SRM_ATTACH_DATA(_data)	_data
#define BCM_SRM_ATTACH_FN(_fn)		_fn
#define BCMPREATTACHDATA(_data)		_data
#define BCMPREATTACHFN(_fn)		_fn
#define BCMPOSTATTACHDATA(_data)	_data
#define BCMPOSTATTACHFN(_fn)		_fn
#define BCMINITDATA(_data)		_data
#define BCMINITFN(_fn)			_fn
#define BCMUNINITFN(_fn)		_fn
#define	BCMNMIATTACHFN(_fn)		_fn
#define	BCMNMIATTACHDATA(_data)		_data
#define	BCMSROMATTACHFN(_fn)		_fn
#define	BCMSROMATTACHDATA(_data)	_data
#define BCMPREATTACHFNSR(_fn)		_fn
#define BCMPREATTACHDATASR(_data)	_data
#define BCMATTACHFNSR(_fn)		_fn
#define BCMATTACHDATASR(_data)		_data
#define CONST				const

#define RECLAIMED()			(bcm_reclaimed)
#define ATTACH_PART_RECLAIMED()		(bcm_attach_part_reclaimed)
#define PREATTACH_PART_RECLAIMED()	(bcm_preattach_part_reclaimed)
#define POSTATTACH_PART_RECLAIMED()	(bcm_postattach_part_reclaimed)

#endif /* BCM_RECLAIM */

#define BCMUCODEDATA(_data)		BCMINITDATA(_data)

#if defined(BCM_DMA_CT) && !defined(BCM_DMA_CT_DISABLED)
#define BCMUCODEFN(_fn)			BCMINITFN(_fn)
#else
#define BCMUCODEFN(_fn)			BCMATTACHFN(_fn)
#endif /* BCM_DMA_CT */

#if !defined STB
#if defined(BCM47XX) && defined(__ARM_ARCH_7A__) && !defined(OEM_ANDROID)
#define BCM47XX_CA9
#else
#undef BCM47XX_CA9
#endif /* BCM47XX && __ARM_ARCH_7A__ && !OEM_ANDROID */
#endif /* STB */

/* BCMFASTPATH Related Macro defines
*/
#ifndef BCMFASTPATH
#if defined(mips) || defined(BCM47XX_CA9) || defined(STB)
#define BCMFASTPATH		__attribute__ ((__section__ (".text.fastpath")))
#define BCMFASTPATH_HOST	__attribute__ ((__section__ (".text.fastpath_host")))
#else /* mips || BCM47XX_CA9 || STB */
#define BCMFASTPATH
#define BCMFASTPATH_HOST
#endif /* mips || BCM47XX_CA9 || STB */
#endif /* BCMFASTPATH */

/* Use the BCMRAMFN() macro to tag functions in source that must be included in RAM (excluded from
 * ROM). This should eliminate the need to manually specify these functions in the ROM config file.
 * It should only be used in special cases where the function must be in RAM for *all* ROM-based
 * chips.
 */
#if defined(BCMROMBUILD)
	#define BCMRAMFN(_fn)	__attribute__ ((__section__ (".text_ram." #_fn), noinline)) _fn
#else
	#define BCMRAMFN(_fn)	_fn
#endif

#define STATIC	static

/* Bus types */
#define	SI_BUS			0	/* SOC Interconnect */
#define	PCI_BUS			1	/* PCI target */
#define	PCMCIA_BUS		2	/* PCMCIA target */
#define SDIO_BUS		3	/* SDIO target */
#define JTAG_BUS		4	/* JTAG */
#define USB_BUS			5	/* USB (does not support R/W REG) */
#define SPI_BUS			6	/* gSPI target */
#define RPC_BUS			7	/* RPC target */

/* Allows size optimization for single-bus image */
#ifdef BCMBUSTYPE
#define BUSTYPE(bus)	(BCMBUSTYPE)
#else
#define BUSTYPE(bus)	(bus)
#endif

#ifdef BCMBUSCORETYPE
#define BUSCORETYPE(ct)		(BCMBUSCORETYPE)
#else
#define BUSCORETYPE(ct)		(ct)
#endif

/* Allows size optimization for single-backplane image */
#ifdef BCMCHIPTYPE
#define CHIPTYPE(bus)	(BCMCHIPTYPE)
#else
#define CHIPTYPE(bus)	(bus)
#endif


/* Allows size optimization for SPROM support */
#if defined(BCMSPROMBUS)
#define SPROMBUS	(BCMSPROMBUS)
#elif defined(SI_PCMCIA_SROM)
#define SPROMBUS	(PCMCIA_BUS)
#else
#define SPROMBUS	(PCI_BUS)
#endif

/* Allows size optimization for single-chip image */
/* XXX These macros are NOT meant to encourage writing chip-specific code.
 * Use them only when it is appropriate for example in PMU PLL/CHIP/SWREG
 * controls and in chip-specific workarounds.
 */
#ifdef BCMCHIPID
#define CHIPID(chip)	(BCMCHIPID)
#else
#define CHIPID(chip)	(chip)
#endif

#ifdef BCMCHIPREV
#define CHIPREV(rev)	(BCMCHIPREV)
#else
#define CHIPREV(rev)	(rev)
#endif

#ifdef BCMPCIEREV
#define PCIECOREREV(rev)	(BCMPCIEREV)
#else
#define PCIECOREREV(rev)	(rev)
#endif

#ifdef BCMPMUREV
#define PMUREV(rev)	(BCMPMUREV)
#else
#define PMUREV(rev)	(rev)
#endif

#ifdef BCMCCREV
#define CCREV(rev)	(BCMCCREV)
#else
#define CCREV(rev)	(rev)
#endif

#ifdef BCMGCIREV
#define GCIREV(rev)	(BCMGCIREV)
#else
#define GCIREV(rev)	(rev)
#endif

/* Defines for DMA Address Width - Shared between OSL and HNDDMA */
#define DMADDR_MASK_32 0x0		/* Address mask for 32-bits */
#define DMADDR_MASK_30 0xc0000000	/* Address mask for 30-bits */
#define DMADDR_MASK_26 0xFC000000	/* Address maks for 26-bits */
#define DMADDR_MASK_0  0xffffffff	/* Address mask for 0-bits (hi-part) */

#define	DMADDRWIDTH_26  26 /* 26-bit addressing capability */
#define	DMADDRWIDTH_30  30 /* 30-bit addressing capability */
#define	DMADDRWIDTH_32  32 /* 32-bit addressing capability */
#define	DMADDRWIDTH_63  63 /* 64-bit addressing capability */
#define	DMADDRWIDTH_64  64 /* 64-bit addressing capability */

typedef struct {
	uint32 loaddr;
	uint32 hiaddr;
} dma64addr_t;

#define PHYSADDR64HI(_pa) ((_pa).hiaddr)
#define PHYSADDR64HISET(_pa, _val) \
	do { \
		(_pa).hiaddr = (_val);		\
	} while (0)
#define PHYSADDR64LO(_pa) ((_pa).loaddr)
#define PHYSADDR64LOSET(_pa, _val) \
	do { \
		(_pa).loaddr = (_val);		\
	} while (0)

#ifdef BCMDMA64OSL
typedef dma64addr_t dmaaddr_t;
#define PHYSADDRHI(_pa) PHYSADDR64HI(_pa)
#define PHYSADDRHISET(_pa, _val) PHYSADDR64HISET(_pa, _val)
#define PHYSADDRLO(_pa)  PHYSADDR64LO(_pa)
#define PHYSADDRLOSET(_pa, _val) PHYSADDR64LOSET(_pa, _val)
#define PHYSADDRTOULONG(_pa, _ulong) \
	do { \
		_ulong = ((unsigned long long)(_pa).hiaddr << 32) | ((_pa).loaddr); \
	} while (0)

#else
typedef unsigned long dmaaddr_t;
#define PHYSADDRHI(_pa) (0)
#define PHYSADDRHISET(_pa, _val)
#define PHYSADDRLO(_pa) ((_pa))
#define PHYSADDRLOSET(_pa, _val) \
	do { \
		(_pa) = (_val);			\
	} while (0)
#endif /* BCMDMA64OSL */
#define PHYSADDRISZERO(_pa) (PHYSADDRLO(_pa) == 0 && PHYSADDRHI(_pa) == 0)

/* One physical DMA segment */
typedef struct  {
	dmaaddr_t addr;
	uint32	  length;
} hnddma_seg_t;

#if defined(MACOSX)
/* In MacOS, the OS API may return large number of segments. Setting this number lower
 * will result in failure of dma map
 */
#define MAX_DMA_SEGS 8
#elif defined(__NetBSD__)
/* In NetBSD we also want more segments because the lower level mbuf mapping api might
 * allocate a large number of segments
 */
#define MAX_DMA_SEGS 16
#elif defined(linux)
#define MAX_DMA_SEGS 8
#else
#define MAX_DMA_SEGS 4
#endif


typedef struct {
	void *oshdmah; /* Opaque handle for OSL to store its information */
	uint origsize; /* Size of the virtual packet */
	uint nsegs;
	hnddma_seg_t segs[MAX_DMA_SEGS];
} hnddma_seg_map_t;


/* packet headroom necessary to accommodate the largest header in the system, (i.e TXOFF).
 * By doing, we avoid the need  to allocate an extra buffer for the header when bridging to WL.
 * There is a compile time check in wlc.c which ensure that this value is at least as big
 * as TXOFF. This value is used in dma_rxfill (hnddma.c).
 */

#if defined(BCM_RPC_NOCOPY) || defined(BCM_RCP_TXNOCOPY)
/* add 40 bytes to allow for extra RPC header and info  */
#define BCMEXTRAHDROOM 260
#else /* BCM_RPC_NOCOPY || BCM_RPC_TXNOCOPY */
#if defined(linux) && (defined(BCM47XX_CA9) || defined(STB))
#if defined(BCM_GMAC3)
#define BCMEXTRAHDROOM 32 /* For FullDongle, no D11 headroom space required. */
#define BCMEXTRAHDROOM_NIC 224
#else
#define BCMEXTRAHDROOM 224
#endif /* ! BCM_GMAC3 */
#else
#ifdef CTFMAP
#define BCMEXTRAHDROOM 208
#else /* CTFMAP */
#define BCMEXTRAHDROOM 204
#endif /* CTFMAP */
#endif /* linux && BCM47XX_CA9 */
#endif /* BCM_RPC_NOCOPY || BCM_RPC_TXNOCOPY */

/* Packet alignment for most efficient SDIO (can change based on platform) */
#ifndef SDALIGN
#define SDALIGN	32
#endif

/* Headroom required for dongle-to-host communication.  Packets allocated
 * locally in the dongle (e.g. for CDC ioctls or RNDIS messages) should
 * leave this much room in front for low-level message headers which may
 * be needed to get across the dongle bus to the host.  (These messages
 * don't go over the network, so room for the full WL header above would
 * be a waste.).
*/
/*
 * XXX: set the numbers to be MAX of all the devices, to avoid problems with ROM builds
 * USB BCMDONGLEHDRSZ and BCMDONGLEPADSZ is 0
 * SDIO BCMDONGLEHDRSZ 12 and BCMDONGLEPADSZ 16
*/
#define BCMDONGLEHDRSZ 12
#define BCMDONGLEPADSZ 16

#define BCMDONGLEOVERHEAD	(BCMDONGLEHDRSZ + BCMDONGLEPADSZ)

#ifdef BCMDBG

#ifndef BCMDBG_ERR
#define BCMDBG_ERR
#endif /* BCMDBG_ERR */

#ifndef BCMDBG_ASSERT
#define BCMDBG_ASSERT
#endif /* BCMDBG_ASSERT */

#endif /* BCMDBG */

#if defined(NO_BCMDBG_ASSERT)
# undef BCMDBG_ASSERT
# undef BCMASSERT_LOG
#endif

#if defined(BCMDBG_ASSERT) || defined(BCMASSERT_LOG)
#define BCMASSERT_SUPPORT
#endif /* BCMDBG_ASSERT || BCMASSERT_LOG */

/* Macros for doing definition and get/set of bitfields
 * Usage example, e.g. a three-bit field (bits 4-6):
 *    #define <NAME>_M	BITFIELD_MASK(3)
 *    #define <NAME>_S	4
 * ...
 *    regval = R_REG(osh, &regs->regfoo);
 *    field = GFIELD(regval, <NAME>);
 *    regval = SFIELD(regval, <NAME>, 1);
 *    W_REG(osh, &regs->regfoo, regval);
 */
#define BITFIELD_MASK(width) \
		(((unsigned)1 << (width)) - 1)
#define GFIELD(val, field) \
		(((val) >> field ## _S) & field ## _M)
#define SFIELD(val, field, bits) \
		(((val) & (~(field ## _M << field ## _S))) | \
		 ((unsigned)(bits) << field ## _S))

/* define BCMSMALL to remove misc features for memory-constrained environments */
#ifdef BCMSMALL
#undef	BCMSPACE
#define bcmspace	FALSE	/* if (bcmspace) code is discarded */
#else
#define	BCMSPACE
#define bcmspace	TRUE	/* if (bcmspace) code is retained */
#endif

/* Max. nvram variable table size */
#ifndef MAXSZ_NVRAM_VARS
#ifdef LARGE_NVRAM_MAXSZ
#define MAXSZ_NVRAM_VARS	(LARGE_NVRAM_MAXSZ * 2)
#else
#if defined(BCMROMBUILD) || defined(DONGLEBUILD)
/* SROM12 changes */
#define	MAXSZ_NVRAM_VARS	6144	/* XXX should be reduced */
#else
#define LARGE_NVRAM_MAXSZ	8192
#define MAXSZ_NVRAM_VARS	(LARGE_NVRAM_MAXSZ * 2)
#endif /* BCMROMBUILD || DONGLEBUILD */
#endif /* LARGE_NVRAM_MAXSZ */
#endif /* !MAXSZ_NVRAM_VARS */

#ifdef ATE_BUILD
#ifndef ATE_NVRAM_MAXSIZE
#define ATE_NVRAM_MAXSIZE 32000
#endif /* ATE_NVRAM_MAXSIZE */
#endif /* ATE_BUILD */

#if defined(EFI) && defined(EFI_WINBLD)
#define __attribute__(x)	/* CSTYLED */
#endif

/* WL_ENAB_RUNTIME_CHECK may be set based upon the #define below (for ROM builds). It may also
 * be defined via makefiles (e.g. ROM auto abandon unoptimized compiles).
 */
#if defined(BCMROMBUILD)
	#ifndef WL_ENAB_RUNTIME_CHECK
		#define WL_ENAB_RUNTIME_CHECK
	#endif
#endif /* BCMROMBUILD */

#ifdef BCMLFRAG /* BCMLFRAG support enab macros  */
	extern bool _bcmlfrag;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMLFRAG_ENAB() (_bcmlfrag)
	#elif defined(BCMLFRAG_DISABLED)
		#define BCMLFRAG_ENAB()	(0)
	#else
		#define BCMLFRAG_ENAB()	(1)
	#endif
#else
	#define BCMLFRAG_ENAB()		(0)
#endif /* BCMLFRAG_ENAB */

#ifdef BCMPCIEDEV /* BCMPCIEDEV support enab macros */
extern bool _pciedevenab;
	#if defined(WL_ENAB_RUNTIME_CHECK)
		#define BCMPCIEDEV_ENAB() (_pciedevenab)
	#elif defined(BCMPCIEDEV_ENABLED)
		#define BCMPCIEDEV_ENAB()	1
	#else
		#define BCMPCIEDEV_ENAB()	0
	#endif
#else
	#define BCMPCIEDEV_ENAB()	0
#endif /* BCMPCIEDEV */

#ifdef BCMRESVFRAGPOOL /* BCMRESVFRAGPOOL support enab macros */
extern bool _resvfragpool_enab;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define  BCMRESVFRAGPOOL_ENAB() (_resvfragpool_enab)
	#elif defined(BCMRESVFRAGPOOL_ENABLED)
		#define BCMRESVFRAGPOOL_ENAB()	1
	#else
		#define BCMRESVFRAGPOOL_ENAB()	0
	#endif
#else
	#define BCMRESVFRAGPOOL_ENAB()	0
#endif /* BCMPCIEDEV */

#ifdef BCMSDIODEV /* BCMSDIODEV support enab macros */
extern bool _sdiodevenab;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMSDIODEV_ENAB() (_sdiodevenab)
	#elif defined(BCMSDIODEV_ENABLED)
		#define BCMSDIODEV_ENAB()	1
	#else
		#define BCMSDIODEV_ENAB()	0
	#endif
#else
	#define BCMSDIODEV_ENAB()	0
#endif /* BCMSDIODEV */

/* Max size for reclaimable NVRAM array */
#ifndef ATE_BUILD
#ifdef DL_NVRAM
#define NVRAM_ARRAY_MAXSIZE	DL_NVRAM
#else
#define NVRAM_ARRAY_MAXSIZE	MAXSZ_NVRAM_VARS
#endif /* DL_NVRAM */
#else
#define NVRAM_ARRAY_MAXSIZE	ATE_NVRAM_MAXSIZE
#endif /* ATE_BUILD */

extern uint32 gFWID;

#ifdef BCMFRWDPOOLREORG /* BCMFRWDPOOLREORG support enab macros  */
	extern bool _bcmfrwdpoolreorg;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMFRWDPOOLREORG_ENAB() (_bcmfrwdpoolreorg)
	#elif defined(BCMFRWDPOOLREORG_DISABLED)
		#define BCMFRWDPOOLREORG_ENAB()	(0)
	#else
		#define BCMFRWDPOOLREORG_ENAB()	(1)
	#endif
#else
	#define BCMFRWDPOOLREORG_ENAB()		(0)
#endif /* BCMFRWDPOOLREORG */

#ifdef BCMPOOLRECLAIM /* BCMPOOLRECLAIM support enab macros  */
	extern bool _bcmpoolreclaim;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMPOOLRECLAIM_ENAB() (_bcmpoolreclaim)
	#elif defined(BCMPOOLRECLAIM_DISABLED)
		#define BCMPOOLRECLAIM_ENAB()	(0)
	#else
		#define BCMPOOLRECLAIM_ENAB()	(1)
	#endif
#else
	#define BCMPOOLRECLAIM_ENAB()		(0)
#endif /* BCMPOOLRECLAIM */

/* Chip related low power flags (lpflags) */

#ifndef PAD
#define _PADLINE(line)  pad ## line
#define _XSTR(line)     _PADLINE(line)
#define PAD             _XSTR(__LINE__)
#endif

#ifndef FRAG_HEADROOM
#define FRAG_HEADROOM	224	/* In absence of SFD, use default headroom of 224 */
#endif

#ifdef DONGLEBUILD
#define MODULE_DETACH(var, detach_func)
#define MODULE_DETACH_TYPECASTED(var, detach_func)
#else
#define MODULE_DETACH(var, detach_func)\
	if (var) { \
		detach_func(var); \
		(var) = NULL; \
	}
#define MODULE_DETACH_TYPECASTED(var, detach_func) detach_func(var)
#endif /* DONGLEBUILD */

#endif /* _bcmdefs_h_ */
