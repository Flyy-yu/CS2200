/**************************************
****    DO NOT MODIFY THIS FILE    ****
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cachesim.h"

void print_help_and_exit(void) {
    printf("cachesim [OPTIONS] < traces/file.trace\n");
    printf("  -c C\t\tTotal size in bytes is 2^C\n");
    printf("  -b B\t\tSize of each block in bytes is 2^B\n");
    printf("  -s S\t\tNumber of blocks per set is 2^S\n");
    printf("  -h\t\tThis helpful output\n");
    exit(0);
}

void print_statistics(struct cache_stats_t* p_stats);

int main(int argc, char* argv[]) {
    int opt;
    uint64_t c = DEFAULT_C;
    uint64_t b = DEFAULT_B;
    uint64_t s = DEFAULT_S;
    FILE* fin  = stdin;

    /* Read arguments */ 
    while(-1 != (opt = getopt(argc, argv, "c:b:s:i:h"))) {
        switch(opt) {
        case 'c':
            c = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'i':
            fin = fopen(optarg, "r");
            break;
        case 'h':
        default:
            print_help_and_exit();
            break;
        }
    }

    printf("Cache Settings\n");
    printf("C: %" PRIu64 "\n", c);
    printf("B: %" PRIu64 "\n", b);
    printf("S: %" PRIu64 "\n", s);
    printf("\n");

    /* Setup the cache */
    cache_init(c, s, b);

    /* Setup statistics */
    struct cache_stats_t stats;
    memset(&stats, 0, sizeof(struct cache_stats_t));
    stats.miss_penalty = 100;
    stats.access_time = 2;

    /* Begin reading the file */ 
    char rw;
    uint64_t address;
    while (!feof(fin)) { 
        int ret = fscanf(fin, "%c %" PRIx64 "\n", &rw, &address); 
        if(ret == 2) {
            cache_access(rw, address, &stats); 
        }
    }

    cache_cleanup(&stats);

    print_statistics(&stats);
    fclose(fin);
    return 0;
}

void print_statistics(struct cache_stats_t* p_stats) {
    printf("Cache Statistics\n");
    printf("Accesses: %" PRIu64 "\n", p_stats->accesses);
    printf("Reads: %" PRIu64 "\n", p_stats->reads);
    printf("Read misses: %" PRIu64 "\n", p_stats->read_misses);
    printf("Writes: %" PRIu64 "\n", p_stats->writes);
    printf("Write misses: %" PRIu64 "\n", p_stats->write_misses);
    printf("Misses: %" PRIu64 "\n", p_stats->misses);
    printf("Writebacks: %" PRIu64 "\n", p_stats->write_backs);
    printf("Access Time: %" PRIu64 "\n", p_stats->access_time);
    printf("Miss Penalty: %" PRIu64 "\n", p_stats->miss_penalty);
    printf("Miss rate: %f\n", p_stats->miss_rate);
    printf("Average access time (AAT): %f\n", p_stats->avg_access_time);
}
