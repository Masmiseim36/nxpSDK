/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************
   INCLUDE FILES
*************************************************************************/

#include "PL_platformTypesPrivate.h"
#if (defined PLATFORM_CORTEXM7 || defined PLATFORM_CORTEXM33 || defined PLATFORM_WINDOWS || defined PLATFORM_LINUX || \
     defined PLATFORM_AARCH64)
#include "arm_math.h"
#endif

/************************************************************************/
/*                                                                      */
/*    Define code section                                               */
/*                                                                      */
/************************************************************************/
// #undef PLATFORM_WINDOWS
// #define PLATFORM_HIFI4

/**********************************************************************************
   FUNCTION PL_MatxVec_Bias
***********************************************************************************/

// For the moment Bias is not considered
PL_ReturnStatus_en PL_Mat_Trans(const Matrix_instance_f32 *in, Matrix_instance_f32 *out)
{
#if (defined PLATFORM_CORTEXM7 || defined PLATFORM_CORTEX33 || defined PLATFORM_WINDOWS || defined PLATFORM_LINUX || \
     defined PLATFORM_AARCH64)
    return arm_mat_trans_f32((arm_matrix_instance_f32 *)in, (arm_matrix_instance_f32 *)out);

#elif defined PLATFORM_HIFI4 || defined PLATFORM_FUSIONF1

    PL_UINT16 RowsNb = in->numRows;
    PL_UINT16 ColsNb = in->numCols;
    PL_FLOAT *pIn    = in->pData;
    PL_FLOAT *pOut   = out->pData;
    PL_UINT32 col, row = RowsNb, ii = 0, j; /* Loop counters */
    PL_ReturnStatus_en status;

    if ((in->numRows != out->numCols) || (in->numCols != out->numRows))
    {
        status = PL_INVALID_PARAMETER_OUTOFRANGE;
    }
    else
    {
        do
        {
            PL_FLOAT *pTemp; /* Temporary output data matrix pointer */
            pTemp = pOut + ii;

            col = ColsNb;

            for (j = 0; j < col; j++)
            {
                *pTemp = *pIn++;

                pTemp += RowsNb;
            }

            ii++;

            row--;

        } while (row > 0);

        status = PL_SUCCESS;
    }

    return (status);

#endif
}

/************************************************************************/
