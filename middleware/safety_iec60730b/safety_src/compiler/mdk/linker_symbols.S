;/*******************************************************************************
;*
;* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
;* Copyright 2016-2019 NXP
;*
;* This software is owned or controlled by NXP and may only be used
;* strictly in accordance with the applicable license terms.  By expressly
;* accepting such terms or by downloading, installing, activating and/or
;* otherwise using the software, you are agreeing that you have read, and
;* that you agree to comply with and are bound by, such license terms.  If
;* you do not agree to be bound by the applicable license terms, then you
;* may not retain, install, activate or otherwise use the software.
;* 
;*
;*******************************************************************************/
;/**HEADER********************************************************************
;* 
;* Copyright (c) 2012 Freescale Semiconductor;	  
;* All Rights Reserved
;*
;*************************************************************************** 
;*
;* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
;* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
;* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
;* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
;* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
;* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
;* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
;* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
;* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
;* THE POSSIBILITY OF SUCH DAMAGE.
;*
;**************************************************************************
;*
;* $FileName: linker_symbols.s$
;* $Version : 3.0$
;* $Date    : Nov-01-2018$
;*
;* Comments:
;*
;*END************************************************************************/

  AREA KERNEL_DATA_START, DATA, ALIGN=4, READWRITE
  EXPORT __KERNEL_DATA_START
__KERNEL_DATA_START
  DCD 0


  END

; EOF

