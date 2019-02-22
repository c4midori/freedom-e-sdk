#include <stdio.h>
#include <stdlib.h>

#include <metal/lock.h>

#define NUM_ITERATIONS 10

METAL_LOCK_DECLARE(my_lock);

int main(void);
int other_main();

/* This flag tells the secondary harts when to start to make sure
 * that they wait until the lock is initialized */
volatile int _start_other = 0;

/* The secondary_main() function can be redefined to start execution
 * on secondary harts. We redefine it here to cause harts with
 * hartid != 0 to execute other_main() */
int secondary_main(void) {
	int hartid = 0;
	__asm__ volatile("csrr %[hartid], mhartid"
			 : [hartid] "=r" (hartid) :: "memory");

	if(hartid == 0) {
		int rc = metal_lock_init(&my_lock);
		if(rc != 0) {
			printf("Failed to initialize my_lock: %d", rc);
			exit(1);
		}

		_start_other = 1;

		return main();
	} else {
		return other_main(hartid);
	}
}

int main(void) {
	int count = 0;

	while(1) {
		metal_lock_take(&my_lock);

		printf("Hart 0\n");
		fflush(stdout);

		metal_lock_give(&my_lock);

		count += 1;
		if(count == NUM_ITERATIONS) {
			return 0;
		}
	}
}

int other_main(int hartid) {
	while(!_start_other) ;	

	int count = 0;

	while(1) {
		metal_lock_take(&my_lock);

		printf("Hart %d\n", hartid);
		fflush(stdout);

		metal_lock_give(&my_lock);

		count += 1;
		if(count == NUM_ITERATIONS) {
			while(1);
		}

	}
}
