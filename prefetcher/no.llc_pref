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
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.6931297;
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) - 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.12456416;
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 2) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.16457348;
			break;
		case 1:
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.6931297;
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) - 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.12456416;
			break;
		case 2:
			pf_addr = ((addr >> LOG2_BLOCK_SIZE) + 1) << LOG2_BLOCK_SIZE;
			prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
			s2s_confidence_table[pf_addr>>LOG2_BLOCK_SIZE] = 0.6931297;
			break;
		default:
			break;
	}
	float perc = ((float)pf_useful/pf_issued);
	float perc1 = ((float)pf_useful/(pf_useful + pf_useless));
	// cout<<"PERC: "<<perc<<" PERC2: "<<perc1<<endl;
	cout<<"OUR MPKI: "<<sim_miss[0][LOAD]<<endl;
	if(perc>=0.50&&perc1>=0.80) which_pf = 0;
	else if(perc>=0.20&&perc1>=0.50) which_pf = 1;
	else which_pf = 2;
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
