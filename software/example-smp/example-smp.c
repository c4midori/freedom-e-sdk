#include <stdio.h>
#include <stdlib.h>

#include <metal/lock.h>

#define NUM_ITERATIONS 10

METAL_LOCK_DECLARE(my_lock);

int main(void);
int secondary_main();

volatile int _start = 0;

int slave_main(void) {
	int hartid = 0;
	__asm__ volatile("csrr %[hartid], mhartid"
			 : [hartid] "=r" (hartid) :: "memory");
	if(hartid == 0) {
		if(metal_lock_init(&my_lock) != 0) {
			exit(1);
		}

		_start = 1;

		return main();
	} else if (hartid == 1){
		return secondary_main();
	} else {
		while(1) {
			__asm__("wfi");
		}
	}
}

int main(void) {
	int count = 0;

	while(1) {
		metal_lock_take(&my_lock);

		printf("Hart 0\n");
		fflush(stdout);

		count += 1;
		if(count == NUM_ITERATIONS) {
			exit(0);
		}

		metal_lock_give(&my_lock);
	}
}

int secondary_main() {
	while(!_start) ;	

	while(1) {
		metal_lock_take(&my_lock);

		printf("Hart 1\n");
		fflush(stdout);

		metal_lock_give(&my_lock);
	}
}
