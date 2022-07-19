/*
    segments_intersect_test_vector.c
        optimized segment intersection test
    by Francesco Montorsi, 06/03/2012
    
    Build this MEX file by entering the appropriate command at the MATLAB prompt:
        mex -v segments_intersect_test_vector.c
        
    Usage in MATLAB:
       
        intersec_vector = segments_intersect_test_vector(XY1, XY2);
        
    where
    
        XY1 = segments to check against the segments of XY2 (Nx4 matrix with each row containing [x1 y1 x2 y2])
        XY2 = segments to check against the segments of XY1 (Mx4 matrix with each row containing [x1 y1 x2 y2])
        intersec_vector = Nx1 vector of detected intersections for each segment of XY1... that is, 
                          intersec_vector(i) is the number of intersections of the i-th segment of XY1 with the
                                             segments of XY2 or -1 if the i-th segment of XY1 has infinite 
                                             intersections with some segment of set XY2 (e.g. they overlap).
        
        
    TESTS (to be run in MATLAB): see test_segments_intersect.m
        
        
    To unload the resulting MEX file from MATLAB run:
    
        clear segments_intersect_test_vector
*/

#include <math.h>
#include "mex.h"
#include "matrix.h"
#include "float.h"      /* for DBL_EPSILON */

#define ENABLE_DEBUG_PRINTF     (0)

#include "segments_intersect.c.inc"


/* the gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    const double *XY1, *XY2;
    double *out;
    int ret;
    size_t M, N, i1, i2;

    /* check for proper number of arguments */
    /* NOTE: You do not need an else statement when using mexErrMsgTxt
     within an if statement, because it will never get to the else
     statement if mexErrMsgTxt is executed. (mexErrMsgTxt breaks you out of
     the MEX-file) */
    if (nrhs!=2) 
        mexErrMsgTxt("Two matrices of segment endpoints required.");
    if (nlhs!=1) 
        mexErrMsgTxt("One output required.");

    N = mxGetM(prhs[0]);        /* number of the first set of segments */
    M = mxGetM(prhs[1]);        /* number of the second set of segments */
    
#if ENABLE_DEBUG_PRINTF
    mexPrintf("first set contains %d segment(s)...\n", N);
    mexPrintf("second set contains %d segment(s)...\n", M);
#endif
    
    /* check to make sure the input argument are matrices correctly sized */
    if (mxGetN(prhs[0]) != 4 || mxGetN(prhs[1]) != 4)
        mexErrMsgTxt("Input matrices should be Nx4 and Mx4 matrices.");
        
    XY1 = mxGetPr(prhs[0]);
    XY2 = mxGetPr(prhs[1]);

#define XY1_cell(row,col)        (XY1[N*col + row])
#define XY2_cell(row,col)        (XY2[M*col + row])

    /*  the output is a vector Nx1 */
    plhs[0] = mxCreateDoubleMatrix((mwSize)N, 1, mxREAL);
    out = mxGetPr(plhs[0]);
    
    /*  call the C subroutine */
    for (i1 = 0; i1 < N; i1++) {
        out[i1] = 0;
        for (i2 = 0; i2 < M; i2++) {
#if ENABLE_DEBUG_PRINTF
            mexPrintf("segment1: (%.2f;%.2f)-(%.2f;%.2f)\n", XY1_cell(i1,0),XY1_cell(i1,1), XY1_cell(i1,2),XY1_cell(i1,3));
            mexPrintf("segment2: (%.2f;%.2f)-(%.2f;%.2f)\n", XY2_cell(i2,0),XY2_cell(i2,1), XY2_cell(i2,2),XY2_cell(i2,3));
#endif
            ret = segment_intersect(XY1_cell(i1,0),XY1_cell(i1,1), XY1_cell(i1,2),XY1_cell(i1,3), 
                                     XY2_cell(i2,0),XY2_cell(i2,1), XY2_cell(i2,2),XY2_cell(i2,3));
            if (ret == -1)
            {
                /* infinite intersections found... */
                out[i1] = -1;
                break;
            }
            else
                out[i1] += ret;
            
#if ENABLE_DEBUG_PRINTF
            mexPrintf("number of intersections up to now: %d\n\n", n_intersec);
#endif
        }
    }

}
