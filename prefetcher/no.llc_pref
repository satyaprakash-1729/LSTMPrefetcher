#include "cache.h"
#include <unordered_map>

unordered_map<uint64_t, double> s2s_confidence_table;


void CACHE::llc_prefetcher_initialize() 
{
}

uint32_t CACHE::llc_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in, int warmup)
{
	uint64_t pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 1) << LOG2_BLOCK_SIZE;
	prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
	s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.6931297;
	pf_addr = ((addr >> LOG2_BLOCK_SIZE) - 1) << LOG2_BLOCK_SIZE;
	prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
	s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.12456416;
	pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 2) << LOG2_BLOCK_SIZE;
	prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
	s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.16457348;
	// pf_addr = ((addr >> LOG2_BLOCK_SIZE) - 2) << LOG2_BLOCK_SIZE;
	// prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
	// s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.010326844;
  return metadata_in;
}

uint32_t CACHE::llc_prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in)
{
  return metadata_in;
}

void CACHE::llc_prefetcher_final_stats()
{
}
