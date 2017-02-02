#include "cachesim.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
/**
 * Subroutint for initializing your cache with the paramters.
 * You may add and initialize any global variables that you might need
 *
 * @param C The total size of your cache is 2^size bytes
 * @param S The total number of sets in your cache are 2^S
 * @param B The size of your block is 2^B bytes
 */

typedef struct{
    int valid;
    uint64_t tag;
    uint64_t LRU;
    int dirty;
} block;

typedef struct 
{
    uint64_t C;
    uint64_t S;
    uint64_t B;
}info;




block **cache;
info *inf;
uint64_t counter = 0;

void cache_init(uint64_t C, uint64_t S, uint64_t B) {


int block_num = C-B;
int set_number = pow(2,C-B-S);
int block_per_set = pow(2,S);
cache = malloc(set_number*sizeof(block *));
for (int i = 0; i < set_number; i=i+1)
{
    cache[i] = malloc (block_per_set*sizeof(block));
}

for (int i = 0; i < set_number; ++i)
{
    for (int j = 0; j < block_per_set; ++j)
    {
        cache[i][j].valid=0;
        cache[i][j].dirty=0;
    }
}
inf = malloc(sizeof(info));
inf->C=C;
inf->S=S;
inf->B=B;
   
}


uint64_t offsetbit(info *inf){
    return inf->B;
}

uint64_t indexbit(info *inf){
    return inf->C-inf->B-inf->S;
}

uint64_t tagbit(info *inf){
    return 64-offsetbit(inf)-indexbit(inf);
}




uint64_t getoffset(uint64_t address,info *inf){
return 1;

}

uint64_t getindex(uint64_t address,info *inf){
    return  (address<<(tagbit(inf)))>> (tagbit(inf)+offsetbit(inf));
    
}

uint64_t gettag(uint64_t address,info *inf){
return address>>(offsetbit(inf)+indexbit(inf));
}



block* getblock(uint64_t address,block **cache,info *inf){
uint64_t index = getindex(address,inf);
uint64_t tag = gettag(address,inf);

for (int i = 0; i < pow(2,inf->S); i=i+ 1)
{ 
   
    if (cache[index][i].tag == tag && cache[index][i].valid == 1)
    {   
        counter = counter +1;
        cache[index][i].LRU=counter;
        return &(cache[index][i]);
    }

}
return NULL;
}


block* find_invalid(uint64_t address,block **cache,info *inf) {
   uint64_t index = getindex(address,inf);
uint64_t tag = gettag(address,inf);

for (int i = 0; i < pow(2,inf->S); i=i+1)
{   
    if (cache[index][i].valid==0)
    {
       counter = counter +1;
        cache[index][i].LRU=counter;
        return &(cache[index][i]);
    }
}
return NULL;
}

block* find_lru(uint64_t address,block **cache,info *inf){
    uint64_t index = getindex(address,inf);
    uint64_t choose= 0;
for (int i = 0; i < pow(2,inf->S); ++i)
{   
    if (cache[index][choose].LRU>cache[index][i].LRU)
    {

        choose = i;
    }
}
 counter = counter +1;
        cache[index][choose].LRU=counter;
        return &(cache[index][choose]);

}


/**
 * Subroutine that simulates one cache event at a time.
 * @param rw The type of access, READ or WRITE
 * @param address The address that is being accessed
 * @param stats The struct that you are supposed to store the stats in
 */
void cache_access (char rw, uint64_t address, struct cache_stats_t *stats) {

if (rw=='r')
{
     block* find_block;
     uint64_t tag=gettag(address,inf);
     uint64_t index=getindex(address,inf);
     stats->accesses++;
     stats->reads++;


   find_block = getblock(address,cache,inf);
   if (find_block==NULL)
   {
        find_block=find_invalid(address,cache,inf);
     if (find_block==NULL)
        {
            find_block = find_lru(address,cache,inf);

            if (find_block->dirty==1)
            {
                stats->write_backs++;

            }

        }

        find_block->valid =1;
        find_block->dirty =0;
        find_block->tag=tag;

        stats->read_misses++;
    }
}

if (rw=='w')
{
    uint64_t tag=gettag(address,inf);
    uint64_t index=getindex(address,inf);
    stats->accesses++;
    stats->writes++;
    block *find_block;
    find_block = getblock(address,cache,inf);
    if (find_block==NULL)
    {
        find_block = find_invalid(address,cache,inf);
        if (find_block==NULL)
        {
            find_block = find_lru(address,cache,inf);
            if (find_block->dirty==1)
            {
                stats->write_backs++;
            }

        }

        find_block->valid =1;
        find_block->dirty=1;
        find_block->tag=tag;
        stats->write_misses++;
        /* code */
    }
    find_block->dirty=1;
}

}

/**
 * Subroutine for cleaning up memory operations and doing any calculations
 *
 */
void cache_cleanup (struct cache_stats_t *stats) {
stats->misses = stats->read_misses+stats->write_misses;
stats->miss_rate= (double)(stats->misses)/(stats->accesses);
stats->avg_access_time= stats->access_time + (stats->miss_rate)*100;
free(cache);
free(inf);
}