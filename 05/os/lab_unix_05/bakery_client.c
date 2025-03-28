/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "bakery.h"


void
bakery_prog_1(char *host)
{
	CLIENT *clnt;
	struct BAKERY  *result_1;
	struct BAKERY  bakery_proc_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, BAKERY_PROG, BAKERY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	bakery_proc_1_arg.op = GET_NUM; 
	bakery_proc_1_arg.pid = getpid(); 

	result_1 = bakery_proc_1(&bakery_proc_1_arg, clnt); 
	if (result_1 == (struct BAKERY *) NULL) 
	{
		clnt_perror (clnt, "call failed");
	}

	printf("received number in queue: %d\n", result_1->num); 

	sleep(rand() % 3); 

	bakery_proc_1_arg.op = GET_DATA; 
	bakery_proc_1_arg.id = result_1->id; 
	bakery_proc_1_arg.pid = getpid(); 
	result_1 = bakery_proc_1(&bakery_proc_1_arg, clnt); 
	if (result_1 == (struct BAKERY *) NULL) 
	{
		clnt_perror (clnt, "call failed");
	}

	printf("received data from server: \"%c\"\n", result_1->res);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	bakery_prog_1 (host);
exit (0);
}
