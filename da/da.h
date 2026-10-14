/*
 * da - v0.0.1
 * 
 * Just for practice.
 *
 * */
#ifndef DA_h
#define DA_H

#include <stdbool.h>

typedef struct {
	void* items;
	size_t item_size;
	size_t size;
	size_t capacity;
	void (*free_func)(void*); // optional: item destructor function use for free inner pointer of item
} Da;


bool da_init(Da* da, size_t item_size, size_t cap, void (*free_func)(void*));
bool da_destroy(Da* da);
size_t da_len(const Da* da);
bool ensure_capacity(Da* da);
bool da_empty(const Da* da);
void* da_get(const Da* da, size_t idx);
bool da_set(Da* da, const void* item);
bool da_append(Da* da, const void* item);
void* da_pop(Da* da);
bool da_insert_at(Da* da, size_t idx, const void* item);
void* da_remove_at(Da* da, size_t idx);
bool da_unshift(Da* da, const void* item);
void* da_shift(Da* da);
void da_reverse(Da* da);
void da_error(const char* err_info);

#ifdef DA_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool da_init(Da* da, size_t item_size, size_t cap, void (*free_func)(void*))
{
	if (!da || item_size == 0 || cap == 0) {
		da_error("Invalid init parameter.");
		return false;
	}

	da->items = malloc(cap * item_size);
	if (da->items == NULL) {
		da_error("Memory allocation failed.");
		return false;
	}

	da->item_size = item_size;
	da->size = 0;
	da->capacity = cap != 0 ? cap : 6;
	da->free_func = free_func;
	return true;
}

bool da_destroy(Da* da)
{
	if (!da) return true;
	if (da->items != NULL && da->free_func) {
		for (size_t i = 0; i < da->size; i++) {
			da->free_func((char*)da->items + i * da->item_size);
		}
	}
	da->size = da->capacity = 0;
	free(da->items);
	da->items = NULL;
	return true;
}

size_t da_len(const Da* da)
{
	return da->size;
}

bool da_empty(const Da* da)
{
	if (!da) {
		da_error("Invalid parameter.");
		return true;
	}
	if (da->size > 0) {
		return false;
	}
	return true;
}

bool ensure_capacity(Da* da)
{
	if (da->size >= da->capacity) {
		da->capacity *= 2;
		da->items = realloc(da->items, da->capacity * da->item_size);
		if (!da->items) { da_error("Failed to expand capacity."); return false; }
	}
	return true;
}

bool da_insert_at(Da *da, size_t idx, const void *item)
{
	if (!da || !item) {
		da_error("Invalid parameter.");
		return false;
	}
	if (idx > da->size) {
		da_error("Index out of range.");
		return false;
	}
	if (!ensure_capacity(da)) return false;
	if (idx == da->size) {
		memcpy((char*)da->items + idx * da->item_size, item, da->item_size);
		da->size++;
		return true;
	}

	size_t move_bytes = (da->size - idx) * da->item_size;
	if (move_bytes > 0) {
		memmove(
			(char*)da->items + (idx + 1) * da->item_size,
			(char*)da->items + idx * da->item_size,
			move_bytes
		);
	}
	memcpy((char*)da->items + idx * da->item_size, item, da->item_size);

	da->size++;
	return true;
}

void* da_get(const Da* da, size_t idx)
{
	if (!da) {
		da_error("Invalid parameter.");
		return NULL;
	}
	if (da_empty(da)) return NULL;
	if (idx >= da->size) {
		da_error("Get index out of range");
		return NULL;
	}
	return (char*)da->items + idx * da->item_size;
}

void* da_remove_at(Da *da, size_t idx)
{
	void* item = da_get(da, idx);
	if (item == NULL) {
		da_error("Nothing to remove!");
		return NULL;
	}

	if (da->free_func) {
		da->free_func((char*)da->items + idx * da->item_size);
	}

	size_t move_bytes = (da->size - idx - 1) * da->item_size;
	if (move_bytes > 0) {
		memmove(
			(char*)da->items + idx * da->item_size,
			(char*)da->items + (idx + 1) * da->item_size,
			move_bytes
	   );
	}
	da->size--;
	return item;
}

bool da_append(Da* da, const void* item)
{
	return da_insert_at(da, da->size, item);
}

void* da_pop(Da* da)
{
	if (da_empty(da)) return NULL;
	return da_remove_at(da, da->size - 1);
}


bool da_unshift(Da* da, const void *item)
{
	return da_insert_at(da, 0, item);
}

void* da_shift(Da* da)
{
	return da_remove_at(da, 0);
}

void da_reverse(Da *da)
{
	if (!da || da->size <= 1) return;
    char small_buf[512];
    char* swap_buf;
    int  allocated = 0;
    if (da->item_size <= sizeof(small_buf)) {
        swap_buf = small_buf;
    } else {
        swap_buf = (char*)malloc(da->item_size);
        if (!swap_buf) {
			da_error("Malloc failed.");
            return;
        }
        allocated = 1;
    }

    char* base = (char*)da->items;
    size_t es = da->item_size;
    size_t left = 0;
    size_t right = da->size - 1;
    while (left < right) {
        char* a = base + left  * es;
        char* b = base + right * es;
        memcpy(swap_buf, a, es);
        memcpy(a, b, es);
        memcpy(b, swap_buf, es);
        left++;
        right--;
    }

    if (allocated) free(swap_buf);
}

void da_error(const char* err_info)
{
	fprintf(stderr, "[Da Error]: %s\n", err_info);
}

#endif // DA_IMPLEMENTATION
#endif // DA_H
