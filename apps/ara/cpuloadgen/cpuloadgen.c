/*
 *
 * @Component			CPULOADGEN
 * @Filename			cpuloadgen.c
 * @Description			Programmable CPU Load Generator,
 *					based on Dhrystone loops
 * @Author			Patrick Titiano (p-titiano@ti.com)
 * @Date			2010
 * @Copyright			Texas Instruments Incorporated
 *
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#include "dhry.h"

#define CPULOADGEN_REVISION ((const char *) "0.94")


#define CPULOADGEN_DEBUG
#ifdef CPULOADGEN_DEBUG
#define dprintf(format, ...)	 printf(format, ## __VA_ARGS__)
#else
#define dprintf(format, ...)
#endif


#ifndef ROPT
#define REG
#else
#define REG register
#endif

#define USEC_PER_SEC    ((uint64_t) 1000000)

/* Global Variables: */

Rec_Pointer Ptr_Glob, Next_Ptr_Glob;
int Int_Glob;
Boolean Bool_Glob;
char Ch_1_Glob, Ch_2_Glob;
int Arr_1_Glob [50];
int Arr_2_Glob [50][50];

char Reg_Define[] = "Register option selected.";

Enumeration Func_1();
/* forward declaration necessary since Enumeration may not simply be int */

int cpu_count = -1;
int *cpuloads = NULL;
long int duration = -1;
pthread_t *threads = NULL;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void dhryStone(unsigned int iterations);
int loadgen(unsigned int cpu, unsigned int load, unsigned int duration);

void Proc_1(Ptr_Val_Par);
void Proc_2(Int_Par_Ref);
void Proc_3(Ptr_Ref_Par);
void Proc_4(void);
void Proc_5(void);
extern void Proc_6(Enum_Val_Par, Enum_Ref_Par);
extern void Proc_7(Int_1_Par_Val, Int_2_Par_Val, Int_Par_Ref);
extern void Proc_8(Arr_1_Par_Ref, Arr_2_Par_Ref, Int_1_Par_Val, Int_2_Par_Val);
extern Enumeration Func_1(Ch_1_Par_Val, Ch_2_Par_Val);
extern Boolean Func_2(Str_1_Par_Ref, Str_2_Par_Ref);
extern Boolean Func_3(Enum_Par_Val);

/* ------------------------------------------------------------------------*//**
 * @FUNCTION		usage
 * @BRIEF		Display list of supported commands.
 * @DESCRIPTION		Display list of supported commands.
 *//*------------------------------------------------------------------------ */
static void usage(void)
{
	printf("Usage:\n");
	printf("\tcpuloadgen [<cpu[n]=load>] [<duration=time>]\n\n");
	printf("Generate adjustable processing load on selected CPU core(s) for a given duration.\n");
	printf("Load is a percentage which may be any integer value between 1 and 100.\n");
	printf("Duration time unit is seconds.\n");
	printf("Arguments may be provided in any order.\n");
	printf("If duration is omitted, generate load(s) endlessly.\n");
	printf("If no argument is given, generate 100%% load on all online CPU cores indefinitely.\n\n");
	printf("e.g.:\n");
	printf(" - Generate 100%% load on all online CPU cores until CTRL+C is pressed:\n");
	printf("	# cpuloadgen\n");
	printf(" - Generate 100%% load on all online CPU cores during 10 seconds:\n");
	printf("	# cpuloadgen duration=10\n");
	printf(" - Generate 50%% load on CPU1 and 100%% load on CPU3 during 10 seconds:\n");
	printf("	# cpuloadgen cpu3=100 cpu1=50 duration=5\n\n");
}


/* ------------------------------------------------------------------------*//**
 * @FUNCTION		free_buffers
 * @BRIEF		free allocated buffers.
 * @DESCRIPTION		free allocated buffers.
 *//*------------------------------------------------------------------------ */
static void free_buffers(void)
{
	if (threads != NULL)
		free(threads);
	if (cpuloads != NULL)
		free(cpuloads);
}


/* ------------------------------------------------------------------------*//**
 * @FUNCTION		einval
 * @BRIEF		Display standard message in case of invalid argument.
 * @RETURNS		-EINVAL
 * @param[in]		arg: invalid argument
 * @DESCRIPTION		Display standard message in case of invalid argument.
 *			Take care of freeing allocated buffers
 *//*------------------------------------------------------------------------ */
static int einval(const char *arg)
{
	fprintf(stderr, "cpuloadgen: invalid argument!!! (%s)\n\n", arg);
	usage();
	free_buffers();
	return -EINVAL;
}


/* ------------------------------------------------------------------------*//**
 * @FUNCTION		sigterm_handler
 * @BRIEF		parent SIGTERM callback function.
 *			Send SIGTERM signal to child process.
 * @DESCRIPTION		parent SIGTERM callback function.
 *			Send SIGTERM signal to child process.
 *//*------------------------------------------------------------------------ */
void sigterm_handler(void)
{
	printf("Halting load generation...\n");
	fflush(stdout);

	free_buffers();

	printf("done.\n\n");
	fflush(stdout);
}


/* ------------------------------------------------------------------------*//**
 * @FUNCTION		thread_loadgen
 * @BRIEF		pthread wrapper around loadgen() function.
 * @param[in]		ptr: pointer to the cpu core id
 * @DESCRIPTION		pthread wrapper around loadgen() function.
 *//*------------------------------------------------------------------------ */
void *thread_loadgen(void *ptr)
{
	unsigned int cpu;

	cpu = *((unsigned int *) ptr);
	pthread_mutex_unlock(&mutex1);
	if (cpu < cpu_count) {
		loadgen(cpu, cpuloads[cpu], duration);
	} else {
		fprintf(stderr, "%s: invalid cpu argument!!! (%d)\n",
			__func__, cpu);
	}

	pthread_exit(NULL);
}


/* ------------------------------------------------------------------------*//**
 * @FUNCTION		main
 * @BRIEF		main entry point
 * @RETURNS		0 on success
 *			-EINVAL in case of invalid argument
 *			-ECHILD in case of failure to fork
 * @param[in, out]	argc: shell input argument number
 * @param[in, out]	argv: shell input argument(s)
 * @DESCRIPTION		main entry point
 *//*------------------------------------------------------------------------ */
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int cpuloadgen_main(int argc, char *argv[])
#endif
{
	int i, ret, load;
	long int duration2;

	cpu_count = 1;
	dprintf("main: found %d CPU cores.\n", cpu_count);
	if (argc > cpu_count + 2) {
		fprintf(stderr, "cpuloadgen: too many arguments!\n\n");
		usage();
		return -EINVAL;
	}

	/* Allocate buffers */
	threads = malloc(cpu_count * sizeof(pthread_t));
	cpuloads = malloc(cpu_count * sizeof(int));
	if ((threads == NULL) || (cpuloads == NULL)) {
		fprintf(stderr, "cpuloadgen: could not allocate buffers!!!\n");
		return -ENOMEM;
	}

	/* Initialize variables */
	for (i = 0; i < cpu_count; i++) {
		threads[i] = -1;
		cpuloads[i] = 100;
	}
	duration = -1;

	/* Parse user arguments */
	if (argc >= 2) {
		dprintf("main: argv[1]=%s\n", argv[1]);
		ret = sscanf(argv[i], "%d", &load);
		if ((ret != 1) ||
			(load < 1) || (load > 100)) {
			return einval(argv[0]);
		}
		cpuloads[0] = load;
		dprintf("Load assigned to CPU0: %d%%\n", cpuloads[0]);
	}
	if (argc == 3) {
		dprintf("main: argv[2]=%s\n", argv[2]);
		ret = sscanf(argv[2], "%ld", &duration2);
		if ((ret != 1) || (duration2 < 1)) {
			return einval(argv[2]);
		}
		duration = duration2;
		dprintf("Duration of the load generation: %lds\n", duration);
	}

	/* Start load generation on cores accordingly */
	for (i = 0; i < cpu_count; i++) {
		if (cpuloads[i] == -1) {
			dprintf("main: no load to be generated on CPU%d\n", i);
			continue;
		}
		/*
		 * Why is a mutex needed here?
		 * There is a race condition between this loop which updates
		 * variable i and thread_loadgen() which also reads it.
		 * Hence locking a mutex here, and unlocking it in
		 * thread_loadgen() after the value was retrieved and saved.
		 */
		pthread_mutex_lock(&mutex1);
		ret = pthread_create(&threads[i], NULL, thread_loadgen, &i);
		if (ret != 0) {
			fprintf(stderr, "cpuloadgen: failed to fork %d! (%d)",
			i, ret);
			continue;
		}
	}

	for (i = 0; i < cpu_count; i++) {
		if (cpuloads[i] == -1) {
			continue;
		}
		pthread_join(threads[i], NULL);
	}

	free_buffers();

	printf("\ndone.\n\n");
	return 0;
}


/* ------------------------------------------------------------------------*//**
 * @FUNCTION		loadgen
 * @BRIEF           Programmable CPU load generator
 * @RETURNS         0, -EINVAL otherwise
 * @param[in]		cpu: target CPU core ID (loaded CPU core)
 * @param[in]		load: load to generate on that CPU ([1-100])
 * @param[in]		duration: how long this CPU core shall be loaded
 *                  (in seconds)
 * @DESCRIPTION		Programmable CPU load generator. Use Dhrystone loops
 *                  to generate load, and apply PWM (Pulse Width Modulation)
 *                  principle on it to make average CPU load vary between
 *                  0 and 100%
 *//*------------------------------------------------------------------------ */
int loadgen(unsigned int cpu, unsigned int load, unsigned int duration)
{
	struct timeval tv_loadgen_start, tv_loadgen_now, tv_loadgen_time;
	struct timeval tv_dhry_start, tv_dhry_end, tv_dhry_time;
    uint64_t dhry_time_usec, total_time_usec, idle_time_usec;
    #ifdef CPULOADGEN_DEBUG
    struct timeval tv_idle_start, tv_idle_end, tv_idle_time;
    unsigned int real_cpu_load;
    uint64_t real_idle_time_usec;
    #endif

    if ((!load) || (!duration)) {
        return -EINVAL;
    }

	printf("Generating %3u%% load on CPU%u...\n", load, cpu);
	gettimeofday(&tv_loadgen_start, NULL);
	dprintf("%s(): CPU%u start time: %luus %luus\n", __func__,
		cpu, tv_loadgen_start.tv_sec, tv_loadgen_start.tv_usec);


	if (load != 100) {
		while (1) {
			/* Generate load (100%) */
			gettimeofday(&tv_dhry_start, NULL);
            printf("1\n");
            fflush(stdout);
			dhryStone(10);
            printf("2\n");
            fflush(stdout);
			gettimeofday(&tv_dhry_end, NULL);

            timersub(&tv_dhry_end, &tv_dhry_start, &tv_dhry_time);
            dhry_time_usec = tv_dhry_time.tv_usec;
            dhry_time_usec += tv_dhry_time.tv_sec * USEC_PER_SEC;
			dprintf("%s(): CPU%u dhrystone run time: %luus\n", __func__,
                    cpu, dhry_time_usec);

			/* Compute needed idle time */
            total_time_usec = dhry_time_usec;
            total_time_usec *= (100 * 100) / (load + 1);
            total_time_usec /= 100;
			dprintf("%s(): CPU%u total time: %luus\n", __func__, cpu,
				    total_time_usec);
			idle_time_usec = total_time_usec - dhry_time_usec;
			dprintf("%s(): CPU%u computed idle time: %luus\n",
                    __func__, cpu, idle_time_usec);

			/* Generate idle time */
			#ifdef CPULOADGEN_DEBUG
			gettimeofday(&tv_idle_start, NULL);
			#endif
			usleep(idle_time_usec);
			#ifdef CPULOADGEN_DEBUG
			gettimeofday(&tv_idle_end, NULL);
            timersub(&tv_idle_end, &tv_idle_start, &tv_idle_time);
            real_idle_time_usec = tv_idle_time.tv_usec;
            real_idle_time_usec += tv_idle_time.tv_sec * USEC_PER_SEC;
			dprintf("%s(): CPU%u effective idle time: %luus\n",
				    __func__, cpu, real_idle_time_usec);
            real_cpu_load = (100 * dhry_time_usec);
            real_cpu_load /= (dhry_time_usec + real_idle_time_usec);
			dprintf("%s(): CPU%u effective CPU Load: %u%%\n",
				    __func__, cpu, real_cpu_load);
            if (real_cpu_load != load) {
                printf("Warning: CPU%u: generated %u%% load instead of %u%%\n",
                       cpu, real_cpu_load, load);
            }
			#endif

            gettimeofday(&tv_loadgen_now, NULL);
            timersub(&tv_loadgen_now, &tv_loadgen_start, &tv_loadgen_time);
            #ifdef CPULOADGEN_DEBUG
            dprintf("%s(): CPU%u elapsed time: %lus %luus\n", __func__,
                    cpu, tv_loadgen_time.tv_sec, tv_loadgen_time.tv_usec);
            #endif
            if (tv_loadgen_time.tv_sec >= duration) {
                break;
            }
		}
	} else {
            printf("la\n");
            return 0;
		while (1) {
			dhryStone(1000000);
			gettimeofday(&tv_loadgen_now, NULL);
			timersub(&tv_loadgen_now, &tv_loadgen_start, &tv_loadgen_time);
            #ifdef CPULOADGEN_DEBUG
            dprintf("%s(): CPU%u elapsed time: %lus %luus\n", __func__,
                    cpu, tv_loadgen_time.tv_sec, tv_loadgen_time.tv_usec);
            #endif
            if (tv_loadgen_time.tv_sec >= duration) {
                break;
            }
		}
	}

	dprintf("Load Generation on CPU%u completed.\n", cpu);
    return 0;
}


void dhryStone(unsigned int iterations)
{
	One_Fifty Int_1_Loc;
	REG One_Fifty Int_2_Loc;
	One_Fifty Int_3_Loc;
	REG char Ch_Index;
	Enumeration Enum_Loc;
	Str_30 Str_1_Loc;
	Str_30 Str_2_Loc;
	REG int Run_Index;
	REG int Number_Of_Runs;

	Next_Ptr_Glob = (Rec_Pointer) malloc(sizeof(Rec_Type));
	Ptr_Glob = (Rec_Pointer) malloc(sizeof (Rec_Type));

	Ptr_Glob->Ptr_Comp = Next_Ptr_Glob;
	Ptr_Glob->Discr = Ident_1;
	Ptr_Glob->variant.var_1.Enum_Comp = Ident_3;
	Ptr_Glob->variant.var_1.Int_Comp = 40;
	strcpy (Ptr_Glob->variant.var_1.Str_Comp,
	"DHRYSTONE PROGRAM, SOME STRING");
	strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

	Arr_2_Glob [8][7] = 10;
    /* Was missing in published program. Without this statement,
    Arr_2_Glob [8][7] would have an undefined value.
    Warning: With 16-Bit processors and Number_Of_Runs > 32000,
    overflow may occur for this array element. */

    Number_Of_Runs = iterations;
	for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index) {
		Proc_5();
		Proc_4();
		/* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
		Int_1_Loc = 2;
		Int_2_Loc = 3;
		strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
		Enum_Loc = Ident_2;
		Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
		/* Bool_Glob == 1 */
		while (Int_1_Loc < Int_2_Loc) { /* loop body executed once */
			Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
			/* Int_3_Loc == 7 */
			Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
			/* Int_3_Loc == 7 */
			Int_1_Loc += 1;
		}
		/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
		Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
		/* Int_Glob == 5 */
		Proc_1 (Ptr_Glob);
		for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index) {
		/* loop body executed twice */
			if (Enum_Loc == Func_1 (Ch_Index, 'C')) {
				/* then, not executed */
				Proc_6 (Ident_1, &Enum_Loc);
				strcpy (Str_2_Loc,
					"DHRYSTONE PROGRAM, 3'RD STRING");
				Int_2_Loc = Run_Index;
				Int_Glob = Run_Index;
			}
		}
		/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
		Int_2_Loc = Int_2_Loc * Int_1_Loc;
		Int_1_Loc = Int_2_Loc / Int_3_Loc;
		Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
		/* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
		Proc_2 (&Int_1_Loc);
		/* Int_1_Loc == 5 */
	}
}


void Proc_1(Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10,
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */

void Proc_2(Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3(Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4(void) /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5(void) /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */
