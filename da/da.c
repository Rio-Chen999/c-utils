#include <stdio.h>
#define DA_IMPLEMENTATION
#include "da.h"

void print_da(Da *arr) 
{
	size_t sz = da_len(arr);
	for (int i = 0; i < sz; i++) {
		printf("%s, ", (char*)da_get(arr, i));
	}
}

void print_da_format(Da *arr) 
{
	print_da(arr);
	printf("\n");
	printf("size = %zu\n", da_len(arr));
	printf("capacity = %zu\n", arr->capacity);
}

int double_n(int a)
{
	return a * 2;
}

int main(void) 
{
	Da arr;
	da_init(&arr, sizeof(char*), 10, NULL);
	da_append(&arr, "2");
	da_append(&arr, "1");
	print_da_format(&arr);
	da_append(&arr, "12");
	da_append(&arr, "21");
	da_append(&arr, "7");
	da_append(&arr, "8");
	da_append(&arr, "100");
	print_da_format(&arr);
	da_insert_at(&arr, 2, "3");
	da_insert_at(&arr, 0, "9");
	print_da_format(&arr);
	da_remove_at(&arr, 5);
	print_da_format(&arr);
	da_append(&arr, "566");
	da_append(&arr, "77");
	print_da_format(&arr);
	da_append(&arr, "78");
	print_da_format(&arr);
	da_reverse(&arr);
	print_da_format(&arr);
	// da_map(&arr, double_n);
	// print_da_format(&arr);
	// printf("pop: %s\n", (char*)da_pop(&arr));
	// print_da_format(&arr);
	// da_unshift(&arr, "555");
	// da_unshift(&arr, "666");
	// print_da_format(&arr);
	// da_shift(&arr);
	// print_da_format(&arr);

	da_destroy(&arr);
	print_da_format(&arr);

	return 0;
}
