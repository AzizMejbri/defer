#ifndef DEFER_H
#define DEFER_H


void  defer(void* ressource, void (*cleanup_function)(void*));
void  cleanup(void);
void* ds_realloc(void* old_ptr, unsigned long size);

#endif
