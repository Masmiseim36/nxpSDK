/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2024  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : Resource.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "Resource.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define _aVarList        NULL
#define _appDrawing      NULL
#define _NumDrawings     0
#define _aScrollerList   NULL
#define _NumScrollers    0
#define _CreateFlags     0

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _apRootList
*/
static APPW_ROOT_INFO * _apRootList[] = {
  &ID_SCREEN_TEMP_CTRL_RootInfo,
};

/*********************************************************************
*
*       _NumScreens
*/
static unsigned _NumScreens = GUI_COUNTOF(_apRootList);

/*********************************************************************
*
*       _NumVars
*/
static unsigned _NumVars = 0;

/*********************************************************************
*
*       _apLang
*/
static GUI_CONST_STORAGE char * _apLang[] = {
  (GUI_CONST_STORAGE char *)acAPPW_Language_0,
};

/*********************************************************************
*
*       _TextInit
*/
static GUI_CONST_STORAGE APPW_TEXT_INIT _TextInit = {
  (const char **)_apLang,
  GUI_COUNTOF(_apLang),
};

/*********************************************************************
*
*       Private data
*
**********************************************************************
*/
/*********************************************************************
*
*       Font data
*/
APPW_FONT APPW__aFont[3];

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW__GetTextInit
*/
void APPW__GetTextInit(GUI_CONST_STORAGE APPW_TEXT_INIT ** ppTextInit) {
  *ppTextInit = &_TextInit;
}

/*********************************************************************
*
*       APPW__GetResource
*/
void APPW__GetResource(APPW_ROOT_INFO         *** pppRootInfo,    int * pNumScreens,
                       APPW_VAR_OBJECT         ** ppaVarList,     int * pNumVars,
                       const APPW_SCROLLER_DEF ** ppaScrollerDef, int * pNumScrollers,
                       APPW_DRAWING_ITEM      *** pppDrawingList, int * pNumDrawings,
                                                                  int * pCreateFlags) {
  *pppRootInfo    = _apRootList;
  *ppaVarList     = _aVarList;
  *ppaScrollerDef = _aScrollerList;
  *pppDrawingList = (APPW_DRAWING_ITEM **)_appDrawing;
  *pNumScreens    = _NumScreens;
  *pNumVars       = _NumVars;
  *pNumScrollers  = _NumScrollers;
  *pNumDrawings   = _NumDrawings;
  *pCreateFlags   = _CreateFlags;
}

/*************************** End of file ****************************/
