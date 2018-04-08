#include<omp.h>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
#include<ctime>
#include<sys/ioctl.h>
#include<unistd.h>
#include <linux/perf_event.h>
//#include <linux/hw_breakpoint.h>
#include<stdint.h>
#include<sys/syscall.h>
#include<cstring>
#include<errno.h>
using namespace std;
#define N 10
#define M 1.0f
#define R 0.5f
#define G 6.67*1e-11f
#define XMAX 100.0f
#define YMAX 200.0f
#define ZMAX 400.0f
#define SEED 2
#define dt 0.005f
#define RUN 60*60
#define NUM_THREADS 48

#define CYCLES 0
#define CACHE_REF 1
#define CACHE_MIS 2
#define L1D_R_AC 3
#define L1D_R_MIS 4
#define PERF_GROUP1 0
#define PERF_GROUP2 3
#define ACTIVE_GROUP 3

#define DEBUG(fd,err) if(fd==-1){ \
cerr<<"Unable to open "<<err<<". Exiting."; \
exit(EXIT_FAILURE); \
}

#define CACHE_LINE_SIZE 64

typedef struct triplet{
	double first;
	double second;
	double third;
	triplet():first(0.0f),second(0.0f),third(0.0f){}
	triplet(double f,double s,double t): first(f),second(s),third(t){}
}triplet;

struct read_format{
	uint64_t nr;            /* The number of events */
        struct{
        	uint64_t value;     /* The value of the event */
                uint64_t id;        /* if PERF_FORMAT_ID */
        }values[];
};
void printStatToFile(struct read_format*,ofstream&,vector<uint64_t>&);
vector<int> init_perf(vector<uint64_t>&);
void printToFile(vector<triplet>&,ofstream&);
ostream& operator <<(ostream&,triplet&);
