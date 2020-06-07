#include "cache.h"
#include "vector"
#include <fstream>

static std::vector<std::string> ips;
static std::vector<std::string> addrs;

std::ofstream fout;

void CACHE::llc_prefetcher_initialize() 
{
	fout.open("/usr/eda/cs259/prakashsatya/data_final.csv");
	fout << "ADDRS\tPCS\n";
}

uint32_t CACHE::llc_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in, int warmup)
{
	try{
	  // if (type == LOAD){
	  	ips.push_back(std::to_string(ip));
	  	// uint64_t lastaddr;
	  	// if(addrs.size())
	  	// 	lastaddr = (uint64_t)stol(addrs.back());
	  	addrs.push_back(std::to_string(addr));
	  	if(ips.size()==11){
	  		// if(addr>>LOG2_PAGE_SIZE == lastaddr>>LOG2_PAGE_SIZE){
		  		std::string addrc = std::accumulate(std::begin(addrs), std::end(addrs), string(),
		                                [](string &ss, string &s)
		                                {
		                                    return ss.empty() ? s : ss + "," + s;
		                                });
			  	std::string ipc = std::accumulate(std::begin(ips), std::end(ips), string(),
			                                [](string &ss, string &s)
			                                {
			                                    return ss.empty() ? s : ss + "," + s;
			                                });
			  	// fout<<addrc<<"\t"<<ipc<<endl;
							  	if(warmup){
							  		std::string cmd = "python3 prefetch_predictor.py "+ ipc +" "+ addrc +" 4 " + std::to_string(LOG2_PAGE_SIZE) + " 0";
									char buffer[128];
								    std::string result = "";
								    FILE* pipe = popen(cmd.c_str(), "r");
								    if (!pipe) throw std::runtime_error("popen() failed!");
								    try {
								        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
								            result += buffer;
								        }
								    } catch (...) {
								        pclose(pipe);
								        throw;
								    }
								    std::cout<<"---"<<result<<endl;
								    size_t pos = 0;
									std::string token;
									std::string delimiter = ", ";
									result = result.substr(1, result.length()-2);
									while ((pos = result.find(delimiter)) != std::string::npos) {
									    token = result.substr(0, pos);
									    uint64_t pf_addr = (((addr+stol(token))>>LOG2_BLOCK_SIZE)) << LOG2_BLOCK_SIZE;
									    prefetch_line(ip, addr, pf_addr, FILL_LLC, 0);
									    result.erase(0, pos + delimiter.length());
									}

								    pclose(pipe);
								}else{
									std::string cmd = "python3 prefetch_predictor.py "+ ipc +" "+ addrc +" 4 " + std::to_string(LOG2_PAGE_SIZE) + " 1";
									char buffer[128];
								    std::string result = "";
								    FILE* pipe = popen(cmd.c_str(), "r");
								    if (!pipe) throw std::runtime_error("popen() failed!");
								    try {
								        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
								            result += buffer;
								        }
								    } catch (...) {
								        pclose(pipe);
								        throw;
								    }
								    std::cout<<"---"<<result<<endl;
									pclose(pipe);
								}
			  // }
			ips.erase(ips.begin());
			addrs.erase(addrs.begin());
	  	}
	  // }
	}catch(...){
		std::cerr<<"Error caught!\n";
	}
  return metadata_in;
}

uint32_t CACHE::llc_prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in)
{
  return metadata_in;
}

void CACHE::llc_prefetcher_final_stats()
{
	fout.close();
}
