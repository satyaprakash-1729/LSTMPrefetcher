#include "cache.h"
#include <unordered_map>

unordered_map<uint64_t, double> s2s_confidence_table;


void CACHE::llc_prefetcher_initialize() 
{
}

uint32_t CACHE::llc_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in, int warmup)
{
	uint64_t pf_addr;
	switch(which_pf){
		case 0:
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.9814297;
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 2) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.00616416;
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) - 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.00657348;
			break;
		case 1:
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.9814297;
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 2) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.00616416;
			break;
		case 2:
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.9814297;
			break;
		default:
			break;
	}
	float perc = ((float)pf_useful/pf_issued);
	float perc1 = ((float)pf_useful/(pf_useful + pf_useless));
	if(perc>=0.50) which_pf = 0;
	else if(perc>=0.20) which_pf = 1;
	else which_pf = 2;
  return metadata_in;
}

uint32_t CACHE::llc_prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in)
{
  return metadata_in;
}

void CACHE::llc_prefetcher_final_stats()
{
}
