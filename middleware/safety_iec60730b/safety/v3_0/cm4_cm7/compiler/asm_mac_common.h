/**HEADER*********************************************************************
*
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
*
*
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
* $FileName: asm_mac.h$
* $Version : 3.0$
* $Date    : Nov-01-2018$
*
* Comments:
*   This file contains macros used by the GCC/IAR and KEIL assembler
*
*END*************************************************************************/

#ifndef __asm_mac_h__
#define __asm_mac_h__   1

#if defined(__IAR_SYSTEMS_ICC__) || defined(__IAR_SYSTEMS_ASM__) /* IAR */
	#define ASM_PREFIX(x)         x
	#define ASM_LABEL(value)      value
	#define ASM_ALIGN(value)      ALIGNROM  value/4
	#define ASM_PUBLIC(label)     PUBLIC label
	#define ASM_RAM_SECTION(label)     SECTION label : DATA (4)
	#define ASM_CODE_SECTION(label)    SECTION label : CODE (4)
	#define ASM_IMPORT_FUNC(label)     import  label
	#define ASM_END                    END
	#define ASM_COMP_SPECIFIC_DIRECTIVES
	#define ASM_OFFSET     1

	/* Note that these macros should NOT be on the beggining of line when used
	   in assembler code. Prepend allways by at least one space.
	   (was not an issue in EWARM 6.40.x, space seems to be needed in 6.50.x) */
	#define ASM_APSR_MASK
	#define ASM_LITERAL     LTORG
	#define ASM_PUBLIC_BEGIN(name)
	#define ASM_PUBLIC_FUNC(name)
	#define ASM_PUBLIC_END(name)

	#define ASM_STATIC_BEGIN(name)
	#define ASM_STATIC_FUNC(name)
	#define ASM_STATIC_END(name)

#else /* MCUXpresso */
	#define ASM_PREFIX(x)   x
	#define ASM_ALIGN(value)        .balign value
	#define ASM_PUBLIC(label)       .global ASM_PREFIX(label)
	#define ASM_LABEL(label)        label:
	#define ASM_OFFSET				0

	#define ASM_CODE_SECTION(name)  .section name
	#define ASM_END                 .end

	#define ASM_COMP_SPECIFIC_DIRECTIVES   .syntax unified
	#define ASM_REG(APSR)   APSR_nzcvq

	#define ASM_LITERAL
	#define ASM_PUBLIC_BEGIN(name) .thumb_func
	#define ASM_PUBLIC_FUNC(name)
	#define ASM_PUBLIC_END(name)
#endif

#endif /* __asm_mac_h__ */
