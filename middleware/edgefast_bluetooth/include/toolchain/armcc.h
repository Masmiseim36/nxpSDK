/*
 * Copyright 2021 NXP
 * Copyright (c) 2010-2014,2017 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_TOOLCHAIN_GCC_H_
#define ZEPHYR_INCLUDE_TOOLCHAIN_GCC_H_

/**
 * @file
 * @brief GCC toolchain abstraction
 *
 * Macros to abstract compiler capabilities for GCC toolchain.
 */

/*
 * Older versions of GCC do not define __BYTE_ORDER__, so it must be manually
 * detected and defined using arch-specific definitions.
 */

#ifndef _LINKER

#if !defined(__ssize_t_defined)
#define __ssize_t_defined
#define unsigned signed
typedef int ssize_t;
#undef unsigned
#endif

#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__            (1)
#endif

#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__         (2)
#endif

#ifndef __BYTE_ORDER__
#if defined(__BIG_ENDIAN__) || defined(__ARMEB__) || \
    defined(__THUMBEB__) || defined(__AARCH64EB__) || \
    defined(__MIPSEB__) || defined(__TC32EB__)

#define __BYTE_ORDER__                  __ORDER_BIG_ENDIAN__

#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || \
      defined(__THUMBEL__) || defined(__AARCH64EL__) || \
      defined(__MIPSEL__) || defined(__TC32EL__)

#define __BYTE_ORDER__                  __ORDER_LITTLE_ENDIAN__

#else
#error "__BYTE_ORDER__ is not defined and cannot be automatically resolved"
#endif
#endif


/* C++11 has static_assert built in */
#ifdef __cplusplus
#define BUILD_ASSERT(EXPR, MSG...) static_assert(EXPR, "" MSG)
#define BUILD_ASSERT_MSG(EXPR, MSG) __DEPRECATED_MACRO BUILD_ASSERT(EXPR, MSG)

/*
 * GCC 4.6 and higher have the C11 _Static_assert built in, and its
 * output is easier to understand than the common BUILD_ASSERT macros.
 */
#elif (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) || \
	(__STDC_VERSION__) >= 201100
#define BUILD_ASSERT(EXPR, MSG...) _Static_assert(EXPR, "" MSG)
#define BUILD_ASSERT_MSG(EXPR, MSG) __DEPRECATED_MACRO BUILD_ASSERT(EXPR, MSG)
#endif

#include <toolchain/common.h>
#include <stdbool.h>

#undef PRId8
#define PRId8    "d"

#undef PRIi8
#define PRIi8    "i"

#undef PRIo8
#define PRIo8    "o"

#undef PRIu8
#define PRIu8    "u"

#undef PRIx8
#define PRIx8    "x"

#undef PRIX8
#define PRIX8    "X"

#ifndef _STRINGIFY
#define _STRINGIFY(x) #x
#endif

#define ALIAS_OF(of) __attribute__((alias(#of)))

#define FUNC_ALIAS(real_func, new_alias, return_type) \
	return_type new_alias() ALIAS_OF(real_func)

#if defined(CONFIG_ARCH_POSIX)
#include <arch/posix/posix_trace.h>

/*let's not segfault if this were to happen for some reason*/
#define CODE_UNREACHABLE \
{\
	posix_print_error_and_exit("CODE_UNREACHABLE reached from %s:%d\n",\
		__FILE__, __LINE__);\
	__builtin_unreachable(); \
}
#else
#define CODE_UNREACHABLE __builtin_unreachable()
#endif
#define FUNC_NORETURN    __attribute__((__noreturn__))

/* The GNU assembler for Cortex-M3 uses # for immediate values, not
 * comments, so the @nobits# trick does not work.
 */
#if defined(CONFIG_ARM)
#define _NODATA_SECTION(segment)  __attribute__((section(#segment)))
#else
#define _NODATA_SECTION(segment)				\
	__attribute__((section(#segment ",\"wa\",@nobits#")))
#endif

/* Unaligned access */
#define UNALIGNED_GET(p)						\
__extension__ ({							\
	struct  __attribute__((__packed__)) {				\
		__typeof__(*(p)) __v;					\
	} *__p = (__typeof__(__p)) (p);					\
	__p->__v;							\
})


#if __GNUC__ >= 7 && defined(CONFIG_ARM)

/* Version of UNALIGNED_PUT() which issues a compiler_barrier() after
 * the store. It is required to workaround an apparent optimization
 * bug in GCC for ARM Cortex-M3 and higher targets, when multiple
 * byte, half-word and word stores (strb, strh, str instructions),
 * which support unaligned access, can be coalesced into store double
 * (strd) instruction, which doesn't support unaligned access (the
 * compilers in question do this optimization ignoring __packed__
 * attribute).
 */
#define UNALIGNED_PUT(v, p)                                             \
do {                                                                    \
	struct __attribute__((__packed__)) {                            \
		__typeof__(*p) __v;                                     \
	} *__p = (__typeof__(__p)) (p);                                 \
	__p->__v = (v);                                                 \
	compiler_barrier();                                             \
} while (false)

#else

#define UNALIGNED_PUT(v, p)                                             \
do {                                                                    \
	struct __attribute__((__packed__)) {                            \
		__typeof__(*p) __v;                                     \
	} *__p = (__typeof__(__p)) (p);                                 \
	__p->__v = (v);                                               \
} while (false)

#endif

/* Double indirection to ensure section names are expanded before
 * stringification
 */
#define __GENERIC_SECTION(segment) __attribute__((section(STRINGIFY(segment))))
#define Z_GENERIC_SECTION(segment) __GENERIC_SECTION(segment)
#if 0
#define ___in_section(a, b, c) \
	__attribute__((section("." Z_STRINGIFY(a)			\
				"." Z_STRINGIFY(b)			\
				"." Z_STRINGIFY(c))))
#else
#define ___in_section(a, b, c) __attribute__((section("." Z_STRINGIFY(a))))
#endif
#define __in_section(a, b, c) ___in_section(a, b, c)

#define __in_section_unique(seg) ___in_section(seg, __FILE__, __COUNTER__)

/* When using XIP, using '__ramfunc' places a function into RAM instead
 * of FLASH. Make sure '__ramfunc' is defined only when
 * CONFIG_ARCH_HAS_RAMFUNC_SUPPORT is defined, so that the compiler can
 * report an error if '__ramfunc' is used but the architecture does not
 * support it.
 */
#if !defined(CONFIG_XIP)
#define __ramfunc
#elif defined(CONFIG_ARCH_HAS_RAMFUNC_SUPPORT)
#define __ramfunc	__attribute__((noinline))			\
			__attribute__((long_call, section(".ramfunc")))
#endif /* !CONFIG_XIP */

#ifndef __fallthrough
#if __GNUC__ >= 7
#define __fallthrough        __attribute__((fallthrough))
#else
#define __fallthrough
#endif	/* __GNUC__ >= 7 */
#endif

#define __get_section_internal(x, type, name) extern type x[]; static type * name = &x[0]
#define __get_section_start(x, type, name) __get_section_internal(Load$$ER##x##$$Base, type, name)
#define __get_section_end(x, type, name) __get_section_internal(Load$$ER##x##$$Limit, type, name)

#ifdef CONFIG_APPLICATION_MEMORY
#define __kernel	__in_section_unique(kernel)
#define __kernel_noinit	__in_section_unique(kernel_noinit)
#define __kernel_bss	__in_section_unique(kernel_bss)
#else
#define __kernel
#define __kernel_noinit	__noinit
#define __kernel_bss
#endif

#define __noinit

#ifndef _OFF_T_DECLARED
typedef int off_t; /* file offset */
#define _OFF_T_DECLARED
#endif

#ifndef STRUCT_PACKED_PRE
#define STRUCT_PACKED_PRE
#endif

#ifndef STRUCT_PACKED_POST
#define STRUCT_PACKED_POST __attribute__((__packed__))
#endif

#ifndef UNION_PACKED_PRE
#define UNION_PACKED_PRE
#endif

#ifndef UNION_PACKED_POST
#define UNION_PACKED_POST __attribute__((__packed__))
#endif

#ifndef ENUM_PACKED_PRE
#define ENUM_PACKED_PRE
#endif

#ifndef ENUM_PACKED_POST
#define ENUM_PACKED_POST __attribute__((__packed__))
#endif

#ifndef __packed
#define __packed        __attribute__((__packed__))
#endif
#ifndef __aligned
#define __aligned(x)	__attribute__((__aligned__(x)))
#endif
#ifndef __ALIGNED
#define __ALIGNED __aligned
#endif
#define __may_alias     __attribute__((__may_alias__))
#ifndef __printf_like
#define __printf_like(f, a)   __attribute__((format (printf, f, a)))
#endif
#define __used		__attribute__((__used__))
#ifndef __deprecated
#define __deprecated	__attribute__((deprecated))
#endif
#define ARG_UNUSED(x) (void)(x)

#define likely(x)   __builtin_expect((bool)!!(x), true)
#define unlikely(x) __builtin_expect((bool)!!(x), false)

#define popcount(x) __builtin_popcount(x)

#ifndef __no_optimization
#define __no_optimization __attribute__((optimize("-O0")))
#endif

#ifndef __weak
#define __weak __attribute__((__weak__))
#endif
#define __unused __attribute__((__unused__))

/* Builtins with availability that depend on the compiler version. */
#if __GNUC__ >= 5
#define HAS_BUILTIN___builtin_add_overflow 1
#define HAS_BUILTIN___builtin_sub_overflow 1
#define HAS_BUILTIN___builtin_mul_overflow 1
#define HAS_BUILTIN___builtin_div_overflow 1
#endif
#if __GNUC__ >= 4
#define HAS_BUILTIN___builtin_clz 1
#define HAS_BUILTIN___builtin_clzl 1
#define HAS_BUILTIN___builtin_clzll 1
#define HAS_BUILTIN___builtin_ctz 1
#define HAS_BUILTIN___builtin_ctzl 1
#define HAS_BUILTIN___builtin_ctzll 1
#endif

/*
 * Be *very* careful with these. You cannot filter out __DEPRECATED_MACRO with
 * -wno-deprecated, which has implications for -Werror.
 */

/*
 * Expands to nothing and generates a warning. Used like
 *
 *   #define FOO __WARN("Please use BAR instead") ...
 *
 * The warning points to the location where the macro is expanded.
 */
#define __WARN(msg) __WARN1(GCC warning msg)
#define __WARN1(s) _Pragma(#s)

/* Generic message */
#ifndef __DEPRECATED_MACRO
#define __DEPRECATED_MACRO __WARN("Macro is deprecated")
#endif

/* These macros allow having ARM asm functions callable from thumb */

#if defined(_ASMLANGUAGE)

#if defined(CONFIG_ARM) && !defined(CONFIG_ARM64)

#if defined(CONFIG_ASSEMBLER_ISA_THUMB2)

#define FUNC_CODE() .thumb;
#define FUNC_INSTR(a)

#else

#define FUNC_CODE() .code 32
#define FUNC_INSTR(a)

#endif /* CONFIG_ASSEMBLER_ISA_THUMB2 */

#else

#define FUNC_CODE()
#define FUNC_INSTR(a)

#endif /* CONFIG_ARM && !CONFIG_ARM64 */

#endif /* _ASMLANGUAGE */

/*
 * These macros are used to declare assembly language symbols that need
 * to be typed properly(func or data) to be visible to the OMF tool.
 * So that the build tool could mark them as an entry point to be linked
 * correctly.  This is an elfism. Use #if 0 for a.out.
 */

#if defined(_ASMLANGUAGE)

#if defined(CONFIG_ARM) || defined(CONFIG_NIOS2) || defined(CONFIG_RISCV) \
	|| defined(CONFIG_XTENSA)
#define GTEXT(sym) .global sym; .type sym, %function
#define GDATA(sym) .global sym; .type sym, %object
#define WTEXT(sym) .weak sym; .type sym, %function
#define WDATA(sym) .weak sym; .type sym, %object
#elif defined(CONFIG_ARC)
/*
 * Need to use assembly macros because ';' is interpreted as the start of
 * a single line comment in the ARC assembler.
 */

.macro glbl_text symbol
	.globl \symbol
	.type \symbol, %function
.endm

.macro glbl_data symbol
	.globl \symbol
	.type \symbol, %object
.endm

.macro weak_data symbol
	.weak \symbol
	.type \symbol, %object
.endm

#define GTEXT(sym) glbl_text sym
#define GDATA(sym) glbl_data sym
#define WDATA(sym) weak_data sym

#else  /* !CONFIG_ARM && !CONFIG_ARC */
#define GTEXT(sym) .globl sym; .type sym, @function
#define GDATA(sym) .globl sym; .type sym, @object
#endif

/*
 * These macros specify the section in which a given function or variable
 * resides.
 *
 * - SECTION_FUNC	allows only one function to reside in a sub-section
 * - SECTION_SUBSEC_FUNC allows multiple functions to reside in a sub-section
 *   This ensures that garbage collection only discards the section
 *   if all functions in the sub-section are not referenced.
 */

#if defined(CONFIG_ARC)
/*
 * Need to use assembly macros because ';' is interpreted as the start of
 * a single line comment in the ARC assembler.
 *
 * Also, '\()' is needed in the .section directive of these macros for
 * correct substitution of the 'section' variable.
 */

.macro section_var section, symbol
	.section .\section\().\symbol
	\symbol :
.endm

.macro section_func section, symbol
	.section .\section\().\symbol, "ax"
	FUNC_CODE()
	PERFOPT_ALIGN
	\symbol :
	FUNC_INSTR(\symbol)
.endm

.macro section_subsec_func section, subsection, symbol
	.section .\section\().\subsection, "ax"
	PERFOPT_ALIGN
	\symbol :
.endm

#define SECTION_VAR(sect, sym) section_var sect, sym
#define SECTION_FUNC(sect, sym) section_func sect, sym
#define SECTION_SUBSEC_FUNC(sect, subsec, sym) \
	section_subsec_func sect, subsec, sym
#else /* !CONFIG_ARC */

#define SECTION_VAR(sect, sym)  .section .sect.##sym; sym :
#define SECTION_FUNC(sect, sym)						\
	.section .sect.sym, "ax";					\
				FUNC_CODE()				\
				PERFOPT_ALIGN; sym :		\
							FUNC_INSTR(sym)
#define SECTION_SUBSEC_FUNC(sect, subsec, sym)				\
		.section .sect.subsec, "ax"; PERFOPT_ALIGN; sym :

#endif /* CONFIG_ARC */

#endif /* _ASMLANGUAGE */

#if defined(_ASMLANGUAGE)
#if defined(CONFIG_ARM) && !defined(CONFIG_ARM64)
#if defined(CONFIG_ASSEMBLER_ISA_THUMB2)
/* '.syntax unified' is a gcc-ism used in thumb-2 asm files */
#define _ASM_FILE_PROLOGUE .text; .syntax unified; .thumb
#else
#define _ASM_FILE_PROLOGUE .text; .code 32
#endif /* CONFIG_ASSEMBLER_ISA_THUMB2 */
#elif defined(CONFIG_ARM64)
#define _ASM_FILE_PROLOGUE .text
#endif /* CONFIG_ARM64 || (CONFIG_ARM && !CONFIG_ARM64)*/
#endif /* _ASMLANGUAGE */

/*
 * These macros generate absolute symbols for GCC
 */

/* create an extern reference to the absolute symbol */

#define GEN_OFFSET_EXTERN(name) extern const char name[]

#define GEN_ABS_SYM_BEGIN(name) \
	EXTERN_C void name(void); \
	void name(void)         \
	{

#define GEN_ABS_SYM_END }

/*
 * Note that GEN_ABSOLUTE_SYM(), depending on the architecture
 * and toolchain, may restrict the range of values permitted
 * for assignment to the named symbol.
 *
 * For example, on x86, "value" is interpreated as signed
 * 32-bit integer. Passing in an unsigned 32-bit integer
 * with MSB set would result in a negative integer.
 * Moreover, GCC would error out if an integer larger
 * than 2^32-1 is passed as "value".
 */

/*
 * GEN_ABSOLUTE_SYM_KCONFIG() is outputted by the build system
 * to generate named symbol/value pairs for kconfigs.
 */

#if defined(CONFIG_ARM) && !defined(CONFIG_ARM64)

/*
 * GNU/ARM backend does not have a proper operand modifier which does not
 * produces prefix # followed by value, such as %0 for PowerPC, Intel, and
 * MIPS. The workaround performed here is using %B0 which converts
 * the value to ~(value). Thus "n"(~(value)) is set in operand constraint
 * to output (value) in the ARM specific GEN_OFFSET macro.
 */

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%B0"                              \
		"\n\t.type\t" #name ",%%object" :  : "n"(~(value)))

#define GEN_ABSOLUTE_SYM_KCONFIG(name, value)       \
	__asm__(".globl\t" #name                    \
		"\n\t.equ\t" #name "," #value       \
		"\n\t.type\t" #name ",%object")

#elif defined(CONFIG_X86)

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%c0"                              \
		"\n\t.type\t" #name ",@object" :  : "n"(value))

#define GEN_ABSOLUTE_SYM_KCONFIG(name, value)       \
	__asm__(".globl\t" #name                    \
		"\n\t.equ\t" #name "," #value       \
		"\n\t.type\t" #name ",@object")

#elif defined(CONFIG_ARC) || defined(CONFIG_ARM64)

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%c0"                              \
		"\n\t.type\t" #name ",@object" :  : "n"(value))

#define GEN_ABSOLUTE_SYM_KCONFIG(name, value)       \
	__asm__(".globl\t" #name                    \
		"\n\t.equ\t" #name "," #value       \
		"\n\t.type\t" #name ",@object")

#elif defined(CONFIG_NIOS2) || defined(CONFIG_RISCV) || defined(CONFIG_XTENSA)

/* No special prefixes necessary for constants in this arch AFAICT */
#define GEN_ABSOLUTE_SYM(name, value)		\
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%0"                              \
		"\n\t.type\t" #name ",%%object" :  : "n"(value))

#define GEN_ABSOLUTE_SYM_KCONFIG(name, value)       \
	__asm__(".globl\t" #name                    \
		"\n\t.equ\t" #name "," #value       \
		"\n\t.type\t" #name ",%object")

#elif defined(CONFIG_ARCH_POSIX)
#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%c0"                              \
		"\n\t.type\t" #name ",@object" :  : "n"(value))

#define GEN_ABSOLUTE_SYM_KCONFIG(name, value)       \
	__asm__(".globl\t" #name                    \
		"\n\t.equ\t" #name "," #value       \
		"\n\t.type\t" #name ",@object")

#elif defined(CONFIG_SPARC)
#define GEN_ABSOLUTE_SYM(name, value)			\
	__asm__(".global\t" #name "\n\t.equ\t" #name	\
		",%0"					\
		"\n\t.type\t" #name ",#object" : : "n"(value))

#define GEN_ABSOLUTE_SYM_KCONFIG(name, value)       \
	__asm__(".globl\t" #name                    \
		"\n\t.equ\t" #name "," #value       \
		"\n\t.type\t" #name ",#object")

#else
#error processor architecture not supported
#endif

#define compiler_barrier() do { \
	__asm__ __volatile__ ("" ::: "memory"); \
} while (false)

/** @brief Return larger value of two provided expressions.
 *
 * Macro ensures that expressions are evaluated only once.
 *
 * @note Macro has limited usage compared to the standard macro as it cannot be
 *	 used:
 *	 - to generate constant integer, e.g. __aligned(Z_MAX(4,5))
 *	 - static variable, e.g. array like static uint8_t array[Z_MAX(...)];
 */
#define Z_MAX(a, b) ({ \
		/* random suffix to avoid naming conflict */ \
		__typeof__(a) _value_a_ = (a); \
		__typeof__(b) _value_b_ = (b); \
		_value_a_ > _value_b_ ? _value_a_ : _value_b_; \
	})

/** @brief Return smaller value of two provided expressions.
 *
 * Macro ensures that expressions are evaluated only once. See @ref Z_MAX for
 * macro limitations.
 */
#define Z_MIN(a, b) ({ \
		/* random suffix to avoid naming conflict */ \
		__typeof__(a) _value_a_ = (a); \
		__typeof__(b) _value_b_ = (b); \
		_value_a_ < _value_b_ ? _value_a_ : _value_b_; \
	})

/** @brief Return a value clamped to a given range.
 *
 * Macro ensures that expressions are evaluated only once. See @ref Z_MAX for
 * macro limitations.
 */
#define Z_CLAMP(val, low, high) ({                                             \
		/* random suffix to avoid naming conflict */                   \
		__typeof__(val) _value_val_ = (val);                           \
		__typeof__(low) _value_low_ = (low);                           \
		__typeof__(high) _value_high_ = (high);                        \
		(_value_val_ < _value_low_)  ? _value_low_ :                   \
		(_value_val_ > _value_high_) ? _value_high_ :                  \
					       _value_val_;                    \
	})

/**
 * @brief Calculate power of two ceiling for some nonzero value
 *
 * @param x Nonzero unsigned long value
 * @return X rounded up to the next power of two
 */
#ifdef CONFIG_64BIT
#define Z_POW2_CEIL(x) ((1UL << (63U - __builtin_clzl(x))) < x ?  \
		1UL << (63U - __builtin_clzl(x) + 1U) : \
		1UL << (63U - __builtin_clzl(x)))
#else
#define Z_POW2_CEIL(x) ((1UL << (31U - __builtin_clzl(x))) < x ?  \
		1UL << (31U - __builtin_clzl(x) + 1U) : \
		1UL << (31U - __builtin_clzl(x)))
#endif

#endif /* !_LINKER */
#endif /* ZEPHYR_INCLUDE_TOOLCHAIN_GCC_H_ */
