/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file implements a generic framework for running Embedded Wizard
*   generated GUI applications on a dedicated target with or without the usage
*   of an operating system.
*   The module ewmain contains three major functions that are used within
*   every Embedded Wizard GUI application: EwInit() - EwProcess() - EwDone().
*   These functions represent the lifecycle of the entire GUI application.
*
*   EwInit() is responsible to initialize the system, to configure the display,
*   to get access to the desired input devices (like keyboard or touch),
*   to initialize the Embedded Wizard Runtime Environment / Graphics Engine,
*   to create an instance of the application class and to initialize all
*   needed peripheral components.
*
*   EwProcess() implements one cycle of the main loop. This function has to be
*   called in an (endless) loop and contains typically the following steps:
*   1. Processing data from your device driver(s)
*   2. Processing key events
*   3. Processing cursor or touch screen events
*   4. Processing timers
*   5. Processing signals
*   6. Updating the screen
*   7. Triggering the garbage collection
*
*   EwDone() is responsible to shutdown the application and to release all
*   used resources.
*
*   Important: Please be aware that every Embedded Wizard GUI application
*   requires the execution in a single GUI task!
*   If you are working with an operating system and your software is using
*   several threads/tasks, please take care to access your GUI application
*   only within the context of your GUI thread/task. Use operating system
*   services to exchange data or events between the GUI thread/task and other
*   worker threads/tasks.
*
*   In order to keep this framework independent from the particular GUI
*   application, the application class and the screen size are taken from the
*   generated code. In this manner, it is not necessary to modify this file
*   when creating new GUI applications. Just set the attributes 'ScreenSize'
*   and 'ApplicationClass' of the profile in the Embedded Wizard IDE.
*
*   For more information concerning the integration of an Embedded Wizard
*   generated GUI application into your main application, please see
*   https://doc.embedded-wizard.de/main-loop
*
*******************************************************************************/


/*******************************************************************************
* FUNCTION:
*   EwInit
*
* DESCRIPTION:
*   EwInit() is responsible to initialize the system, to configure the display,
*   to get access to the desired input devices (like keyboard or touch),
*   to initialize the Embedded Wizard Runtime Environment / Graphics Engine,
*   to create an instance of the application class and to initialize all
*   needed peripheral components.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   1 if successful, 0 otherwise.
*
*******************************************************************************/
int EwInit( void );


/*******************************************************************************
* FUNCTION:
*   EwDone
*
* DESCRIPTION:
*   EwDone() is responsible to shutdown the application and to release all
*   used resources.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwDone( void );


/*******************************************************************************
* FUNCTION:
*   EwProcess
*
* DESCRIPTION:
*   EwProcess() implements one cycle of the main loop. This function has to be
*   called in an (endless) loop and contains typically the following steps:
*   1. Processing data from your device driver(s)
*   2. Processing key events
*   3. Processing cursor or touch screen events
*   4. Processing timers
*   5. Processing signals
*   6. Updating the screen
*   7. Triggering the garbage collection
*   For more information concerning the integration of an Embedded Wizard
*   generated GUI application into your main application, please see
*   https://doc.embedded-wizard.de/main-loop
*
* ARGUMENTS:
*   None.
*
* RETURN VALUE:
*   1, if further processing is needed, 0 otherwise.
*
*******************************************************************************/
int EwProcess( void );


/*******************************************************************************
* FUNCTION:
*   EwPrintSystemInfo
*
* DESCRIPTION:
*   This function prints system and configuration information - very helpful in
*   case of any support issues.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPrintSystemInfo( void );


/* msy, mli */
