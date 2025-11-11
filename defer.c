
#include "defer.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFER_MAX 254
#define EMPTINESS 255

struct defer_stack{
    void  *ressources[DEFER_MAX]            ;      // an array of void pointers (array of ressources)
    void (*cleanup_funcs[DEFER_MAX])(void*);      // an array of function pointers (array of cleanup functions)
};

static struct defer_stack ds;
static unsigned char peak_ptr = EMPTINESS;

static inline unsigned char ds_empty() { return peak_ptr == EMPTINESS; }

void defer(void* ressource, void (*cleanup_func) (void*)){
    if ( ds_empty() ) peak_ptr = 0;
    ds.ressources[peak_ptr] = ressource;
    ds.cleanup_funcs[peak_ptr] = cleanup_func;
    peak_ptr++;
    if(peak_ptr >= DEFER_MAX){
        fprintf(stderr, "\e[31mDefer Stack Overflow: Too many cleanups scheduled ...\e[m\n");
        cleanup();
        exit(EXIT_FAILURE);
    } 
}

void cleanup(){
    if ( ds_empty() ) return;
    peak_ptr--;
    while(peak_ptr != 0){
        ds.cleanup_funcs[peak_ptr](ds.ressources[peak_ptr]);
        ds.ressources[peak_ptr] = ds.cleanup_funcs[peak_ptr] = NULL;
        peak_ptr--;
    } 
    (*ds.cleanup_funcs)(*ds.ressources);
    peak_ptr = EMPTINESS;
}

// an alternative to realloc (or rather a wrapper) that is defer safe (hence ds)
void* ds_realloc(void* old_ptr, unsigned long size){
    void* new_ptr = realloc(old_ptr, size);
    if ( new_ptr == NULL){
        if ( ds_empty() ) exit(EXIT_FAILURE);
        peak_ptr--;
        while(peak_ptr != 0){
            if ( ds.ressources[peak_ptr] != old_ptr )
                ds.cleanup_funcs[peak_ptr](ds.ressources[peak_ptr]);
            peak_ptr--;
        }
        if ( *ds.ressources != old_ptr)
            (*ds.cleanup_funcs)(*ds.ressources);
        peak_ptr = EMPTINESS;
        fprintf(stderr, "realloc failed, exiting ...");
        exit(EXIT_FAILURE);
    }
    if ( ds_empty() ) goto ret_statement; 
    for(unsigned char i = 0; i < peak_ptr ; i++)
        if ( ds.ressources[i] == old_ptr  )
            ds.ressources[i] = new_ptr;
ret_statement:   
    return new_ptr; 
}

