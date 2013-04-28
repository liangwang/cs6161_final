/* 
 * A quick library for self-reading of performance counters.
 * 
 * Set environment variable PFMX_EVENTS to the names of the counters to read.
 * usage:
 *      init_pfm_counters();
 *      start_pfm_counters();
 *      
 *      {
 *       do some work;
 *       read_pfm_counters(); 
 *       output_pfm_counters(); //optional if you want default output layout
 *      }
 *     
 *     stop_pfm_counters();
 *     pfm_cleanup();
 *
 * Author: Wei Wang <wwang@virginia.edu>
 */

#ifndef __PFM_LIBX__
#define __PFM_LIBX__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialized performance counters
 * Returns 0 if successful
 * int *counters returns the number of counters to read
 */
int init_pfm_counters(int *counters);

int start_pfm_counters();

int stop_pfm_counters();

/*
 * Read counters. 
 * Output parameter *data, if set, will contain the reading between
 * the previous call to read_pfm_counters, and current call to 
 * read_pfm_counters.
 */
int read_pfm_counters(uint64_t *data);

/*
 * print the pfm using default format
 */
int output_pfm_counters();
	
int pfm_cleanup();

#ifdef __cplusplus
}
#endif

#endif
