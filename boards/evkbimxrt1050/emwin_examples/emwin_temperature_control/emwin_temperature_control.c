/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "emwin_support.h"

#include "DIALOG.h"
#include "Resource.h"

#include <stdio.h>

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_gpt.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_PXP PXP
#define EXAMPLE_GPT GPT2
#define EXAMPLE_GPT_TICK_TO_MS 25

//
// Widget IDs
//
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_KNOB_0 (GUI_ID_USER + 0x01)
#define ID_WINDOW_1 (GUI_ID_USER + 0x02)

#define ID_BUTTON_0 (GUI_ID_USER + 0x11)
#define ID_BUTTON_1 (GUI_ID_USER + 0x12)
#define ID_BUTTON_2 (GUI_ID_USER + 0x13)
#define ID_BUTTON_3 (GUI_ID_USER + 0x14)
#define ID_BUTTON_4 (GUI_ID_USER + 0x15)
#define ID_BUTTON_5 (GUI_ID_USER + 0x16)
#define ID_BUTTON_6 (GUI_ID_USER + 0x17)

#define ID_IMAGE_0 (GUI_ID_USER + 0x20)

#define ID_TEMPERATURE (GUI_ID_USER + 0x30)

//
// Some dimension defines
//
#define KNOB_X0 307
#define KNOB_Y0 92

#define KNOB_OFFSET 450
#define FAN_CONTROL_0_X0 KNOB_X0 + 20
#define FAN_CONTROL_0_Y0 KNOB_Y0 + 255
#define FAN_CONTROL_SIZE_X 50
#define FAN_CONTROL_SIZE_Y 22
#define FAN_CONTROL_1_X0 FAN_CONTROL_0_X0 + FAN_CONTROL_SIZE_X + 1
#define FAN_CONTROL_1_Y0 FAN_CONTROL_0_Y0

//
// Colors
//
#define DARK_BLUE GUI_MAKE_COLOR(0x613600)
#define LIGHT_BLUE GUI_MAKE_COLOR(0xaa7d67)
#define BLUE GUI_MAKE_COLOR(0x855a41)
#define LEMON GUI_MAKE_COLOR(0x00d6d3)
#define LIGHTGRAY GUI_MAKE_COLOR(0xEEEEEE)

#define MAX_TEMPERATURE 30
#define MIN_TEMPERATURE 2

/*********************************************************************
 *
 *       Structures
 *
 **********************************************************************
 */
typedef struct
{
    int ID;              // Id of button
    char aName[20];      // Name to be displayed
    int RealTemperature; // Current temperature
    int SetTemperature;  // Temperature to be achieved
    int Focus;           // Is this button focused
    int FanState;        // Set the state of the fan button, 3 states: 0-2
} ROOM_DATA;

typedef struct
{
    int ID;
    int Focus;
    char aName[20];
} TOGGLE_BUTTON;

/*********************************************************************
 *
 *       Static data
 *
 **********************************************************************
 */
//
// Fill ROOM_DATA struct
//
static ROOM_DATA _aRooms[5] = {
    {0, "Bedroom", 14, 0, 1, 0},        {0, "Living Room", 21, 0, 0, 0},    {0, "Kitchen", 19, 0, 0, 0},
    {0, "Child's Room 1", 21, 0, 0, 0}, {0, "Child's Room 2", 22, 0, 0, 0},
};

//
// Fill toggle button structure
//
static TOGGLE_BUTTON _aButton[3] = {
    {0, 0, "On"},
    {0, 0, "Auto"},
};

//
// Set pointer to a font, used for an easier exchange of fonts
//
static GUI_CONST_STORAGE GUI_FONT *pFont23pBold = &GUI_Font_AA4_16_B;
static GUI_CONST_STORAGE GUI_FONT *pFont32pBold = &GUI_Font_AA4_16_B;
static GUI_CONST_STORAGE GUI_FONT *pFont23p     = &GUI_Font_AA4_16_L;
static GUI_CONST_STORAGE GUI_FONT *pFont32p     = &GUI_Font_AA4_16_L;

/*********************************************************************
 *
 *       _aDialogCreate
 */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Bedroom", ID_BUTTON_0, 0, 48, 240, 45, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Living Room", ID_BUTTON_1, 0, 93, 240, 45, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Kitchen", ID_BUTTON_2, 0, 138, 240, 45, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Child's Room 1", ID_BUTTON_3, 0, 183, 240, 45, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Child's Room 2", ID_BUTTON_4, 0, 228, 240, 44, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "On", ID_BUTTON_5, 307, 230, FAN_CONTROL_SIZE_X, FAN_CONTROL_SIZE_Y, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Auto", ID_BUTTON_6, 361, 230, FAN_CONTROL_SIZE_X, FAN_CONTROL_SIZE_Y, 0, 0x0, 0},
    {IMAGE_CreateIndirect, "", ID_IMAGE_0, 395, 4, 80, 40, 0, 0x0, 0}};

/*********************************************************************
 *
 *       _aGradient
 */
static const GUI_COLOR _aGradient[] = {
#if GUI_USE_ARGB
    0xFF4C94BC, 0xFF4C94BC, 0xFF4C94BB, 0xFF4C94BA, 0xFF4C94BA, 0xFF4C94B9, 0xFF4C94B9, 0xFF4C94B8, 0xFF4C94B7,
    0xFF4C94B7, 0xFF4C94B6, 0xFF4C94B5, 0xFF4C94B5, 0xFF4C94B4, 0xFF4C94B3, 0xFF4C94B3, 0xFF4C94B2, 0xFF4C94B1,
    0xFF4C94B0, 0xFF4C94AF, 0xFF4C94AE, 0xFF4C94AE, 0xFF4C94AD, 0xFF4C94AC, 0xFF4C94AB, 0xFF4C94AA, 0xFF4C94A9,
    0xFF4C94A8, 0xFF4C94A7, 0xFF4C94A6, 0xFF4C94A5, 0xFF4C94A4, 0xFF4C94A3, 0xFF4C94A2, 0xFF4C94A1, 0xFF4C94A0,
    0xFF4C949F, 0xFF4C949E, 0xFF4C949D, 0xFF4C949C, 0xFF4C949B, 0xFF4C949A, 0xFF4C9499, 0xFF4C9498, 0xFF4C9497,
    0xFF4C9496, 0xFF4C9494, 0xFF4C9493, 0xFF4C9492, 0xFF4C9491, 0xFF4C9490, 0xFF4C948F, 0xFF4C948E, 0xFF4C948C,
    0xFF4C948B, 0xFF4C948A, 0xFF4C9489, 0xFF4C9488, 0xFF4C9487, 0xFF4C9485, 0xFF4C9484, 0xFF4C9483, 0xFF4C9482,
    0xFF4C9481, 0xFF4C947F, 0xFF4C947E, 0xFF4C947D, 0xFF4C947C, 0xFF4C947B, 0xFF4C9479, 0xFF4C9478, 0xFF4C9477,
    0xFF4C9476, 0xFF4C9475, 0xFF4C9474, 0xFF4C9472, 0xFF4C9471, 0xFF4C9470, 0xFF4C946F, 0xFF4C946E, 0xFF4C946D,
    0xFF4C946B, 0xFF4C946A, 0xFF4C9469, 0xFF4C9468, 0xFF4C9467, 0xFF4C9466, 0xFF4C9465, 0xFF4C9463, 0xFF4C9462,
    0xFF4C9461, 0xFF4C9460, 0xFF4C945F, 0xFF4C945E, 0xFF4C945D, 0xFF4C945C, 0xFF4C945B, 0xFF4C945A, 0xFF4C9459,
    0xFF4C9458, 0xFF4C9457, 0xFF4C9456, 0xFF4C9455, 0xFF4C9454, 0xFF4C9453, 0xFF4C9452, 0xFF4C9451, 0xFF4C9450,
    0xFF4C944F, 0xFF4C944E, 0xFF4C944D, 0xFF4C944C, 0xFF4C944C, 0xFF4C944B, 0xFF4C944A, 0xFF4C9449, 0xFF4C9448,
    0xFF4C9447, 0xFF4C9447, 0xFF4C9446, 0xFF4C9445, 0xFF4C9444, 0xFF4C9444, 0xFF4C9443, 0xFF4C9442, 0xFF4C9442,
    0xFF4C9441, 0xFF4C9441, 0xFF4C9440, 0xFF4D943F, 0xFF4D943F, 0xFF4E943E, 0xFF4F943E, 0xFF4F933D, 0xFF50933D,
    0xFF51933C, 0xFF52933C, 0xFF52933B, 0xFF53933B, 0xFF54933A, 0xFF55933A, 0xFF569239, 0xFF569239, 0xFF579239,
    0xFF589238, 0xFF599238, 0xFF5A9237, 0xFF5B9137, 0xFF5C9137, 0xFF5D9136, 0xFF5E9136, 0xFF5F9135, 0xFF609135,
    0xFF619035, 0xFF629034, 0xFF639034, 0xFF649034, 0xFF669033, 0xFF678F33, 0xFF688F33, 0xFF698F33, 0xFF6A8F32,
    0xFF6B8F32, 0xFF6D8E32, 0xFF6E8E32, 0xFF6F8E31, 0xFF708E31, 0xFF718D31, 0xFF738D31, 0xFF748D30, 0xFF758D30,
    0xFF778D30, 0xFF788C30, 0xFF798C30, 0xFF7A8C2F, 0xFF7C8C2F, 0xFF7D8B2F, 0xFF7E8B2F, 0xFF808B2F, 0xFF818B2E,
    0xFF828A2E, 0xFF848A2E, 0xFF858A2E, 0xFF868A2E, 0xFF88892E, 0xFF89892E, 0xFF8A892D, 0xFF8C892D, 0xFF8D882D,
    0xFF8F882D, 0xFF90882D, 0xFF91882D, 0xFF93882D, 0xFF94872D, 0xFF95872D, 0xFF97872C, 0xFF98872C, 0xFF9A862C,
    0xFF9B862C, 0xFF9C862C, 0xFF9E862C, 0xFF9F852C, 0xFFA0852C, 0xFFA2852C, 0xFFA3852C, 0xFFA4842C, 0xFFA6842C,
    0xFFA7842C, 0xFFA8842C, 0xFFAA832B, 0xFFAB832B, 0xFFAC832B, 0xFFAD832B, 0xFFAF822B, 0xFFB0822B, 0xFFB1822B,
    0xFFB3822B, 0xFFB4822B, 0xFFB5812B, 0xFFB6812B, 0xFFB7812B, 0xFFB9812B, 0xFFBA802B, 0xFFBB802B, 0xFFBC802B,
    0xFFBD802B, 0xFFBE802B, 0xFFC07F2B, 0xFFC17F2B, 0xFFC27F2B, 0xFFC37F2B, 0xFFC47F2A, 0xFFC57E2A, 0xFFC67E2A,
    0xFFC77E2A, 0xFFC87E2A, 0xFFC97E2A, 0xFFCA7E2A, 0xFFCB7D2A, 0xFFCC7D2A, 0xFFCD7D2A, 0xFFCE7D2A, 0xFFCE7D2A,
    0xFFCF7D2A, 0xFFD07C2A, 0xFFD17C2A, 0xFFD27C2A, 0xFFD27C2A, 0xFFD37C2A, 0xFFD47C2A, 0xFFD57C29, 0xFFD57C29,
    0xFFD67B29, 0xFFD77B29, 0xFFD77B29, 0xFFD87B29
#else
    0x00BC944C, 0x00BC944C, 0x00BB944C, 0x00BA944C, 0x00BA944C, 0x00B9944C, 0x00B9944C, 0x00B8944C, 0x00B7944C,
    0x00B7944C, 0x00B6944C, 0x00B5944C, 0x00B5944C, 0x00B4944C, 0x00B3944C, 0x00B3944C, 0x00B2944C, 0x00B1944C,
    0x00B0944C, 0x00AF944C, 0x00AE944C, 0x00AE944C, 0x00AD944C, 0x00AC944C, 0x00AB944C, 0x00AA944C, 0x00A9944C,
    0x00A8944C, 0x00A7944C, 0x00A6944C, 0x00A5944C, 0x00A4944C, 0x00A3944C, 0x00A2944C, 0x00A1944C, 0x00A0944C,
    0x009F944C, 0x009E944C, 0x009D944C, 0x009C944C, 0x009B944C, 0x009A944C, 0x0099944C, 0x0098944C, 0x0097944C,
    0x0096944C, 0x0094944C, 0x0093944C, 0x0092944C, 0x0091944C, 0x0090944C, 0x008F944C, 0x008E944C, 0x008C944C,
    0x008B944C, 0x008A944C, 0x0089944C, 0x0088944C, 0x0087944C, 0x0085944C, 0x0084944C, 0x0083944C, 0x0082944C,
    0x0081944C, 0x007F944C, 0x007E944C, 0x007D944C, 0x007C944C, 0x007B944C, 0x0079944C, 0x0078944C, 0x0077944C,
    0x0076944C, 0x0075944C, 0x0074944C, 0x0072944C, 0x0071944C, 0x0070944C, 0x006F944C, 0x006E944C, 0x006D944C,
    0x006B944C, 0x006A944C, 0x0069944C, 0x0068944C, 0x0067944C, 0x0066944C, 0x0065944C, 0x0063944C, 0x0062944C,
    0x0061944C, 0x0060944C, 0x005F944C, 0x005E944C, 0x005D944C, 0x005C944C, 0x005B944C, 0x005A944C, 0x0059944C,
    0x0058944C, 0x0057944C, 0x0056944C, 0x0055944C, 0x0054944C, 0x0053944C, 0x0052944C, 0x0051944C, 0x0050944C,
    0x004F944C, 0x004E944C, 0x004D944C, 0x004C944C, 0x004C944C, 0x004B944C, 0x004A944C, 0x0049944C, 0x0048944C,
    0x0047944C, 0x0047944C, 0x0046944C, 0x0045944C, 0x0044944C, 0x0044944C, 0x0043944C, 0x0042944C, 0x0042944C,
    0x0041944C, 0x0041944C, 0x0040944C, 0x003F944D, 0x003F944D, 0x003E944E, 0x003E944F, 0x003D934F, 0x003D9350,
    0x003C9351, 0x003C9352, 0x003B9352, 0x003B9353, 0x003A9354, 0x003A9355, 0x00399256, 0x00399256, 0x00399257,
    0x00389258, 0x00389259, 0x0037925A, 0x0037915B, 0x0037915C, 0x0036915D, 0x0036915E, 0x0035915F, 0x00359160,
    0x00359061, 0x00349062, 0x00349063, 0x00349064, 0x00339066, 0x00338F67, 0x00338F68, 0x00338F69, 0x00328F6A,
    0x00328F6B, 0x00328E6D, 0x00328E6E, 0x00318E6F, 0x00318E70, 0x00318D71, 0x00318D73, 0x00308D74, 0x00308D75,
    0x00308D77, 0x00308C78, 0x00308C79, 0x002F8C7A, 0x002F8C7C, 0x002F8B7D, 0x002F8B7E, 0x002F8B80, 0x002E8B81,
    0x002E8A82, 0x002E8A84, 0x002E8A85, 0x002E8A86, 0x002E8988, 0x002E8989, 0x002D898A, 0x002D898C, 0x002D888D,
    0x002D888F, 0x002D8890, 0x002D8891, 0x002D8893, 0x002D8794, 0x002D8795, 0x002C8797, 0x002C8798, 0x002C869A,
    0x002C869B, 0x002C869C, 0x002C869E, 0x002C859F, 0x002C85A0, 0x002C85A2, 0x002C85A3, 0x002C84A4, 0x002C84A6,
    0x002C84A7, 0x002C84A8, 0x002B83AA, 0x002B83AB, 0x002B83AC, 0x002B83AD, 0x002B82AF, 0x002B82B0, 0x002B82B1,
    0x002B82B3, 0x002B82B4, 0x002B81B5, 0x002B81B6, 0x002B81B7, 0x002B81B9, 0x002B80BA, 0x002B80BB, 0x002B80BC,
    0x002B80BD, 0x002B80BE, 0x002B7FC0, 0x002B7FC1, 0x002B7FC2, 0x002B7FC3, 0x002A7FC4, 0x002A7EC5, 0x002A7EC6,
    0x002A7EC7, 0x002A7EC8, 0x002A7EC9, 0x002A7ECA, 0x002A7DCB, 0x002A7DCC, 0x002A7DCD, 0x002A7DCE, 0x002A7DCE,
    0x002A7DCF, 0x002A7CD0, 0x002A7CD1, 0x002A7CD2, 0x002A7CD2, 0x002A7CD3, 0x002A7CD4, 0x00297CD5, 0x00297CD5,
    0x00297BD6, 0x00297BD7, 0x00297BD7, 0x00297BD8
#endif
};
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_EnableLcdInterrupt(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
    };

    /* Reset the LCD. */
    GPIO_PinInit(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, &config);

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);

    while (i--)
    {
    }

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
}

void BOARD_InitLcdifPixelClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31,
        .postDivider = 8,
        .numerator   = 0,
        .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

void BOARD_InitGPT(void)
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);

    gptConfig.enableFreeRun = true;
    gptConfig.divider       = 3000;

    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);
    GPT_StartTimer(EXAMPLE_GPT);
}



/*********************************************************************
 *
 *       _GetSelectedRoom
 *
 * Function description:
 *  This routine returns the index of of a room inside the ROOM_DATA
 *  structure.
 *
 */
static int _GetSelectedRoom(void)
{
    int i;

    for (i = 0; i < 5; i++)
    {
        if (_aRooms[i].Focus)
        {
            return i;
        }
    }
    return 0;
}

/*********************************************************************
 *
 *       _SetFanButtonState
 *
 * Function description:
 *  This routine sets the states of the fan control buttons.
 *
 */
static void _SetFanButtonState(WM_MESSAGE *pMsg, int Id)
{
    int IndexButton;
    int IndexRoom;
    int i;
    WM_HWIN hItem;

    IndexButton                     = Id - ID_BUTTON_5;
    _aButton[IndexButton].Focus     = (_aButton[IndexButton].Focus) ? 0 : 1;
    _aButton[1 - IndexButton].Focus = 0;
    for (i = 0; i < 2; i++)
    {
        WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aButton[i].ID));
    }
    IndexRoom                   = _GetSelectedRoom();
    _aRooms[IndexRoom].FanState = (_aButton[IndexButton].Focus == 0) ? 0 : (IndexButton) ? 2 : 1;
    hItem                       = WM_GetDialogItem(pMsg->hWin, IndexRoom + ID_BUTTON_0);
    WM_InvalidateWindow(hItem);
}

/*********************************************************************
 *
 *       _cbButton
 *
 * Function description:
 *  This is the callback function for the buttons which represent the
 *  different rooms.
 *
 */
static void _cbButton(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    GUI_COLOR ColorFrame;
    GUI_COLOR Color;
    GUI_COLOR ColorText;
    GUI_CONST_STORAGE GUI_FONT *pFont23;
    GUI_CONST_STORAGE GUI_FONT *pFont32;
    int Index;
    int xSize;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            //
            // Get index and check if it is focused or not
            //
            Index = WM_GetId(pMsg->hWin) - ID_BUTTON_0;
            if (_aRooms[Index].Focus == 1)
            { // Set colors dependend on focus state
                Color      = LIGHT_BLUE;
                ColorText  = LEMON;
                ColorFrame = GUI_WHITE;
                pFont23    = pFont23pBold;
                pFont32    = pFont32pBold;
            }
            else
            {
                Color      = DARK_BLUE;
                ColorText  = GUI_WHITE;
                ColorFrame = GUI_WHITE;
                pFont23    = pFont23p;
                pFont32    = pFont32p;
            }
            //
            // Draw thin frame around the button
            //
            WM_GetClientRectEx(pMsg->hWin, &Rect);
            GUI_SetColor(ColorFrame);
            GUI_FillRectEx(&Rect);
            GUI_SetColor(Color);
            //
            // Resize drawing rect
            //
            Rect.x0 += 1;
            Rect.x1 -= 1;
            Rect.y1 -= 1;
            //
            // Draw rest of the button
            //
            GUI_AA_FillRoundedRectEx(&Rect, 3);
            //
            // Only button 0 needs a thin gradient
            //
            if (WM_GetId(pMsg->hWin) == ID_BUTTON_0)
            {
                xSize = WM_GetWindowSizeX(pMsg->hWin);
                GUI_DrawGradientV(0, 0, xSize - 1, 4, GUI_BLACK, Color);
                GUI_DrawGradientV(0, 0, 0, 4, GUI_BLACK, ColorFrame);
                GUI_DrawGradientV(xSize - 1, 0, xSize, 4, GUI_BLACK, ColorFrame);
            }
            //
            // Draw Text
            //
            GUI_SetFont(pFont23);
            GUI_SetTextMode(GUI_TM_TRANS);
            GUI_SetColor(ColorText);
            Rect.x0 += 10;
            GUI_DispStringInRect((char *)_aRooms[Index].aName, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
            //
            // Display fan status
            //
            GUI_SetFont(pFont23);
            GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
            GUI_GotoXY(Rect.x1 / 2 + 20, Rect.y1 / 2);
            if (_aRooms[Index].FanState == 0)
            {
                GUI_DispString("Off");
            }
            else if (_aRooms[Index].FanState == 1)
            {
                GUI_DispString("On");
            }
            else
            {
                GUI_DispString("Auto");
            }
            //
            // Display temperature with a bigger font
            //
            GUI_SetFont(pFont32);
            GUI_SetTextAlign(GUI_TA_RIGHT | GUI_TA_VCENTER);
            GUI_GotoXY(Rect.x1 - 38, Rect.y1 / 2);
            GUI_DispDecSpace(_aRooms[Index].RealTemperature, 2);
            GUI_DispString(
                "\xc2\xb0"
                "C");
            break;
        default:
            BUTTON_Callback(pMsg); // The original callback
            break;
    }
}

/*********************************************************************
 *
 *       _cbButtonFan
 */
static void _cbButtonFan(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    int Index;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            WM_GetClientRectEx(pMsg->hWin, &Rect);
            if (BUTTON_IsPressed(pMsg->hWin))
            {
                GUI_DrawBitmap(&bmButtonRoundPressed_50x22, 0, 0);
            }
            else
            {
                GUI_DrawBitmap(&bmButtonRound_50x22, 0, 0);
            }
            Index = WM_GetId(pMsg->hWin) - ID_BUTTON_5;
            if (_aButton[Index].Focus == 1)
            { // Choos text color
                GUI_SetColor(LEMON);
            }
            else
            {
                GUI_SetColor(GUI_DARKGRAY);
            }
            //
            // Draw Text
            //
            GUI_SetFont(pFont23pBold);
            GUI_SetTextMode(GUI_TM_TRANS);
            GUI_DispStringInRect(_aButton[Index].aName, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
            break;
        default:
            BUTTON_Callback(pMsg); // The original callback
            break;
    }
}

/*********************************************************************
 *
 *       _cbKnob
 *
 * Function description:
 *   Fills the background of the KNOB with transparency
 */
static void _cbKnob(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            KNOB_Callback(pMsg); // The original callback
            break;
        default:
            KNOB_Callback(pMsg); // The original callback
            break;
    }
}

/*********************************************************************
 *
 *       _DrawKnob
 *
 * Function description:
 *   Fill memory device used for drawing the KNOB
 */
static void _DrawKnob(GUI_MEMDEV_Handle hMem, GUI_COLOR Color)
{
    int xSizeMem, ySizeMem;

    xSizeMem = GUI_MEMDEV_GetXSize(hMem);
    ySizeMem = GUI_MEMDEV_GetYSize(hMem);
    GUI_AA_PreserveTrans(1);
    GUI_MEMDEV_Select(hMem);
    GUI_SetBkColor(GUI_TRANSPARENT);
    GUI_Clear();
    GUI_SetColor(Color);
    GUI_FillRect(xSizeMem - 16, 0, xSizeMem, ySizeMem - 1);
    GUI_SetColor(DARK_BLUE);
    GUI_DrawLine(xSizeMem - 16, ySizeMem / 2, xSizeMem, ySizeMem / 2);
    GUI_SetColor(Color);
    GUI_MEMDEV_Select(0);
    GUI_AA_PreserveTrans(0);
}

/*********************************************************************
 *
 *       _OnRelease
 */
static void _OnRelease(WM_MESSAGE *pMsg, int Id)
{
    WM_HWIN hItem, hParent;
    int Value;
    int Index;
    int i;

    Index = Id - ID_BUTTON_0;
    for (i = 0; i < 5; i++)
    {
        _aRooms[i].Focus = 0;
        if (_aRooms[i].ID != Id)
        {
            WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aRooms[i].ID));
        }
    }
    hParent = pMsg->hWin;
    hItem   = WM_GetDialogItem(hParent, ID_KNOB_0);
    Value   = 2700 - ((_aRooms[Index].RealTemperature - MIN_TEMPERATURE) * 2700) / (MAX_TEMPERATURE - MIN_TEMPERATURE);
    KNOB_SetPos(hItem, Value);
    _aRooms[Index].Focus = 1;
    //
    // Check states for fan buttons
    //
    if (_aRooms[Index].FanState == 0)
    {
        _aButton[0].Focus = 0;
        _aButton[1].Focus = 0;
    }
    else if (_aRooms[Index].FanState == 1)
    {
        _aButton[0].Focus = 1;
        _aButton[1].Focus = 0;
    }
    else
    {
        _aButton[0].Focus = 0;
        _aButton[1].Focus = 1;
    }
    //
    // Tell fan buttons to redraw
    //
    for (i = 0; i < 2; i++)
    {
        WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aButton[i].ID));
    }
    WM_InvalidateWindow(pMsg->hWin);
}

/*********************************************************************
 *
 *       _cbDialog
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    static GUI_MEMDEV_Handle hMemKnob;
    static GUI_COLOR ColorKnob = GUI_WHITE;
    WM_HWIN hItem;
    WM_HWIN hParent;
    int NCode;
    int Id;
    int i;
    int Value;
    int Index;
    GUI_COLOR Color;
    GUI_RECT Rect;
    char acBuffer[8];

    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            //
            // Create KNOB
            //
            KNOB_CreateUser(307, 92, 105, 105, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, ID_KNOB_0, 0);
            //
            // Create memory device for KNOB
            //
            hMemKnob = GUI_MEMDEV_CreateFixed32(0, 0, 100, 5);
            _DrawKnob(hMemKnob, ColorKnob);
            //
            // Set several KNOB properties
            //
            hItem = WM_GetDialogItem(pMsg->hWin, ID_KNOB_0);
            WM_SetCallback(hItem, _cbKnob);
            KNOB_SetDevice(hItem, hMemKnob);
            KNOB_SetRange(hItem, 0, 2700);
            KNOB_SetOffset(hItem, -KNOB_OFFSET);
            KNOB_SetPeriod(hItem, 500);
            Value =
                2700 - ((_aRooms[0].RealTemperature - MIN_TEMPERATURE) * 2700) / (MAX_TEMPERATURE - MIN_TEMPERATURE);
            KNOB_SetPos(hItem, Value);
            //
            // Initialization Buttons for each room
            //
            for (i = 0; i < 5; i++)
            {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0 + i);
                WM_SetCallback(hItem, _cbButton);
                _aRooms[i].ID = ID_BUTTON_0 + i;
            }
            //
            // Initialization Buttons for fan
            //
            for (i = 0; i < 2; i++)
            {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5 + i);
                WM_SetCallback(hItem, _cbButtonFan);
                _aButton[i].ID = ID_BUTTON_5 + i;
            }
            //
            // Init logo
            //
            hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
            IMAGE_SetBitmap(hItem, &bmSEGGER_80x40);
            break;
        case WM_PAINT:
            Rect.x0 = 0;
            Rect.y0 = 0;
            Rect.x1 = 480;
            Rect.y1 = 272;
            GUI_SetColor(DARK_BLUE);
            GUI_FillRectEx(&Rect);

            Rect.x0 = 0;
            Rect.y0 = 24;
            Rect.x1 = 480;
            Rect.y1 = 47;
            GUI_SetColor(LIGHT_BLUE);
            GUI_FillRectEx(&Rect);

            GUI_DrawGradientV(0, 48, 480 - 1, 52, GUI_BLACK, DARK_BLUE);
            GUI_SetColor(GUI_WHITE);
            GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
            GUI_SetTextMode(GUI_TM_TRANS);
            GUI_SetFont(pFont23pBold);
            GUI_DispStringAt("Temperature control", 10, 10);

            GUI_SetFont(pFont23p);
            GUI_DispStringAt("Room:", 10, 35);
            GUI_DispStringAt("Fan state:", 101, 35);
            GUI_DispStringAt("Temp.:", 174, 35);

            Index   = _GetSelectedRoom();
            Rect.x0 = 325;
            Rect.y0 = 109;
            Rect.x1 = 394;
            Rect.y1 = 178;
            Value   = _aRooms[Index].RealTemperature;

            Index = ((Value - MIN_TEMPERATURE) * (GUI_COUNTOF(_aGradient) - 1)) / (MAX_TEMPERATURE - MIN_TEMPERATURE);
            Color = _aGradient[Index];
            GUI_SetColor(Color);
            GUI_FillRectEx(&Rect);
            sprintf(acBuffer,
                    "%i \xc2\xb0"
                    "C",
                    Value);
            GUI_SetColor(GUI_WHITE);
            GUI_DrawBitmap(&bmKNOB_124x124, 297, 82);
            GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch (Id)
            {
                case ID_KNOB_0: // Notifications sent by 'Knob'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, Id);
                            Value = KNOB_GetValue(hItem);
                            //
                            // Invalidate button
                            //
                            hParent = WM_GetFirstChild(WM_GetDesktopWindowEx(0));
                            for (i = 0; i < 5; i++)
                            {
                                if (_aRooms[i].Focus == 1)
                                {
                                    hParent = WM_GetFirstChild(WM_GetDesktopWindowEx(0));
                                    _aRooms[i].RealTemperature =
                                        MAX_TEMPERATURE - Value * (MAX_TEMPERATURE - MIN_TEMPERATURE) / 2700;
                                    WM_InvalidateWindow(WM_GetDialogItem(hParent, _aRooms[i].ID));
                                    break;
                                }
                            }
                            WM_InvalidateWindow(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_BUTTON_0: // Notifications sent by 'Bedroom'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _OnRelease(pMsg, Id);
                            break;
                    }
                    break;
                case ID_BUTTON_1: // Notifications sent by 'Living Room'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _OnRelease(pMsg, Id);
                            break;
                    }
                    break;
                case ID_BUTTON_2: // Notifications sent by 'Kitchen'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _OnRelease(pMsg, Id);
                            break;
                    }
                    break;
                case ID_BUTTON_3: // Notifications sent by 'Child's Room 1'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _OnRelease(pMsg, Id);
                            break;
                    }
                    break;
                case ID_BUTTON_4: // Notifications sent by 'Child's Room 2'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _OnRelease(pMsg, Id);
                            break;
                    }
                    break;
                case ID_BUTTON_5: // Notifications sent by 'On'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _SetFanButtonState(pMsg, Id);
                            break;
                    }
                    break;
                case ID_BUTTON_6: // Notifications sent by 'Auto'
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            _SetFanButtonState(pMsg, Id);
                            break;
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitI2C1Pins();
    BOARD_InitSemcPins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();
    BOARD_InitGPT();

    GUI_Init();
    WM_MULTIBUF_Enable(1);
    GUI_UC_SetEncodeUTF8();
    //
    // Create application window...
    //
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    //
    // ...and keep it alive
    //
    WM_Exec();

    while (1)
    {
        /* Poll touch controller for update */
        if (BOARD_Touch_Poll())
        {
            GUI_Delay(5);
        }
    }
}
