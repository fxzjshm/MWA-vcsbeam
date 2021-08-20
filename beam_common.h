/********************************************************
 *                                                      *
 * Licensed under the Academic Free License version 3.0 *
 *                                                      *
 ********************************************************/

#ifndef BEAM_COMMON_H
#define BEAM_COMMON_H

#include <inttypes.h>
#include "mwa_hyperbeam.h"
#include "calibration.h"
#include <cuComplex.h>
#include <mwalib.h>

#define MAX_POLS   4

#define N_COPOL    2
#define R2C_SIGN   -1.0
#define NDELAYS    16

#define MWA_LAT -26.703319        /* Array latitude. degrees North */
#define MWA_LON 116.67081         /* Array longitude. degrees East */
#define MWA_HGT 377               /* Array altitude. meters above sea level */

#define cudaCheckErrors(msg) \
    do { \
        cudaError_t __err = cudaGetLastError(); \
        if (__err != cudaSuccess) { \
            fprintf(stderr, "Fatal error: %s (%s at %s:%d)\n", \
                msg, cudaGetErrorString(__err), \
                __FILE__, __LINE__); \
            fprintf(stderr, "*** FAILED - ABORTING\n"); \
            exit(1); \
        } \
    } while (0)


struct beam_geom {
    double mean_ra;
    double mean_dec;
    double az;
    double el;
    double lmst;
    double fracmjd;
    double intmjd;
    double unit_N;
    double unit_E;
    double unit_H;
};


/* Running get_jones from within make_beam */
void get_jones(
        // an array of pointings [pointing][ra/dec][characters]
        int                    npointing, // number of pointings
        VoltageMetadata*       volt_metadata,
        MetafitsMetadata      *metafits_metadata,
        int                    coarse_chan_idx,
        struct                 calibration *cal,
        cuDoubleComplex     ***D,
        cuDoubleComplex       *B,
        cuDoubleComplex    ****invJi                   // output
);

void create_antenna_lists( MetafitsMetadata *metafits_metadata, uint32_t *polX_idxs, uint32_t *polY_idxs );

void int8_to_uint8(int n, int shift, char * to_convert);
void float2int8_trunc(float *f, int n, float min, float max, int8_t *i);
void float_to_unit8(float * in, int n, int8_t *out);

void flatten_bandpass(
        int nstep,
        int nchan,
        int npol,
        void *data);

void dec2hms( char *out, double in, int sflag );
void utc2mjd( char *, double *, double * ); // "2000-01-01T00:00:00" --> MJD_int + MJD_fraction
void mjd2lst( double, double * );

double parse_dec( char* ); // "01:23:45.67" --> Dec in degrees
double parse_ra( char* );  // "01:23:45.67" --> RA  in degrees

/**** MATRIX OPERATIONS ****/

void cp2x2(cuDoubleComplex *Min, cuDoubleComplex *Mout);
void inv2x2(cuDoubleComplex *Min, cuDoubleComplex *Mout);
void inv2x2d(double *Min, double *Mout);
void inv2x2S(cuDoubleComplex *Min, cuDoubleComplex **Mout);
void mult2x2d(cuDoubleComplex *M1, cuDoubleComplex *M2, cuDoubleComplex *Mout);
void mult2x2d_RxC(double *M1, cuDoubleComplex *M2, cuDoubleComplex *Mout);
void conj2x2(cuDoubleComplex *M, cuDoubleComplex *Mout);
double norm2x2(cuDoubleComplex *M, cuDoubleComplex *Mout);

void parse_pointing_file( const char *filename, double **ras_hours, double **decs_degs, unsigned int *npointings );

#endif
