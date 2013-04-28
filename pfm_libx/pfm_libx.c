#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/prctl.h>
#include <err.h>

#include <perfmon/pfmlib_perf_event.h>
#include "perf_util.h"

#include "pfm_libx.h"

#define ENV_EVENTS "PFMX_EVENTS"

/* static const char *gen_events[]={ */
/* 	"PERF_COUNT_HW_CPU_CYCLES", */
/* 	"PERF_COUNT_HW_INSTRUCTIONS", */
/* 	NULL */
/* }; */

perf_event_desc_t *fds = NULL;
int num_fds = 0;

int init_pfm_counters(int *counters)
{
	int ret, i;
	char *events = NULL;
		
	/* get event names from environment variable */
	events = getenv(ENV_EVENTS);
	if(events == NULL){
		num_fds = 0;
		fds = NULL;
		return 1;
	}

	/*
	 * Initialize pfm library (required before we can use it)
	 */
	ret = pfm_initialize();
	if (ret != PFM_SUCCESS)
		errx(1, "Cannot initialize library: %s", pfm_strerror(ret));

	ret = perf_setup_list_events(events, &fds, &num_fds);
	if (ret || !num_fds)
		errx(1, "cannot setup events");

	fds[0].fd = -1;
	for(i=0; i < num_fds; i++) {
		/* request timing information necessary for scaling */
		fds[i].hw.read_format = PERF_FORMAT_SCALE;

		fds[i].hw.disabled = 1; /* do not start now */

		/* each event is in an independent group 
		   (multiplexing likely) */
		fds[i].fd = perf_event_open(&fds[i].hw, 0, -1, -1, 0);
		if (fds[i].fd == -1)
			err(1, "cannot open event %d", i);
	}
	
	if(counters !=  NULL)
		*counters = num_fds;

	return 0;
}
	

int start_pfm_counters()
{
	int ret;
	/*
	 * enable all counters attached to this thread and created by it
	 */
	ret = prctl(PR_TASK_PERF_EVENTS_ENABLE);
	if (ret)
		err(1, "prctl(enable) failed");

	return ret;
}

int stop_pfm_counters()
{
	int ret;
	/*
	 * disable all counters attached to this thread
	 */
	ret = prctl(PR_TASK_PERF_EVENTS_DISABLE);
	if (ret)
		err(1, "prctl(disable) failed");

	return ret;
}

int read_pfm_counters( uint64_t *data)
{
	int ret, i;
	uint64_t values[3];
	/*
	 * now read the results. We use pfp_event_count because
	 * libpfm guarantees that counters for the events always
	 * come first.
	 */
	memset(values, 0, sizeof(values));

	for (i=0; i < num_fds; i++) {
		ret = read(fds[i].fd, values, sizeof(values));
		if (ret < sizeof(values)) {
			if (ret == -1)
				err(1, "cannot read results: %s", 
				    strerror(errno));
			else
				warnx("could not read event%d", i);
		}
		/*
		 * scaling is systematic because we may be sharing the PMU and
		 * thus may be multiplexed
		 */
		fds[i].prev_value = fds[i].value;
		fds[i].value = perf_scale(values);
		fds[i].enabled = values[1];
		fds[i].running = values[2];

		/* set output parameters */
		if(data != NULL)
			data[i] =  fds[i].value - fds[i].prev_value;
		 

	}
	

	return 0;
}

int output_pfm_counters()
{
  int i;

  for(i=0; i < num_fds; i++) 
    {
      double ratio;
      uint64_t val;
      
      val = fds[i].value - fds[i].prev_value;
      
      ratio = 0.0;
      if (fds[i].enabled)
	ratio = 1.0 * fds[i].running / fds[i].enabled;
      
      /* separate groups */
      if (perf_is_group_leader(fds, i))
	putchar('\n');
      
      if (fds[i].value < fds[i].prev_value) 
	{
	  printf("inconsistent scaling %s (cur=%'"PRIu64" : "
		 "prev=%'"PRIu64")\n", 
		 fds[i].name, fds[i].value, fds[i].prev_value);
	  continue;
	}
      printf("%'20"PRIu64" %s (%.2f%% scaling, ena=%'"PRIu64","
	     "run=%'"PRIu64")\n",
	     val,
	     fds[i].name,
	     (1.0-ratio)*100.0,
	     fds[i].enabled,
	     fds[i].running);
    }

  return 0;
}


int pfm_cleanup()
{
	int i;

	for(i = 0; i < num_fds; i++){
		close(fds[i].fd);
	}
	free(fds);
	

	/* free libpfm resources cleanly */
	pfm_terminate();
	
	
	return 0;
}

