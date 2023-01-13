/*------------------------------------------------------------------------
 *
 * OpenVG 1.0.1 Reference Implementation sample code
 * -------------------------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 */

/**
 * \file
 * \brief        Tiger sample application. Resizing the application window
 *                        rerenders the tiger in the new resolution. Pressing 1,2,3
 *                        or 4 sets pixel zoom factor, mouse moves inside the zoomed
 *                        image (mouse move works on OpenGL >= 1.2).
 * \note
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "tiger_path.h"
/*-------------------------------------------------------------------*/

#ifdef HG_FLAT_INCLUDES
#include "openvg.h"
#include "vgu.h"
#include "egl.h"
#else
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "EGL/egl.h"
#endif

#include "fsl_soc_src.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UNREF(X)           ((void)(X))
#define DEMO_WINDOW_WIDTH  600
#define DEMO_WINDOW_HEIGHT 600
#define DEMO_RENDER_WIDTH  600
#define DEMO_RENDER_HEIGHT 600

typedef struct
{
    VGFillRule m_fillRule;
    VGPaintMode m_paintMode;
    VGCapStyle m_capStyle;
    VGJoinStyle m_joinStyle;
    float m_miterLimit;
    float m_strokeWidth;
    VGPaint m_fillPaint;
    VGPaint m_strokePaint;
    VGPath m_path;
} PathData;

typedef struct
{
    PathData *m_paths;
    int m_numPaths;
} PS;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

const float aspectRatio = 612.0f / 792.0f;
int renderWidth         = 0;
int renderHeight        = 0;
EGLConfig eglConfig;
EGLContext eglContext;
PS *PS_tiger = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}


static PS *PS_construct(const char *commands, int commandCount, const float *points, int pointCount)
{
    PS *ps          = (PS *)malloc(sizeof(PS));
    int p           = 0;
    int c           = 0;
    int i           = 0;
    int paths       = 0;
    int maxElements = 0;
    unsigned char *cmd;
    UNREF(pointCount);

    assert(ps && "malloc failed");

    while (c < commandCount)
    {
        int elements, e;
        c += 4;
        p += 8;
        elements = (int)points[p++];
        assert(elements > 0);
        if (elements > maxElements)
            maxElements = elements;
        for (e = 0; e < elements; e++)
        {
            switch (commands[c])
            {
                case 'M':
                    p += 2;
                    break;
                case 'L':
                    p += 2;
                    break;
                case 'C':
                    p += 6;
                    break;
                case 'E':
                    break;
                default:
                    assert(0); // unknown command
            }
            c++;
        }
        paths++;
    }

    ps->m_numPaths = paths;
    ps->m_paths    = (PathData *)malloc(paths * sizeof(PathData));
    assert(ps->m_paths && "malloc failed");

    cmd = (unsigned char *)malloc(maxElements);
    assert(cmd && "malloc failed");

    i = 0;
    p = 0;
    c = 0;
    while (c < commandCount)
    {
        int elements, startp, e;
        float color[4];

        // fill type
        int paintMode             = 0;
        ps->m_paths[i].m_fillRule = VG_NON_ZERO;
        switch (commands[c])
        {
            case 'N':
                break;
            case 'F':
                ps->m_paths[i].m_fillRule = VG_NON_ZERO;
                paintMode |= VG_FILL_PATH;
                break;
            case 'E':
                ps->m_paths[i].m_fillRule = VG_EVEN_ODD;
                paintMode |= VG_FILL_PATH;
                break;
            default:
                assert(0); // unknown command
        }
        c++;

        // stroke
        switch (commands[c])
        {
            case 'N':
                break;
            case 'S':
                paintMode |= VG_STROKE_PATH;
                break;
            default:
                assert(0); // unknown command
        }
        ps->m_paths[i].m_paintMode = (VGPaintMode)paintMode;
        c++;

        // line cap
        switch (commands[c])
        {
            case 'B':
                ps->m_paths[i].m_capStyle = VG_CAP_BUTT;
                break;
            case 'R':
                ps->m_paths[i].m_capStyle = VG_CAP_ROUND;
                break;
            case 'S':
                ps->m_paths[i].m_capStyle = VG_CAP_SQUARE;
                break;
            default:
                assert(0); // unknown command
        }
        c++;

        // line join
        switch (commands[c])
        {
            case 'M':
                ps->m_paths[i].m_joinStyle = VG_JOIN_MITER;
                break;
            case 'R':
                ps->m_paths[i].m_joinStyle = VG_JOIN_ROUND;
                break;
            case 'B':
                ps->m_paths[i].m_joinStyle = VG_JOIN_BEVEL;
                break;
            default:
                assert(0); // unknown command
        }
        c++;

        // the rest of stroke attributes
        ps->m_paths[i].m_miterLimit  = points[p++];
        ps->m_paths[i].m_strokeWidth = points[p++];

        // paints
        color[0]                     = points[p++];
        color[1]                     = points[p++];
        color[2]                     = points[p++];
        color[3]                     = 1.0f;
        ps->m_paths[i].m_strokePaint = vgCreatePaint();
        vgSetParameteri(ps->m_paths[i].m_strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
        vgSetParameterfv(ps->m_paths[i].m_strokePaint, VG_PAINT_COLOR, 4, color);

        color[0]                   = points[p++];
        color[1]                   = points[p++];
        color[2]                   = points[p++];
        color[3]                   = 1.0f;
        ps->m_paths[i].m_fillPaint = vgCreatePaint();
        vgSetParameteri(ps->m_paths[i].m_fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
        vgSetParameterfv(ps->m_paths[i].m_fillPaint, VG_PAINT_COLOR, 4, color);

        // read number of elements

        elements = (int)points[p++];
        assert(elements > 0);
        startp = p;
        for (e = 0; e < elements; e++)
        {
            switch (commands[c])
            {
                case 'M':
                    cmd[e] = VG_MOVE_TO | VG_ABSOLUTE;
                    p += 2;
                    break;
                case 'L':
                    cmd[e] = VG_LINE_TO | VG_ABSOLUTE;
                    p += 2;
                    break;
                case 'C':
                    cmd[e] = VG_CUBIC_TO | VG_ABSOLUTE;
                    p += 6;
                    break;
                case 'E':
                    cmd[e] = VG_CLOSE_PATH;
                    break;
                default:
                    assert(0); // unknown command
            }
            c++;
        }

        ps->m_paths[i].m_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0,
                                             (unsigned int)VG_PATH_CAPABILITY_ALL);
        vgAppendPathData(ps->m_paths[i].m_path, elements, cmd, points + startp);
        i++;
    }
    free(cmd);
    return ps;
}

static void PS_destruct(PS *ps)
{
    int i;
    assert(ps);
    for (i = 0; i < ps->m_numPaths; i++)
    {
        vgDestroyPaint(ps->m_paths[i].m_fillPaint);
        assert(vgGetError() == VG_NO_ERROR);
        vgDestroyPaint(ps->m_paths[i].m_strokePaint);
        assert(vgGetError() == VG_NO_ERROR);
        vgDestroyPath(ps->m_paths[i].m_path);
        assert(vgGetError() == VG_NO_ERROR);
    }
    free(ps->m_paths);
    free(ps);
}

static void PS_clear()
{
    float clearColor[4] = {0.0, 1.0, 0.0, 1.0};
    vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
    assert(vgGetError() == VG_NO_ERROR);

    vgClear(0, 0, renderWidth, renderHeight);
    assert(vgGetError() == VG_NO_ERROR);
}

static void PS_render(PS *ps)
{
    int i;
    assert(ps);
    vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);

    for (i = 0; i < ps->m_numPaths; i++)
    {
        vgSeti(VG_FILL_RULE, ps->m_paths[i].m_fillRule);
        vgSetPaint(ps->m_paths[i].m_fillPaint, VG_FILL_PATH);

        if (ps->m_paths[i].m_paintMode & VG_STROKE_PATH)
        {
            vgSetf(VG_STROKE_LINE_WIDTH, ps->m_paths[i].m_strokeWidth);
            vgSeti(VG_STROKE_CAP_STYLE, ps->m_paths[i].m_capStyle);
            vgSeti(VG_STROKE_JOIN_STYLE, ps->m_paths[i].m_joinStyle);
            vgSetf(VG_STROKE_MITER_LIMIT, ps->m_paths[i].m_miterLimit);
            vgSetPaint(ps->m_paths[i].m_strokePaint, VG_STROKE_PATH);
        }

        vgDrawPath(ps->m_paths[i].m_path, ps->m_paths[i].m_paintMode);
        assert(vgGetError() == VG_NO_ERROR);
    }
}

static int init(NativeWindowType window, EGLDisplay *eglDpy, EGLSurface *eglSurf)
{
    static const EGLint s_configAttribs[] = {EGL_RED_SIZE,
                                             5,
                                             EGL_GREEN_SIZE,
                                             6,
                                             EGL_BLUE_SIZE,
                                             5,
                                             EGL_ALPHA_SIZE,
                                             0,
                                             EGL_LUMINANCE_SIZE,
                                             EGL_DONT_CARE,
                                             EGL_SURFACE_TYPE,
                                             EGL_WINDOW_BIT,
                                             EGL_RENDERABLE_TYPE,
                                             EGL_OPENVG_BIT,
                                             EGL_SAMPLES,
                                             1,
                                             EGL_NONE};
    EGLBoolean rv;
    EGLint numconfigs;
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;

    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY)
    {
        assert(!"eglGetDisplay failed");
        return -1;
    }
    else
    {
        printf("eglGetDisplay successful\n");
    }

    rv = eglInitialize(eglDisplay, NULL, NULL);
    if (rv == EGL_FALSE)
    {
        assert(!"eglInitialize failed");
        return -1;
    }
    else
    {
        printf("eglInitialize successful\n");
    }

    if (eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglInitialize has errors");
        return -1;
    }
    else
    {
        printf("eglGetError successful\n");
    }
    *eglDpy = eglDisplay;

    rv = eglBindAPI(EGL_OPENVG_API);
    if (rv == EGL_FALSE)
    {
        assert(!"eglBindAPI failed");
        return -1;
    }
    else
    {
        printf("eglBindAPI successful\n");
    }

    rv = eglChooseConfig(eglDisplay, s_configAttribs, &eglConfig, 1, &numconfigs);
    if (rv == EGL_FALSE)
    {
        assert(!"eglChooseConfig failed");
        return -1;
    }
    else
    {
        printf("eglChooseConfig successful\n");
    }

    if (eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglChooseConfig has errors");
    }
    if (numconfigs != 1)
    {
        assert(!"eglChooseConfig hasn't found any configs\n");
        return -1;
    }

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, window, NULL);
    if (EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglCreateWindowSurface has errors");
    }
    else
    {
        printf("eglCreateWindowSurface successful\n");
    }
    *eglSurf = eglSurface;

    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
    if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglCreateContext has errors");
    }
    else
    {
        printf("eglCreateContext successful\n");
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if (eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglMakeCurrent has errors");
    }
    else
    {
        printf("eglMakeCurrent successful\n");
    }

    return 0;
}

static int deinit(EGLDisplay eglDisplay, EGLSurface eglSurface)
{
    EGLBoolean rv;
    rv = eglDestroyContext(eglDisplay, eglContext);
    if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglDestroyContext failed.");
    }
    else
    {
        printf("eglDestroyContext successful\n");
    }

    rv = eglDestroySurface(eglDisplay, eglSurface);
    if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglDestroySurface failed.");
    }
    else
    {
        printf("eglDestroySurface successful\n");
    }

    rv = eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglMakeCurrent failed.");
    }
    else
    {
        printf("eglMakeCurrent successful\n");
    }

    rv = eglTerminate(eglDisplay);
    if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglTerminate failed.");
    }
    else
    {
        printf("eglTerminate successful\n");
    }

    rv = eglReleaseThread();
    if (rv != EGL_TRUE) // || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglReleaseThread failed.");
    }
    else
    {
        printf("eglReleaseThread successful\n");
    }

    return 0;
}

static void render_tiger(EGLDisplay eglDisplay, EGLSurface eglSurface, int w, int h)
{
    EGLBoolean rv;

    if (renderWidth != w || renderHeight != h)
    {
        float clearColor[4] = {1, 1, 1, 1};
        float scale         = w / (tigerMaxX - tigerMinX);

        renderWidth  = w;
        renderHeight = h;

        rv = eglSwapBuffers(eglDisplay, eglSurface); // force EGL to recognize resize
        if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
        {
            assert(!"eglSwapBuffers has errors");
        }
        else
        {
            printf("eglSwapBuffers-resize successful\n");
        }

        vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
        vgClear(0, 0, w, h);

        vgLoadIdentity();
        vgScale(scale, scale);
        vgTranslate(-tigerMinX, -tigerMinY + 0.5f * (h / scale - (tigerMaxY - tigerMinY)));

        PS_render(PS_tiger);
        assert(vgGetError() == VG_NO_ERROR);
    }

    rv = eglSwapBuffers(eglDisplay, eglSurface);
    if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglSwapBuffers has errors");
    }
    else
    {
        printf("eglSwapBuffers successful\n");
    }
}

static void render_clear(EGLDisplay eglDisplay, EGLSurface eglSurface, int w, int h)
{
    EGLBoolean rv;

    if (renderWidth != w || renderHeight != h)
    {
        float clearColor[4] = {1, 1, 1, 1};
        float scale         = w / (tigerMaxX - tigerMinX);

        renderWidth  = w;
        renderHeight = h;

        rv = eglSwapBuffers(eglDisplay, eglSurface); // force EGL to recognize resize
        if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
        {
            assert(!"eglSwapBuffers has errors");
        }
        else
        {
            printf("eglSwapBuffers-resize successful\n");
        }

        vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
        vgClear(0, 0, w, h);

        vgLoadIdentity();
        vgScale(scale, scale);
        vgTranslate(-tigerMinX, -tigerMinY + 0.5f * (h / scale - (tigerMaxY - tigerMinY)));

        PS_clear();
        assert(vgGetError() == VG_NO_ERROR);
    }

    rv = eglSwapBuffers(eglDisplay, eglSurface);
    if (rv != EGL_TRUE || eglGetError() != EGL_SUCCESS)
    {
        assert(!"eglSwapBuffers has errors");
    }
    else
    {
        printf("eglSwapBuffers successful\n");
    }
}

static void tiger_task(void)
{
    int rv;
    NativeDisplayType fbDpy;
    NativeWindowType fbWin;
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;

    printf("construct\n");
    fbDpy = fbGetDisplay(NULL);
    assert(fbDpy != 0);

    fbWin = fbCreateWindow(fbDpy, 50, 50, DEMO_WINDOW_WIDTH, DEMO_WINDOW_HEIGHT);
    assert(fbWin != 0);

    rv = init(fbWin, &eglDisplay, &eglSurface);
    assert(rv == 0);

    PS_tiger = PS_construct(tigerCommands, tigerCommandCount, tigerPoints, tigerPointCount);
    assert(PS_tiger);

    for (int i = 0; i < 100; ++i)
        render_tiger(eglDisplay, eglSurface, DEMO_RENDER_WIDTH, DEMO_RENDER_HEIGHT);

    printf("destroy-openvg\n");
    PS_destruct(PS_tiger);

    printf("destroy-egl\n");
    rv = deinit(eglDisplay, eglSurface);
    assert(rv == 0);

    printf("destroy-native-window\n");
    fbDestroyWindow(fbWin);

    printf("destroy-native-display\n");
    fbDestroyDisplay(fbDpy);

    printf("tiger_task done\n");
}

void __attribute__((noinline)) all_done(void)
{
    while (1)
        ;
}

static void main_task(void *pvParameters)
{
    tiger_task();

    printf("main_task done\n");

    all_done();
    vTaskDelete(NULL);
}

/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitLpuartPins();
    BOARD_InitMipiPanelPins();
    BOARD_InitDebugConsole();

    if (xTaskCreate(main_task, "main_task", configMINIMAL_STACK_SIZE + 700, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
    {
        assert(!"Task creation failed!.\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following line
       will never be reached.  If the following line does execute, then there was
       insufficient FreeRTOS heap memory available for the idle and/or timer tasks
       to be created.
       See the memory management section on the FreeRTOS web site
       for more details.  */
    for (;;)
        ;
}
