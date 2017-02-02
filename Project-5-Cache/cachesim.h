/**********************************************
****** 		DO NOT MODIFY THIS FILE		*******
***********************************************/

#ifndef CACHESIM_H
#define CACHESIM_H

#include <inttypes.h>

struct cache_stats_t {
    uint64_t accesses;
    uint64_t reads;
    uint64_t read_misses;
    uint64_t writes;
    uint64_t write_misses;
    uint64_t misses;
	uint64_t write_backs;
   
	uint64_t access_time;
    uint64_t miss_penalty;
    double   miss_rate;
    double   avg_access_time;
};

void cache_init(uint64_t C, uint64_t S, uint64_t B);
void cache_access (char rw, uint64_t address, struct cache_stats_t *stats);
void cache_cleanup (struct cache_stats_t *stats);

static const uint64_t DEFAULT_C = 15;   /* 32KB Cache */
static const uint64_t DEFAULT_B = 5;    /* 32-byte blocks */
static const uint64_t DEFAULT_S = 3;    /* 8 blocks per set */

static const char READ = 'r';
static const char WRITE = 'w';

#endif