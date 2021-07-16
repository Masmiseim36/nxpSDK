/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software and related documentation ("Software") are intellectual
* property owned by TARA Systems and are copyright of TARA Systems.
* Any modification, copying, reproduction or redistribution of the Software in
* whole or in part by any means not in accordance with the End-User License
* Agreement for Embedded Wizard is expressly prohibited. The removal of this
* preamble is expressly prohibited.
* 
********************************************************************************
*
* DESCRIPTION:
*   This is an internal header of the Embedded Wizard Graphics Engine EWGFX. It
*   resolves the dependencies to the external graphics subsystem functionality
*   and replaces the missing functionality by Embedded Wizard own software pixel
*   driver.
*
*   The product of this header file is a set of arrays containing pointers to
*   external drawing functions or to the functions of the software pixel driver.
*
*   This header file also performs a rudimentary integrity test of the external
*   drawing and surface adaptation. If there are incomplete functionality, the
*   'C' preprocessor will report error messages.
*
*******************************************************************************/


/* Support of a platform specific screen surface? */
#if defined EwGfxLockScreenSurface ||                                          \
    defined EwGfxUnlockScreenSurface
  #define EwProvideScreenSurface
#endif


/* Support of a platform specific native surface? */
#if defined EwGfxCreateNativeSurface ||                                        \
    defined EwGfxCreateConstNativeSurface ||                                   \
    defined EwGfxDestroyNativeSurface ||                                       \
    defined EwGfxLockNativeSurface ||                                          \
    defined EwGfxUnlockNativeSurface
  #define EwProvideNativeSurface
#endif


/* Support of a platform specific index8 surface? */
#if defined EwGfxCreateIndex8Surface ||                                        \
    defined EwGfxCreateConstIndex8Surface ||                                   \
    defined EwGfxDestroyIndex8Surface ||                                       \
    defined EwGfxLockIndex8Surface ||                                          \
    defined EwGfxUnlockIndex8Surface
  #define EwProvideIndex8Surface
#endif


/* Support of a platform specific alpha8 surface? */
#if defined EwGfxCreateAlpha8Surface ||                                        \
    defined EwGfxCreateConstAlpha8Surface ||                                   \
    defined EwGfxDestroyAlpha8Surface ||                                       \
    defined EwGfxLockAlpha8Surface ||                                          \
    defined EwGfxUnlockAlpha8Surface
  #define EwProvideAlpha8Surface
#endif


/* Support of a platform specific rgb565 surface? */
#if defined EwGfxCreateRGB565Surface ||                                        \
    defined EwGfxCreateConstRGB565Surface ||                                   \
    defined EwGfxDestroyRGB565Surface ||                                       \
    defined EwGfxLockRGB565Surface ||                                          \
    defined EwGfxUnlockRGB565Surface
  #define EwProvideRGB565Surface
#endif


/* Verify the integrity of the screen surface adaptation */
#if defined EwProvideScreenSurface &&                                          \
    (                                                                          \
      !defined EwGfxLockScreenSurface ||                                       \
      !defined EwGfxUnlockScreenSurface                                        \
    )
  #error "Incomplete screen surface support. Some functions are missing."
#endif


/* Verify the integrity of the native surface adaptation */
#if defined EwProvideNativeSurface &&                                          \
    (                                                                          \
      !defined EwGfxCreateNativeSurface ||                                     \
      !defined EwGfxDestroyNativeSurface ||                                    \
      !defined EwGfxLockNativeSurface ||                                       \
      !defined EwGfxUnlockNativeSurface                                        \
    )
  #error "Incomplete native surface support. Some functions are missing."
#endif


/* Verify the integrity of the index8 surface adaptation */
#if defined EwProvideIndex8Surface &&                                          \
    (                                                                          \
      !defined EwGfxCreateIndex8Surface ||                                     \
      !defined EwGfxDestroyIndex8Surface ||                                    \
      !defined EwGfxLockIndex8Surface ||                                       \
      !defined EwGfxUnlockIndex8Surface                                        \
    )
  #error "Incomplete index8 surface support. Some functions are missing."
#endif


/* Verify the integrity of the alpha8 surface adaptation */
#if defined EwProvideAlpha8Surface &&                                          \
    (                                                                          \
      !defined EwGfxCreateAlpha8Surface ||                                     \
      !defined EwGfxDestroyAlpha8Surface ||                                    \
      !defined EwGfxLockAlpha8Surface ||                                       \
      !defined EwGfxUnlockAlpha8Surface                                        \
    )
  #error "Incomplete alpha8 surface support. Some functions are missing."
#endif


/* Verify the integrity of the rgb565 surface adaptation */
#if defined EwProvideRGB565Surface &&                                          \
    (                                                                          \
      !defined EwGfxCreateRGB565Surface ||                                     \
      !defined EwGfxDestroyRGB565Surface ||                                    \
      !defined EwGfxLockRGB565Surface ||                                       \
      !defined EwGfxUnlockRGB565Surface                                        \
    )
  #error "Incomplete rgb565 surface support. Some functions are missing."
#endif


/* Platform specific drawing operations involving screen surface as the
   destination? */
#if defined EwGfxScreenDrawLineSolid ||                                        \
    defined EwGfxScreenDrawLineSolidBlend ||                                   \
    defined EwGfxScreenFillSolid ||                                            \
    defined EwGfxScreenFillSolidBlend ||                                       \
    defined EwGfxScreenDrawLineGradient ||                                     \
    defined EwGfxScreenDrawLineGradientBlend ||                                \
    defined EwGfxScreenFillGradient ||                                         \
    defined EwGfxScreenFillGradientBlend ||                                    \
    defined EwGfxScreenFillLinearGradient ||                                   \
    defined EwGfxScreenFillLinearGradientBlend ||                              \
    defined EwGfxScreenPolygonSolid ||                                         \
    defined EwGfxScreenPolygonSolidBlend ||                                    \
    defined EwGfxScreenPolygonGradient ||                                      \
    defined EwGfxScreenPolygonGradientBlend ||                                 \
    defined EwGfxScreenPolygonLinearGradient ||                                \
    defined EwGfxScreenPolygonLinearGradientBlend ||                           \
    defined EwGfxScreenPolygonAntialiasedSolid ||                              \
    defined EwGfxScreenPolygonAntialiasedSolidBlend ||                         \
    defined EwGfxScreenPolygonAntialiasedGradient ||                           \
    defined EwGfxScreenPolygonAntialiasedGradientBlend ||                      \
    defined EwGfxScreenPolygonAntialiasedLinearGradient ||                     \
    defined EwGfxScreenPolygonAntialiasedLinearGradientBlend
  #define EwNeedScreenSurface
#endif


/* Platform specific drawing operations involving screen surface as the 
   destination and native surface as source? */
#if defined EwGfxScreenCopyNative ||                                           \
    defined EwGfxScreenCopyNativeBlend ||                                      \
    defined EwGfxScreenCopyNativeGradient ||                                   \
    defined EwGfxScreenCopyNativeGradientBlend ||                              \
    defined EwGfxScreenCopyNativeLinearGradient ||                             \
    defined EwGfxScreenCopyNativeLinearGradientBlend ||                        \
    defined EwGfxScreenTileNative ||                                           \
    defined EwGfxScreenTileNativeBlend ||                                      \
    defined EwGfxScreenTileNativeSolid ||                                      \
    defined EwGfxScreenTileNativeSolidBlend ||                                 \
    defined EwGfxScreenTileNativeGradient ||                                   \
    defined EwGfxScreenTileNativeGradientBlend ||                              \
    defined EwGfxScreenTileNativeLinearGradient ||                             \
    defined EwGfxScreenTileNativeLinearGradientBlend ||                        \
    defined EwGfxScreenWarpPerspNative ||                                      \
    defined EwGfxScreenWarpPerspNativeBlend ||                                 \
    defined EwGfxScreenWarpPerspNativeGradient ||                              \
    defined EwGfxScreenWarpPerspNativeGradientBlend ||                         \
    defined EwGfxScreenWarpPerspNativeFilter ||                                \
    defined EwGfxScreenWarpPerspNativeFilterBlend ||                           \
    defined EwGfxScreenWarpPerspNativeFilterGradient ||                        \
    defined EwGfxScreenWarpPerspNativeFilterGradientBlend ||                   \
    defined EwGfxScreenWarpAffineNative ||                                     \
    defined EwGfxScreenWarpAffineNativeBlend ||                                \
    defined EwGfxScreenWarpAffineNativeGradient ||                             \
    defined EwGfxScreenWarpAffineNativeGradientBlend ||                        \
    defined EwGfxScreenWarpAffineNativeFilter ||                               \
    defined EwGfxScreenWarpAffineNativeFilterBlend ||                          \
    defined EwGfxScreenWarpAffineNativeFilterGradient ||                       \
    defined EwGfxScreenWarpAffineNativeFilterGradientBlend ||                  \
    defined EwGfxScreenScaleNative ||                                          \
    defined EwGfxScreenScaleNativeBlend ||                                     \
    defined EwGfxScreenScaleNativeGradient ||                                  \
    defined EwGfxScreenScaleNativeGradientBlend ||                             \
    defined EwGfxScreenScaleNativeLinearGradient ||                            \
    defined EwGfxScreenScaleNativeLinearGradientBlend ||                       \
    defined EwGfxScreenScaleNativeFilter ||                                    \
    defined EwGfxScreenScaleNativeFilterBlend ||                               \
    defined EwGfxScreenScaleNativeFilterGradient ||                            \
    defined EwGfxScreenScaleNativeFilterGradientBlend ||                       \
    defined EwGfxScreenScaleNativeFilterLinearGradient ||                      \
    defined EwGfxScreenScaleNativeFilterLinearGradientBlend
  #define EwNeedScreenSurface
  #define EwNeedNativeSurface
#endif


/* Platform specific drawing operations involving native surface as source and
   destination? */
#if defined EwGfxDrawLineSolid ||                                              \
    defined EwGfxDrawLineSolidBlend ||                                         \
    defined EwGfxFillSolid ||                                                  \
    defined EwGfxFillSolidBlend ||                                             \
    defined EwGfxDrawLineGradient ||                                           \
    defined EwGfxDrawLineGradientBlend ||                                      \
    defined EwGfxFillGradient ||                                               \
    defined EwGfxFillGradientBlend ||                                          \
    defined EwGfxFillLinearGradient ||                                         \
    defined EwGfxFillLinearGradientBlend ||                                    \
    defined EwGfxCopyNative ||                                                 \
    defined EwGfxCopyNativeBlend ||                                            \
    defined EwGfxCopyNativeGradient ||                                         \
    defined EwGfxCopyNativeGradientBlend ||                                    \
    defined EwGfxCopyNativeLinearGradient ||                                   \
    defined EwGfxCopyNativeLinearGradientBlend ||                              \
    defined EwGfxTileNative ||                                                 \
    defined EwGfxTileNativeBlend ||                                            \
    defined EwGfxTileNativeSolid ||                                            \
    defined EwGfxTileNativeSolidBlend ||                                       \
    defined EwGfxTileNativeGradient ||                                         \
    defined EwGfxTileNativeGradientBlend ||                                    \
    defined EwGfxTileNativeLinearGradient ||                                   \
    defined EwGfxTileNativeLinearGradientBlend ||                              \
    defined EwGfxWarpPerspNative ||                                            \
    defined EwGfxWarpPerspNativeBlend ||                                       \
    defined EwGfxWarpPerspNativeGradient ||                                    \
    defined EwGfxWarpPerspNativeGradientBlend ||                               \
    defined EwGfxWarpPerspNativeFilter ||                                      \
    defined EwGfxWarpPerspNativeFilterBlend ||                                 \
    defined EwGfxWarpPerspNativeFilterGradient ||                              \
    defined EwGfxWarpPerspNativeFilterGradientBlend ||                         \
    defined EwGfxWarpAffineNative ||                                           \
    defined EwGfxWarpAffineNativeBlend ||                                      \
    defined EwGfxWarpAffineNativeGradient ||                                   \
    defined EwGfxWarpAffineNativeGradientBlend ||                              \
    defined EwGfxWarpAffineNativeFilter ||                                     \
    defined EwGfxWarpAffineNativeFilterBlend ||                                \
    defined EwGfxWarpAffineNativeFilterGradient ||                             \
    defined EwGfxWarpAffineNativeFilterGradientBlend ||                        \
    defined EwGfxScaleNative ||                                                \
    defined EwGfxScaleNativeBlend ||                                           \
    defined EwGfxScaleNativeGradient ||                                        \
    defined EwGfxScaleNativeGradientBlend ||                                   \
    defined EwGfxScaleNativeLinearGradient ||                                  \
    defined EwGfxScaleNativeLinearGradientBlend ||                             \
    defined EwGfxScaleNativeFilter ||                                          \
    defined EwGfxScaleNativeFilterBlend ||                                     \
    defined EwGfxScaleNativeFilterGradient ||                                  \
    defined EwGfxScaleNativeFilterGradientBlend ||                             \
    defined EwGfxScaleNativeFilterLinearGradient ||                            \
    defined EwGfxScaleNativeFilterLinearGradientBlend ||                       \
    defined EwGfxPolygonSolid ||                                               \
    defined EwGfxPolygonSolidBlend ||                                          \
    defined EwGfxPolygonGradient ||                                            \
    defined EwGfxPolygonGradientBlend ||                                       \
    defined EwGfxPolygonLinearGradient ||                                      \
    defined EwGfxPolygonLinearGradientBlend ||                                 \
    defined EwGfxPolygonAntialiasedSolid ||                                    \
    defined EwGfxPolygonAntialiasedSolidBlend ||                               \
    defined EwGfxPolygonAntialiasedGradient ||                                 \
    defined EwGfxPolygonAntialiasedGradientBlend ||                            \
    defined EwGfxPolygonAntialiasedLinearGradient ||                           \
    defined EwGfxPolygonAntialiasedLinearGradientBlend
  #define EwNeedNativeSurface
#endif


/* Platform specific drawing operations involving index8 surface as source and
   the screen surface as destination? */
#if defined EwGfxScreenCopyIndex8 ||                                           \
    defined EwGfxScreenCopyIndex8Blend ||                                      \
    defined EwGfxScreenCopyIndex8Gradient ||                                   \
    defined EwGfxScreenCopyIndex8GradientBlend ||                              \
    defined EwGfxScreenCopyIndex8LinearGradient ||                             \
    defined EwGfxScreenCopyIndex8LinearGradientBlend ||                        \
    defined EwGfxScreenTileIndex8 ||                                           \
    defined EwGfxScreenTileIndex8Blend ||                                      \
    defined EwGfxScreenTileIndex8Solid ||                                      \
    defined EwGfxScreenTileIndex8SolidBlend ||                                 \
    defined EwGfxScreenTileIndex8Gradient ||                                   \
    defined EwGfxScreenTileIndex8GradientBlend ||                              \
    defined EwGfxScreenTileIndex8LinearGradient ||                             \
    defined EwGfxScreenTileIndex8LinearGradientBlend ||                        \
    defined EwGfxScreenWarpPerspIndex8 ||                                      \
    defined EwGfxScreenWarpPerspIndex8Blend ||                                 \
    defined EwGfxScreenWarpPerspIndex8Gradient ||                              \
    defined EwGfxScreenWarpPerspIndex8GradientBlend ||                         \
    defined EwGfxScreenWarpPerspIndex8Filter ||                                \
    defined EwGfxScreenWarpPerspIndex8FilterBlend ||                           \
    defined EwGfxScreenWarpPerspIndex8FilterGradient ||                        \
    defined EwGfxScreenWarpPerspIndex8FilterGradientBlend ||                   \
    defined EwGfxScreenWarpAffineIndex8 ||                                     \
    defined EwGfxScreenWarpAffineIndex8Blend ||                                \
    defined EwGfxScreenWarpAffineIndex8Gradient ||                             \
    defined EwGfxScreenWarpAffineIndex8GradientBlend ||                        \
    defined EwGfxScreenWarpAffineIndex8Filter ||                               \
    defined EwGfxScreenWarpAffineIndex8FilterBlend ||                          \
    defined EwGfxScreenWarpAffineIndex8FilterGradient ||                       \
    defined EwGfxScreenWarpAffineIndex8FilterGradientBlend ||                  \
    defined EwGfxScreenScaleIndex8 ||                                          \
    defined EwGfxScreenScaleIndex8Blend ||                                     \
    defined EwGfxScreenScaleIndex8Gradient ||                                  \
    defined EwGfxScreenScaleIndex8GradientBlend ||                             \
    defined EwGfxScreenScaleIndex8LinearGradient ||                            \
    defined EwGfxScreenScaleIndex8LinearGradientBlend ||                       \
    defined EwGfxScreenScaleIndex8Filter ||                                    \
    defined EwGfxScreenScaleIndex8FilterBlend ||                               \
    defined EwGfxScreenScaleIndex8FilterGradient ||                            \
    defined EwGfxScreenScaleIndex8FilterGradientBlend ||                       \
    defined EwGfxScreenScaleIndex8FilterLinearGradient ||                      \
    defined EwGfxScreenScaleIndex8FilterLinearGradientBlend
  #define EwNeedScreenSurface
  #define EwNeedIndex8Surface
#endif


/* Platform specific drawing operations involving index8 surface as source and
   the native surface as destination? */
#if defined EwGfxCopyIndex8 ||                                                 \
    defined EwGfxCopyIndex8Blend ||                                            \
    defined EwGfxCopyIndex8Gradient ||                                         \
    defined EwGfxCopyIndex8GradientBlend ||                                    \
    defined EwGfxCopyIndex8LinearGradient ||                                   \
    defined EwGfxCopyIndex8LinearGradientBlend ||                              \
    defined EwGfxTileIndex8 ||                                                 \
    defined EwGfxTileIndex8Blend ||                                            \
    defined EwGfxTileIndex8Solid ||                                            \
    defined EwGfxTileIndex8SolidBlend ||                                       \
    defined EwGfxTileIndex8Gradient ||                                         \
    defined EwGfxTileIndex8GradientBlend ||                                    \
    defined EwGfxTileIndex8LinearGradient ||                                   \
    defined EwGfxTileIndex8LinearGradientBlend ||                              \
    defined EwGfxWarpPerspIndex8 ||                                            \
    defined EwGfxWarpPerspIndex8Blend ||                                       \
    defined EwGfxWarpPerspIndex8Gradient ||                                    \
    defined EwGfxWarpPerspIndex8GradientBlend ||                               \
    defined EwGfxWarpPerspIndex8Filter ||                                      \
    defined EwGfxWarpPerspIndex8FilterBlend ||                                 \
    defined EwGfxWarpPerspIndex8FilterGradient ||                              \
    defined EwGfxWarpPerspIndex8FilterGradientBlend ||                         \
    defined EwGfxWarpAffineIndex8 ||                                           \
    defined EwGfxWarpAffineIndex8Blend ||                                      \
    defined EwGfxWarpAffineIndex8Gradient ||                                   \
    defined EwGfxWarpAffineIndex8GradientBlend ||                              \
    defined EwGfxWarpAffineIndex8Filter ||                                     \
    defined EwGfxWarpAffineIndex8FilterBlend ||                                \
    defined EwGfxWarpAffineIndex8FilterGradient ||                             \
    defined EwGfxWarpAffineIndex8FilterGradientBlend ||                        \
    defined EwGfxScaleIndex8 ||                                                \
    defined EwGfxScaleIndex8Blend ||                                           \
    defined EwGfxScaleIndex8Gradient ||                                        \
    defined EwGfxScaleIndex8GradientBlend ||                                   \
    defined EwGfxScaleIndex8LinearGradient ||                                  \
    defined EwGfxScaleIndex8LinearGradientBlend ||                             \
    defined EwGfxScaleIndex8Filter ||                                          \
    defined EwGfxScaleIndex8FilterBlend ||                                     \
    defined EwGfxScaleIndex8FilterGradient ||                                  \
    defined EwGfxScaleIndex8FilterGradientBlend ||                             \
    defined EwGfxScaleIndex8FilterLinearGradient ||                            \
    defined EwGfxScaleIndex8FilterLinearGradientBlend
  #define EwNeedNativeSurface
  #define EwNeedIndex8Surface
#endif


/* Platform specific drawing operations involving alpha8 surface as source and
   the screen surface as destination? */
#if defined EwGfxScreenCopyAlpha8Gradient ||                                   \
    defined EwGfxScreenCopyAlpha8GradientBlend ||                              \
    defined EwGfxScreenCopyAlpha8LinearGradient ||                             \
    defined EwGfxScreenCopyAlpha8LinearGradientBlend ||                        \
    defined EwGfxScreenTileAlpha8Solid ||                                      \
    defined EwGfxScreenTileAlpha8SolidBlend ||                                 \
    defined EwGfxScreenTileAlpha8Gradient ||                                   \
    defined EwGfxScreenTileAlpha8GradientBlend ||                              \
    defined EwGfxScreenTileAlpha8LinearGradient ||                             \
    defined EwGfxScreenTileAlpha8LinearGradientBlend ||                        \
    defined EwGfxScreenWarpPerspAlpha8Gradient ||                              \
    defined EwGfxScreenWarpPerspAlpha8GradientBlend ||                         \
    defined EwGfxScreenWarpPerspAlpha8FilterGradient ||                        \
    defined EwGfxScreenWarpPerspAlpha8FilterGradientBlend ||                   \
    defined EwGfxScreenWarpAffineAlpha8Gradient ||                             \
    defined EwGfxScreenWarpAffineAlpha8GradientBlend ||                        \
    defined EwGfxScreenWarpAffineAlpha8FilterGradient ||                       \
    defined EwGfxScreenWarpAffineAlpha8FilterGradientBlend ||                  \
    defined EwGfxScreenScaleAlpha8Gradient ||                                  \
    defined EwGfxScreenScaleAlpha8GradientBlend ||                             \
    defined EwGfxScreenScaleAlpha8FilterGradient ||                            \
    defined EwGfxScreenScaleAlpha8FilterGradientBlend ||                       \
    defined EwGfxScreenScaleAlpha8LinearGradient ||                            \
    defined EwGfxScreenScaleAlpha8LinearGradientBlend ||                       \
    defined EwGfxScreenScaleAlpha8FilterLinearGradient ||                      \
    defined EwGfxScreenScaleAlpha8FilterLinearGradientBlend
  #define EwNeedScreenSurface
  #define EwNeedAlpha8Surface
#endif


/* Platform specific drawing operations involving alpha8 surface as source and
   the native surface as destination? */
#if defined EwGfxCopyAlpha8Gradient ||                                         \
    defined EwGfxCopyAlpha8GradientBlend ||                                    \
    defined EwGfxCopyAlpha8LinearGradient ||                                   \
    defined EwGfxCopyAlpha8LinearGradientBlend ||                              \
    defined EwGfxTileAlpha8Solid ||                                            \
    defined EwGfxTileAlpha8SolidBlend ||                                       \
    defined EwGfxTileAlpha8Gradient ||                                         \
    defined EwGfxTileAlpha8GradientBlend ||                                    \
    defined EwGfxTileAlpha8LinearGradient ||                                   \
    defined EwGfxTileAlpha8LinearGradientBlend ||                              \
    defined EwGfxWarpPerspAlpha8Gradient ||                                    \
    defined EwGfxWarpPerspAlpha8GradientBlend ||                               \
    defined EwGfxWarpPerspAlpha8FilterGradient ||                              \
    defined EwGfxWarpPerspAlpha8FilterGradientBlend ||                         \
    defined EwGfxWarpAffineAlpha8Gradient ||                                   \
    defined EwGfxWarpAffineAlpha8GradientBlend ||                              \
    defined EwGfxWarpAffineAlpha8FilterGradient ||                             \
    defined EwGfxWarpAffineAlpha8FilterGradientBlend ||                        \
    defined EwGfxScaleAlpha8Gradient ||                                        \
    defined EwGfxScaleAlpha8GradientBlend ||                                   \
    defined EwGfxScaleAlpha8FilterGradient ||                                  \
    defined EwGfxScaleAlpha8FilterGradientBlend ||                             \
    defined EwGfxScaleAlpha8LinearGradient ||                                  \
    defined EwGfxScaleAlpha8LinearGradientBlend ||                             \
    defined EwGfxScaleAlpha8FilterLinearGradient ||                            \
    defined EwGfxScaleAlpha8FilterLinearGradientBlend
  #define EwNeedNativeSurface
  #define EwNeedAlpha8Surface
#endif


/* Platform specific drawing operations involving rgb565 surface as source and
   the screen surface as destination? */
#if defined EwGfxScreenCopyRGB565 ||                                           \
    defined EwGfxScreenCopyRGB565Gradient ||                                   \
    defined EwGfxScreenCopyRGB565GradientBlend ||                              \
    defined EwGfxScreenCopyRGB565LinearGradient ||                             \
    defined EwGfxScreenCopyRGB565LinearGradientBlend ||                        \
    defined EwGfxScreenTileRGB565 ||                                           \
    defined EwGfxScreenTileRGB565Solid ||                                      \
    defined EwGfxScreenTileRGB565SolidBlend ||                                 \
    defined EwGfxScreenTileRGB565Gradient ||                                   \
    defined EwGfxScreenTileRGB565GradientBlend ||                              \
    defined EwGfxScreenTileRGB565LinearGradient ||                             \
    defined EwGfxScreenTileRGB565LinearGradientBlend ||                        \
    defined EwGfxScreenWarpPerspRGB565 ||                                      \
    defined EwGfxScreenWarpPerspRGB565Gradient ||                              \
    defined EwGfxScreenWarpPerspRGB565GradientBlend ||                         \
    defined EwGfxScreenWarpPerspRGB565Filter ||                                \
    defined EwGfxScreenWarpPerspRGB565FilterGradient ||                        \
    defined EwGfxScreenWarpPerspRGB565FilterGradientBlend ||                   \
    defined EwGfxScreenWarpAffineRGB565 ||                                     \
    defined EwGfxScreenWarpAffineRGB565Gradient ||                             \
    defined EwGfxScreenWarpAffineRGB565GradientBlend ||                        \
    defined EwGfxScreenWarpAffineRGB565Filter ||                               \
    defined EwGfxScreenWarpAffineRGB565FilterGradient ||                       \
    defined EwGfxScreenWarpAffineRGB565FilterGradientBlend ||                  \
    defined EwGfxScreenScaleRGB565 ||                                          \
    defined EwGfxScreenScaleRGB565Gradient ||                                  \
    defined EwGfxScreenScaleRGB565GradientBlend ||                             \
    defined EwGfxScreenScaleRGB565LinearGradient ||                            \
    defined EwGfxScreenScaleRGB565LinearGradientBlend ||                       \
    defined EwGfxScreenScaleRGB565Filter ||                                    \
    defined EwGfxScreenScaleRGB565FilterGradient ||                            \
    defined EwGfxScreenScaleRGB565FilterGradientBlend ||                       \
    defined EwGfxScreenScaleRGB565FilterLinearGradient ||                      \
    defined EwGfxScreenScaleRGB565FilterLinearGradientBlend
  #define EwNeedScreenSurface
  #define EwNeedRGB565Surface
#endif


/* Platform specific drawing operations involving index8 surface as source and
   the native surface as destination? */
#if defined EwGfxCopyRGB565 ||                                                 \
    defined EwGfxCopyRGB565Gradient ||                                         \
    defined EwGfxCopyRGB565GradientBlend ||                                    \
    defined EwGfxCopyRGB565LinearGradient ||                                   \
    defined EwGfxCopyRGB565LinearGradientBlend ||                              \
    defined EwGfxTileRGB565 ||                                                 \
    defined EwGfxTileRGB565Solid ||                                            \
    defined EwGfxTileRGB565SolidBlend ||                                       \
    defined EwGfxTileRGB565Gradient ||                                         \
    defined EwGfxTileRGB565GradientBlend ||                                    \
    defined EwGfxTileRGB565LinearGradient ||                                   \
    defined EwGfxTileRGB565LinearGradientBlend ||                              \
    defined EwGfxWarpPerspRGB565 ||                                            \
    defined EwGfxWarpPerspRGB565Gradient ||                                    \
    defined EwGfxWarpPerspRGB565GradientBlend ||                               \
    defined EwGfxWarpPerspRGB565Filter ||                                      \
    defined EwGfxWarpPerspRGB565FilterGradient ||                              \
    defined EwGfxWarpPerspRGB565FilterGradientBlend ||                         \
    defined EwGfxWarpAffineRGB565 ||                                           \
    defined EwGfxWarpAffineRGB565Gradient ||                                   \
    defined EwGfxWarpAffineRGB565GradientBlend ||                              \
    defined EwGfxWarpAffineRGB565Filter ||                                     \
    defined EwGfxWarpAffineRGB565FilterGradient ||                             \
    defined EwGfxWarpAffineRGB565FilterGradientBlend ||                        \
    defined EwGfxScaleRGB565 ||                                                \
    defined EwGfxScaleRGB565Gradient ||                                        \
    defined EwGfxScaleRGB565GradientBlend ||                                   \
    defined EwGfxScaleRGB565LinearGradient ||                                  \
    defined EwGfxScaleRGB565LinearGradientBlend ||                             \
    defined EwGfxScaleRGB565Filter ||                                          \
    defined EwGfxScaleRGB565FilterGradient ||                                  \
    defined EwGfxScaleRGB565FilterGradientBlend ||                             \
    defined EwGfxScaleRGB565FilterLinearGradient ||                            \
    defined EwGfxScaleRGB565FilterLinearGradientBlend
  #define EwNeedNativeSurface
  #define EwNeedRGB565Surface
#endif


/* Contradictory screen surface format configuration */
#if !defined EW_USE_PIXEL_FORMAT_SCREEN &&                            \
     defined EwProvideScreenSurface
  #error "Screen surface is defined but not used by the viewport."
#endif

/* Verify the integrity of the adaptation of screen target drawing operations */
#if defined EwNeedScreenSurface && !defined EwProvideScreenSurface
  #error "Screen surface drawing operations without screen surface support."
#endif


/* Verify the integrity of the adaptation of native drawing operations */
#if defined EwNeedNativeSurface && !defined EwProvideNativeSurface
  #error "Native surface drawing operations without native surface support."
#endif


/* Verify the integrity of the adaptation of index8 drawing operations */
#if defined EwNeedIndex8Surface && !defined EwProvideIndex8Surface
  #error "Index8 surface drawing operations without index8 surface support."
#endif


/* Verify the integrity of the adaptation of alpha8 drawing operations */
#if defined EwNeedAlpha8Surface && !defined EwProvideAlpha8Surface
  #error "Alpha8 drawing operations found but no alpha8 surface support."
#endif


/* Verify the integrity of the adaptation of rgb565 drawing operations */
#if defined EwNeedRGB565Surface && !defined EwProvideRGB565Surface
  #error "Rgb565 surface drawing operations without rgb565 surface support."
#endif


/* Verify the integrity of the framebuffer/display adaptation */
#if !defined EwGfxInitViewport || !defined EwGfxDoneViewport
  #error "Incomplete viewport adaptation."
#endif

#if !defined EwGfxBeginUpdate || !defined EwGfxEndUpdate
  #error "Incomplete viewport adaptation."
#endif


/* Ignore all Software Pixel Driver worker functions responsable for the
   warp operations if this functionality is not needed. */
#ifdef EW_DONT_USE_WARP_FUNCTIONS
  #define EwWarpNativeRow                               0
  #define EwWarpNativeRowFilter                         0
  #define EwWarpNativeRowBlend                          0
  #define EwWarpNativeRowFilterBlend                    0
  #define EwWarpNativeRowGradient                       0
  #define EwWarpNativeRowSolid                          0
  #define EwWarpNativeRowFilterGradient                 0
  #define EwWarpNativeRowFilterSolid                    0
  #define EwWarpNativeRowGradientBlend                  0
  #define EwWarpNativeRowSolidBlend                     0
  #define EwWarpNativeRowFilterGradientBlend            0
  #define EwWarpNativeRowFilterSolidBlend               0
  #define EwWarpIndex8Row                               0
  #define EwWarpIndex8RowFilter                         0
  #define EwWarpIndex8RowBlend                          0
  #define EwWarpIndex8RowFilterBlend                    0
  #define EwWarpIndex8RowGradient                       0
  #define EwWarpIndex8RowSolid                          0
  #define EwWarpIndex8RowFilterGradient                 0
  #define EwWarpIndex8RowFilterSolid                    0
  #define EwWarpIndex8RowGradientBlend                  0
  #define EwWarpIndex8RowSolidBlend                     0
  #define EwWarpIndex8RowFilterGradientBlend            0
  #define EwWarpIndex8RowFilterSolidBlend               0
  #define EwWarpAlpha8RowGradient                       0
  #define EwWarpAlpha8RowSolid                          0
  #define EwWarpAlpha8RowFilterGradient                 0
  #define EwWarpAlpha8RowFilterSolid                    0
  #define EwWarpAlpha8RowGradientBlend                  0
  #define EwWarpAlpha8RowSolidBlend                     0
  #define EwWarpAlpha8RowFilterGradientBlend            0
  #define EwWarpAlpha8RowFilterSolidBlend               0
  #define EwWarpRGB565Row                               0
  #define EwWarpRGB565RowFilter                         0
  #define EwWarpRGB565RowGradient                       0
  #define EwWarpRGB565RowSolid                          0
  #define EwWarpRGB565RowFilterGradient                 0
  #define EwWarpRGB565RowFilterSolid                    0
  #define EwWarpRGB565RowGradientBlend                  0
  #define EwWarpRGB565RowSolidBlend                     0
  #define EwWarpRGB565RowFilterGradientBlend            0
  #define EwWarpRGB565RowFilterSolidBlend               0
  #define EwScreenWarpNativeRow                         0
  #define EwScreenWarpNativeRowFilter                   0
  #define EwScreenWarpNativeRowBlend                    0
  #define EwScreenWarpNativeRowFilterBlend              0
  #define EwScreenWarpNativeRowGradient                 0
  #define EwScreenWarpNativeRowSolid                    0
  #define EwScreenWarpNativeRowFilterGradient           0
  #define EwScreenWarpNativeRowFilterSolid              0
  #define EwScreenWarpNativeRowGradientBlend            0
  #define EwScreenWarpNativeRowSolidBlend               0
  #define EwScreenWarpNativeRowFilterGradientBlend      0
  #define EwScreenWarpNativeRowFilterSolidBlend         0
  #define EwScreenWarpIndex8Row                         0
  #define EwScreenWarpIndex8RowFilter                   0
  #define EwScreenWarpIndex8RowBlend                    0
  #define EwScreenWarpIndex8RowFilterBlend              0
  #define EwScreenWarpIndex8RowGradient                 0
  #define EwScreenWarpIndex8RowSolid                    0
  #define EwScreenWarpIndex8RowFilterGradient           0
  #define EwScreenWarpIndex8RowFilterSolid              0
  #define EwScreenWarpIndex8RowGradientBlend            0
  #define EwScreenWarpIndex8RowSolidBlend               0
  #define EwScreenWarpIndex8RowFilterGradientBlend      0
  #define EwScreenWarpIndex8RowFilterSolidBlend         0
  #define EwScreenWarpAlpha8RowGradient                 0
  #define EwScreenWarpAlpha8RowSolid                    0
  #define EwScreenWarpAlpha8RowFilterGradient           0
  #define EwScreenWarpAlpha8RowFilterSolid              0
  #define EwScreenWarpAlpha8RowGradientBlend            0
  #define EwScreenWarpAlpha8RowSolidBlend               0
  #define EwScreenWarpAlpha8RowFilterGradientBlend      0
  #define EwScreenWarpAlpha8RowFilterSolidBlend         0
  #define EwScreenWarpRGB565Row                         0
  #define EwScreenWarpRGB565RowFilter                   0
  #define EwScreenWarpRGB565RowGradient                 0
  #define EwScreenWarpRGB565RowSolid                    0
  #define EwScreenWarpRGB565RowFilterGradient           0
  #define EwScreenWarpRGB565RowFilterSolid              0
  #define EwScreenWarpRGB565RowGradientBlend            0
  #define EwScreenWarpRGB565RowSolidBlend               0
  #define EwScreenWarpRGB565RowFilterGradientBlend      0
  #define EwScreenWarpRGB565RowFilterSolidBlend         0
#endif


/* Ignore all Software Pixel Driver worker functions responsable for the
   copy/warp operations involving Index8 surfaces if this functionality
   is not needed. */
#ifdef EW_DONT_USE_INDEX8_SURFACES
  #define EwCopyIndex8Row                               0
  #define EwCopyIndex8RowBlend                          0
  #define EwCopyIndex8RowSolid                          0
  #define EwCopyIndex8RowSolidBlend                     0
  #define EwCopyIndex8RowGradient                       0
  #define EwCopyIndex8RowGradientBlend                  0
  #define EwWarpIndex8Row                               0
  #define EwWarpIndex8RowFilter                         0
  #define EwWarpIndex8RowBlend                          0
  #define EwWarpIndex8RowFilterBlend                    0
  #define EwWarpIndex8RowGradient                       0
  #define EwWarpIndex8RowSolid                          0
  #define EwWarpIndex8RowFilterGradient                 0
  #define EwWarpIndex8RowFilterSolid                    0
  #define EwWarpIndex8RowGradientBlend                  0
  #define EwWarpIndex8RowSolidBlend                     0
  #define EwWarpIndex8RowFilterGradientBlend            0
  #define EwWarpIndex8RowFilterSolidBlend               0
  #define EwScreenCopyIndex8Row                         0
  #define EwScreenCopyIndex8RowBlend                    0
  #define EwScreenCopyIndex8RowSolid                    0
  #define EwScreenCopyIndex8RowSolidBlend               0
  #define EwScreenCopyIndex8RowGradient                 0
  #define EwScreenCopyIndex8RowGradientBlend            0
  #define EwScreenWarpIndex8Row                         0
  #define EwScreenWarpIndex8RowFilter                   0
  #define EwScreenWarpIndex8RowBlend                    0
  #define EwScreenWarpIndex8RowFilterBlend              0
  #define EwScreenWarpIndex8RowGradient                 0
  #define EwScreenWarpIndex8RowSolid                    0
  #define EwScreenWarpIndex8RowFilterGradient           0
  #define EwScreenWarpIndex8RowFilterSolid              0
  #define EwScreenWarpIndex8RowGradientBlend            0
  #define EwScreenWarpIndex8RowSolidBlend               0
  #define EwScreenWarpIndex8RowFilterGradientBlend      0
  #define EwScreenWarpIndex8RowFilterSolidBlend         0
#endif


/* Ignore all Software Pixel Driver worker functions responsable for the
   copy/warp operations involving RGB565 surfaces if this functionality
   is not needed. */
#ifdef EW_DONT_USE_RGB565_SURFACES
  #define EwCopyRGB565Row                               0
  #define EwCopyRGB565RowSolid                          0
  #define EwCopyRGB565RowSolidBlend                     0
  #define EwCopyRGB565RowGradient                       0
  #define EwCopyRGB565RowGradientBlend                  0
  #define EwWarpRGB565Row                               0
  #define EwWarpRGB565RowFilter                         0
  #define EwWarpRGB565RowGradient                       0
  #define EwWarpRGB565RowSolid                          0
  #define EwWarpRGB565RowFilterGradient                 0
  #define EwWarpRGB565RowFilterSolid                    0
  #define EwWarpRGB565RowGradientBlend                  0
  #define EwWarpRGB565RowSolidBlend                     0
  #define EwWarpRGB565RowFilterGradientBlend            0
  #define EwWarpRGB565RowFilterSolidBlend               0
  #define EwScreenCopyRGB565Row                         0
  #define EwScreenCopyRGB565RowSolid                    0
  #define EwScreenCopyRGB565RowSolidBlend               0
  #define EwScreenCopyRGB565RowGradient                 0
  #define EwScreenCopyRGB565RowGradientBlend            0
  #define EwScreenWarpRGB565Row                         0
  #define EwScreenWarpRGB565RowFilter                   0
  #define EwScreenWarpRGB565RowGradient                 0
  #define EwScreenWarpRGB565RowSolid                    0
  #define EwScreenWarpRGB565RowFilterGradient           0
  #define EwScreenWarpRGB565RowFilterSolid              0
  #define EwScreenWarpRGB565RowGradientBlend            0
  #define EwScreenWarpRGB565RowSolidBlend               0
  #define EwScreenWarpRGB565RowFilterGradientBlend      0
  #define EwScreenWarpRGB565RowFilterSolidBlend         0
#endif


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'draw a line' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxDrawLineGradient
  #define EwSetPixelSolid            0
#else
  #define EwGfxDrawLineGradient      0
#endif

#ifdef EwGfxDrawLineGradientBlend
  #define EwSetPixelSolidBlend       0
#else
  #define EwGfxDrawLineGradientBlend 0
#endif

#ifndef EwGfxDrawLineSolid
  #define EwGfxDrawLineSolid         EwGfxDrawLineGradient
#endif

#ifndef EwGfxDrawLineSolidBlend
  #define EwGfxDrawLineSolidBlend    EwGfxDrawLineGradientBlend
#endif

#ifdef EwGfxScreenDrawLineGradient
  #define EwScreenSetPixelSolid            0
#else
  #define EwGfxScreenDrawLineGradient      0
#endif

#ifdef EwGfxScreenDrawLineGradientBlend
  #define EwScreenSetPixelSolidBlend       0
#else
  #define EwGfxScreenDrawLineGradientBlend 0
#endif

#ifndef EwGfxScreenDrawLineSolid
  #define EwGfxScreenDrawLineSolid         EwGfxScreenDrawLineGradient
#endif

#ifndef EwGfxScreenDrawLineSolidBlend
  #define EwGfxScreenDrawLineSolidBlend    EwGfxScreenDrawLineGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XLineDriver LineDrivers[] =
{
  0, 0,
  EwGfxDrawLineSolid,                EwGfxDrawLineSolidBlend, 
  EwGfxDrawLineGradient,             EwGfxDrawLineGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  0, 0, /* Gap */
  0, 0,
  EwGfxScreenDrawLineSolid,          EwGfxScreenDrawLineSolidBlend, 
  EwGfxScreenDrawLineGradient,       EwGfxScreenDrawLineGradientBlend,
#endif

  0
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XLineWorker LineWorkers[] =
{
  0, 0,
  EwSetPixelSolid,                  EwSetPixelSolidBlend, 
  EwSetPixelSolid,                  EwSetPixelSolidBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  0, 0, /* Gap */
  0, 0,
  EwScreenSetPixelSolid,            EwScreenSetPixelSolidBlend, 
  EwScreenSetPixelSolid,            EwScreenSetPixelSolidBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'fill rectangle' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxFillGradient
  #define EwFillRowGradient                   0
  #define EwFillRowSolid                      0
#else
  #define EwGfxFillGradient                   0
#endif

#ifdef EwGfxFillGradientBlend
  #define EwFillRowGradientBlend              0
  #define EwFillRowSolidBlend                 0
#else
  #define EwGfxFillGradientBlend              0
#endif

#ifdef EwGfxFillLinearGradient
  #define EwFillRowSolid                      0
#else
  #define EwGfxFillLinearGradient             EwGfxFillGradient
#endif

#ifdef EwGfxFillLinearGradientBlend
  #define EwFillRowSolidBlend                 0
#else
  #define EwGfxFillLinearGradientBlend        EwGfxFillGradientBlend
#endif

#ifndef EwGfxFillSolid
  #define EwGfxFillSolid                      EwGfxFillLinearGradient
#endif

#ifndef EwGfxFillSolidBlend
  #define EwGfxFillSolidBlend                 EwGfxFillLinearGradientBlend
#endif

#ifdef EwGfxScreenFillGradient
  #define EwScreenFillRowGradient             0
  #define EwScreenFillRowSolid                0
#else
  #define EwGfxScreenFillGradient             0
#endif

#ifdef EwGfxScreenFillGradientBlend
  #define EwScreenFillRowGradientBlend        0
  #define EwScreenFillRowSolidBlend           0
#else
  #define EwGfxScreenFillGradientBlend        0
#endif

#ifdef EwGfxScreenFillLinearGradient
  #define EwScreenFillRowSolid                0
#else
  #define EwGfxScreenFillLinearGradient       EwGfxScreenFillGradient
#endif

#ifdef EwGfxScreenFillLinearGradientBlend
  #define EwScreenFillRowSolidBlend           0
#else
  #define EwGfxScreenFillLinearGradientBlend  EwGfxScreenFillGradientBlend
#endif

#ifndef EwGfxScreenFillSolid
  #define EwGfxScreenFillSolid                EwGfxScreenFillLinearGradient
#endif

#ifndef EwGfxScreenFillSolidBlend
  #define EwGfxScreenFillSolidBlend           EwGfxScreenFillLinearGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XFillDriver FillDrivers[] =
{
  0, 0,
  EwGfxFillSolid,                   EwGfxFillSolidBlend, 
  EwGfxFillGradient,                EwGfxFillGradientBlend,
  EwGfxFillLinearGradient,          EwGfxFillLinearGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  0, 0,
  EwGfxScreenFillSolid,             EwGfxScreenFillSolidBlend, 
  EwGfxScreenFillGradient,          EwGfxScreenFillGradientBlend,
  EwGfxScreenFillLinearGradient,    EwGfxScreenFillLinearGradientBlend,
#endif

  0
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XFillWorker FillWorkers[] =
{
  0, 0,
  EwFillRowSolid,                   EwFillRowSolidBlend, 
  EwFillRowGradient,                EwFillRowGradientBlend,
  EwFillRowGradient,                EwFillRowGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  0, 0,
  EwScreenFillRowSolid,             EwScreenFillRowSolidBlend, 
  EwScreenFillRowGradient,          EwScreenFillRowGradientBlend,
  EwScreenFillRowGradient,          EwScreenFillRowGradientBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'copy surface' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxCopyNative
  #define EwCopyNativeRow                          0
#else
  #define EwGfxCopyNative                          0
#endif

#ifdef EwGfxCopyNativeBlend
  #define EwCopyNativeRowBlend                     0
#else
  #define EwGfxCopyNativeBlend                     0
#endif

#ifdef EwGfxCopyNativeGradient
  #define EwCopyNativeRowGradient                  0
  #define EwCopyNativeRowSolid                     0
#else
  #define EwGfxCopyNativeGradient                  0
#endif

#ifdef EwGfxCopyNativeGradientBlend
  #define EwCopyNativeRowGradientBlend             0
  #define EwCopyNativeRowSolidBlend                0
#else
  #define EwGfxCopyNativeGradientBlend             0
#endif

#ifdef EwGfxCopyNativeLinearGradient
  #define EwCopyNativeRowSolid                     0
#else
  #define EwGfxCopyNativeLinearGradient            EwGfxCopyNativeGradient
#endif

#ifdef EwGfxCopyNativeLinearGradientBlend
  #define EwCopyNativeRowSolidBlend                0
#else
  #define EwGfxCopyNativeLinearGradientBlend       EwGfxCopyNativeGradientBlend
#endif

#ifndef EwGfxCopyNativeSolid
  #define EwGfxCopyNativeSolid                     EwGfxCopyNativeLinearGradient
#endif

#ifndef EwGfxCopyNativeSolidBlend
  #define EwGfxCopyNativeSolidBlend                EwGfxCopyNativeLinearGradientBlend
#endif

#ifdef EwGfxCopyIndex8
  #define EwCopyIndex8Row                          0
#else
  #define EwGfxCopyIndex8                          0
#endif

#ifdef EwGfxCopyIndex8Blend
  #define EwCopyIndex8RowBlend                     0
#else
  #define EwGfxCopyIndex8Blend                     0
#endif

#ifdef EwGfxCopyIndex8Gradient
  #define EwCopyIndex8RowGradient                  0
  #define EwCopyIndex8RowSolid                     0
#else
  #define EwGfxCopyIndex8Gradient                  0
#endif

#ifdef EwGfxCopyIndex8GradientBlend
  #define EwCopyIndex8RowGradientBlend             0
  #define EwCopyIndex8RowSolidBlend                0
#else
  #define EwGfxCopyIndex8GradientBlend             0
#endif

#ifdef EwGfxCopyIndex8LinearGradient
  #define EwCopyIndex8RowSolid                     0
#else
  #define EwGfxCopyIndex8LinearGradient            EwGfxCopyIndex8Gradient
#endif

#ifdef EwGfxCopyIndex8LinearGradientBlend
  #define EwCopyIndex8RowSolidBlend                0
#else
  #define EwGfxCopyIndex8LinearGradientBlend       EwGfxCopyIndex8GradientBlend
#endif

#ifndef EwGfxCopyIndex8Solid
  #define EwGfxCopyIndex8Solid                     EwGfxCopyIndex8LinearGradient
#endif

#ifndef EwGfxCopyIndex8SolidBlend
  #define EwGfxCopyIndex8SolidBlend                EwGfxCopyIndex8LinearGradientBlend
#endif

#ifdef EwGfxCopyAlpha8Gradient
  #define EwCopyAlpha8RowGradient                  0
  #define EwCopyAlpha8RowSolid                     0
#else
  #define EwGfxCopyAlpha8Gradient                  0
#endif

#ifdef EwGfxCopyAlpha8GradientBlend
  #define EwCopyAlpha8RowGradientBlend             0
  #define EwCopyAlpha8RowSolidBlend                0
#else
  #define EwGfxCopyAlpha8GradientBlend             0
#endif

#ifdef EwGfxCopyAlpha8LinearGradient
  #define EwCopyAlpha8RowSolid                     0
#else
  #define EwGfxCopyAlpha8LinearGradient            EwGfxCopyAlpha8Gradient
#endif

#ifdef EwGfxCopyAlpha8LinearGradientBlend
  #define EwCopyAlpha8RowSolidBlend                0
#else
  #define EwGfxCopyAlpha8LinearGradientBlend       EwGfxCopyAlpha8GradientBlend
#endif

#ifndef EwGfxCopyAlpha8Solid
  #define EwGfxCopyAlpha8Solid                     EwGfxCopyAlpha8LinearGradient
#endif

#ifndef EwGfxCopyAlpha8SolidBlend
  #define EwGfxCopyAlpha8SolidBlend                EwGfxCopyAlpha8LinearGradientBlend
#endif

#ifdef EwGfxCopyRGB565
  #define EwCopyRGB565Row                          0
#else
  #define EwGfxCopyRGB565                          0
#endif

#ifdef EwGfxCopyRGB565Gradient
  #define EwCopyRGB565RowGradient                  0
  #define EwCopyRGB565RowSolid                     0
#else
  #define EwGfxCopyRGB565Gradient                  0
#endif

#ifdef EwGfxCopyRGB565GradientBlend
  #define EwCopyRGB565RowGradientBlend             0
  #define EwCopyRGB565RowSolidBlend                0
#else
  #define EwGfxCopyRGB565GradientBlend             0
#endif

#ifdef EwGfxCopyRGB565LinearGradient
  #define EwCopyRGB565RowSolid                     0
#else
  #define EwGfxCopyRGB565LinearGradient            EwGfxCopyRGB565Gradient
#endif

#ifdef EwGfxCopyRGB565LinearGradientBlend
  #define EwCopyRGB565RowSolidBlend                0
#else
  #define EwGfxCopyRGB565LinearGradientBlend       EwGfxCopyRGB565GradientBlend
#endif

#ifndef EwGfxCopyRGB565Solid
  #define EwGfxCopyRGB565Solid                     EwGfxCopyRGB565LinearGradient
#endif

#ifndef EwGfxCopyRGB565SolidBlend
  #define EwGfxCopyRGB565SolidBlend                EwGfxCopyRGB565LinearGradientBlend
#endif

#ifdef EwGfxScreenCopyNative
  #define EwScreenCopyNativeRow                    0
#else
  #define EwGfxScreenCopyNative                    0
#endif

#ifdef EwGfxScreenCopyNativeBlend
  #define EwScreenCopyNativeRowBlend               0
#else
  #define EwGfxScreenCopyNativeBlend               0
#endif

#ifdef EwGfxScreenCopyNativeGradient
  #define EwScreenCopyNativeRowGradient            0
  #define EwScreenCopyNativeRowSolid               0
#else
  #define EwGfxScreenCopyNativeGradient            0
#endif

#ifdef EwGfxScreenCopyNativeGradientBlend
  #define EwScreenCopyNativeRowGradientBlend       0
  #define EwScreenCopyNativeRowSolidBlend          0
#else
  #define EwGfxScreenCopyNativeGradientBlend       0
#endif

#ifdef EwGfxScreenCopyNativeLinearGradient
  #define EwScreenCopyNativeRowSolid               0
#else
  #define EwGfxScreenCopyNativeLinearGradient      EwGfxScreenCopyNativeGradient
#endif

#ifdef EwGfxScreenCopyNativeLinearGradientBlend
  #define EwScreenCopyNativeRowSolidBlend          0
#else
  #define EwGfxScreenCopyNativeLinearGradientBlend EwGfxScreenCopyNativeGradientBlend
#endif

#ifndef EwGfxScreenCopyNativeSolid
  #define EwGfxScreenCopyNativeSolid               EwGfxScreenCopyNativeLinearGradient
#endif

#ifndef EwGfxScreenCopyNativeSolidBlend
  #define EwGfxScreenCopyNativeSolidBlend          EwGfxScreenCopyNativeLinearGradientBlend
#endif

#ifdef EwGfxScreenCopyIndex8
  #define EwScreenCopyIndex8Row                    0
#else
  #define EwGfxScreenCopyIndex8                    0
#endif

#ifdef EwGfxScreenCopyIndex8Blend
  #define EwScreenCopyIndex8RowBlend               0
#else
  #define EwGfxScreenCopyIndex8Blend               0
#endif

#ifdef EwGfxScreenCopyIndex8Gradient
  #define EwScreenCopyIndex8RowGradient            0
  #define EwScreenCopyIndex8RowSolid               0
#else
  #define EwGfxScreenCopyIndex8Gradient            0
#endif

#ifdef EwGfxScreenCopyIndex8GradientBlend
  #define EwScreenCopyIndex8RowGradientBlend       0
  #define EwScreenCopyIndex8RowSolidBlend          0
#else
  #define EwGfxScreenCopyIndex8GradientBlend       0
#endif

#ifdef EwGfxScreenCopyIndex8LinearGradient
  #define EwScreenCopyIndex8RowSolid               0
#else
  #define EwGfxScreenCopyIndex8LinearGradient      EwGfxScreenCopyIndex8Gradient
#endif

#ifdef EwGfxScreenCopyIndex8LinearGradientBlend
  #define EwScreenCopyIndex8RowSolidBlend          0
#else
  #define EwGfxScreenCopyIndex8LinearGradientBlend EwGfxScreenCopyIndex8GradientBlend
#endif

#ifndef EwGfxScreenCopyIndex8Solid
  #define EwGfxScreenCopyIndex8Solid               EwGfxScreenCopyIndex8LinearGradient
#endif

#ifndef EwGfxScreenCopyIndex8SolidBlend
  #define EwGfxScreenCopyIndex8SolidBlend          EwGfxScreenCopyIndex8LinearGradientBlend
#endif

#ifdef EwGfxScreenCopyAlpha8Gradient
  #define EwScreenCopyAlpha8RowGradient            0
  #define EwScreenCopyAlpha8RowSolid               0
#else
  #define EwGfxScreenCopyAlpha8Gradient            0
#endif

#ifdef EwGfxScreenCopyAlpha8GradientBlend
  #define EwScreenCopyAlpha8RowGradientBlend       0
  #define EwScreenCopyAlpha8RowSolidBlend          0
#else
  #define EwGfxScreenCopyAlpha8GradientBlend       0
#endif

#ifdef EwGfxScreenCopyAlpha8LinearGradient
  #define EwScreenCopyAlpha8RowSolid               0
#else
  #define EwGfxScreenCopyAlpha8LinearGradient      EwGfxScreenCopyAlpha8Gradient
#endif

#ifdef EwGfxScreenCopyAlpha8LinearGradientBlend
  #define EwScreenCopyAlpha8RowSolidBlend          0
#else
  #define EwGfxScreenCopyAlpha8LinearGradientBlend EwGfxScreenCopyAlpha8GradientBlend
#endif

#ifndef EwGfxScreenCopyAlpha8Solid
  #define EwGfxScreenCopyAlpha8Solid               EwGfxScreenCopyAlpha8LinearGradient
#endif

#ifndef EwGfxScreenCopyAlpha8SolidBlend
  #define EwGfxScreenCopyAlpha8SolidBlend          EwGfxScreenCopyAlpha8LinearGradientBlend
#endif

#ifdef EwGfxScreenCopyRGB565
  #define EwScreenCopyRGB565Row                    0
#else
  #define EwGfxScreenCopyRGB565                    0
#endif

#ifdef EwGfxScreenCopyRGB565Gradient
  #define EwScreenCopyRGB565RowGradient            0
  #define EwScreenCopyRGB565RowSolid               0
#else
  #define EwGfxScreenCopyRGB565Gradient            0
#endif

#ifdef EwGfxScreenCopyRGB565GradientBlend
  #define EwScreenCopyRGB565RowGradientBlend       0
  #define EwScreenCopyRGB565RowSolidBlend          0
#else
  #define EwGfxScreenCopyRGB565GradientBlend       0
#endif

#ifdef EwGfxScreenCopyRGB565LinearGradient
  #define EwScreenCopyRGB565RowSolid               0
#else
  #define EwGfxScreenCopyRGB565LinearGradient      EwGfxScreenCopyRGB565Gradient
#endif

#ifdef EwGfxScreenCopyRGB565LinearGradientBlend
  #define EwScreenCopyRGB565RowSolidBlend          0
#else
  #define EwGfxScreenCopyRGB565LinearGradientBlend EwGfxScreenCopyRGB565GradientBlend
#endif

#ifndef EwGfxScreenCopyRGB565Solid
  #define EwGfxScreenCopyRGB565Solid               EwGfxScreenCopyRGB565LinearGradient
#endif

#ifndef EwGfxScreenCopyRGB565SolidBlend
  #define EwGfxScreenCopyRGB565SolidBlend          EwGfxScreenCopyRGB565LinearGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XCopyDriver CopyDrivers[] =
{
  EwGfxCopyNative,                     EwGfxCopyNativeBlend,
  EwGfxCopyNativeSolid,                EwGfxCopyNativeSolidBlend,
  EwGfxCopyNativeGradient,             EwGfxCopyNativeGradientBlend,
  EwGfxCopyNativeLinearGradient,       EwGfxCopyNativeLinearGradientBlend,
  EwGfxCopyIndex8,                     EwGfxCopyIndex8Blend,
  EwGfxCopyIndex8Solid,                EwGfxCopyIndex8SolidBlend,
  EwGfxCopyIndex8Gradient,             EwGfxCopyIndex8GradientBlend,
  EwGfxCopyIndex8LinearGradient,       EwGfxCopyIndex8LinearGradientBlend,
  0,                                   0,
  EwGfxCopyAlpha8Solid,                EwGfxCopyAlpha8SolidBlend,
  EwGfxCopyAlpha8Gradient,             EwGfxCopyAlpha8GradientBlend,
  EwGfxCopyAlpha8LinearGradient,       EwGfxCopyAlpha8LinearGradientBlend,
  EwGfxCopyRGB565,                     EwGfxCopyRGB565,
  EwGfxCopyRGB565Solid,                EwGfxCopyRGB565SolidBlend,
  EwGfxCopyRGB565Gradient,             EwGfxCopyRGB565GradientBlend,
  EwGfxCopyRGB565LinearGradient,       EwGfxCopyRGB565LinearGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwGfxScreenCopyNative,               EwGfxScreenCopyNativeBlend,
  EwGfxScreenCopyNativeSolid,          EwGfxScreenCopyNativeSolidBlend,
  EwGfxScreenCopyNativeGradient,       EwGfxScreenCopyNativeGradientBlend,
  EwGfxScreenCopyNativeLinearGradient, EwGfxScreenCopyNativeLinearGradientBlend,
  EwGfxScreenCopyIndex8,               EwGfxScreenCopyIndex8Blend,
  EwGfxScreenCopyIndex8Solid,          EwGfxScreenCopyIndex8SolidBlend,
  EwGfxScreenCopyIndex8Gradient,       EwGfxScreenCopyIndex8GradientBlend,
  EwGfxScreenCopyIndex8LinearGradient, EwGfxScreenCopyIndex8LinearGradientBlend,
  0,                                   0,
  EwGfxScreenCopyAlpha8Solid,          EwGfxScreenCopyAlpha8SolidBlend,
  EwGfxScreenCopyAlpha8Gradient,       EwGfxScreenCopyAlpha8GradientBlend,
  EwGfxScreenCopyAlpha8LinearGradient, EwGfxScreenCopyAlpha8LinearGradientBlend,
  EwGfxScreenCopyRGB565,               EwGfxScreenCopyRGB565,
  EwGfxScreenCopyRGB565Solid,          EwGfxScreenCopyRGB565SolidBlend,
  EwGfxScreenCopyRGB565Gradient,       EwGfxScreenCopyRGB565GradientBlend,
  EwGfxScreenCopyRGB565LinearGradient, EwGfxScreenCopyRGB565LinearGradientBlend,
#endif

  0
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XCopyWorker CopyWorkers[] =
{
  EwCopyNativeRow,                     EwCopyNativeRowBlend,
  EwCopyNativeRowSolid,                EwCopyNativeRowSolidBlend,
  EwCopyNativeRowGradient,             EwCopyNativeRowGradientBlend,
  EwCopyNativeRowGradient,             EwCopyNativeRowGradientBlend,
  EwCopyIndex8Row,                     EwCopyIndex8RowBlend,
  EwCopyIndex8RowSolid,                EwCopyIndex8RowSolidBlend,
  EwCopyIndex8RowGradient,             EwCopyIndex8RowGradientBlend,
  EwCopyIndex8RowGradient,             EwCopyIndex8RowGradientBlend,
  0,                                   0,
  EwCopyAlpha8RowSolid,                EwCopyAlpha8RowSolidBlend,
  EwCopyAlpha8RowGradient,             EwCopyAlpha8RowGradientBlend,
  EwCopyAlpha8RowGradient,             EwCopyAlpha8RowGradientBlend,
  EwCopyRGB565Row,                     EwCopyRGB565Row,
  EwCopyRGB565RowSolid,                EwCopyRGB565RowSolidBlend,
  EwCopyRGB565RowGradient,             EwCopyRGB565RowGradientBlend,
  EwCopyRGB565RowGradient,             EwCopyRGB565RowGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwScreenCopyNativeRow,               EwScreenCopyNativeRowBlend,
  EwScreenCopyNativeRowSolid,          EwScreenCopyNativeRowSolidBlend,
  EwScreenCopyNativeRowGradient,       EwScreenCopyNativeRowGradientBlend,
  EwScreenCopyNativeRowGradient,       EwScreenCopyNativeRowGradientBlend,
  EwScreenCopyIndex8Row,               EwScreenCopyIndex8RowBlend,
  EwScreenCopyIndex8RowSolid,          EwScreenCopyIndex8RowSolidBlend,
  EwScreenCopyIndex8RowGradient,       EwScreenCopyIndex8RowGradientBlend,
  EwScreenCopyIndex8RowGradient,       EwScreenCopyIndex8RowGradientBlend,
  0,                                   0,
  EwScreenCopyAlpha8RowSolid,          EwScreenCopyAlpha8RowSolidBlend,
  EwScreenCopyAlpha8RowGradient,       EwScreenCopyAlpha8RowGradientBlend,
  EwScreenCopyAlpha8RowGradient,       EwScreenCopyAlpha8RowGradientBlend,
  EwScreenCopyRGB565Row,               EwScreenCopyRGB565Row,
  EwScreenCopyRGB565RowSolid,          EwScreenCopyRGB565RowSolidBlend,
  EwScreenCopyRGB565RowGradient,       EwScreenCopyRGB565RowGradientBlend,
  EwScreenCopyRGB565RowGradient,       EwScreenCopyRGB565RowGradientBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'tile surface' operation. For missing tile
   functionality multiple copy operations will be used. */
#ifndef EwGfxTileNative
  #define EwGfxTileNative                          0
#endif

#ifndef EwGfxTileNativeBlend
  #define EwGfxTileNativeBlend                     0
#endif

#ifndef EwGfxTileNativeSolid
  #define EwGfxTileNativeSolid                     0
#endif

#ifndef EwGfxTileNativeSolidBlend
  #define EwGfxTileNativeSolidBlend                0
#endif

#ifndef EwGfxTileNativeGradient
  #define EwGfxTileNativeGradient                  0
#endif

#ifndef EwGfxTileNativeGradientBlend
  #define EwGfxTileNativeGradientBlend             0
#endif

#ifndef EwGfxTileNativeLinearGradient
  #define EwGfxTileNativeLinearGradient            0
#endif

#ifndef EwGfxTileNativeLinearGradientBlend
  #define EwGfxTileNativeLinearGradientBlend       0
#endif

#ifndef EwGfxTileIndex8
  #define EwGfxTileIndex8                          0
#endif

#ifndef EwGfxTileIndex8Blend
  #define EwGfxTileIndex8Blend                     0
#endif

#ifndef EwGfxTileIndex8Solid
  #define EwGfxTileIndex8Solid                     0
#endif

#ifndef EwGfxTileIndex8SolidBlend
  #define EwGfxTileIndex8SolidBlend                0
#endif

#ifndef EwGfxTileIndex8Gradient
  #define EwGfxTileIndex8Gradient                  0
#endif

#ifndef EwGfxTileIndex8GradientBlend
  #define EwGfxTileIndex8GradientBlend             0
#endif

#ifndef EwGfxTileIndex8LinearGradient
  #define EwGfxTileIndex8LinearGradient            0
#endif

#ifndef EwGfxTileIndex8LinearGradientBlend
  #define EwGfxTileIndex8LinearGradientBlend       0
#endif

#ifndef EwGfxTileAlpha8Solid
  #define EwGfxTileAlpha8Solid                     0
#endif

#ifndef EwGfxTileAlpha8SolidBlend
  #define EwGfxTileAlpha8SolidBlend                0
#endif

#ifndef EwGfxTileAlpha8Gradient
  #define EwGfxTileAlpha8Gradient                  0
#endif

#ifndef EwGfxTileAlpha8GradientBlend
  #define EwGfxTileAlpha8GradientBlend             0
#endif

#ifndef EwGfxTileAlpha8LinearGradient
  #define EwGfxTileAlpha8LinearGradient            0
#endif

#ifndef EwGfxTileAlpha8LinearGradientBlend
  #define EwGfxTileAlpha8LinearGradientBlend       0
#endif

#ifndef EwGfxTileRGB565
  #define EwGfxTileRGB565                          0
#endif

#ifndef EwGfxTileRGB565Solid
  #define EwGfxTileRGB565Solid                     0
#endif

#ifndef EwGfxTileRGB565SolidBlend
  #define EwGfxTileRGB565SolidBlend                0
#endif

#ifndef EwGfxTileRGB565Gradient
  #define EwGfxTileRGB565Gradient                  0
#endif

#ifndef EwGfxTileRGB565GradientBlend
  #define EwGfxTileRGB565GradientBlend             0
#endif

#ifndef EwGfxTileRGB565LinearGradient
  #define EwGfxTileRGB565LinearGradient            0
#endif

#ifndef EwGfxTileRGB565LinearGradientBlend
  #define EwGfxTileRGB565LinearGradientBlend       0
#endif

#ifndef EwGfxScreenTileNative
  #define EwGfxScreenTileNative                    0
#endif

#ifndef EwGfxScreenTileNativeBlend
  #define EwGfxScreenTileNativeBlend               0
#endif

#ifndef EwGfxScreenTileNativeSolid
  #define EwGfxScreenTileNativeSolid               0
#endif

#ifndef EwGfxScreenTileNativeSolidBlend
  #define EwGfxScreenTileNativeSolidBlend          0
#endif

#ifndef EwGfxScreenTileNativeGradient
  #define EwGfxScreenTileNativeGradient            0
#endif

#ifndef EwGfxScreenTileNativeGradientBlend
  #define EwGfxScreenTileNativeGradientBlend       0
#endif

#ifndef EwGfxScreenTileNativeLinearGradient
  #define EwGfxScreenTileNativeLinearGradient      0
#endif

#ifndef EwGfxScreenTileNativeLinearGradientBlend
  #define EwGfxScreenTileNativeLinearGradientBlend 0
#endif

#ifndef EwGfxScreenTileIndex8
  #define EwGfxScreenTileIndex8                    0
#endif

#ifndef EwGfxScreenTileIndex8Blend
  #define EwGfxScreenTileIndex8Blend               0
#endif

#ifndef EwGfxScreenTileIndex8Solid
  #define EwGfxScreenTileIndex8Solid               0
#endif

#ifndef EwGfxScreenTileIndex8SolidBlend
  #define EwGfxScreenTileIndex8SolidBlend          0
#endif

#ifndef EwGfxScreenTileIndex8Gradient
  #define EwGfxScreenTileIndex8Gradient            0
#endif

#ifndef EwGfxScreenTileIndex8GradientBlend
  #define EwGfxScreenTileIndex8GradientBlend       0
#endif

#ifndef EwGfxScreenTileIndex8LinearGradient
  #define EwGfxScreenTileIndex8LinearGradient      0
#endif

#ifndef EwGfxScreenTileIndex8LinearGradientBlend
  #define EwGfxScreenTileIndex8LinearGradientBlend 0
#endif

#ifndef EwGfxScreenTileAlpha8Solid
  #define EwGfxScreenTileAlpha8Solid               0
#endif

#ifndef EwGfxScreenTileAlpha8SolidBlend
  #define EwGfxScreenTileAlpha8SolidBlend          0
#endif

#ifndef EwGfxScreenTileAlpha8Gradient
  #define EwGfxScreenTileAlpha8Gradient            0
#endif

#ifndef EwGfxScreenTileAlpha8GradientBlend
  #define EwGfxScreenTileAlpha8GradientBlend       0
#endif

#ifndef EwGfxScreenTileAlpha8LinearGradient
  #define EwGfxScreenTileAlpha8LinearGradient      0
#endif

#ifndef EwGfxScreenTileAlpha8LinearGradientBlend
  #define EwGfxScreenTileAlpha8LinearGradientBlend 0
#endif

#ifndef EwGfxScreenTileRGB565
  #define EwGfxScreenTileRGB565                    0
#endif

#ifndef EwGfxScreenTileRGB565Solid
  #define EwGfxScreenTileRGB565Solid               0
#endif

#ifndef EwGfxScreenTileRGB565SolidBlend
  #define EwGfxScreenTileRGB565SolidBlend          0
#endif

#ifndef EwGfxScreenTileRGB565Gradient
  #define EwGfxScreenTileRGB565Gradient            0
#endif

#ifndef EwGfxScreenTileRGB565GradientBlend
  #define EwGfxScreenTileRGB565GradientBlend       0
#endif

#ifndef EwGfxScreenTileRGB565LinearGradient
  #define EwGfxScreenTileRGB565LinearGradient      0
#endif

#ifndef EwGfxScreenTileRGB565LinearGradientBlend
  #define EwGfxScreenTileRGB565LinearGradientBlend 0
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XTileDriver TileDrivers[] =
{
  EwGfxTileNative,                     EwGfxTileNativeBlend,
  EwGfxTileNativeSolid,                EwGfxTileNativeSolidBlend,
  EwGfxTileNativeGradient,             EwGfxTileNativeGradientBlend,
  EwGfxTileNativeLinearGradient,       EwGfxTileNativeLinearGradientBlend,
  EwGfxTileIndex8,                     EwGfxTileIndex8Blend,
  EwGfxTileIndex8Solid,                EwGfxTileIndex8SolidBlend,
  EwGfxTileIndex8Gradient,             EwGfxTileIndex8GradientBlend,
  EwGfxTileIndex8LinearGradient,       EwGfxTileIndex8LinearGradientBlend,
  0,                                   0,
  EwGfxTileAlpha8Solid,                EwGfxTileAlpha8SolidBlend,
  EwGfxTileAlpha8Gradient,             EwGfxTileAlpha8GradientBlend,
  EwGfxTileAlpha8LinearGradient,       EwGfxTileAlpha8LinearGradientBlend,
  EwGfxTileRGB565,                     EwGfxTileRGB565,
  EwGfxTileRGB565Solid,                EwGfxTileRGB565SolidBlend,
  EwGfxTileRGB565Gradient,             EwGfxTileRGB565GradientBlend,
  EwGfxTileRGB565LinearGradient,       EwGfxTileRGB565LinearGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwGfxScreenTileNative,               EwGfxScreenTileNativeBlend,
  EwGfxScreenTileNativeSolid,          EwGfxScreenTileNativeSolidBlend,
  EwGfxScreenTileNativeGradient,       EwGfxScreenTileNativeGradientBlend,
  EwGfxScreenTileNativeLinearGradient, EwGfxScreenTileNativeLinearGradientBlend,
  EwGfxScreenTileIndex8,               EwGfxScreenTileIndex8Blend,
  EwGfxScreenTileIndex8Solid,          EwGfxScreenTileIndex8SolidBlend,
  EwGfxScreenTileIndex8Gradient,       EwGfxScreenTileIndex8GradientBlend,
  EwGfxScreenTileIndex8LinearGradient, EwGfxScreenTileIndex8LinearGradientBlend,
  0,                                   0,
  EwGfxScreenTileAlpha8Solid,          EwGfxScreenTileAlpha8SolidBlend,
  EwGfxScreenTileAlpha8Gradient,       EwGfxScreenTileAlpha8GradientBlend,
  EwGfxScreenTileAlpha8LinearGradient, EwGfxScreenTileAlpha8LinearGradientBlend,
  EwGfxScreenTileRGB565,               EwGfxScreenTileRGB565,
  EwGfxScreenTileRGB565Solid,          EwGfxScreenTileRGB565SolidBlend,
  EwGfxScreenTileRGB565Gradient,       EwGfxScreenTileRGB565GradientBlend,
  EwGfxScreenTileRGB565LinearGradient, EwGfxScreenTileRGB565LinearGradientBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'warp perspective surface' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxWarpPerspNative
  #define EwWarpPerspNativeRow                       0
#else
  #define EwGfxWarpPerspNative                       0
  #define EwWarpPerspNativeRow                       EwWarpNativeRow
#endif

#ifdef EwGfxWarpPerspNativeBlend
  #define EwWarpPerspNativeRowBlend                  0
#else
  #define EwGfxWarpPerspNativeBlend                  0
  #define EwWarpPerspNativeRowBlend                  EwWarpNativeRowBlend
#endif

#ifdef EwGfxWarpPerspNativeGradient
  #define EwWarpPerspNativeRowGradient               0
  #define EwWarpPerspNativeRowSolid                  0
#else
  #define EwGfxWarpPerspNativeGradient               0
  #define EwWarpPerspNativeRowGradient               EwWarpNativeRowGradient
#endif

#ifdef EwGfxWarpPerspNativeGradientBlend
  #define EwWarpPerspNativeRowGradientBlend          0
  #define EwWarpPerspNativeRowSolidBlend             0
#else
  #define EwGfxWarpPerspNativeGradientBlend          0
  #define EwWarpPerspNativeRowGradientBlend \
    EwWarpNativeRowGradientBlend
#endif

#ifdef EwGfxWarpPerspNativeFilter
  #define EwWarpPerspNativeRowFilter                 0
#else
  #define EwGfxWarpPerspNativeFilter                 0
  #define EwWarpPerspNativeRowFilter                 EwWarpNativeRowFilter
#endif

#ifdef EwGfxWarpPerspNativeFilterBlend
  #define EwWarpPerspNativeRowFilterBlend            0
#else
  #define EwGfxWarpPerspNativeFilterBlend            0
  #define EwWarpPerspNativeRowFilterBlend            EwWarpNativeRowFilterBlend
#endif

#ifdef EwGfxWarpPerspNativeFilterGradient
  #define EwWarpPerspNativeRowFilterGradient         0
  #define EwWarpPerspNativeRowFilterSolid            0
#else
  #define EwGfxWarpPerspNativeFilterGradient         0
  #define EwWarpPerspNativeRowFilterGradient \
    EwWarpNativeRowFilterGradient
#endif

#ifdef EwGfxWarpPerspNativeFilterGradientBlend
  #define EwWarpPerspNativeRowFilterGradientBlend    0
  #define EwWarpPerspNativeRowFilterSolidBlend       0
#else
  #define EwGfxWarpPerspNativeFilterGradientBlend    0
  #define EwWarpPerspNativeRowFilterGradientBlend \
    EwWarpNativeRowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspNativeSolid
  #define EwGfxWarpPerspNativeSolid          EwGfxWarpPerspNativeGradient
#endif

#ifndef EwGfxWarpPerspNativeSolidBlend
  #define EwGfxWarpPerspNativeSolidBlend     EwGfxWarpPerspNativeGradientBlend
#endif

#ifndef EwGfxWarpPerspNativeFilterSolid
  #define EwGfxWarpPerspNativeFilterSolid    EwGfxWarpPerspNativeFilterGradient
#endif

#ifndef EwGfxWarpPerspNativeFilterSolidBlend
  #define EwGfxWarpPerspNativeFilterSolidBlend \
    EwGfxWarpPerspNativeFilterGradientBlend
#endif

#ifndef EwWarpPerspNativeRowSolid
  #define EwWarpPerspNativeRowSolid             EwWarpNativeRowSolid
#endif

#ifndef EwWarpPerspNativeRowSolidBlend
  #define EwWarpPerspNativeRowSolidBlend        EwWarpNativeRowSolidBlend
#endif

#ifndef EwWarpPerspNativeRowFilterSolid
  #define EwWarpPerspNativeRowFilterSolid       EwWarpNativeRowFilterSolid
#endif

#ifndef EwWarpPerspNativeRowFilterSolidBlend
  #define EwWarpPerspNativeRowFilterSolidBlend  EwWarpNativeRowFilterSolidBlend
#endif

#ifdef EwGfxWarpPerspIndex8
  #define EwWarpPerspIndex8Row                       0
#else
  #define EwGfxWarpPerspIndex8                       0
  #define EwWarpPerspIndex8Row                       EwWarpIndex8Row
#endif

#ifdef EwGfxWarpPerspIndex8Blend
  #define EwWarpPerspIndex8RowBlend                  0
#else
  #define EwGfxWarpPerspIndex8Blend                  0
  #define EwWarpPerspIndex8RowBlend                  EwWarpIndex8RowBlend
#endif

#ifdef EwGfxWarpPerspIndex8Gradient
  #define EwWarpPerspIndex8RowGradient               0
  #define EwWarpPerspIndex8RowSolid                  0
#else
  #define EwGfxWarpPerspIndex8Gradient               0
  #define EwWarpPerspIndex8RowGradient               EwWarpIndex8RowGradient
#endif

#ifdef EwGfxWarpPerspIndex8GradientBlend
  #define EwWarpPerspIndex8RowGradientBlend          0
  #define EwWarpPerspIndex8RowSolidBlend             0
#else
  #define EwGfxWarpPerspIndex8GradientBlend          0
  #define EwWarpPerspIndex8RowGradientBlend \
    EwWarpIndex8RowGradientBlend
#endif

#ifdef EwGfxWarpPerspIndex8Filter
  #define EwWarpPerspIndex8RowFilter                 0
#else
  #define EwGfxWarpPerspIndex8Filter                 0
  #define EwWarpPerspIndex8RowFilter                 EwWarpIndex8RowFilter
#endif

#ifdef EwGfxWarpPerspIndex8FilterBlend
  #define EwWarpPerspIndex8RowFilterBlend            0
#else
  #define EwGfxWarpPerspIndex8FilterBlend            0
  #define EwWarpPerspIndex8RowFilterBlend            EwWarpIndex8RowFilterBlend
#endif

#ifdef EwGfxWarpPerspIndex8FilterGradient
  #define EwWarpPerspIndex8RowFilterGradient         0
  #define EwWarpPerspIndex8RowFilterSolid            0
#else
  #define EwGfxWarpPerspIndex8FilterGradient         0
  #define EwWarpPerspIndex8RowFilterGradient \
    EwWarpIndex8RowFilterGradient
#endif

#ifdef EwGfxWarpPerspIndex8FilterGradientBlend
  #define EwWarpPerspIndex8RowFilterGradientBlend    0
  #define EwWarpPerspIndex8RowFilterSolidBlend       0
#else
  #define EwGfxWarpPerspIndex8FilterGradientBlend    0
  #define EwWarpPerspIndex8RowFilterGradientBlend \
    EwWarpIndex8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspIndex8Solid
  #define EwGfxWarpPerspIndex8Solid          EwGfxWarpPerspIndex8Gradient
#endif

#ifndef EwGfxWarpPerspIndex8SolidBlend
  #define EwGfxWarpPerspIndex8SolidBlend     EwGfxWarpPerspIndex8GradientBlend
#endif

#ifndef EwGfxWarpPerspIndex8FilterSolid
  #define EwGfxWarpPerspIndex8FilterSolid    EwGfxWarpPerspIndex8FilterGradient
#endif

#ifndef EwGfxWarpPerspIndex8FilterSolidBlend
  #define EwGfxWarpPerspIndex8FilterSolidBlend \
    EwGfxWarpPerspIndex8FilterGradientBlend
#endif

#ifndef EwWarpPerspIndex8RowSolid
  #define EwWarpPerspIndex8RowSolid             EwWarpIndex8RowSolid
#endif

#ifndef EwWarpPerspIndex8RowSolidBlend
  #define EwWarpPerspIndex8RowSolidBlend        EwWarpIndex8RowSolidBlend
#endif

#ifndef EwWarpPerspIndex8RowFilterSolid
  #define EwWarpPerspIndex8RowFilterSolid       EwWarpIndex8RowFilterSolid
#endif

#ifndef EwWarpPerspIndex8RowFilterSolidBlend
  #define EwWarpPerspIndex8RowFilterSolidBlend  EwWarpIndex8RowFilterSolidBlend
#endif

#ifdef EwGfxWarpPerspAlpha8Gradient
  #define EwWarpPerspAlpha8RowGradient               0
  #define EwWarpPerspAlpha8RowSolid                  0
#else
  #define EwGfxWarpPerspAlpha8Gradient               0
  #define EwWarpPerspAlpha8RowGradient               EwWarpAlpha8RowGradient
#endif

#ifdef EwGfxWarpPerspAlpha8GradientBlend
  #define EwWarpPerspAlpha8RowGradientBlend          0
  #define EwWarpPerspAlpha8RowSolidBlend             0
#else
  #define EwGfxWarpPerspAlpha8GradientBlend          0
  #define EwWarpPerspAlpha8RowGradientBlend \
    EwWarpAlpha8RowGradientBlend
#endif

#ifdef EwGfxWarpPerspAlpha8FilterGradient
  #define EwWarpPerspAlpha8RowFilterGradient         0
  #define EwWarpPerspAlpha8RowFilterSolid            0
#else
  #define EwGfxWarpPerspAlpha8FilterGradient         0
  #define EwWarpPerspAlpha8RowFilterGradient \
    EwWarpAlpha8RowFilterGradient
#endif

#ifdef EwGfxWarpPerspAlpha8FilterGradientBlend
  #define EwWarpPerspAlpha8RowFilterGradientBlend    0
  #define EwWarpPerspAlpha8RowFilterSolidBlend       0
#else
  #define EwGfxWarpPerspAlpha8FilterGradientBlend    0
  #define EwWarpPerspAlpha8RowFilterGradientBlend \
    EwWarpAlpha8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspAlpha8Solid
  #define EwGfxWarpPerspAlpha8Solid          EwGfxWarpPerspAlpha8Gradient
#endif

#ifndef EwGfxWarpPerspAlpha8SolidBlend
  #define EwGfxWarpPerspAlpha8SolidBlend     EwGfxWarpPerspAlpha8GradientBlend
#endif

#ifndef EwGfxWarpPerspAlpha8FilterSolid
  #define EwGfxWarpPerspAlpha8FilterSolid    EwGfxWarpPerspAlpha8FilterGradient
#endif

#ifndef EwGfxWarpPerspAlpha8FilterSolidBlend
  #define EwGfxWarpPerspAlpha8FilterSolidBlend \
    EwGfxWarpPerspAlpha8FilterGradientBlend
#endif

#ifndef EwWarpPerspAlpha8RowSolid
  #define EwWarpPerspAlpha8RowSolid             EwWarpAlpha8RowSolid
#endif

#ifndef EwWarpPerspAlpha8RowSolidBlend
  #define EwWarpPerspAlpha8RowSolidBlend        EwWarpAlpha8RowSolidBlend
#endif

#ifndef EwWarpPerspAlpha8RowFilterSolid
  #define EwWarpPerspAlpha8RowFilterSolid       EwWarpAlpha8RowFilterSolid
#endif

#ifndef EwWarpPerspAlpha8RowFilterSolidBlend
  #define EwWarpPerspAlpha8RowFilterSolidBlend  EwWarpAlpha8RowFilterSolidBlend
#endif

#ifdef EwGfxWarpPerspRGB565
  #define EwWarpPerspRGB565Row                       0
#else
  #define EwGfxWarpPerspRGB565                       0
  #define EwWarpPerspRGB565Row                       EwWarpRGB565Row
#endif

#ifdef EwGfxWarpPerspRGB565Gradient
  #define EwWarpPerspRGB565RowGradient               0
  #define EwWarpPerspRGB565RowSolid                  0
#else
  #define EwGfxWarpPerspRGB565Gradient               0
  #define EwWarpPerspRGB565RowGradient               EwWarpRGB565RowGradient
#endif

#ifdef EwGfxWarpPerspRGB565GradientBlend
  #define EwWarpPerspRGB565RowGradientBlend          0
  #define EwWarpPerspRGB565RowSolidBlend             0
#else
  #define EwGfxWarpPerspRGB565GradientBlend          0
  #define EwWarpPerspRGB565RowGradientBlend \
    EwWarpRGB565RowGradientBlend
#endif

#ifdef EwGfxWarpPerspRGB565Filter
  #define EwWarpPerspRGB565RowFilter                 0
#else
  #define EwGfxWarpPerspRGB565Filter                 0
  #define EwWarpPerspRGB565RowFilter                 EwWarpRGB565RowFilter
#endif

#ifdef EwGfxWarpPerspRGB565FilterGradient
  #define EwWarpPerspRGB565RowFilterGradient         0
  #define EwWarpPerspRGB565RowFilterSolid            0
#else
  #define EwGfxWarpPerspRGB565FilterGradient         0
  #define EwWarpPerspRGB565RowFilterGradient \
    EwWarpRGB565RowFilterGradient
#endif

#ifdef EwGfxWarpPerspRGB565FilterGradientBlend
  #define EwWarpPerspRGB565RowFilterGradientBlend    0
  #define EwWarpPerspRGB565RowFilterSolidBlend       0
#else
  #define EwGfxWarpPerspRGB565FilterGradientBlend    0
  #define EwWarpPerspRGB565RowFilterGradientBlend \
    EwWarpRGB565RowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspRGB565Solid
  #define EwGfxWarpPerspRGB565Solid          EwGfxWarpPerspRGB565Gradient
#endif

#ifndef EwGfxWarpPerspRGB565SolidBlend
  #define EwGfxWarpPerspRGB565SolidBlend     EwGfxWarpPerspRGB565GradientBlend
#endif

#ifndef EwGfxWarpPerspRGB565FilterSolid
  #define EwGfxWarpPerspRGB565FilterSolid    EwGfxWarpPerspRGB565FilterGradient
#endif

#ifndef EwGfxWarpPerspRGB565FilterSolidBlend
  #define EwGfxWarpPerspRGB565FilterSolidBlend \
    EwGfxWarpPerspRGB565FilterGradientBlend
#endif

#ifndef EwWarpPerspRGB565RowSolid
  #define EwWarpPerspRGB565RowSolid             EwWarpRGB565RowSolid
#endif

#ifndef EwWarpPerspRGB565RowSolidBlend
  #define EwWarpPerspRGB565RowSolidBlend        EwWarpRGB565RowSolidBlend
#endif

#ifndef EwWarpPerspRGB565RowFilterSolid
  #define EwWarpPerspRGB565RowFilterSolid       EwWarpRGB565RowFilterSolid
#endif

#ifndef EwWarpPerspRGB565RowFilterSolidBlend
  #define EwWarpPerspRGB565RowFilterSolidBlend  EwWarpRGB565RowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpPerspNative
  #define EwScreenWarpPerspNativeRow                       0
#else
  #define EwGfxScreenWarpPerspNative                       0
  #define EwScreenWarpPerspNativeRow                       EwScreenWarpNativeRow
#endif

#ifdef EwGfxScreenWarpPerspNativeBlend
  #define EwScreenWarpPerspNativeRowBlend                  0
#else
  #define EwGfxScreenWarpPerspNativeBlend                  0
  #define EwScreenWarpPerspNativeRowBlend                  EwScreenWarpNativeRowBlend
#endif

#ifdef EwGfxScreenWarpPerspNativeGradient
  #define EwScreenWarpPerspNativeRowGradient               0
  #define EwScreenWarpPerspNativeRowSolid                  0
#else
  #define EwGfxScreenWarpPerspNativeGradient               0
  #define EwScreenWarpPerspNativeRowGradient               EwScreenWarpNativeRowGradient
#endif

#ifdef EwGfxScreenWarpPerspNativeGradientBlend
  #define EwScreenWarpPerspNativeRowGradientBlend          0
  #define EwScreenWarpPerspNativeRowSolidBlend             0
#else
  #define EwGfxScreenWarpPerspNativeGradientBlend          0
  #define EwScreenWarpPerspNativeRowGradientBlend \
    EwScreenWarpNativeRowGradientBlend
#endif

#ifdef EwGfxScreenWarpPerspNativeFilter
  #define EwScreenWarpPerspNativeRowFilter                 0
#else
  #define EwGfxScreenWarpPerspNativeFilter                 0
  #define EwScreenWarpPerspNativeRowFilter                 EwScreenWarpNativeRowFilter
#endif

#ifdef EwGfxScreenWarpPerspNativeFilterBlend
  #define EwScreenWarpPerspNativeRowFilterBlend            0
#else
  #define EwGfxScreenWarpPerspNativeFilterBlend            0
  #define EwScreenWarpPerspNativeRowFilterBlend            EwScreenWarpNativeRowFilterBlend
#endif

#ifdef EwGfxScreenWarpPerspNativeFilterGradient
  #define EwScreenWarpPerspNativeRowFilterGradient         0
  #define EwScreenWarpPerspNativeRowFilterSolid            0
#else
  #define EwGfxScreenWarpPerspNativeFilterGradient         0
  #define EwScreenWarpPerspNativeRowFilterGradient \
    EwScreenWarpNativeRowFilterGradient
#endif

#ifdef EwGfxScreenWarpPerspNativeFilterGradientBlend
  #define EwScreenWarpPerspNativeRowFilterGradientBlend    0
  #define EwScreenWarpPerspNativeRowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpPerspNativeFilterGradientBlend    0
  #define EwScreenWarpPerspNativeRowFilterGradientBlend \
    EwScreenWarpNativeRowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpPerspNativeSolid
  #define EwGfxScreenWarpPerspNativeSolid          EwGfxScreenWarpPerspNativeGradient
#endif

#ifndef EwGfxScreenWarpPerspNativeSolidBlend
  #define EwGfxScreenWarpPerspNativeSolidBlend     EwGfxScreenWarpPerspNativeGradientBlend
#endif

#ifndef EwGfxScreenWarpPerspNativeFilterSolid
  #define EwGfxScreenWarpPerspNativeFilterSolid    EwGfxScreenWarpPerspNativeFilterGradient
#endif

#ifndef EwGfxScreenWarpPerspNativeFilterSolidBlend
  #define EwGfxScreenWarpPerspNativeFilterSolidBlend \
    EwGfxScreenWarpPerspNativeFilterGradientBlend
#endif

#ifndef EwScreenWarpPerspNativeRowSolid
  #define EwScreenWarpPerspNativeRowSolid             EwScreenWarpNativeRowSolid
#endif

#ifndef EwScreenWarpPerspNativeRowSolidBlend
  #define EwScreenWarpPerspNativeRowSolidBlend        EwScreenWarpNativeRowSolidBlend
#endif

#ifndef EwScreenWarpPerspNativeRowFilterSolid
  #define EwScreenWarpPerspNativeRowFilterSolid       EwScreenWarpNativeRowFilterSolid
#endif

#ifndef EwScreenWarpPerspNativeRowFilterSolidBlend
  #define EwScreenWarpPerspNativeRowFilterSolidBlend  EwScreenWarpNativeRowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpPerspIndex8
  #define EwScreenWarpPerspIndex8Row                       0
#else
  #define EwGfxScreenWarpPerspIndex8                       0
  #define EwScreenWarpPerspIndex8Row                       EwScreenWarpIndex8Row
#endif

#ifdef EwGfxScreenWarpPerspIndex8Blend
  #define EwScreenWarpPerspIndex8RowBlend                  0
#else
  #define EwGfxScreenWarpPerspIndex8Blend                  0
  #define EwScreenWarpPerspIndex8RowBlend                  EwScreenWarpIndex8RowBlend
#endif

#ifdef EwGfxScreenWarpPerspIndex8Gradient
  #define EwScreenWarpPerspIndex8RowGradient               0
  #define EwScreenWarpPerspIndex8RowSolid                  0
#else
  #define EwGfxScreenWarpPerspIndex8Gradient               0
  #define EwScreenWarpPerspIndex8RowGradient               EwScreenWarpIndex8RowGradient
#endif

#ifdef EwGfxScreenWarpPerspIndex8GradientBlend
  #define EwScreenWarpPerspIndex8RowGradientBlend          0
  #define EwScreenWarpPerspIndex8RowSolidBlend             0
#else
  #define EwGfxScreenWarpPerspIndex8GradientBlend          0
  #define EwScreenWarpPerspIndex8RowGradientBlend \
    EwScreenWarpIndex8RowGradientBlend
#endif

#ifdef EwGfxScreenWarpPerspIndex8Filter
  #define EwScreenWarpPerspIndex8RowFilter                 0
#else
  #define EwGfxScreenWarpPerspIndex8Filter                 0
  #define EwScreenWarpPerspIndex8RowFilter                 EwScreenWarpIndex8RowFilter
#endif

#ifdef EwGfxScreenWarpPerspIndex8FilterBlend
  #define EwScreenWarpPerspIndex8RowFilterBlend            0
#else
  #define EwGfxScreenWarpPerspIndex8FilterBlend            0
  #define EwScreenWarpPerspIndex8RowFilterBlend            EwScreenWarpIndex8RowFilterBlend
#endif

#ifdef EwGfxScreenWarpPerspIndex8FilterGradient
  #define EwScreenWarpPerspIndex8RowFilterGradient         0
  #define EwScreenWarpPerspIndex8RowFilterSolid            0
#else
  #define EwGfxScreenWarpPerspIndex8FilterGradient         0
  #define EwScreenWarpPerspIndex8RowFilterGradient \
    EwScreenWarpIndex8RowFilterGradient
#endif

#ifdef EwGfxScreenWarpPerspIndex8FilterGradientBlend
  #define EwScreenWarpPerspIndex8RowFilterGradientBlend    0
  #define EwScreenWarpPerspIndex8RowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpPerspIndex8FilterGradientBlend    0
  #define EwScreenWarpPerspIndex8RowFilterGradientBlend \
    EwScreenWarpIndex8RowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpPerspIndex8Solid
  #define EwGfxScreenWarpPerspIndex8Solid          EwGfxScreenWarpPerspIndex8Gradient
#endif

#ifndef EwGfxScreenWarpPerspIndex8SolidBlend
  #define EwGfxScreenWarpPerspIndex8SolidBlend     EwGfxScreenWarpPerspIndex8GradientBlend
#endif

#ifndef EwGfxScreenWarpPerspIndex8FilterSolid
  #define EwGfxScreenWarpPerspIndex8FilterSolid    EwGfxScreenWarpPerspIndex8FilterGradient
#endif

#ifndef EwGfxScreenWarpPerspIndex8FilterSolidBlend
  #define EwGfxScreenWarpPerspIndex8FilterSolidBlend \
    EwGfxScreenWarpPerspIndex8FilterGradientBlend
#endif

#ifndef EwScreenWarpPerspIndex8RowSolid
  #define EwScreenWarpPerspIndex8RowSolid             EwScreenWarpIndex8RowSolid
#endif

#ifndef EwScreenWarpPerspIndex8RowSolidBlend
  #define EwScreenWarpPerspIndex8RowSolidBlend        EwScreenWarpIndex8RowSolidBlend
#endif

#ifndef EwScreenWarpPerspIndex8RowFilterSolid
  #define EwScreenWarpPerspIndex8RowFilterSolid       EwScreenWarpIndex8RowFilterSolid
#endif

#ifndef EwScreenWarpPerspIndex8RowFilterSolidBlend
  #define EwScreenWarpPerspIndex8RowFilterSolidBlend  EwScreenWarpIndex8RowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpPerspAlpha8Gradient
  #define EwScreenWarpPerspAlpha8RowGradient               0
  #define EwScreenWarpPerspAlpha8RowSolid                  0
#else
  #define EwGfxScreenWarpPerspAlpha8Gradient               0
  #define EwScreenWarpPerspAlpha8RowGradient               EwScreenWarpAlpha8RowGradient
#endif

#ifdef EwGfxScreenWarpPerspAlpha8GradientBlend
  #define EwScreenWarpPerspAlpha8RowGradientBlend          0
  #define EwScreenWarpPerspAlpha8RowSolidBlend             0
#else
  #define EwGfxScreenWarpPerspAlpha8GradientBlend          0
  #define EwScreenWarpPerspAlpha8RowGradientBlend \
    EwScreenWarpAlpha8RowGradientBlend
#endif

#ifdef EwGfxScreenWarpPerspAlpha8FilterGradient
  #define EwScreenWarpPerspAlpha8RowFilterGradient         0
  #define EwScreenWarpPerspAlpha8RowFilterSolid            0
#else
  #define EwGfxScreenWarpPerspAlpha8FilterGradient         0
  #define EwScreenWarpPerspAlpha8RowFilterGradient \
    EwScreenWarpAlpha8RowFilterGradient
#endif

#ifdef EwGfxScreenWarpPerspAlpha8FilterGradientBlend
  #define EwScreenWarpPerspAlpha8RowFilterGradientBlend    0
  #define EwScreenWarpPerspAlpha8RowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpPerspAlpha8FilterGradientBlend    0
  #define EwScreenWarpPerspAlpha8RowFilterGradientBlend \
    EwScreenWarpAlpha8RowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpPerspAlpha8Solid
  #define EwGfxScreenWarpPerspAlpha8Solid          EwGfxScreenWarpPerspAlpha8Gradient
#endif

#ifndef EwGfxScreenWarpPerspAlpha8SolidBlend
  #define EwGfxScreenWarpPerspAlpha8SolidBlend     EwGfxScreenWarpPerspAlpha8GradientBlend
#endif

#ifndef EwGfxScreenWarpPerspAlpha8FilterSolid
  #define EwGfxScreenWarpPerspAlpha8FilterSolid    EwGfxScreenWarpPerspAlpha8FilterGradient
#endif

#ifndef EwGfxScreenWarpPerspAlpha8FilterSolidBlend
  #define EwGfxScreenWarpPerspAlpha8FilterSolidBlend \
    EwGfxScreenWarpPerspAlpha8FilterGradientBlend
#endif

#ifndef EwScreenWarpPerspAlpha8RowSolid
  #define EwScreenWarpPerspAlpha8RowSolid             EwScreenWarpAlpha8RowSolid
#endif

#ifndef EwScreenWarpPerspAlpha8RowSolidBlend
  #define EwScreenWarpPerspAlpha8RowSolidBlend        EwScreenWarpAlpha8RowSolidBlend
#endif

#ifndef EwScreenWarpPerspAlpha8RowFilterSolid
  #define EwScreenWarpPerspAlpha8RowFilterSolid       EwScreenWarpAlpha8RowFilterSolid
#endif

#ifndef EwScreenWarpPerspAlpha8RowFilterSolidBlend
  #define EwScreenWarpPerspAlpha8RowFilterSolidBlend  EwScreenWarpAlpha8RowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpPerspRGB565
  #define EwScreenWarpPerspRGB565Row                       0
#else
  #define EwGfxScreenWarpPerspRGB565                       0
  #define EwScreenWarpPerspRGB565Row                       EwScreenWarpRGB565Row
#endif

#ifdef EwGfxScreenWarpPerspRGB565Gradient
  #define EwScreenWarpPerspRGB565RowGradient               0
  #define EwScreenWarpPerspRGB565RowSolid                  0
#else
  #define EwGfxScreenWarpPerspRGB565Gradient               0
  #define EwScreenWarpPerspRGB565RowGradient               EwScreenWarpRGB565RowGradient
#endif

#ifdef EwGfxScreenWarpPerspRGB565GradientBlend
  #define EwScreenWarpPerspRGB565RowGradientBlend          0
  #define EwScreenWarpPerspRGB565RowSolidBlend             0
#else
  #define EwGfxScreenWarpPerspRGB565GradientBlend          0
  #define EwScreenWarpPerspRGB565RowGradientBlend \
    EwScreenWarpRGB565RowGradientBlend
#endif

#ifdef EwGfxScreenWarpPerspRGB565Filter
  #define EwScreenWarpPerspRGB565RowFilter                 0
#else
  #define EwGfxScreenWarpPerspRGB565Filter                 0
  #define EwScreenWarpPerspRGB565RowFilter                 EwScreenWarpRGB565RowFilter
#endif

#ifdef EwGfxScreenWarpPerspRGB565FilterGradient
  #define EwScreenWarpPerspRGB565RowFilterGradient         0
  #define EwScreenWarpPerspRGB565RowFilterSolid            0
#else
  #define EwGfxScreenWarpPerspRGB565FilterGradient         0
  #define EwScreenWarpPerspRGB565RowFilterGradient \
    EwScreenWarpRGB565RowFilterGradient
#endif

#ifdef EwGfxScreenWarpPerspRGB565FilterGradientBlend
  #define EwScreenWarpPerspRGB565RowFilterGradientBlend    0
  #define EwScreenWarpPerspRGB565RowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpPerspRGB565FilterGradientBlend    0
  #define EwScreenWarpPerspRGB565RowFilterGradientBlend \
    EwScreenWarpRGB565RowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpPerspRGB565Solid
  #define EwGfxScreenWarpPerspRGB565Solid          EwGfxScreenWarpPerspRGB565Gradient
#endif

#ifndef EwGfxScreenWarpPerspRGB565SolidBlend
  #define EwGfxScreenWarpPerspRGB565SolidBlend     EwGfxScreenWarpPerspRGB565GradientBlend
#endif

#ifndef EwGfxScreenWarpPerspRGB565FilterSolid
  #define EwGfxScreenWarpPerspRGB565FilterSolid    EwGfxScreenWarpPerspRGB565FilterGradient
#endif

#ifndef EwGfxScreenWarpPerspRGB565FilterSolidBlend
  #define EwGfxScreenWarpPerspRGB565FilterSolidBlend \
    EwGfxScreenWarpPerspRGB565FilterGradientBlend
#endif

#ifndef EwScreenWarpPerspRGB565RowSolid
  #define EwScreenWarpPerspRGB565RowSolid             EwScreenWarpRGB565RowSolid
#endif

#ifndef EwScreenWarpPerspRGB565RowSolidBlend
  #define EwScreenWarpPerspRGB565RowSolidBlend        EwScreenWarpRGB565RowSolidBlend
#endif

#ifndef EwScreenWarpPerspRGB565RowFilterSolid
  #define EwScreenWarpPerspRGB565RowFilterSolid       EwScreenWarpRGB565RowFilterSolid
#endif

#ifndef EwScreenWarpPerspRGB565RowFilterSolidBlend
  #define EwScreenWarpPerspRGB565RowFilterSolidBlend  EwScreenWarpRGB565RowFilterSolidBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XWarpDriver WarpPerspDrivers[] =
{
  EwGfxWarpPerspNative,                      EwGfxWarpPerspNativeBlend,
  EwGfxWarpPerspNativeSolid,                 EwGfxWarpPerspNativeSolidBlend,
  EwGfxWarpPerspNativeGradient,              EwGfxWarpPerspNativeGradientBlend,
  0,                                         0,
  EwGfxWarpPerspNativeFilter,                EwGfxWarpPerspNativeFilterBlend,
  EwGfxWarpPerspNativeFilterSolid,           EwGfxWarpPerspNativeFilterSolidBlend,
  EwGfxWarpPerspNativeFilterGradient,        EwGfxWarpPerspNativeFilterGradientBlend,
  0,                                         0,
  EwGfxWarpPerspIndex8,                      EwGfxWarpPerspIndex8Blend,
  EwGfxWarpPerspIndex8Solid,                 EwGfxWarpPerspIndex8SolidBlend,
  EwGfxWarpPerspIndex8Gradient,              EwGfxWarpPerspIndex8GradientBlend,
  0,                                         0,
  EwGfxWarpPerspIndex8Filter,                EwGfxWarpPerspIndex8FilterBlend,
  EwGfxWarpPerspIndex8FilterSolid,           EwGfxWarpPerspIndex8FilterSolidBlend,
  EwGfxWarpPerspIndex8FilterGradient,        EwGfxWarpPerspIndex8FilterGradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxWarpPerspAlpha8Solid,                 EwGfxWarpPerspAlpha8SolidBlend,
  EwGfxWarpPerspAlpha8Gradient,              EwGfxWarpPerspAlpha8GradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxWarpPerspAlpha8FilterSolid,           EwGfxWarpPerspAlpha8FilterSolidBlend,
  EwGfxWarpPerspAlpha8FilterGradient,        EwGfxWarpPerspAlpha8FilterGradientBlend,
  0,                                         0,
  EwGfxWarpPerspRGB565,                      EwGfxWarpPerspRGB565,
  EwGfxWarpPerspRGB565Solid,                 EwGfxWarpPerspRGB565SolidBlend,
  EwGfxWarpPerspRGB565Gradient,              EwGfxWarpPerspRGB565GradientBlend,
  0,                                         0,
  EwGfxWarpPerspRGB565Filter,                EwGfxWarpPerspRGB565Filter,
  EwGfxWarpPerspRGB565FilterSolid,           EwGfxWarpPerspRGB565FilterSolidBlend,
  EwGfxWarpPerspRGB565FilterGradient,        EwGfxWarpPerspRGB565FilterGradientBlend,
  0,                                         0,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwGfxScreenWarpPerspNative,                EwGfxScreenWarpPerspNativeBlend,
  EwGfxScreenWarpPerspNativeSolid,           EwGfxScreenWarpPerspNativeSolidBlend,
  EwGfxScreenWarpPerspNativeGradient,        EwGfxScreenWarpPerspNativeGradientBlend,
  0,                                         0,
  EwGfxScreenWarpPerspNativeFilter,          EwGfxScreenWarpPerspNativeFilterBlend,
  EwGfxScreenWarpPerspNativeFilterSolid,     EwGfxScreenWarpPerspNativeFilterSolidBlend,
  EwGfxScreenWarpPerspNativeFilterGradient,  EwGfxScreenWarpPerspNativeFilterGradientBlend,
  0,                                         0,
  EwGfxScreenWarpPerspIndex8,                EwGfxScreenWarpPerspIndex8Blend,
  EwGfxScreenWarpPerspIndex8Solid,           EwGfxScreenWarpPerspIndex8SolidBlend,
  EwGfxScreenWarpPerspIndex8Gradient,        EwGfxScreenWarpPerspIndex8GradientBlend,
  0,                                         0,
  EwGfxScreenWarpPerspIndex8Filter,          EwGfxScreenWarpPerspIndex8FilterBlend,
  EwGfxScreenWarpPerspIndex8FilterSolid,     EwGfxScreenWarpPerspIndex8FilterSolidBlend,
  EwGfxScreenWarpPerspIndex8FilterGradient,  EwGfxScreenWarpPerspIndex8FilterGradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxScreenWarpPerspAlpha8Solid,           EwGfxScreenWarpPerspAlpha8SolidBlend,
  EwGfxScreenWarpPerspAlpha8Gradient,        EwGfxScreenWarpPerspAlpha8GradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxScreenWarpPerspAlpha8FilterSolid,     EwGfxScreenWarpPerspAlpha8FilterSolidBlend,
  EwGfxScreenWarpPerspAlpha8FilterGradient,  EwGfxScreenWarpPerspAlpha8FilterGradientBlend,
  0,                                         0,
  EwGfxScreenWarpPerspRGB565,                EwGfxScreenWarpPerspRGB565,
  EwGfxScreenWarpPerspRGB565Solid,           EwGfxScreenWarpPerspRGB565SolidBlend,
  EwGfxScreenWarpPerspRGB565Gradient,        EwGfxScreenWarpPerspRGB565GradientBlend,
  0,                                         0,
  EwGfxScreenWarpPerspRGB565Filter,          EwGfxScreenWarpPerspRGB565Filter,
  EwGfxScreenWarpPerspRGB565FilterSolid,     EwGfxScreenWarpPerspRGB565FilterSolidBlend,
  EwGfxScreenWarpPerspRGB565FilterGradient,  EwGfxScreenWarpPerspRGB565FilterGradientBlend,
#endif

  0
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XWarpWorker WarpPerspWorkers[] =
{
  EwWarpPerspNativeRow,                      EwWarpPerspNativeRowBlend,
  EwWarpPerspNativeRowSolid,                 EwWarpPerspNativeRowSolidBlend,
  EwWarpPerspNativeRowGradient,              EwWarpPerspNativeRowGradientBlend,
  0,                                         0,
  EwWarpPerspNativeRowFilter,                EwWarpPerspNativeRowFilterBlend,
  EwWarpPerspNativeRowFilterSolid,           EwWarpPerspNativeRowFilterSolidBlend,
  EwWarpPerspNativeRowFilterGradient,        EwWarpPerspNativeRowFilterGradientBlend,
  0,                                         0,
  EwWarpPerspIndex8Row,                      EwWarpPerspIndex8RowBlend,
  EwWarpPerspIndex8RowSolid,                 EwWarpPerspIndex8RowSolidBlend,
  EwWarpPerspIndex8RowGradient,              EwWarpPerspIndex8RowGradientBlend,
  0,                                         0,
  EwWarpPerspIndex8RowFilter,                EwWarpPerspIndex8RowFilterBlend,
  EwWarpPerspIndex8RowFilterSolid,           EwWarpPerspIndex8RowFilterSolidBlend,
  EwWarpPerspIndex8RowFilterGradient,        EwWarpPerspIndex8RowFilterGradientBlend,
  0,                                         0,
  0,                                         0,
  EwWarpPerspAlpha8RowSolid,                 EwWarpPerspAlpha8RowSolidBlend,
  EwWarpPerspAlpha8RowGradient,              EwWarpPerspAlpha8RowGradientBlend,
  0,                                         0,
  0,                                         0,
  EwWarpPerspAlpha8RowFilterSolid,           EwWarpPerspAlpha8RowFilterSolidBlend,
  EwWarpPerspAlpha8RowFilterGradient,        EwWarpPerspAlpha8RowFilterGradientBlend,
  0,                                         0,
  EwWarpPerspRGB565Row,                      EwWarpPerspRGB565Row,
  EwWarpPerspRGB565RowSolid,                 EwWarpPerspRGB565RowSolidBlend,
  EwWarpPerspRGB565RowGradient,              EwWarpPerspRGB565RowGradientBlend,
  0,                                         0,
  EwWarpPerspRGB565RowFilter,                EwWarpPerspRGB565RowFilter,
  EwWarpPerspRGB565RowFilterSolid,           EwWarpPerspRGB565RowFilterSolidBlend,
  EwWarpPerspRGB565RowFilterGradient,        EwWarpPerspRGB565RowFilterGradientBlend,
  0,                                         0,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwScreenWarpPerspNativeRow,                EwScreenWarpPerspNativeRowBlend,
  EwScreenWarpPerspNativeRowSolid,           EwScreenWarpPerspNativeRowSolidBlend,
  EwScreenWarpPerspNativeRowGradient,        EwScreenWarpPerspNativeRowGradientBlend,
  0,                                         0,
  EwScreenWarpPerspNativeRowFilter,          EwScreenWarpPerspNativeRowFilterBlend,
  EwScreenWarpPerspNativeRowFilterSolid,     EwScreenWarpPerspNativeRowFilterSolidBlend,
  EwScreenWarpPerspNativeRowFilterGradient,  EwScreenWarpPerspNativeRowFilterGradientBlend,
  0,                                         0,
  EwScreenWarpPerspIndex8Row,                EwScreenWarpPerspIndex8RowBlend,
  EwScreenWarpPerspIndex8RowSolid,           EwScreenWarpPerspIndex8RowSolidBlend,
  EwScreenWarpPerspIndex8RowGradient,        EwScreenWarpPerspIndex8RowGradientBlend,
  0,                                         0,
  EwScreenWarpPerspIndex8RowFilter,          EwScreenWarpPerspIndex8RowFilterBlend,
  EwScreenWarpPerspIndex8RowFilterSolid,     EwScreenWarpPerspIndex8RowFilterSolidBlend,
  EwScreenWarpPerspIndex8RowFilterGradient,  EwScreenWarpPerspIndex8RowFilterGradientBlend,
  0,                                         0,
  0,                                         0,
  EwScreenWarpPerspAlpha8RowSolid,           EwScreenWarpPerspAlpha8RowSolidBlend,
  EwScreenWarpPerspAlpha8RowGradient,        EwScreenWarpPerspAlpha8RowGradientBlend,
  0,                                         0,
  0,                                         0,
  EwScreenWarpPerspAlpha8RowFilterSolid,     EwScreenWarpPerspAlpha8RowFilterSolidBlend,
  EwScreenWarpPerspAlpha8RowFilterGradient,  EwScreenWarpPerspAlpha8RowFilterGradientBlend,
  0,                                         0,
  EwScreenWarpPerspRGB565Row,                EwScreenWarpPerspRGB565Row,
  EwScreenWarpPerspRGB565RowSolid,           EwScreenWarpPerspRGB565RowSolidBlend,
  EwScreenWarpPerspRGB565RowGradient,        EwScreenWarpPerspRGB565RowGradientBlend,
  0,                                         0,
  EwScreenWarpPerspRGB565RowFilter,          EwScreenWarpPerspRGB565RowFilter,
  EwScreenWarpPerspRGB565RowFilterSolid,     EwScreenWarpPerspRGB565RowFilterSolidBlend,
  EwScreenWarpPerspRGB565RowFilterGradient,  EwScreenWarpPerspRGB565RowFilterGradientBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'warp affine surface' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxWarpAffineNative
  #define EwWarpAffineNativeRow                       0
#else
  #define EwGfxWarpAffineNative                       0
  #define EwWarpAffineNativeRow                       EwWarpNativeRow
#endif

#ifdef EwGfxWarpAffineNativeBlend
  #define EwWarpAffineNativeRowBlend                  0
#else
  #define EwGfxWarpAffineNativeBlend                  0
  #define EwWarpAffineNativeRowBlend                  EwWarpNativeRowBlend
#endif

#ifdef EwGfxWarpAffineNativeGradient
  #define EwWarpAffineNativeRowGradient               0
  #define EwWarpAffineNativeRowSolid                  0
#else
  #define EwGfxWarpAffineNativeGradient               0
  #define EwWarpAffineNativeRowGradient               EwWarpNativeRowGradient
#endif

#ifdef EwGfxWarpAffineNativeGradientBlend
  #define EwWarpAffineNativeRowGradientBlend          0
  #define EwWarpAffineNativeRowSolidBlend             0
#else
  #define EwGfxWarpAffineNativeGradientBlend          0
  #define EwWarpAffineNativeRowGradientBlend \
    EwWarpNativeRowGradientBlend
#endif

#ifdef EwGfxWarpAffineNativeFilter
  #define EwWarpAffineNativeRowFilter                 0
#else
  #define EwGfxWarpAffineNativeFilter                 0
  #define EwWarpAffineNativeRowFilter                 EwWarpNativeRowFilter
#endif

#ifdef EwGfxWarpAffineNativeFilterBlend
  #define EwWarpAffineNativeRowFilterBlend            0
#else
  #define EwGfxWarpAffineNativeFilterBlend            0
  #define EwWarpAffineNativeRowFilterBlend            EwWarpNativeRowFilterBlend
#endif

#ifdef EwGfxWarpAffineNativeFilterGradient
  #define EwWarpAffineNativeRowFilterGradient         0
  #define EwWarpAffineNativeRowFilterSolid            0
#else
  #define EwGfxWarpAffineNativeFilterGradient         0
  #define EwWarpAffineNativeRowFilterGradient \
    EwWarpNativeRowFilterGradient
#endif

#ifdef EwGfxWarpAffineNativeFilterGradientBlend
  #define EwWarpAffineNativeRowFilterGradientBlend    0
  #define EwWarpAffineNativeRowFilterSolidBlend       0
#else
  #define EwGfxWarpAffineNativeFilterGradientBlend    0
  #define EwWarpAffineNativeRowFilterGradientBlend \
    EwWarpNativeRowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineNativeSolid
  #define EwGfxWarpAffineNativeSolid        EwGfxWarpAffineNativeGradient
#endif

#ifndef EwGfxWarpAffineNativeSolidBlend
  #define EwGfxWarpAffineNativeSolidBlend   EwGfxWarpAffineNativeGradientBlend
#endif

#ifndef EwGfxWarpAffineNativeFilterSolid
  #define EwGfxWarpAffineNativeFilterSolid  EwGfxWarpAffineNativeFilterGradient
#endif

#ifndef EwGfxWarpAffineNativeFilterSolidBlend
  #define EwGfxWarpAffineNativeFilterSolidBlend \
    EwGfxWarpAffineNativeFilterGradientBlend
#endif

#ifndef EwWarpAffineNativeRowSolid
  #define EwWarpAffineNativeRowSolid             EwWarpNativeRowSolid
#endif

#ifndef EwWarpAffineNativeRowSolidBlend
  #define EwWarpAffineNativeRowSolidBlend        EwWarpNativeRowSolidBlend
#endif

#ifndef EwWarpAffineNativeRowFilterSolid
  #define EwWarpAffineNativeRowFilterSolid       EwWarpNativeRowFilterSolid
#endif

#ifndef EwWarpAffineNativeRowFilterSolidBlend
  #define EwWarpAffineNativeRowFilterSolidBlend  EwWarpNativeRowFilterSolidBlend
#endif

#ifdef EwGfxWarpAffineIndex8
  #define EwWarpAffineIndex8Row                       0
#else
  #define EwGfxWarpAffineIndex8                       0
  #define EwWarpAffineIndex8Row                       EwWarpIndex8Row
#endif

#ifdef EwGfxWarpAffineIndex8Blend
  #define EwWarpAffineIndex8RowBlend                  0
#else
  #define EwGfxWarpAffineIndex8Blend                  0
  #define EwWarpAffineIndex8RowBlend                  EwWarpIndex8RowBlend
#endif

#ifdef EwGfxWarpAffineIndex8Gradient
  #define EwWarpAffineIndex8RowGradient               0
  #define EwWarpAffineIndex8RowSolid                  0
#else
  #define EwGfxWarpAffineIndex8Gradient               0
  #define EwWarpAffineIndex8RowGradient               EwWarpIndex8RowGradient
#endif

#ifdef EwGfxWarpAffineIndex8GradientBlend
  #define EwWarpAffineIndex8RowGradientBlend          0
  #define EwWarpAffineIndex8RowSolidBlend             0
#else
  #define EwGfxWarpAffineIndex8GradientBlend          0
  #define EwWarpAffineIndex8RowGradientBlend \
    EwWarpIndex8RowGradientBlend
#endif

#ifdef EwGfxWarpAffineIndex8Filter
  #define EwWarpAffineIndex8RowFilter                 0
#else
  #define EwGfxWarpAffineIndex8Filter                 0
  #define EwWarpAffineIndex8RowFilter                 EwWarpIndex8RowFilter
#endif

#ifdef EwGfxWarpAffineIndex8FilterBlend
  #define EwWarpAffineIndex8RowFilterBlend            0
#else
  #define EwGfxWarpAffineIndex8FilterBlend            0
  #define EwWarpAffineIndex8RowFilterBlend            EwWarpIndex8RowFilterBlend
#endif

#ifdef EwGfxWarpAffineIndex8FilterGradient
  #define EwWarpAffineIndex8RowFilterGradient         0
  #define EwWarpAffineIndex8RowFilterSolid            0
#else
  #define EwGfxWarpAffineIndex8FilterGradient         0
  #define EwWarpAffineIndex8RowFilterGradient \
    EwWarpIndex8RowFilterGradient
#endif

#ifdef EwGfxWarpAffineIndex8FilterGradientBlend
  #define EwWarpAffineIndex8RowFilterGradientBlend    0
  #define EwWarpAffineIndex8RowFilterSolidBlend       0
#else
  #define EwGfxWarpAffineIndex8FilterGradientBlend    0
  #define EwWarpAffineIndex8RowFilterGradientBlend  \
    EwWarpIndex8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineIndex8Solid
  #define EwGfxWarpAffineIndex8Solid        EwGfxWarpAffineIndex8Gradient
#endif

#ifndef EwGfxWarpAffineIndex8SolidBlend
  #define EwGfxWarpAffineIndex8SolidBlend   EwGfxWarpAffineIndex8GradientBlend
#endif

#ifndef EwGfxWarpAffineIndex8FilterSolid
  #define EwGfxWarpAffineIndex8FilterSolid  EwGfxWarpAffineIndex8FilterGradient
#endif

#ifndef EwGfxWarpAffineIndex8FilterSolidBlend
  #define EwGfxWarpAffineIndex8FilterSolidBlend \
    EwGfxWarpAffineIndex8FilterGradientBlend
#endif

#ifndef EwWarpAffineIndex8RowSolid
  #define EwWarpAffineIndex8RowSolid             EwWarpIndex8RowSolid
#endif

#ifndef EwWarpAffineIndex8RowSolidBlend
  #define EwWarpAffineIndex8RowSolidBlend        EwWarpIndex8RowSolidBlend
#endif

#ifndef EwWarpAffineIndex8RowFilterSolid
  #define EwWarpAffineIndex8RowFilterSolid       EwWarpIndex8RowFilterSolid
#endif

#ifndef EwWarpAffineIndex8RowFilterSolidBlend
  #define EwWarpAffineIndex8RowFilterSolidBlend  EwWarpIndex8RowFilterSolidBlend
#endif

#ifdef EwGfxWarpAffineAlpha8Gradient
  #define EwWarpAffineAlpha8RowGradient               0
  #define EwWarpAffineAlpha8RowSolid                  0
#else
  #define EwGfxWarpAffineAlpha8Gradient               0
  #define EwWarpAffineAlpha8RowGradient               EwWarpAlpha8RowGradient
#endif

#ifdef EwGfxWarpAffineAlpha8GradientBlend
  #define EwWarpAffineAlpha8RowGradientBlend          0
  #define EwWarpAffineAlpha8RowSolidBlend             0
#else
  #define EwGfxWarpAffineAlpha8GradientBlend          0
  #define EwWarpAffineAlpha8RowGradientBlend \
    EwWarpAlpha8RowGradientBlend
#endif

#ifdef EwGfxWarpAffineAlpha8FilterGradient
  #define EwWarpAffineAlpha8RowFilterGradient         0
  #define EwWarpAffineAlpha8RowFilterSolid            0
#else
  #define EwGfxWarpAffineAlpha8FilterGradient         0
  #define EwWarpAffineAlpha8RowFilterGradient \
    EwWarpAlpha8RowFilterGradient
#endif

#ifdef EwGfxWarpAffineAlpha8FilterGradientBlend
  #define EwWarpAffineAlpha8RowFilterGradientBlend    0
  #define EwWarpAffineAlpha8RowFilterSolidBlend       0
#else
  #define EwGfxWarpAffineAlpha8FilterGradientBlend    0
  #define EwWarpAffineAlpha8RowFilterGradientBlend \
    EwWarpAlpha8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineAlpha8Solid
  #define EwGfxWarpAffineAlpha8Solid         EwGfxWarpAffineAlpha8Gradient
#endif

#ifndef EwGfxWarpAffineAlpha8SolidBlend
  #define EwGfxWarpAffineAlpha8SolidBlend    EwGfxWarpAffineAlpha8GradientBlend
#endif

#ifndef EwGfxWarpAffineAlpha8FilterSolid
  #define EwGfxWarpAffineAlpha8FilterSolid      \
    EwGfxWarpAffineAlpha8FilterGradient
#endif

#ifndef EwGfxWarpAffineAlpha8FilterSolidBlend
  #define EwGfxWarpAffineAlpha8FilterSolidBlend \
    EwGfxWarpAffineAlpha8FilterGradientBlend
#endif

#ifndef EwWarpAffineAlpha8RowSolid
  #define EwWarpAffineAlpha8RowSolid             EwWarpAlpha8RowSolid
#endif

#ifndef EwWarpAffineAlpha8RowSolidBlend
  #define EwWarpAffineAlpha8RowSolidBlend        EwWarpAlpha8RowSolidBlend
#endif

#ifndef EwWarpAffineAlpha8RowFilterSolid
  #define EwWarpAffineAlpha8RowFilterSolid       EwWarpAlpha8RowFilterSolid
#endif

#ifndef EwWarpAffineAlpha8RowFilterSolidBlend
  #define EwWarpAffineAlpha8RowFilterSolidBlend  EwWarpAlpha8RowFilterSolidBlend
#endif

#ifdef EwGfxWarpAffineRGB565
  #define EwWarpAffineRGB565Row                       0
#else
  #define EwGfxWarpAffineRGB565                       0
  #define EwWarpAffineRGB565Row                       EwWarpRGB565Row
#endif

#ifdef EwGfxWarpAffineRGB565Gradient
  #define EwWarpAffineRGB565RowGradient               0
  #define EwWarpAffineRGB565RowSolid                  0
#else
  #define EwGfxWarpAffineRGB565Gradient               0
  #define EwWarpAffineRGB565RowGradient               EwWarpRGB565RowGradient
#endif

#ifdef EwGfxWarpAffineRGB565GradientBlend
  #define EwWarpAffineRGB565RowGradientBlend          0
  #define EwWarpAffineRGB565RowSolidBlend             0
#else
  #define EwGfxWarpAffineRGB565GradientBlend          0
  #define EwWarpAffineRGB565RowGradientBlend \
    EwWarpRGB565RowGradientBlend
#endif

#ifdef EwGfxWarpAffineRGB565Filter
  #define EwWarpAffineRGB565RowFilter                 0
#else
  #define EwGfxWarpAffineRGB565Filter                 0
  #define EwWarpAffineRGB565RowFilter                 EwWarpRGB565RowFilter
#endif

#ifdef EwGfxWarpAffineRGB565FilterGradient
  #define EwWarpAffineRGB565RowFilterGradient         0
  #define EwWarpAffineRGB565RowFilterSolid            0
#else
  #define EwGfxWarpAffineRGB565FilterGradient         0
  #define EwWarpAffineRGB565RowFilterGradient \
    EwWarpRGB565RowFilterGradient
#endif

#ifdef EwGfxWarpAffineRGB565FilterGradientBlend
  #define EwWarpAffineRGB565RowFilterGradientBlend    0
  #define EwWarpAffineRGB565RowFilterSolidBlend       0
#else
  #define EwGfxWarpAffineRGB565FilterGradientBlend    0
  #define EwWarpAffineRGB565RowFilterGradientBlend  \
    EwWarpRGB565RowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineRGB565Solid
  #define EwGfxWarpAffineRGB565Solid        EwGfxWarpAffineRGB565Gradient
#endif

#ifndef EwGfxWarpAffineRGB565SolidBlend
  #define EwGfxWarpAffineRGB565SolidBlend   EwGfxWarpAffineRGB565GradientBlend
#endif

#ifndef EwGfxWarpAffineRGB565FilterSolid
  #define EwGfxWarpAffineRGB565FilterSolid  EwGfxWarpAffineRGB565FilterGradient
#endif

#ifndef EwGfxWarpAffineRGB565FilterSolidBlend
  #define EwGfxWarpAffineRGB565FilterSolidBlend \
    EwGfxWarpAffineRGB565FilterGradientBlend
#endif

#ifndef EwWarpAffineRGB565RowSolid
  #define EwWarpAffineRGB565RowSolid             EwWarpRGB565RowSolid
#endif

#ifndef EwWarpAffineRGB565RowSolidBlend
  #define EwWarpAffineRGB565RowSolidBlend        EwWarpRGB565RowSolidBlend
#endif

#ifndef EwWarpAffineRGB565RowFilterSolid
  #define EwWarpAffineRGB565RowFilterSolid       EwWarpRGB565RowFilterSolid
#endif

#ifndef EwWarpAffineRGB565RowFilterSolidBlend
  #define EwWarpAffineRGB565RowFilterSolidBlend  EwWarpRGB565RowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpAffineNative
  #define EwScreenWarpAffineNativeRow                       0
#else
  #define EwGfxScreenWarpAffineNative                       0
  #define EwScreenWarpAffineNativeRow                       EwScreenWarpNativeRow
#endif

#ifdef EwGfxScreenWarpAffineNativeBlend
  #define EwScreenWarpAffineNativeRowBlend                  0
#else
  #define EwGfxScreenWarpAffineNativeBlend                  0
  #define EwScreenWarpAffineNativeRowBlend                  EwScreenWarpNativeRowBlend
#endif

#ifdef EwGfxScreenWarpAffineNativeGradient
  #define EwScreenWarpAffineNativeRowGradient               0
  #define EwScreenWarpAffineNativeRowSolid                  0
#else
  #define EwGfxScreenWarpAffineNativeGradient               0
  #define EwScreenWarpAffineNativeRowGradient               EwScreenWarpNativeRowGradient
#endif

#ifdef EwGfxScreenWarpAffineNativeGradientBlend
  #define EwScreenWarpAffineNativeRowGradientBlend          0
  #define EwScreenWarpAffineNativeRowSolidBlend             0
#else
  #define EwGfxScreenWarpAffineNativeGradientBlend          0
  #define EwScreenWarpAffineNativeRowGradientBlend \
    EwScreenWarpNativeRowGradientBlend
#endif

#ifdef EwGfxScreenWarpAffineNativeFilter
  #define EwScreenWarpAffineNativeRowFilter                 0
#else
  #define EwGfxScreenWarpAffineNativeFilter                 0
  #define EwScreenWarpAffineNativeRowFilter                 EwScreenWarpNativeRowFilter
#endif

#ifdef EwGfxScreenWarpAffineNativeFilterBlend
  #define EwScreenWarpAffineNativeRowFilterBlend            0
#else
  #define EwGfxScreenWarpAffineNativeFilterBlend            0
  #define EwScreenWarpAffineNativeRowFilterBlend            EwScreenWarpNativeRowFilterBlend
#endif

#ifdef EwGfxScreenWarpAffineNativeFilterGradient
  #define EwScreenWarpAffineNativeRowFilterGradient         0
  #define EwScreenWarpAffineNativeRowFilterSolid            0
#else
  #define EwGfxScreenWarpAffineNativeFilterGradient         0
  #define EwScreenWarpAffineNativeRowFilterGradient \
    EwScreenWarpNativeRowFilterGradient
#endif

#ifdef EwGfxScreenWarpAffineNativeFilterGradientBlend
  #define EwScreenWarpAffineNativeRowFilterGradientBlend    0
  #define EwScreenWarpAffineNativeRowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpAffineNativeFilterGradientBlend    0
  #define EwScreenWarpAffineNativeRowFilterGradientBlend \
    EwScreenWarpNativeRowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpAffineNativeSolid
  #define EwGfxScreenWarpAffineNativeSolid        EwGfxScreenWarpAffineNativeGradient
#endif

#ifndef EwGfxScreenWarpAffineNativeSolidBlend
  #define EwGfxScreenWarpAffineNativeSolidBlend   EwGfxScreenWarpAffineNativeGradientBlend
#endif

#ifndef EwGfxScreenWarpAffineNativeFilterSolid
  #define EwGfxScreenWarpAffineNativeFilterSolid  EwGfxScreenWarpAffineNativeFilterGradient
#endif

#ifndef EwGfxScreenWarpAffineNativeFilterSolidBlend
  #define EwGfxScreenWarpAffineNativeFilterSolidBlend \
    EwGfxScreenWarpAffineNativeFilterGradientBlend
#endif

#ifndef EwScreenWarpAffineNativeRowSolid
  #define EwScreenWarpAffineNativeRowSolid             EwScreenWarpNativeRowSolid
#endif

#ifndef EwScreenWarpAffineNativeRowSolidBlend
  #define EwScreenWarpAffineNativeRowSolidBlend        EwScreenWarpNativeRowSolidBlend
#endif

#ifndef EwScreenWarpAffineNativeRowFilterSolid
  #define EwScreenWarpAffineNativeRowFilterSolid       EwScreenWarpNativeRowFilterSolid
#endif

#ifndef EwScreenWarpAffineNativeRowFilterSolidBlend
  #define EwScreenWarpAffineNativeRowFilterSolidBlend  EwScreenWarpNativeRowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpAffineIndex8
  #define EwScreenWarpAffineIndex8Row                       0
#else
  #define EwGfxScreenWarpAffineIndex8                       0
  #define EwScreenWarpAffineIndex8Row                       EwScreenWarpIndex8Row
#endif

#ifdef EwGfxScreenWarpAffineIndex8Blend
  #define EwScreenWarpAffineIndex8RowBlend                  0
#else
  #define EwGfxScreenWarpAffineIndex8Blend                  0
  #define EwScreenWarpAffineIndex8RowBlend                  EwScreenWarpIndex8RowBlend
#endif

#ifdef EwGfxScreenWarpAffineIndex8Gradient
  #define EwScreenWarpAffineIndex8RowGradient               0
  #define EwScreenWarpAffineIndex8RowSolid                  0
#else
  #define EwGfxScreenWarpAffineIndex8Gradient               0
  #define EwScreenWarpAffineIndex8RowGradient               EwScreenWarpIndex8RowGradient
#endif

#ifdef EwGfxScreenWarpAffineIndex8GradientBlend
  #define EwScreenWarpAffineIndex8RowGradientBlend          0
  #define EwScreenWarpAffineIndex8RowSolidBlend             0
#else
  #define EwGfxScreenWarpAffineIndex8GradientBlend          0
  #define EwScreenWarpAffineIndex8RowGradientBlend \
    EwScreenWarpIndex8RowGradientBlend
#endif

#ifdef EwGfxScreenWarpAffineIndex8Filter
  #define EwScreenWarpAffineIndex8RowFilter                 0
#else
  #define EwGfxScreenWarpAffineIndex8Filter                 0
  #define EwScreenWarpAffineIndex8RowFilter                 EwScreenWarpIndex8RowFilter
#endif

#ifdef EwGfxScreenWarpAffineIndex8FilterBlend
  #define EwScreenWarpAffineIndex8RowFilterBlend            0
#else
  #define EwGfxScreenWarpAffineIndex8FilterBlend            0
  #define EwScreenWarpAffineIndex8RowFilterBlend            EwScreenWarpIndex8RowFilterBlend
#endif

#ifdef EwGfxScreenWarpAffineIndex8FilterGradient
  #define EwScreenWarpAffineIndex8RowFilterGradient         0
  #define EwScreenWarpAffineIndex8RowFilterSolid            0
#else
  #define EwGfxScreenWarpAffineIndex8FilterGradient         0
  #define EwScreenWarpAffineIndex8RowFilterGradient \
    EwScreenWarpIndex8RowFilterGradient
#endif

#ifdef EwGfxScreenWarpAffineIndex8FilterGradientBlend
  #define EwScreenWarpAffineIndex8RowFilterGradientBlend    0
  #define EwScreenWarpAffineIndex8RowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpAffineIndex8FilterGradientBlend    0
  #define EwScreenWarpAffineIndex8RowFilterGradientBlend  \
    EwScreenWarpIndex8RowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpAffineIndex8Solid
  #define EwGfxScreenWarpAffineIndex8Solid        EwGfxScreenWarpAffineIndex8Gradient
#endif

#ifndef EwGfxScreenWarpAffineIndex8SolidBlend
  #define EwGfxScreenWarpAffineIndex8SolidBlend   EwGfxScreenWarpAffineIndex8GradientBlend
#endif

#ifndef EwGfxScreenWarpAffineIndex8FilterSolid
  #define EwGfxScreenWarpAffineIndex8FilterSolid  EwGfxScreenWarpAffineIndex8FilterGradient
#endif

#ifndef EwGfxScreenWarpAffineIndex8FilterSolidBlend
  #define EwGfxScreenWarpAffineIndex8FilterSolidBlend \
    EwGfxScreenWarpAffineIndex8FilterGradientBlend
#endif

#ifndef EwScreenWarpAffineIndex8RowSolid
  #define EwScreenWarpAffineIndex8RowSolid             EwScreenWarpIndex8RowSolid
#endif

#ifndef EwScreenWarpAffineIndex8RowSolidBlend
  #define EwScreenWarpAffineIndex8RowSolidBlend        EwScreenWarpIndex8RowSolidBlend
#endif

#ifndef EwScreenWarpAffineIndex8RowFilterSolid
  #define EwScreenWarpAffineIndex8RowFilterSolid       EwScreenWarpIndex8RowFilterSolid
#endif

#ifndef EwScreenWarpAffineIndex8RowFilterSolidBlend
  #define EwScreenWarpAffineIndex8RowFilterSolidBlend  EwScreenWarpIndex8RowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpAffineAlpha8Gradient
  #define EwScreenWarpAffineAlpha8RowGradient               0
  #define EwScreenWarpAffineAlpha8RowSolid                  0
#else
  #define EwGfxScreenWarpAffineAlpha8Gradient               0
  #define EwScreenWarpAffineAlpha8RowGradient               EwScreenWarpAlpha8RowGradient
#endif

#ifdef EwGfxScreenWarpAffineAlpha8GradientBlend
  #define EwScreenWarpAffineAlpha8RowGradientBlend          0
  #define EwScreenWarpAffineAlpha8RowSolidBlend             0
#else
  #define EwGfxScreenWarpAffineAlpha8GradientBlend          0
  #define EwScreenWarpAffineAlpha8RowGradientBlend \
    EwScreenWarpAlpha8RowGradientBlend
#endif

#ifdef EwGfxScreenWarpAffineAlpha8FilterGradient
  #define EwScreenWarpAffineAlpha8RowFilterGradient         0
  #define EwScreenWarpAffineAlpha8RowFilterSolid            0
#else
  #define EwGfxScreenWarpAffineAlpha8FilterGradient         0
  #define EwScreenWarpAffineAlpha8RowFilterGradient \
    EwScreenWarpAlpha8RowFilterGradient
#endif

#ifdef EwGfxScreenWarpAffineAlpha8FilterGradientBlend
  #define EwScreenWarpAffineAlpha8RowFilterGradientBlend    0
  #define EwScreenWarpAffineAlpha8RowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpAffineAlpha8FilterGradientBlend    0
  #define EwScreenWarpAffineAlpha8RowFilterGradientBlend \
    EwScreenWarpAlpha8RowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpAffineAlpha8Solid
  #define EwGfxScreenWarpAffineAlpha8Solid         EwGfxScreenWarpAffineAlpha8Gradient
#endif

#ifndef EwGfxScreenWarpAffineAlpha8SolidBlend
  #define EwGfxScreenWarpAffineAlpha8SolidBlend    EwGfxScreenWarpAffineAlpha8GradientBlend
#endif

#ifndef EwGfxScreenWarpAffineAlpha8FilterSolid
  #define EwGfxScreenWarpAffineAlpha8FilterSolid      \
    EwGfxScreenWarpAffineAlpha8FilterGradient
#endif

#ifndef EwGfxScreenWarpAffineAlpha8FilterSolidBlend
  #define EwGfxScreenWarpAffineAlpha8FilterSolidBlend \
    EwGfxScreenWarpAffineAlpha8FilterGradientBlend
#endif

#ifndef EwScreenWarpAffineAlpha8RowSolid
  #define EwScreenWarpAffineAlpha8RowSolid             EwScreenWarpAlpha8RowSolid
#endif

#ifndef EwScreenWarpAffineAlpha8RowSolidBlend
  #define EwScreenWarpAffineAlpha8RowSolidBlend        EwScreenWarpAlpha8RowSolidBlend
#endif

#ifndef EwScreenWarpAffineAlpha8RowFilterSolid
  #define EwScreenWarpAffineAlpha8RowFilterSolid       EwScreenWarpAlpha8RowFilterSolid
#endif

#ifndef EwScreenWarpAffineAlpha8RowFilterSolidBlend
  #define EwScreenWarpAffineAlpha8RowFilterSolidBlend  EwScreenWarpAlpha8RowFilterSolidBlend
#endif

#ifdef EwGfxScreenWarpAffineRGB565
  #define EwScreenWarpAffineRGB565Row                       0
#else
  #define EwGfxScreenWarpAffineRGB565                       0
  #define EwScreenWarpAffineRGB565Row                       EwScreenWarpRGB565Row
#endif

#ifdef EwGfxScreenWarpAffineRGB565Gradient
  #define EwScreenWarpAffineRGB565RowGradient               0
  #define EwScreenWarpAffineRGB565RowSolid                  0
#else
  #define EwGfxScreenWarpAffineRGB565Gradient               0
  #define EwScreenWarpAffineRGB565RowGradient               EwScreenWarpRGB565RowGradient
#endif

#ifdef EwGfxScreenWarpAffineRGB565GradientBlend
  #define EwScreenWarpAffineRGB565RowGradientBlend          0
  #define EwScreenWarpAffineRGB565RowSolidBlend             0
#else
  #define EwGfxScreenWarpAffineRGB565GradientBlend          0
  #define EwScreenWarpAffineRGB565RowGradientBlend \
    EwScreenWarpRGB565RowGradientBlend
#endif

#ifdef EwGfxScreenWarpAffineRGB565Filter
  #define EwScreenWarpAffineRGB565RowFilter                 0
#else
  #define EwGfxScreenWarpAffineRGB565Filter                 0
  #define EwScreenWarpAffineRGB565RowFilter                 EwScreenWarpRGB565RowFilter
#endif

#ifdef EwGfxScreenWarpAffineRGB565FilterGradient
  #define EwScreenWarpAffineRGB565RowFilterGradient         0
  #define EwScreenWarpAffineRGB565RowFilterSolid            0
#else
  #define EwGfxScreenWarpAffineRGB565FilterGradient         0
  #define EwScreenWarpAffineRGB565RowFilterGradient \
    EwScreenWarpRGB565RowFilterGradient
#endif

#ifdef EwGfxScreenWarpAffineRGB565FilterGradientBlend
  #define EwScreenWarpAffineRGB565RowFilterGradientBlend    0
  #define EwScreenWarpAffineRGB565RowFilterSolidBlend       0
#else
  #define EwGfxScreenWarpAffineRGB565FilterGradientBlend    0
  #define EwScreenWarpAffineRGB565RowFilterGradientBlend  \
    EwScreenWarpRGB565RowFilterGradientBlend
#endif

#ifndef EwGfxScreenWarpAffineRGB565Solid
  #define EwGfxScreenWarpAffineRGB565Solid        EwGfxScreenWarpAffineRGB565Gradient
#endif

#ifndef EwGfxScreenWarpAffineRGB565SolidBlend
  #define EwGfxScreenWarpAffineRGB565SolidBlend   EwGfxScreenWarpAffineRGB565GradientBlend
#endif

#ifndef EwGfxScreenWarpAffineRGB565FilterSolid
  #define EwGfxScreenWarpAffineRGB565FilterSolid  EwGfxScreenWarpAffineRGB565FilterGradient
#endif

#ifndef EwGfxScreenWarpAffineRGB565FilterSolidBlend
  #define EwGfxScreenWarpAffineRGB565FilterSolidBlend \
    EwGfxScreenWarpAffineRGB565FilterGradientBlend
#endif

#ifndef EwScreenWarpAffineRGB565RowSolid
  #define EwScreenWarpAffineRGB565RowSolid             EwScreenWarpRGB565RowSolid
#endif

#ifndef EwScreenWarpAffineRGB565RowSolidBlend
  #define EwScreenWarpAffineRGB565RowSolidBlend        EwScreenWarpRGB565RowSolidBlend
#endif

#ifndef EwScreenWarpAffineRGB565RowFilterSolid
  #define EwScreenWarpAffineRGB565RowFilterSolid       EwScreenWarpRGB565RowFilterSolid
#endif

#ifndef EwScreenWarpAffineRGB565RowFilterSolidBlend
  #define EwScreenWarpAffineRGB565RowFilterSolidBlend  EwScreenWarpRGB565RowFilterSolidBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XWarpDriver WarpAffineDrivers[] =
{
  EwGfxWarpAffineNative,                     EwGfxWarpAffineNativeBlend,
  EwGfxWarpAffineNativeSolid,                EwGfxWarpAffineNativeSolidBlend,
  EwGfxWarpAffineNativeGradient,             EwGfxWarpAffineNativeGradientBlend,
  0,                                         0,
  EwGfxWarpAffineNativeFilter,               EwGfxWarpAffineNativeFilterBlend,
  EwGfxWarpAffineNativeFilterSolid,          EwGfxWarpAffineNativeFilterSolidBlend,
  EwGfxWarpAffineNativeFilterGradient,       EwGfxWarpAffineNativeFilterGradientBlend,
  0,                                         0,
  EwGfxWarpAffineIndex8,                     EwGfxWarpAffineIndex8Blend,
  EwGfxWarpAffineIndex8Solid,                EwGfxWarpAffineIndex8SolidBlend,
  EwGfxWarpAffineIndex8Gradient,             EwGfxWarpAffineIndex8GradientBlend,
  0,                                         0,
  EwGfxWarpAffineIndex8Filter,               EwGfxWarpAffineIndex8FilterBlend,
  EwGfxWarpAffineIndex8FilterSolid,          EwGfxWarpAffineIndex8FilterSolidBlend,
  EwGfxWarpAffineIndex8FilterGradient,       EwGfxWarpAffineIndex8FilterGradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxWarpAffineAlpha8Solid,                EwGfxWarpAffineAlpha8SolidBlend,
  EwGfxWarpAffineAlpha8Gradient,             EwGfxWarpAffineAlpha8GradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxWarpAffineAlpha8FilterSolid,          EwGfxWarpAffineAlpha8FilterSolidBlend,
  EwGfxWarpAffineAlpha8FilterGradient,       EwGfxWarpAffineAlpha8FilterGradientBlend,
  0,                                         0,
  EwGfxWarpAffineRGB565,                     EwGfxWarpAffineRGB565,
  EwGfxWarpAffineRGB565Solid,                EwGfxWarpAffineRGB565SolidBlend,
  EwGfxWarpAffineRGB565Gradient,             EwGfxWarpAffineRGB565GradientBlend,
  0,                                         0,
  EwGfxWarpAffineRGB565Filter,               EwGfxWarpAffineRGB565Filter,
  EwGfxWarpAffineRGB565FilterSolid,          EwGfxWarpAffineRGB565FilterSolidBlend,
  EwGfxWarpAffineRGB565FilterGradient,       EwGfxWarpAffineRGB565FilterGradientBlend,
  0,                                         0,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwGfxScreenWarpAffineNative,               EwGfxScreenWarpAffineNativeBlend,
  EwGfxScreenWarpAffineNativeSolid,          EwGfxScreenWarpAffineNativeSolidBlend,
  EwGfxScreenWarpAffineNativeGradient,       EwGfxScreenWarpAffineNativeGradientBlend,
  0,                                         0,
  EwGfxScreenWarpAffineNativeFilter,         EwGfxScreenWarpAffineNativeFilterBlend,
  EwGfxScreenWarpAffineNativeFilterSolid,    EwGfxScreenWarpAffineNativeFilterSolidBlend,
  EwGfxScreenWarpAffineNativeFilterGradient, EwGfxScreenWarpAffineNativeFilterGradientBlend,
  0,                                         0,
  EwGfxScreenWarpAffineIndex8,               EwGfxScreenWarpAffineIndex8Blend,
  EwGfxScreenWarpAffineIndex8Solid,          EwGfxScreenWarpAffineIndex8SolidBlend,
  EwGfxScreenWarpAffineIndex8Gradient,       EwGfxScreenWarpAffineIndex8GradientBlend,
  0,                                         0,
  EwGfxScreenWarpAffineIndex8Filter,         EwGfxScreenWarpAffineIndex8FilterBlend,
  EwGfxScreenWarpAffineIndex8FilterSolid,    EwGfxScreenWarpAffineIndex8FilterSolidBlend,
  EwGfxScreenWarpAffineIndex8FilterGradient, EwGfxScreenWarpAffineIndex8FilterGradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxScreenWarpAffineAlpha8Solid,          EwGfxScreenWarpAffineAlpha8SolidBlend,
  EwGfxScreenWarpAffineAlpha8Gradient,       EwGfxScreenWarpAffineAlpha8GradientBlend,
  0,                                         0,
  0,                                         0,
  EwGfxScreenWarpAffineAlpha8FilterSolid,    EwGfxScreenWarpAffineAlpha8FilterSolidBlend,
  EwGfxScreenWarpAffineAlpha8FilterGradient, EwGfxScreenWarpAffineAlpha8FilterGradientBlend,
  0,                                         0,
  EwGfxScreenWarpAffineRGB565,               EwGfxScreenWarpAffineRGB565,
  EwGfxScreenWarpAffineRGB565Solid,          EwGfxScreenWarpAffineRGB565SolidBlend,
  EwGfxScreenWarpAffineRGB565Gradient,       EwGfxScreenWarpAffineRGB565GradientBlend,
  0,                                         0,
  EwGfxScreenWarpAffineRGB565Filter,         EwGfxScreenWarpAffineRGB565Filter,
  EwGfxScreenWarpAffineRGB565FilterSolid,    EwGfxScreenWarpAffineRGB565FilterSolidBlend,
  EwGfxScreenWarpAffineRGB565FilterGradient, EwGfxScreenWarpAffineRGB565FilterGradientBlend,
#endif

  0
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XWarpWorker WarpAffineWorkers[] =
{
  EwWarpAffineNativeRow,                     EwWarpAffineNativeRowBlend,
  EwWarpAffineNativeRowSolid,                EwWarpAffineNativeRowSolidBlend,
  EwWarpAffineNativeRowGradient,             EwWarpAffineNativeRowGradientBlend,
  EwWarpAffineNativeRowGradient,             EwWarpAffineNativeRowGradientBlend,
  EwWarpAffineNativeRowFilter,               EwWarpAffineNativeRowFilterBlend,
  EwWarpAffineNativeRowFilterSolid,          EwWarpAffineNativeRowFilterSolidBlend,
  EwWarpAffineNativeRowFilterGradient,       EwWarpAffineNativeRowFilterGradientBlend,
  EwWarpAffineNativeRowFilterGradient,       EwWarpAffineNativeRowFilterGradientBlend,
  EwWarpAffineIndex8Row,                     EwWarpAffineIndex8RowBlend,
  EwWarpAffineIndex8RowSolid,                EwWarpAffineIndex8RowSolidBlend,
  EwWarpAffineIndex8RowGradient,             EwWarpAffineIndex8RowGradientBlend,
  EwWarpAffineIndex8RowGradient,             EwWarpAffineIndex8RowGradientBlend,
  EwWarpAffineIndex8RowFilter,               EwWarpAffineIndex8RowFilterBlend,
  EwWarpAffineIndex8RowFilterSolid,          EwWarpAffineIndex8RowFilterSolidBlend,
  EwWarpAffineIndex8RowFilterGradient,       EwWarpAffineIndex8RowFilterGradientBlend,
  EwWarpAffineIndex8RowFilterGradient,       EwWarpAffineIndex8RowFilterGradientBlend,
  0,                                         0,
  EwWarpAffineAlpha8RowSolid,                EwWarpAffineAlpha8RowSolidBlend,
  EwWarpAffineAlpha8RowGradient,             EwWarpAffineAlpha8RowGradientBlend,
  EwWarpAffineAlpha8RowGradient,             EwWarpAffineAlpha8RowGradientBlend,
  0,                                         0,
  EwWarpAffineAlpha8RowFilterSolid,          EwWarpAffineAlpha8RowFilterSolidBlend,
  EwWarpAffineAlpha8RowFilterGradient,       EwWarpAffineAlpha8RowFilterGradientBlend,
  EwWarpAffineAlpha8RowFilterGradient,       EwWarpAffineAlpha8RowFilterGradientBlend,
  EwWarpAffineRGB565Row,                     EwWarpAffineRGB565Row,
  EwWarpAffineRGB565RowSolid,                EwWarpAffineRGB565RowSolidBlend,
  EwWarpAffineRGB565RowGradient,             EwWarpAffineRGB565RowGradientBlend,
  EwWarpAffineRGB565RowGradient,             EwWarpAffineRGB565RowGradientBlend,
  EwWarpAffineRGB565RowFilter,               EwWarpAffineRGB565RowFilter,
  EwWarpAffineRGB565RowFilterSolid,          EwWarpAffineRGB565RowFilterSolidBlend,
  EwWarpAffineRGB565RowFilterGradient,       EwWarpAffineRGB565RowFilterGradientBlend,
  EwWarpAffineRGB565RowFilterGradient,       EwWarpAffineRGB565RowFilterGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwScreenWarpAffineNativeRow,               EwScreenWarpAffineNativeRowBlend,
  EwScreenWarpAffineNativeRowSolid,          EwScreenWarpAffineNativeRowSolidBlend,
  EwScreenWarpAffineNativeRowGradient,       EwScreenWarpAffineNativeRowGradientBlend,
  EwScreenWarpAffineNativeRowGradient,       EwScreenWarpAffineNativeRowGradientBlend,
  EwScreenWarpAffineNativeRowFilter,         EwScreenWarpAffineNativeRowFilterBlend,
  EwScreenWarpAffineNativeRowFilterSolid,    EwScreenWarpAffineNativeRowFilterSolidBlend,
  EwScreenWarpAffineNativeRowFilterGradient, EwScreenWarpAffineNativeRowFilterGradientBlend,
  EwScreenWarpAffineNativeRowFilterGradient, EwScreenWarpAffineNativeRowFilterGradientBlend,
  EwScreenWarpAffineIndex8Row,               EwScreenWarpAffineIndex8RowBlend,
  EwScreenWarpAffineIndex8RowSolid,          EwScreenWarpAffineIndex8RowSolidBlend,
  EwScreenWarpAffineIndex8RowGradient,       EwScreenWarpAffineIndex8RowGradientBlend,
  EwScreenWarpAffineIndex8RowGradient,       EwScreenWarpAffineIndex8RowGradientBlend,
  EwScreenWarpAffineIndex8RowFilter,         EwScreenWarpAffineIndex8RowFilterBlend,
  EwScreenWarpAffineIndex8RowFilterSolid,    EwScreenWarpAffineIndex8RowFilterSolidBlend,
  EwScreenWarpAffineIndex8RowFilterGradient, EwScreenWarpAffineIndex8RowFilterGradientBlend,
  EwScreenWarpAffineIndex8RowFilterGradient, EwScreenWarpAffineIndex8RowFilterGradientBlend,
  0,                                         0,
  EwScreenWarpAffineAlpha8RowSolid,          EwScreenWarpAffineAlpha8RowSolidBlend,
  EwScreenWarpAffineAlpha8RowGradient,       EwScreenWarpAffineAlpha8RowGradientBlend,
  EwScreenWarpAffineAlpha8RowGradient,       EwScreenWarpAffineAlpha8RowGradientBlend,
  0,                                         0,
  EwScreenWarpAffineAlpha8RowFilterSolid,    EwScreenWarpAffineAlpha8RowFilterSolidBlend,
  EwScreenWarpAffineAlpha8RowFilterGradient, EwScreenWarpAffineAlpha8RowFilterGradientBlend,
  EwScreenWarpAffineAlpha8RowFilterGradient, EwScreenWarpAffineAlpha8RowFilterGradientBlend,
  EwScreenWarpAffineRGB565Row,               EwScreenWarpAffineRGB565Row,
  EwScreenWarpAffineRGB565RowSolid,          EwScreenWarpAffineRGB565RowSolidBlend,
  EwScreenWarpAffineRGB565RowGradient,       EwScreenWarpAffineRGB565RowGradientBlend,
  EwScreenWarpAffineRGB565RowGradient,       EwScreenWarpAffineRGB565RowGradientBlend,
  EwScreenWarpAffineRGB565RowFilter,         EwScreenWarpAffineRGB565RowFilter,
  EwScreenWarpAffineRGB565RowFilterSolid,    EwScreenWarpAffineRGB565RowFilterSolidBlend,
  EwScreenWarpAffineRGB565RowFilterGradient, EwScreenWarpAffineRGB565RowFilterGradientBlend,
  EwScreenWarpAffineRGB565RowFilterGradient, EwScreenWarpAffineRGB565RowFilterGradientBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'scale surface' operation. For missing scale
   functionality the 'warp affine' operation will be used. */
#ifndef EwGfxScaleNative
  #define EwGfxScaleNative                 EwGfxWarpAffineNative
#endif

#ifndef EwGfxScaleNativeBlend
  #define EwGfxScaleNativeBlend            EwGfxWarpAffineNativeBlend
#endif

#ifndef EwGfxScaleNativeSolid
  #define EwGfxScaleNativeSolid            EwGfxWarpAffineNativeSolid
#endif

#ifndef EwGfxScaleNativeSolidBlend
  #define EwGfxScaleNativeSolidBlend       EwGfxWarpAffineNativeSolidBlend
#endif

#ifndef EwGfxScaleNativeGradient
  #define EwGfxScaleNativeGradient         EwGfxWarpAffineNativeGradient
#endif

#ifndef EwGfxScaleNativeGradientBlend
  #define EwGfxScaleNativeGradientBlend    EwGfxWarpAffineNativeGradientBlend
#endif

#ifndef EwGfxScaleNativeLinearGradient
  #define EwGfxScaleNativeLinearGradient   EwGfxScaleNativeGradient
#endif

#ifndef EwGfxScaleNativeLinearGradientBlend
  #define EwGfxScaleNativeLinearGradientBlend \
    EwGfxScaleNativeGradientBlend
#endif

#ifndef EwGfxScaleNativeFilter
  #define EwGfxScaleNativeFilter           EwGfxWarpAffineNativeFilter
#endif

#ifndef EwGfxScaleNativeFilterBlend
  #define EwGfxScaleNativeFilterBlend      EwGfxWarpAffineNativeFilterBlend
#endif

#ifndef EwGfxScaleNativeFilterSolid
  #define EwGfxScaleNativeFilterSolid      EwGfxWarpAffineNativeFilterSolid
#endif

#ifndef EwGfxScaleNativeFilterSolidBlend
  #define EwGfxScaleNativeFilterSolidBlend EwGfxWarpAffineNativeFilterSolidBlend
#endif

#ifndef EwGfxScaleNativeFilterGradient
  #define EwGfxScaleNativeFilterGradient   EwGfxWarpAffineNativeFilterGradient
#endif

#ifndef EwGfxScaleNativeFilterGradientBlend
  #define EwGfxScaleNativeFilterGradientBlend \
    EwGfxWarpAffineNativeFilterGradientBlend
#endif

#ifndef EwGfxScaleNativeFilterLinearGradient
  #define EwGfxScaleNativeFilterLinearGradient \
    EwGfxScaleNativeFilterGradient
#endif

#ifndef EwGfxScaleNativeFilterLinearGradientBlend
  #define EwGfxScaleNativeFilterLinearGradientBlend \
    EwGfxScaleNativeFilterGradientBlend
#endif

#ifndef EwGfxScaleIndex8
  #define EwGfxScaleIndex8                 EwGfxWarpAffineIndex8
#endif

#ifndef EwGfxScaleIndex8Blend
  #define EwGfxScaleIndex8Blend            EwGfxWarpAffineIndex8Blend
#endif

#ifndef EwGfxScaleIndex8Solid
  #define EwGfxScaleIndex8Solid            EwGfxWarpAffineIndex8Solid
#endif

#ifndef EwGfxScaleIndex8SolidBlend
  #define EwGfxScaleIndex8SolidBlend       EwGfxWarpAffineIndex8SolidBlend
#endif

#ifndef EwGfxScaleIndex8Gradient
  #define EwGfxScaleIndex8Gradient         EwGfxWarpAffineIndex8Gradient
#endif

#ifndef EwGfxScaleIndex8GradientBlend
  #define EwGfxScaleIndex8GradientBlend    EwGfxWarpAffineIndex8GradientBlend
#endif

#ifndef EwGfxScaleIndex8LinearGradient
  #define EwGfxScaleIndex8LinearGradient   EwGfxScaleIndex8Gradient
#endif

#ifndef EwGfxScaleIndex8LinearGradientBlend
  #define EwGfxScaleIndex8LinearGradientBlend \
    EwGfxScaleIndex8GradientBlend
#endif

#ifndef EwGfxScaleIndex8Filter
  #define EwGfxScaleIndex8Filter           EwGfxWarpAffineIndex8Filter
#endif

#ifndef EwGfxScaleIndex8FilterBlend
  #define EwGfxScaleIndex8FilterBlend      EwGfxWarpAffineIndex8FilterBlend
#endif

#ifndef EwGfxScaleIndex8FilterSolid
  #define EwGfxScaleIndex8FilterSolid      EwGfxWarpAffineIndex8FilterSolid
#endif

#ifndef EwGfxScaleIndex8FilterSolidBlend
  #define EwGfxScaleIndex8FilterSolidBlend EwGfxWarpAffineIndex8FilterSolidBlend
#endif

#ifndef EwGfxScaleIndex8FilterGradient
  #define EwGfxScaleIndex8FilterGradient   EwGfxWarpAffineIndex8FilterGradient
#endif

#ifndef EwGfxScaleIndex8FilterGradientBlend
  #define EwGfxScaleIndex8FilterGradientBlend \
    EwGfxWarpAffineIndex8FilterGradientBlend
#endif

#ifndef EwGfxScaleIndex8FilterLinearGradient
  #define EwGfxScaleIndex8FilterLinearGradient \
    EwGfxScaleIndex8FilterGradient
#endif

#ifndef EwGfxScaleIndex8FilterLinearGradientBlend
  #define EwGfxScaleIndex8FilterLinearGradientBlend \
    EwGfxScaleIndex8FilterGradientBlend
#endif


#ifndef EwGfxScaleAlpha8Solid
  #define EwGfxScaleAlpha8Solid            EwGfxWarpAffineAlpha8Solid
#endif

#ifndef EwGfxScaleAlpha8SolidBlend
  #define EwGfxScaleAlpha8SolidBlend       EwGfxWarpAffineAlpha8SolidBlend
#endif

#ifndef EwGfxScaleAlpha8Gradient
  #define EwGfxScaleAlpha8Gradient         EwGfxWarpAffineAlpha8Gradient
#endif

#ifndef EwGfxScaleAlpha8GradientBlend
  #define EwGfxScaleAlpha8GradientBlend    EwGfxWarpAffineAlpha8GradientBlend
#endif

#ifndef EwGfxScaleAlpha8LinearGradient
  #define EwGfxScaleAlpha8LinearGradient   EwGfxScaleAlpha8Gradient
#endif

#ifndef EwGfxScaleAlpha8LinearGradientBlend
  #define EwGfxScaleAlpha8LinearGradientBlend \
    EwGfxScaleAlpha8GradientBlend
#endif

#ifndef EwGfxScaleAlpha8FilterSolid
  #define EwGfxScaleAlpha8FilterSolid      EwGfxWarpAffineAlpha8FilterSolid
#endif

#ifndef EwGfxScaleAlpha8FilterSolidBlend
  #define EwGfxScaleAlpha8FilterSolidBlend EwGfxWarpAffineAlpha8FilterSolidBlend
#endif

#ifndef EwGfxScaleAlpha8FilterGradient
  #define EwGfxScaleAlpha8FilterGradient   EwGfxWarpAffineAlpha8FilterGradient
#endif

#ifndef EwGfxScaleAlpha8FilterGradientBlend
  #define EwGfxScaleAlpha8FilterGradientBlend \
    EwGfxWarpAffineAlpha8FilterGradientBlend
#endif

#ifndef EwGfxScaleAlpha8FilterLinearGradient
  #define EwGfxScaleAlpha8FilterLinearGradient \
    EwGfxScaleAlpha8FilterGradient
#endif

#ifndef EwGfxScaleAlpha8FilterLinearGradientBlend
  #define EwGfxScaleAlpha8FilterLinearGradientBlend \
    EwGfxScaleAlpha8FilterGradientBlend
#endif

#ifndef EwGfxScaleRGB565
  #define EwGfxScaleRGB565                 EwGfxWarpAffineRGB565
#endif

#ifndef EwGfxScaleRGB565Solid
  #define EwGfxScaleRGB565Solid            EwGfxWarpAffineRGB565Solid
#endif

#ifndef EwGfxScaleRGB565SolidBlend
  #define EwGfxScaleRGB565SolidBlend       EwGfxWarpAffineRGB565SolidBlend
#endif

#ifndef EwGfxScaleRGB565Gradient
  #define EwGfxScaleRGB565Gradient         EwGfxWarpAffineRGB565Gradient
#endif

#ifndef EwGfxScaleRGB565GradientBlend
  #define EwGfxScaleRGB565GradientBlend    EwGfxWarpAffineRGB565GradientBlend
#endif

#ifndef EwGfxScaleRGB565LinearGradient
  #define EwGfxScaleRGB565LinearGradient   EwGfxScaleRGB565Gradient
#endif

#ifndef EwGfxScaleRGB565LinearGradientBlend
  #define EwGfxScaleRGB565LinearGradientBlend \
    EwGfxScaleRGB565GradientBlend
#endif

#ifndef EwGfxScaleRGB565Filter
  #define EwGfxScaleRGB565Filter           EwGfxWarpAffineRGB565Filter
#endif

#ifndef EwGfxScaleRGB565FilterSolid
  #define EwGfxScaleRGB565FilterSolid      EwGfxWarpAffineRGB565FilterSolid
#endif

#ifndef EwGfxScaleRGB565FilterSolidBlend
  #define EwGfxScaleRGB565FilterSolidBlend EwGfxWarpAffineRGB565FilterSolidBlend
#endif

#ifndef EwGfxScaleRGB565FilterGradient
  #define EwGfxScaleRGB565FilterGradient   EwGfxWarpAffineRGB565FilterGradient
#endif

#ifndef EwGfxScaleRGB565FilterGradientBlend
  #define EwGfxScaleRGB565FilterGradientBlend \
    EwGfxWarpAffineRGB565FilterGradientBlend
#endif

#ifndef EwGfxScaleRGB565FilterLinearGradient
  #define EwGfxScaleRGB565FilterLinearGradient \
    EwGfxScaleRGB565FilterGradient
#endif

#ifndef EwGfxScaleRGB565FilterLinearGradientBlend
  #define EwGfxScaleRGB565FilterLinearGradientBlend \
    EwGfxScaleRGB565FilterGradientBlend
#endif

#ifndef EwGfxScreenScaleNative
  #define EwGfxScreenScaleNative                 EwGfxScreenWarpAffineNative
#endif

#ifndef EwGfxScreenScaleNativeBlend
  #define EwGfxScreenScaleNativeBlend            EwGfxScreenWarpAffineNativeBlend
#endif

#ifndef EwGfxScreenScaleNativeSolid
  #define EwGfxScreenScaleNativeSolid            EwGfxScreenWarpAffineNativeSolid
#endif

#ifndef EwGfxScreenScaleNativeSolidBlend
  #define EwGfxScreenScaleNativeSolidBlend       EwGfxScreenWarpAffineNativeSolidBlend
#endif

#ifndef EwGfxScreenScaleNativeGradient
  #define EwGfxScreenScaleNativeGradient         EwGfxScreenWarpAffineNativeGradient
#endif

#ifndef EwGfxScreenScaleNativeGradientBlend
  #define EwGfxScreenScaleNativeGradientBlend    EwGfxScreenWarpAffineNativeGradientBlend
#endif

#ifndef EwGfxScreenScaleNativeLinearGradient
  #define EwGfxScreenScaleNativeLinearGradient   EwGfxScreenScaleNativeGradient
#endif

#ifndef EwGfxScreenScaleNativeLinearGradientBlend
  #define EwGfxScreenScaleNativeLinearGradientBlend \
    EwGfxScreenScaleNativeGradientBlend
#endif

#ifndef EwGfxScreenScaleNativeFilter
  #define EwGfxScreenScaleNativeFilter           EwGfxScreenWarpAffineNativeFilter
#endif

#ifndef EwGfxScreenScaleNativeFilterBlend
  #define EwGfxScreenScaleNativeFilterBlend      EwGfxScreenWarpAffineNativeFilterBlend
#endif

#ifndef EwGfxScreenScaleNativeFilterSolid
  #define EwGfxScreenScaleNativeFilterSolid      EwGfxScreenWarpAffineNativeFilterSolid
#endif

#ifndef EwGfxScreenScaleNativeFilterSolidBlend
  #define EwGfxScreenScaleNativeFilterSolidBlend EwGfxScreenWarpAffineNativeFilterSolidBlend
#endif

#ifndef EwGfxScreenScaleNativeFilterGradient
  #define EwGfxScreenScaleNativeFilterGradient   EwGfxScreenWarpAffineNativeFilterGradient
#endif

#ifndef EwGfxScreenScaleNativeFilterGradientBlend
  #define EwGfxScreenScaleNativeFilterGradientBlend \
    EwGfxScreenWarpAffineNativeFilterGradientBlend
#endif

#ifndef EwGfxScreenScaleNativeFilterLinearGradient
  #define EwGfxScreenScaleNativeFilterLinearGradient \
    EwGfxScreenScaleNativeFilterGradient
#endif

#ifndef EwGfxScreenScaleNativeFilterLinearGradientBlend
  #define EwGfxScreenScaleNativeFilterLinearGradientBlend \
    EwGfxScreenScaleNativeFilterGradientBlend
#endif

#ifndef EwGfxScreenScaleIndex8
  #define EwGfxScreenScaleIndex8                 EwGfxScreenWarpAffineIndex8
#endif

#ifndef EwGfxScreenScaleIndex8Blend
  #define EwGfxScreenScaleIndex8Blend            EwGfxScreenWarpAffineIndex8Blend
#endif

#ifndef EwGfxScreenScaleIndex8Solid
  #define EwGfxScreenScaleIndex8Solid            EwGfxScreenWarpAffineIndex8Solid
#endif

#ifndef EwGfxScreenScaleIndex8SolidBlend
  #define EwGfxScreenScaleIndex8SolidBlend       EwGfxScreenWarpAffineIndex8SolidBlend
#endif

#ifndef EwGfxScreenScaleIndex8Gradient
  #define EwGfxScreenScaleIndex8Gradient         EwGfxScreenWarpAffineIndex8Gradient
#endif

#ifndef EwGfxScreenScaleIndex8GradientBlend
  #define EwGfxScreenScaleIndex8GradientBlend    EwGfxScreenWarpAffineIndex8GradientBlend
#endif

#ifndef EwGfxScreenScaleIndex8LinearGradient
  #define EwGfxScreenScaleIndex8LinearGradient   EwGfxScreenScaleIndex8Gradient
#endif

#ifndef EwGfxScreenScaleIndex8LinearGradientBlend
  #define EwGfxScreenScaleIndex8LinearGradientBlend \
    EwGfxScreenScaleIndex8GradientBlend
#endif

#ifndef EwGfxScreenScaleIndex8Filter
  #define EwGfxScreenScaleIndex8Filter           EwGfxScreenWarpAffineIndex8Filter
#endif

#ifndef EwGfxScreenScaleIndex8FilterBlend
  #define EwGfxScreenScaleIndex8FilterBlend      EwGfxScreenWarpAffineIndex8FilterBlend
#endif

#ifndef EwGfxScreenScaleIndex8FilterSolid
  #define EwGfxScreenScaleIndex8FilterSolid      EwGfxScreenWarpAffineIndex8FilterSolid
#endif

#ifndef EwGfxScreenScaleIndex8FilterSolidBlend
  #define EwGfxScreenScaleIndex8FilterSolidBlend EwGfxScreenWarpAffineIndex8FilterSolidBlend
#endif

#ifndef EwGfxScreenScaleIndex8FilterGradient
  #define EwGfxScreenScaleIndex8FilterGradient   EwGfxScreenWarpAffineIndex8FilterGradient
#endif

#ifndef EwGfxScreenScaleIndex8FilterGradientBlend
  #define EwGfxScreenScaleIndex8FilterGradientBlend \
    EwGfxScreenWarpAffineIndex8FilterGradientBlend
#endif

#ifndef EwGfxScreenScaleIndex8FilterLinearGradient
  #define EwGfxScreenScaleIndex8FilterLinearGradient \
    EwGfxScreenScaleIndex8FilterGradient
#endif

#ifndef EwGfxScreenScaleIndex8FilterLinearGradientBlend
  #define EwGfxScreenScaleIndex8FilterLinearGradientBlend \
    EwGfxScreenScaleIndex8FilterGradientBlend
#endif

#ifndef EwGfxScreenScaleAlpha8Solid
  #define EwGfxScreenScaleAlpha8Solid            EwGfxScreenWarpAffineAlpha8Solid
#endif

#ifndef EwGfxScreenScaleAlpha8SolidBlend
  #define EwGfxScreenScaleAlpha8SolidBlend       EwGfxScreenWarpAffineAlpha8SolidBlend
#endif

#ifndef EwGfxScreenScaleAlpha8Gradient
  #define EwGfxScreenScaleAlpha8Gradient         EwGfxScreenWarpAffineAlpha8Gradient
#endif

#ifndef EwGfxScreenScaleAlpha8GradientBlend
  #define EwGfxScreenScaleAlpha8GradientBlend    EwGfxScreenWarpAffineAlpha8GradientBlend
#endif

#ifndef EwGfxScreenScaleAlpha8LinearGradient
  #define EwGfxScreenScaleAlpha8LinearGradient   EwGfxScreenScaleAlpha8Gradient
#endif

#ifndef EwGfxScreenScaleAlpha8LinearGradientBlend
  #define EwGfxScreenScaleAlpha8LinearGradientBlend \
    EwGfxScreenScaleAlpha8GradientBlend
#endif

#ifndef EwGfxScreenScaleAlpha8FilterSolid
  #define EwGfxScreenScaleAlpha8FilterSolid      EwGfxScreenWarpAffineAlpha8FilterSolid
#endif

#ifndef EwGfxScreenScaleAlpha8FilterSolidBlend
  #define EwGfxScreenScaleAlpha8FilterSolidBlend EwGfxScreenWarpAffineAlpha8FilterSolidBlend
#endif

#ifndef EwGfxScreenScaleAlpha8FilterGradient
  #define EwGfxScreenScaleAlpha8FilterGradient   EwGfxScreenWarpAffineAlpha8FilterGradient
#endif

#ifndef EwGfxScreenScaleAlpha8FilterGradientBlend
  #define EwGfxScreenScaleAlpha8FilterGradientBlend \
    EwGfxScreenWarpAffineAlpha8FilterGradientBlend
#endif

#ifndef EwGfxScreenScaleAlpha8FilterLinearGradient
  #define EwGfxScreenScaleAlpha8FilterLinearGradient \
    EwGfxScreenScaleAlpha8FilterGradient
#endif

#ifndef EwGfxScreenScaleAlpha8FilterLinearGradientBlend
  #define EwGfxScreenScaleAlpha8FilterLinearGradientBlend \
    EwGfxScreenScaleAlpha8FilterGradientBlend
#endif

#ifndef EwGfxScreenScaleRGB565
  #define EwGfxScreenScaleRGB565                 EwGfxScreenWarpAffineRGB565
#endif

#ifndef EwGfxScreenScaleRGB565Solid
  #define EwGfxScreenScaleRGB565Solid            EwGfxScreenWarpAffineRGB565Solid
#endif

#ifndef EwGfxScreenScaleRGB565SolidBlend
  #define EwGfxScreenScaleRGB565SolidBlend       EwGfxScreenWarpAffineRGB565SolidBlend
#endif

#ifndef EwGfxScreenScaleRGB565Gradient
  #define EwGfxScreenScaleRGB565Gradient         EwGfxScreenWarpAffineRGB565Gradient
#endif

#ifndef EwGfxScreenScaleRGB565GradientBlend
  #define EwGfxScreenScaleRGB565GradientBlend    EwGfxScreenWarpAffineRGB565GradientBlend
#endif

#ifndef EwGfxScreenScaleRGB565LinearGradient
  #define EwGfxScreenScaleRGB565LinearGradient   EwGfxScreenScaleRGB565Gradient
#endif

#ifndef EwGfxScreenScaleRGB565LinearGradientBlend
  #define EwGfxScreenScaleRGB565LinearGradientBlend \
    EwGfxScreenScaleRGB565GradientBlend
#endif

#ifndef EwGfxScreenScaleRGB565Filter
  #define EwGfxScreenScaleRGB565Filter           EwGfxScreenWarpAffineRGB565Filter
#endif

#ifndef EwGfxScreenScaleRGB565FilterSolid
  #define EwGfxScreenScaleRGB565FilterSolid      EwGfxScreenWarpAffineRGB565FilterSolid
#endif

#ifndef EwGfxScreenScaleRGB565FilterSolidBlend
  #define EwGfxScreenScaleRGB565FilterSolidBlend EwGfxScreenWarpAffineRGB565FilterSolidBlend
#endif

#ifndef EwGfxScreenScaleRGB565FilterGradient
  #define EwGfxScreenScaleRGB565FilterGradient   EwGfxScreenWarpAffineRGB565FilterGradient
#endif

#ifndef EwGfxScreenScaleRGB565FilterGradientBlend
  #define EwGfxScreenScaleRGB565FilterGradientBlend \
    EwGfxScreenWarpAffineRGB565FilterGradientBlend
#endif

#ifndef EwGfxScreenScaleRGB565FilterLinearGradient
  #define EwGfxScreenScaleRGB565FilterLinearGradient \
    EwGfxScreenScaleRGB565FilterGradient
#endif

#ifndef EwGfxScreenScaleRGB565FilterLinearGradientBlend
  #define EwGfxScreenScaleRGB565FilterLinearGradientBlend \
    EwGfxScreenScaleRGB565FilterGradientBlend
#endif

/* Register the functionality provided by the underlying graphics subsystem. */
static const XWarpDriver WarpScaleDrivers[] =
{
  EwGfxScaleNative,                           EwGfxScaleNativeBlend,
  EwGfxScaleNativeSolid,                      EwGfxScaleNativeSolidBlend,
  EwGfxScaleNativeGradient,                   EwGfxScaleNativeGradientBlend,
  EwGfxScaleNativeLinearGradient,             EwGfxScaleNativeLinearGradientBlend,
  EwGfxScaleNativeFilter,                     EwGfxScaleNativeFilterBlend,
  EwGfxScaleNativeFilterSolid,                EwGfxScaleNativeFilterSolidBlend,
  EwGfxScaleNativeFilterGradient,             EwGfxScaleNativeFilterGradientBlend,
  EwGfxScaleNativeFilterLinearGradient,       EwGfxScaleNativeFilterLinearGradientBlend,
  EwGfxScaleIndex8,                           EwGfxScaleIndex8Blend,
  EwGfxScaleIndex8Solid,                      EwGfxScaleIndex8SolidBlend,
  EwGfxScaleIndex8Gradient,                   EwGfxScaleIndex8GradientBlend,
  EwGfxScaleIndex8LinearGradient,             EwGfxScaleIndex8LinearGradientBlend,
  EwGfxScaleIndex8Filter,                     EwGfxScaleIndex8FilterBlend,
  EwGfxScaleIndex8FilterSolid,                EwGfxScaleIndex8FilterSolidBlend,
  EwGfxScaleIndex8FilterGradient,             EwGfxScaleIndex8FilterGradientBlend,
  EwGfxScaleIndex8FilterLinearGradient,       EwGfxScaleIndex8FilterLinearGradientBlend,
  0,                                          0,
  EwGfxScaleAlpha8Solid,                      EwGfxScaleAlpha8SolidBlend,
  EwGfxScaleAlpha8Gradient,                   EwGfxScaleAlpha8GradientBlend,
  EwGfxScaleAlpha8LinearGradient,             EwGfxScaleAlpha8LinearGradientBlend,
  0,                                          0,
  EwGfxScaleAlpha8FilterSolid,                EwGfxScaleAlpha8FilterSolidBlend,
  EwGfxScaleAlpha8FilterGradient,             EwGfxScaleAlpha8FilterGradientBlend,
  EwGfxScaleAlpha8FilterLinearGradient,       EwGfxScaleAlpha8FilterLinearGradientBlend,
  EwGfxScaleRGB565,                           EwGfxScaleRGB565,
  EwGfxScaleRGB565Solid,                      EwGfxScaleRGB565SolidBlend,
  EwGfxScaleRGB565Gradient,                   EwGfxScaleRGB565GradientBlend,
  EwGfxScaleRGB565LinearGradient,             EwGfxScaleRGB565LinearGradientBlend,
  EwGfxScaleRGB565Filter,                     EwGfxScaleRGB565Filter,
  EwGfxScaleRGB565FilterSolid,                EwGfxScaleRGB565FilterSolidBlend,
  EwGfxScaleRGB565FilterGradient,             EwGfxScaleRGB565FilterGradientBlend,
  EwGfxScaleRGB565FilterLinearGradient,       EwGfxScaleRGB565FilterLinearGradientBlend,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  EwGfxScreenScaleNative,                     EwGfxScreenScaleNativeBlend,
  EwGfxScreenScaleNativeSolid,                EwGfxScreenScaleNativeSolidBlend,
  EwGfxScreenScaleNativeGradient,             EwGfxScreenScaleNativeGradientBlend,
  EwGfxScreenScaleNativeLinearGradient,       EwGfxScreenScaleNativeLinearGradientBlend,
  EwGfxScreenScaleNativeFilter,               EwGfxScreenScaleNativeFilterBlend,
  EwGfxScreenScaleNativeFilterSolid,          EwGfxScreenScaleNativeFilterSolidBlend,
  EwGfxScreenScaleNativeFilterGradient,       EwGfxScreenScaleNativeFilterGradientBlend,
  EwGfxScreenScaleNativeFilterLinearGradient, EwGfxScreenScaleNativeFilterLinearGradientBlend,
  EwGfxScreenScaleIndex8,                     EwGfxScreenScaleIndex8Blend,
  EwGfxScreenScaleIndex8Solid,                EwGfxScreenScaleIndex8SolidBlend,
  EwGfxScreenScaleIndex8Gradient,             EwGfxScreenScaleIndex8GradientBlend,
  EwGfxScreenScaleIndex8LinearGradient,       EwGfxScreenScaleIndex8LinearGradientBlend,
  EwGfxScreenScaleIndex8Filter,               EwGfxScreenScaleIndex8FilterBlend,
  EwGfxScreenScaleIndex8FilterSolid,          EwGfxScreenScaleIndex8FilterSolidBlend,
  EwGfxScreenScaleIndex8FilterGradient,       EwGfxScreenScaleIndex8FilterGradientBlend,
  EwGfxScreenScaleIndex8FilterLinearGradient, EwGfxScreenScaleIndex8FilterLinearGradientBlend,
  0,                                          0,
  EwGfxScreenScaleAlpha8Solid,                EwGfxScreenScaleAlpha8SolidBlend,
  EwGfxScreenScaleAlpha8Gradient,             EwGfxScreenScaleAlpha8GradientBlend,
  EwGfxScreenScaleAlpha8LinearGradient,       EwGfxScreenScaleAlpha8LinearGradientBlend,
  0,                                          0,
  EwGfxScreenScaleAlpha8FilterSolid,          EwGfxScreenScaleAlpha8FilterSolidBlend,
  EwGfxScreenScaleAlpha8FilterGradient,       EwGfxScreenScaleAlpha8FilterGradientBlend,
  EwGfxScreenScaleAlpha8FilterLinearGradient, EwGfxScreenScaleAlpha8FilterLinearGradientBlend,
  EwGfxScreenScaleRGB565,                     EwGfxScreenScaleRGB565,
  EwGfxScreenScaleRGB565Solid,                EwGfxScreenScaleRGB565SolidBlend,
  EwGfxScreenScaleRGB565Gradient,             EwGfxScreenScaleRGB565GradientBlend,
  EwGfxScreenScaleRGB565LinearGradient,       EwGfxScreenScaleRGB565LinearGradientBlend,
  EwGfxScreenScaleRGB565Filter,               EwGfxScreenScaleRGB565Filter,
  EwGfxScreenScaleRGB565FilterSolid,          EwGfxScreenScaleRGB565FilterSolidBlend,
  EwGfxScreenScaleRGB565FilterGradient,       EwGfxScreenScaleRGB565FilterGradientBlend,
  EwGfxScreenScaleRGB565FilterLinearGradient, EwGfxScreenScaleRGB565FilterLinearGradientBlend,
#endif

  0
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'fill polygon' operation. */
#ifndef EwGfxPolygonGradient
  #define EwGfxPolygonGradient                 0
#endif

#ifndef EwGfxPolygonGradientBlend
  #define EwGfxPolygonGradientBlend            0
#endif

#ifndef EwGfxPolygonAntialiasedGradient
  #define EwGfxPolygonAntialiasedGradient      0
#endif

#ifndef EwGfxPolygonAntialiasedGradientBlend
  #define EwGfxPolygonAntialiasedGradientBlend 0
#endif

#ifndef EwGfxPolygonLinearGradient
  #define EwGfxPolygonLinearGradient           EwGfxPolygonGradient
#endif

#ifndef EwGfxPolygonLinearGradientBlend
  #define EwGfxPolygonLinearGradientBlend      EwGfxPolygonGradientBlend
#endif

#ifndef EwGfxPolygonAntialiasedLinearGradient
  #define EwGfxPolygonAntialiasedLinearGradient \
    EwGfxPolygonAntialiasedGradient
#endif

#ifndef EwGfxPolygonAntialiasedLinearGradientBlend
  #define EwGfxPolygonAntialiasedLinearGradientBlend \
    EwGfxPolygonAntialiasedGradientBlend
#endif

#ifndef EwGfxPolygonSolid
  #define EwGfxPolygonSolid                 EwGfxPolygonLinearGradient
#endif

#ifndef EwGfxPolygonSolidBlend
  #define EwGfxPolygonSolidBlend            EwGfxPolygonLinearGradientBlend
#endif

#ifndef EwGfxPolygonAntialiasedSolid
  #define EwGfxPolygonAntialiasedSolid      EwGfxPolygonAntialiasedLinearGradient
#endif

#ifndef EwGfxPolygonAntialiasedSolidBlend
  #define EwGfxPolygonAntialiasedSolidBlend EwGfxPolygonAntialiasedLinearGradientBlend
#endif

#ifndef EwGfxScreenPolygonGradient
  #define EwGfxScreenPolygonGradient                 0
#endif

#ifndef EwGfxScreenPolygonGradientBlend
  #define EwGfxScreenPolygonGradientBlend            0
#endif

#ifndef EwGfxScreenPolygonAntialiasedGradient
  #define EwGfxScreenPolygonAntialiasedGradient      0
#endif

#ifndef EwGfxScreenPolygonAntialiasedGradientBlend
  #define EwGfxScreenPolygonAntialiasedGradientBlend 0
#endif

#ifndef EwGfxScreenPolygonLinearGradient
  #define EwGfxScreenPolygonLinearGradient           EwGfxScreenPolygonGradient
#endif

#ifndef EwGfxScreenPolygonLinearGradientBlend
  #define EwGfxScreenPolygonLinearGradientBlend      EwGfxScreenPolygonGradientBlend
#endif

#ifndef EwGfxScreenPolygonAntialiasedLinearGradient
  #define EwGfxScreenPolygonAntialiasedLinearGradient \
    EwGfxScreenPolygonAntialiasedGradient
#endif

#ifndef EwGfxScreenPolygonAntialiasedLinearGradientBlend
  #define EwGfxScreenPolygonAntialiasedLinearGradientBlend \
    EwGfxScreenPolygonAntialiasedGradientBlend
#endif

#ifndef EwGfxScreenPolygonSolid
  #define EwGfxScreenPolygonSolid                 EwGfxScreenPolygonLinearGradient
#endif

#ifndef EwGfxScreenPolygonSolidBlend
  #define EwGfxScreenPolygonSolidBlend            EwGfxScreenPolygonLinearGradientBlend
#endif

#ifndef EwGfxScreenPolygonAntialiasedSolid
  #define EwGfxScreenPolygonAntialiasedSolid      EwGfxScreenPolygonAntialiasedLinearGradient
#endif

#ifndef EwGfxScreenPolygonAntialiasedSolidBlend
  #define EwGfxScreenPolygonAntialiasedSolidBlend EwGfxScreenPolygonAntialiasedLinearGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XPolygonDriver PolygonDrivers[] =
{
  0, 0,
  EwGfxPolygonSolid,                           EwGfxPolygonSolidBlend,
  EwGfxPolygonGradient,                        EwGfxPolygonGradientBlend,
  EwGfxPolygonLinearGradient,                  EwGfxPolygonLinearGradientBlend,
  0, 0,
  EwGfxPolygonAntialiasedSolid,                EwGfxPolygonAntialiasedSolidBlend,
  EwGfxPolygonAntialiasedGradient,             EwGfxPolygonAntialiasedGradientBlend,
  EwGfxPolygonAntialiasedLinearGradient,       EwGfxPolygonAntialiasedLinearGradientBlend,
  0, 0,

#ifdef EW_USE_PIXEL_FORMAT_SCREEN
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* Gap */
  0, 0,
  EwGfxScreenPolygonSolid,                     EwGfxScreenPolygonSolidBlend,
  EwGfxScreenPolygonGradient,                  EwGfxScreenPolygonGradientBlend,
  EwGfxScreenPolygonLinearGradient,            EwGfxScreenPolygonLinearGradientBlend,
  0, 0,
  EwGfxScreenPolygonAntialiasedSolid,          EwGfxScreenPolygonAntialiasedSolidBlend,
  EwGfxScreenPolygonAntialiasedGradient,       EwGfxScreenPolygonAntialiasedGradientBlend,
  EwGfxScreenPolygonAntialiasedLinearGradient, EwGfxScreenPolygonAntialiasedLinearGradientBlend,
  0, 0,
#endif

  0
};


/* pba */
