#include <stdio.h>

#include <metal/memory.h>
#include <metal/itim.h>

METAL_PLACE_IN_ITIM
int itim_var;

int main() {
	int stack_var;

	struct metal_memory *mem;

	mem = metal_get_memory_from_address((uintptr_t) &itim_var);
	if(!mem) {
		return 1;
	}

	printf("itim_var\n");
	printf("--------\n");
	printf("address:  0x%08x\n", &itim_var);
	printf("mem base: 0x%08x\n", metal_memory_get_base_address(mem));
	printf("mem size: 0x%08x\n", metal_memory_get_size(mem));
	printf("\n");

	mem = metal_get_memory_from_address((uintptr_t) &stack_var);
	if(!mem) {
		return 2;
	}

	printf("stack_var\n");
	printf("---------\n");
	printf("address:  0x%08x\n", &stack_var);
	printf("mem base: 0x%08x\n", metal_memory_get_base_address(mem));
	printf("mem size: 0x%08x\n", metal_memory_get_size(mem));
	printf("\n");

	mem = metal_get_memory_from_address((uintptr_t) main);
	if(!mem) {
		return 2;
	}

	printf("main\n");
	printf("----\n");
	printf("address:  0x%08x\n", main);
	printf("mem base: 0x%08x\n", metal_memory_get_base_address(mem));
	printf("mem size: 0x%08x\n", metal_memory_get_size(mem));

	return 0;
}

