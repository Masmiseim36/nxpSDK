/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VECTORS_H_
#define _VECTORS_H_

#include "safety_config.h" /* for device definition */

/*******************************************************************************
 * API
 ******************************************************************************/
void DefaultISR(void); /* function prototype for DefaultISR in vectors.c */
extern void start(void);

void NMI_isr(void); /* included in vector.c */
void hard_fault_handler_c(uint32_t *hardfault_args);
typedef void pointer(void); /* Interrupt Vector Table Function Pointers */
extern void __startup(void);
extern uint32_t __BOOT_STACK_ADDRESS[];

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//        Address     Vector IRQ   Source module   Source description
#define VECTOR_000 (vector_entry) & Image$$ARM_LIB_STACK$$ZI$$Limit /* Initial SP           */
#define VECTOR_001 (vector_entry) & start                           /* Initial PC           */
#ifndef VECTOR_002
#define VECTOR_002 NMI_isr // 0x0000_0008 2 -          ARM core        Non-maskable Interrupt (NMI)
#endif
#ifndef VECTOR_003
#define VECTOR_003 0 // 0x0000_000C 3 -          ARM core        Hard Fault
#endif
#ifndef VECTOR_004
#define VECTOR_004 0 // 0x0000_0010 4 -
#endif
#ifndef VECTOR_005
#define VECTOR_005 0 // 0x0000_0014 5 -          ARM core         Bus Fault
#endif
#ifndef VECTOR_006
#define VECTOR_006 0 // 0x0000_0018 6 -          ARM core         Usage Fault
#endif
#ifndef VECTOR_007
#define VECTOR_007 0 // 0x0000_001C 7 -
#endif
#ifndef VECTOR_008
#define VECTOR_008 0 // 0x0000_0020 8 -
#endif
#ifndef VECTOR_009
#define VECTOR_009 0 // 0x0000_0024 9 -
#endif
#ifndef VECTOR_010
#define VECTOR_010 0 // 0x0000_0028 10 -
#endif
#ifndef VECTOR_011
#define VECTOR_011 DefaultISR // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
#endif
#ifndef VECTOR_0012
#define VECTOR_012 0 // 0x0000_0030 12 -         ARM core         Debug Monitor
#endif
#ifndef VECTOR_013
#define VECTOR_013 0 // 0x0000_0034 13 -
#endif
#ifndef VECTOR_014
#define VECTOR_014 \
    DefaultISR // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
#endif
#ifndef VECTOR_015
#define VECTOR_015 DefaultISR // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
#endif
#ifndef VECTOR_016
#define VECTOR_016 DefaultISR // 0x0000_0040
#endif
#ifndef VECTOR_017
#define VECTOR_017 DefaultISR // 0x0000_0044
#endif
#ifndef VECTOR_018
#define VECTOR_018 DefaultISR // 0x0000_0048
#endif
#ifndef VECTOR_019
#define VECTOR_019 DefaultISR // 0x0000_004C
#endif
#ifndef VECTOR_020
#define VECTOR_020 DefaultISR // 0x0000_0050
#endif
#ifndef VECTOR_021
#define VECTOR_021 DefaultISR // 0x0000_0054
#endif
#ifndef VECTOR_022
#define VECTOR_022 DefaultISR // 0x0000_0058
#endif
#ifndef VECTOR_023
#define VECTOR_023 DefaultISR // 0x0000_005C
#endif
#ifndef VECTOR_024
#define VECTOR_024 DefaultISR // 0x0000_0060
#endif
#ifndef VECTOR_025
#define VECTOR_025 DefaultISR // 0x0000_0064
#endif
#ifndef VECTOR_026
#define VECTOR_026 DefaultISR // 0x0000_0068
#endif
#ifndef VECTOR_027
#define VECTOR_027 DefaultISR // 0x0000_006C
#endif
#ifndef VECTOR_028
#define VECTOR_028 DefaultISR // 0x0000_0070
#endif
#ifndef VECTOR_029
#define VECTOR_029 DefaultISR // 0x0000_0074
#endif
#ifndef VECTOR_030
#define VECTOR_030 DefaultISR // 0x0000_0078
#endif
#ifndef VECTOR_031
#define VECTOR_031 DefaultISR // 0x0000_007C
#endif
#ifndef VECTOR_032
#define VECTOR_032 DefaultISR // 0x0000_0080
#endif
#ifndef VECTOR_033
#define VECTOR_033 DefaultISR // 0x0000_0084
#endif
#ifndef VECTOR_034
#define VECTOR_034 DefaultISR // 0x0000_0088
#endif
#ifndef VECTOR_035
#define VECTOR_035 DefaultISR // 0x0000_008C
#endif
#ifndef VECTOR_036
#define VECTOR_036 DefaultISR // 0x0000_0090
#endif
#ifndef VECTOR_037
#define VECTOR_037 DefaultISR // 0x0000_0094
#endif
#ifndef VECTOR_038
#define VECTOR_038 DefaultISR // 0x0000_0098
#endif
#ifndef VECTOR_039
#define VECTOR_039 DefaultISR // 0x0000_009C
#endif
#ifndef VECTOR_040
#define VECTOR_040 DefaultISR // 0x0000_00A0
#endif
#ifndef VECTOR_041
#define VECTOR_041 DefaultISR // 0x0000_00A4
#endif
#ifndef VECTOR_042
#define VECTOR_042 DefaultISR // 0x0000_00A8
#endif
#ifndef VECTOR_043
#define VECTOR_043 DefaultISR // 0x0000_00AC
#endif
#ifndef VECTOR_044
#define VECTOR_044 DefaultISR // 0x0000_00B0
#endif
#ifndef VECTOR_045
#define VECTOR_045 DefaultISR // 0x0000_00B4
#endif
#ifndef VECTOR_046
#define VECTOR_046 DefaultISR // 0x0000_00B8
#endif
#ifndef VECTOR_047
#define VECTOR_047 DefaultISR // 0x0000_00BC
#endif
#ifndef VECTOR_048
#define VECTOR_048 DefaultISR // 0x0000_00C0
#endif
#ifndef VECTOR_049
#define VECTOR_049 DefaultISR // 0x0000_00C4
#endif
#ifndef VECTOR_050
#define VECTOR_050 DefaultISR // 0x0000_00C8
#endif
#ifndef VECTOR_051
#define VECTOR_051 DefaultISR // 0x0000_00CC
#endif
#ifndef VECTOR_052
#define VECTOR_052 DefaultISR // 0x0000_00D0
#endif
#ifndef VECTOR_053
#define VECTOR_053 DefaultISR // 0x0000_00D4
#endif
#ifndef VECTOR_054
#define VECTOR_054 DefaultISR // 0x0000_00D8
#endif
#ifndef VECTOR_055
#define VECTOR_055 DefaultISR // 0x0000_00DC
#endif
#ifndef VECTOR_056
#define VECTOR_056 DefaultISR // 0x0000_00E0
#endif
#ifndef VECTOR_057
#define VECTOR_057 DefaultISR // 0x0000_00E4
#endif
#ifndef VECTOR_058
#define VECTOR_058 DefaultISR // 0x0000_00E8
#endif
#ifndef VECTOR_059
#define VECTOR_059 DefaultISR // 0x0000_00EC
#endif
#ifndef VECTOR_060
#define VECTOR_060 DefaultISR // 0x0000_00F0
#endif
#ifndef VECTOR_061
#define VECTOR_061 DefaultISR // 0x0000_00F4
#endif
#ifndef VECTOR_062
#define VECTOR_062 DefaultISR // 0x0000_00F8
#endif
#ifndef VECTOR_063
#define VECTOR_063 DefaultISR // 0x0000_00FC
#endif
#ifndef VECTOR_064
#define VECTOR_064 DefaultISR // 0x0000_0100
#endif
#ifndef VECTOR_065
#define VECTOR_065 DefaultISR // 0x0000_0104
#endif
#ifndef VECTOR_066
#define VECTOR_066 DefaultISR // 0x0000_0108
#endif
#ifndef VECTOR_067
#define VECTOR_067 DefaultISR // 0x0000_010C
#endif
#ifndef VECTOR_068
#define VECTOR_068 DefaultISR // 0x0000_0110
#endif
#ifndef VECTOR_069
#define VECTOR_069 DefaultISR // 0x0000_0114
#endif
#ifndef VECTOR_070
#define VECTOR_070 DefaultISR // 0x0000_0118
#endif
#ifndef VECTOR_071
#define VECTOR_071 DefaultISR // 0x0000_011C
#endif
#ifndef VECTOR_072
#define VECTOR_072 DefaultISR // 0x0000_0120
#endif
#ifndef VECTOR_073
#define VECTOR_073 DefaultISR // 0x0000_0124
#endif
#ifndef VECTOR_074
#define VECTOR_074 DefaultISR // 0x0000_0128
#endif
#ifndef VECTOR_075
#define VECTOR_075 DefaultISR // 0x0000_012C
#endif
#ifndef VECTOR_076
#define VECTOR_076 DefaultISR // 0x0000_0130
#endif
#ifndef VECTOR_077
#define VECTOR_077 DefaultISR // 0x0000_0134
#endif
#ifndef VECTOR_078
#define VECTOR_078 DefaultISR // 0x0000_0138
#endif
#ifndef VECTOR_079
#define VECTOR_079 DefaultISR // 0x0000_013C
#endif
#ifndef VECTOR_080
#define VECTOR_080 DefaultISR // 0x0000_0140
#endif
#ifndef VECTOR_081
#define VECTOR_081 DefaultISR // 0x0000_0144
#endif
#ifndef VECTOR_082
#define VECTOR_082 DefaultISR // 0x0000_0148
#endif
#ifndef VECTOR_083
#define VECTOR_083 DefaultISR // 0x0000_014C
#endif
#ifndef VECTOR_084
#define VECTOR_084 DefaultISR // 0x0000_0150
#endif
#ifndef VECTOR_085
#define VECTOR_085 DefaultISR // 0x0000_0154
#endif
#ifndef VECTOR_086
#define VECTOR_086 DefaultISR // 0x0000_0158
#endif
#ifndef VECTOR_087
#define VECTOR_087 DefaultISR // 0x0000_015C
#endif
#ifndef VECTOR_088
#define VECTOR_088 DefaultISR // 0x0000_0160
#endif
#ifndef VECTOR_089
#define VECTOR_089 DefaultISR // 0x0000_0164
#endif
#ifndef VECTOR_090
#define VECTOR_090 DefaultISR // 90 -
#endif
#ifndef VECTOR_091
#define VECTOR_091 DefaultISR // 91 -
#endif
#ifndef VECTOR_092
#define VECTOR_092 DefaultISR // 92 -
#endif
#ifndef VECTOR_093
#define VECTOR_093 DefaultISR // 93 -
#endif
#ifndef VECTOR_094
#define VECTOR_094 DefaultISR // 94 -
#endif
#ifndef VECTOR_095
#define VECTOR_095 DefaultISR // 95 -
#endif
#ifndef VECTOR_096
#define VECTOR_096 DefaultISR // 96 -
#endif
#ifndef VECTOR_097
#define VECTOR_097 DefaultISR // 97 -
#endif
#ifndef VECTOR_098
#define VECTOR_098 DefaultISR // 98 -
#endif
#ifndef VECTOR_099
#define VECTOR_099 DefaultISR // 99 -
#endif
#ifndef VECTOR_100
#define VECTOR_100 DefaultISR // 100 -
#endif
#ifndef VECTOR_101
#define VECTOR_101 DefaultISR // 101 -
#endif
#ifndef VECTOR_102
#define VECTOR_102 DefaultISR // 102 -
#endif
#ifndef VECTOR_103
#define VECTOR_103 DefaultISR // 103 -
#endif
#ifndef VECTOR_104
#define VECTOR_104 DefaultISR // 104 -
#endif
#ifndef VECTOR_105
#define VECTOR_105 DefaultISR // 105 -
#endif
#ifndef VECTOR_106
#define VECTOR_106 DefaultISR // 106 -
#endif
#ifndef VECTOR_107
#define VECTOR_107 DefaultISR // 107 -
#endif
#ifndef VECTOR_108
#define VECTOR_108 DefaultISR // 108 -
#endif
#ifndef VECTOR_109
#define VECTOR_109 DefaultISR // 109 -
#endif
#ifndef VECTOR_110
#define VECTOR_110 DefaultISR // 110 -
#endif
#ifndef VECTOR_111
#define VECTOR_111 DefaultISR // 111 -
#endif
#ifndef VECTOR_112
#define VECTOR_112 DefaultISR // 112 -
#endif
#ifndef VECTOR_113
#define VECTOR_113 DefaultISR // 113 -
#endif
#ifndef VECTOR_114
#define VECTOR_114 DefaultISR // 114 -
#endif
#ifndef VECTOR_115
#define VECTOR_115 DefaultISR // 115 -
#endif
#ifndef VECTOR_116
#define VECTOR_116 DefaultISR // 116 -
#endif
#ifndef VECTOR_117
#define VECTOR_117 DefaultISR // 117 -
#endif
#ifndef VECTOR_118
#define VECTOR_118 DefaultISR // 118 -
#endif
#ifndef VECTOR_119
#define VECTOR_119 DefaultISR // 119 -
#endif
#ifndef VECTOR_120
#define VECTOR_120 DefaultISR // 120 -
#endif
#ifndef VECTOR_121
#define VECTOR_121 DefaultISR // 121 -
#endif
#ifndef VECTOR_122
#define VECTOR_122 DefaultISR // 122 -
#endif
#ifndef VECTOR_123
#define VECTOR_123 DefaultISR // 123 -
#endif
#ifndef VECTOR_124
#define VECTOR_124 DefaultISR // 124 -
#endif
#ifndef VECTOR_125
#define VECTOR_125 DefaultISR // 125 -
#endif
#ifndef VECTOR_126
#define VECTOR_126 DefaultISR // 126 -
#endif
#ifndef VECTOR_127
#define VECTOR_127 DefaultISR // 127 -
#endif
#ifndef VECTOR_128
#define VECTOR_128 DefaultISR // 128 -
#endif
#ifndef VECTOR_129
#define VECTOR_129 DefaultISR // 129 -
#endif
#ifndef VECTOR_130
#define VECTOR_130 DefaultISR // 130 -
#endif
#ifndef VECTOR_131
#define VECTOR_131 DefaultISR // 131 -
#endif
#ifndef VECTOR_132
#define VECTOR_132 DefaultISR // 132 -
#endif
#ifndef VECTOR_133
#define VECTOR_133 DefaultISR // 133 -
#endif
#ifndef VECTOR_134
#define VECTOR_134 DefaultISR // 134 -
#endif
#ifndef VECTOR_135
#define VECTOR_135 DefaultISR // 135 -
#endif
#ifndef VECTOR_136
#define VECTOR_136 DefaultISR // 136 -
#endif
#ifndef VECTOR_137
#define VECTOR_137 DefaultISR // 137 -
#endif
#ifndef VECTOR_138
#define VECTOR_138 DefaultISR // 138 -
#endif
#ifndef VECTOR_139
#define VECTOR_139 DefaultISR // 139 -
#endif
#ifndef VECTOR_140
#define VECTOR_140 DefaultISR // 140 -
#endif
#ifndef VECTOR_141
#define VECTOR_141 DefaultISR // 141 -
#endif
#ifndef VECTOR_142
#define VECTOR_142 DefaultISR // 142 -
#endif
#ifndef VECTOR_143
#define VECTOR_143 DefaultISR // 143 -
#endif
#ifndef VECTOR_144
#define VECTOR_144 DefaultISR // 144 -
#endif
#ifndef VECTOR_145
#define VECTOR_145 DefaultISR // 145 -
#endif
#ifndef VECTOR_146
#define VECTOR_146 DefaultISR // 146 -
#endif
#ifndef VECTOR_147
#define VECTOR_147 DefaultISR // 147 -
#endif
#ifndef VECTOR_148
#define VECTOR_148 DefaultISR // 148 -
#endif
#ifndef VECTOR_149
#define VECTOR_149 DefaultISR // 149 -
#endif
#ifndef VECTOR_150
#define VECTOR_150 DefaultISR // 150 -
#endif
#ifndef VECTOR_151
#define VECTOR_151 DefaultISR // 151 -
#endif
#ifndef VECTOR_152
#define VECTOR_152 DefaultISR // 152 -
#endif
#ifndef VECTOR_153
#define VECTOR_153 DefaultISR // 153 -
#endif
#ifndef VECTOR_154
#define VECTOR_154 DefaultISR // 154 -
#endif
#ifndef VECTOR_155
#define VECTOR_155 DefaultISR // 155 -
#endif
#ifndef VECTOR_156
#define VECTOR_156 DefaultISR // 156 -
#endif
#ifndef VECTOR_157
#define VECTOR_157 DefaultISR // 157 -
#endif
#ifndef VECTOR_158
#define VECTOR_158 DefaultISR // 158 -
#endif
#ifndef VECTOR_159
#define VECTOR_159 DefaultISR // 159 -
#endif
#ifndef VECTOR_160
#define VECTOR_160 DefaultISR // 160 -
#endif
#ifndef VECTOR_161
#define VECTOR_161 DefaultISR // 161 -
#endif
#ifndef VECTOR_162
#define VECTOR_162 DefaultISR // 162 -
#endif
#ifndef VECTOR_163
#define VECTOR_163 DefaultISR // 163 -
#endif
#ifndef VECTOR_164
#define VECTOR_164 DefaultISR // 164 -
#endif
#ifndef VECTOR_165
#define VECTOR_165 DefaultISR // 165 -
#endif
#ifndef VECTOR_166
#define VECTOR_166 DefaultISR // 166 -
#endif
#ifndef VECTOR_167
#define VECTOR_167 DefaultISR // 167 -
#endif
#ifndef VECTOR_168
#define VECTOR_168 DefaultISR // 168 -
#endif
#ifndef VECTOR_169
#define VECTOR_169 DefaultISR // 169 -
#endif
#ifndef VECTOR_170
#define VECTOR_170 DefaultISR // 170 -
#endif
#ifndef VECTOR_171
#define VECTOR_171 DefaultISR // 171 -
#endif
#ifndef VECTOR_172
#define VECTOR_172 DefaultISR // 172 -
#endif
#ifndef VECTOR_173
#define VECTOR_173 DefaultISR // 173 -
#endif
#ifndef VECTOR_174
#define VECTOR_174 DefaultISR // 174 -
#endif
#ifndef VECTOR_175
#define VECTOR_175 DefaultISR // 175 -
#endif
#ifndef VECTOR_176
#define VECTOR_176 DefaultISR
#endif
#ifndef VECTOR_177
#define VECTOR_177 DefaultISR
#endif
#ifndef VECTOR_178
#define VECTOR_178 DefaultISR
#endif
#ifndef VECTOR_179
#define VECTOR_179 DefaultISR
#endif
#ifndef VECTOR_180
#define VECTOR_180 DefaultISR
#endif
#ifndef VECTOR_181
#define VECTOR_181 DefaultISR
#endif
#ifndef VECTOR_182
#define VECTOR_182 DefaultISR
#endif
#ifndef VECTOR_183
#define VECTOR_183 DefaultISR
#endif
#ifndef VECTOR_184
#define VECTOR_184 DefaultISR
#endif
#ifndef VECTOR_185
#define VECTOR_185 DefaultISR
#endif
#ifndef VECTOR_186
#define VECTOR_186 DefaultISR
#endif
#ifndef VECTOR_187
#define VECTOR_187 DefaultISR
#endif
#ifndef VECTOR_188
#define VECTOR_188 DefaultISR
#endif
#ifndef VECTOR_189
#define VECTOR_189 DefaultISR
#endif
#ifndef VECTOR_190
#define VECTOR_190 DefaultISR
#endif
#ifndef VECTOR_191
#define VECTOR_191 DefaultISR
#endif
#ifndef VECTOR_192
#define VECTOR_192 DefaultISR
#endif
#ifndef VECTOR_193
#define VECTOR_193 DefaultISR
#endif
#ifndef VECTOR_194
#define VECTOR_194 DefaultISR
#endif
#ifndef VECTOR_195
#define VECTOR_195 DefaultISR
#endif
#ifndef VECTOR_196
#define VECTOR_196 DefaultISR
#endif
#ifndef VECTOR_197
#define VECTOR_197 DefaultISR
#endif
#ifndef VECTOR_198
#define VECTOR_198 DefaultISR
#endif
#ifndef VECTOR_199
#define VECTOR_199 DefaultISR
#endif
#ifndef VECTOR_200
#define VECTOR_200 DefaultISR
#endif
#ifndef VECTOR_201
#define VECTOR_201 DefaultISR
#endif
#ifndef VECTOR_202
#define VECTOR_202 DefaultISR
#endif
#ifndef VECTOR_203
#define VECTOR_203 DefaultISR
#endif
#ifndef VECTOR_204
#define VECTOR_204 DefaultISR
#endif
#ifndef VECTOR_205
#define VECTOR_205 DefaultISR
#endif
#ifndef VECTOR_206
#define VECTOR_206 DefaultISR
#endif
#ifndef VECTOR_207
#define VECTOR_207 DefaultISR
#endif
#ifndef VECTOR_208
#define VECTOR_208 DefaultISR
#endif
#ifndef VECTOR_209
#define VECTOR_209 DefaultISR
#endif
#ifndef VECTOR_210
#define VECTOR_210 DefaultISR
#endif
#ifndef VECTOR_211
#define VECTOR_211 DefaultISR
#endif
#ifndef VECTOR_212
#define VECTOR_212 DefaultISR
#endif
#ifndef VECTOR_213
#define VECTOR_213 DefaultISR
#endif
#ifndef VECTOR_214
#define VECTOR_214 DefaultISR
#endif
#ifndef VECTOR_215
#define VECTOR_215 DefaultISR
#endif
#ifndef VECTOR_216
#define VECTOR_216 DefaultISR
#endif
#ifndef VECTOR_217
#define VECTOR_217 DefaultISR
#endif
#ifndef VECTOR_218
#define VECTOR_218 DefaultISR
#endif
#ifndef VECTOR_219
#define VECTOR_219 DefaultISR
#endif
#ifndef VECTOR_220
#define VECTOR_220 DefaultISR
#endif
#ifndef VECTOR_221
#define VECTOR_221 DefaultISR
#endif
#ifndef VECTOR_222
#define VECTOR_222 DefaultISR
#endif
#ifndef VECTOR_223
#define VECTOR_223 DefaultISR
#endif
#ifndef VECTOR_224
#define VECTOR_224 DefaultISR
#endif
#ifndef VECTOR_225
#define VECTOR_225 DefaultISR
#endif
#ifndef VECTOR_226
#define VECTOR_226 DefaultISR
#endif
#ifndef VECTOR_227
#define VECTOR_227 DefaultISR
#endif
#ifndef VECTOR_228
#define VECTOR_228 DefaultISR
#endif
#ifndef VECTOR_229
#define VECTOR_229 DefaultISR
#endif
#ifndef VECTOR_230
#define VECTOR_230 DefaultISR
#endif
#ifndef VECTOR_231
#define VECTOR_231 DefaultISR
#endif
#ifndef VECTOR_232
#define VECTOR_232 DefaultISR
#endif
#ifndef VECTOR_233
#define VECTOR_233 DefaultISR
#endif
#ifndef VECTOR_234
#define VECTOR_234 DefaultISR
#endif
#ifndef VECTOR_235
#define VECTOR_235 DefaultISR
#endif
#ifndef VECTOR_236
#define VECTOR_236 DefaultISR
#endif
#ifndef VECTOR_237
#define VECTOR_237 DefaultISR
#endif
#ifndef VECTOR_238
#define VECTOR_238 DefaultISR
#endif
#ifndef VECTOR_239
#define VECTOR_239 DefaultISR
#endif
#ifndef VECTOR_240
#define VECTOR_240 DefaultISR
#endif
#ifndef VECTOR_241
#define VECTOR_241 DefaultISR
#endif
#ifndef VECTOR_242
#define VECTOR_242 DefaultISR
#endif
#ifndef VECTOR_243
#define VECTOR_243 DefaultISR
#endif
#ifndef VECTOR_244
#define VECTOR_244 DefaultISR
#endif
#ifndef VECTOR_245
#define VECTOR_245 DefaultISR
#endif
#ifndef VECTOR_246
#define VECTOR_246 DefaultISR
#endif
#ifndef VECTOR_247
#define VECTOR_247 DefaultISR
#endif
#ifndef VECTOR_248
#define VECTOR_248 DefaultISR
#endif
#ifndef VECTOR_249
#define VECTOR_249 DefaultISR
#endif
#ifndef VECTOR_250
#define VECTOR_250 DefaultISR
#endif
#ifndef VECTOR_251
#define VECTOR_251 DefaultISR
#endif
#ifndef VECTOR_252
#define VECTOR_252 DefaultISR
#endif
#ifndef VECTOR_253
#define VECTOR_253 DefaultISR
#endif
#ifndef VECTOR_254
#define VECTOR_254 DefaultISR
#endif
#ifndef VECTOR_255
#define VECTOR_255 DefaultISR
#endif

#if (!defined(_IMX_))
#define CONFIG_1 (unsigned long)0xFFFFFFFF
#define CONFIG_2 (unsigned long)0xFFFFFFFF
#define CONFIG_3 (unsigned long)0xFFFFFFFF
#define CONFIG_4 (unsigned long)0xFFFFFFFE
#endif

#endif /* _VECTORS_H_ */
