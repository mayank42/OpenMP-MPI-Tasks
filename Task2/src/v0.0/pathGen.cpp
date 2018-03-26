#include "sim.hpp"
#define VERSION "v0.0"
static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,int cpu, int group_fd, unsigned long flags){
	int ret;
        ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,group_fd, flags);
        return ret;
}
int main(int argc,char **argv){
	int num_threads = NUM_THREADS;
	if(argc>1){
		num_threads = atoi(argv[1]);
	}
	vector<triplet> velocity(N,triplet(0.0,0.0,0.0));
	vector<triplet> position(N);
	vector<triplet> force(N);
	srand(SEED);
	double p1,p2,p3;
	for(int a=0;a<N;++a){
		p1 = rand()*1.0/(double)RAND_MAX;
		p2 = rand()*1.0/(double)RAND_MAX;
		p3 = rand()*1.0/(double)RAND_MAX;
		position[a] = triplet(p1*XMAX,p2*YMAX,p3*ZMAX);
	}
	cout<<fixed<<setprecision(2);
	
	stringstream traj;
	traj<<"./log/"<<VERSION<<"/trajectory_"<<num_threads<<".csv";
	ofstream trajFile(traj.str().c_str());
	if(!trajFile.is_open()){
		cout<<"Unable to open file. Exiting."<<endl;
		return 0;
	}
	
	stringstream tlog;
	tlog<<"./log/"<<VERSION<<"/timeLog_"<<num_threads<<".csv";
	ofstream timeLog(tlog.str().c_str());
	if(!timeLog.is_open()){
		cout<<"Unable to open time log. Exiting."<<endl;
		trajFile.close();
		return 0;
	}
	
	stringstream pLog;
	pLog<<"./log/"<<VERSION<<"/perfLog_"<<num_threads<<".csv";
	ofstream perfLog(pLog.str().c_str());
	if(!perfLog.is_open()){
		cout<<"Unable to open perf log. Exiting."<<endl;
		trajFile.close();
		timeLog.close();
		return 0;
	}
	
	timeLog<<fixed<<setprecision(6);
	trajFile<<fixed<<setprecision(14);
	
	printToFile(position,trajFile);
	double f1,f2,f3,f;
	triplet r;
	double dist;
	int totalRun = (int)(RUN/dt);
	omp_set_num_threads(num_threads);
	vector<uint64_t> id(7);
	int statFd = init_perf(id);
	char buf[4096];
	struct read_format* rf = (struct read_format*) buf;
	ioctl(statFd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);	  	
	double begin = omp_get_wtime();
	ioctl(statFd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
	#pragma omp parallel for schedule(guided) shared(force) 
	for(int a=0;a<N;++a)force[a].first=force[a].second=force[a].third=0.0;
	#pragma omp parallel for schedule(guided) shared(position,force) private(r,dist,f,f1,f2,f3) collapse(2)
	for(int a=0;a<N;++a){
		for(int b=0;b<N;++b){
			if(b<=a)continue;
			r.first = position[b].first - position[a].first;
			r.second = position[b].second - position[a].second;
			r.third = position[b].third - position[a].third;
			dist = sqrt(r.first*r.first+r.second*r.second+r.third*r.third);
			f = G*M*M/dist;
			f1 = f*r.first/dist;
			f2 = f*r.second/dist;
			f3 = f*r.third/dist;
			#pragma omp atomic
			force[a].first+=f1;
			#pragma omp atomic
			force[a].second+=f2;
			#pragma omp atomic
			force[a].third+=f3;
			#pragma omp atomic
			force[b].first+=-1*f1;
			#pragma omp atomic
			force[b].second+=-1*f2;
			#pragma omp atomic
			force[b].third+=-1*f3;		
		}
	}
	ioctl(statFd,PERF_EVENT_IOC_DISABLE,PERF_IOC_FLAG_GROUP);	
	double end = omp_get_wtime();
	timeLog<<"0,"<<end-begin<<endl;
	read(statFd,buf,sizeof(buf));
	printStatToFile(rf,perfLog,id);
	for(int run=0;run<totalRun;++run){
		if(run%1000==0)printToFile(position,trajFile);
	  	ioctl(statFd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
		begin = omp_get_wtime();
	  	ioctl(statFd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
		#pragma omp parallel for schedule(guided) shared(velocity,force)
		for(int a=0;a<N;++a){
			velocity[a].first+=force[a].first*dt/(2*M);
			velocity[a].second+=force[a].second*dt/(2*M);
			velocity[a].third+=force[a].third*dt/(2*M);
		}
		#pragma omp parallel for schedule(guided) shared(position,velocity)
		for(int a=0;a<N;++a){
			position[a].first+=velocity[a].first*dt;
			while(!(position[a].first>=0 && position[a].first<=XMAX)){
				if(position[a].first>XMAX)position[a].first = 2*XMAX-position[a].first;
				else if(position[a].first<0)position[a].first = -1*position[a].first;
			}
			position[a].second+=velocity[a].second*dt;
			while(!(position[a].second>=0 && position[a].second<=YMAX)){
				if(position[a].second>YMAX)position[a].second = 2*YMAX-position[a].second;
				else if(position[a].second<0)position[a].second = -1*position[a].second;
			}
			position[a].third+=velocity[a].third*dt;
			while(!(position[a].third>=0 && position[a].third<=ZMAX)){
				if(position[a].third>ZMAX)position[a].third = 2*ZMAX-position[a].third;
				else if(position[a].third<0)position[a].third = -1*position[a].third;
			}
		}
		#pragma omp parallel for schedule(guided) shared(force)
		for(int a=0;a<N;++a)force[a].first=force[a].second=force[a].third=0.0;
		#pragma omp parallel for schedule(guided) shared(position,force) private(r,dist,f,f1,f2,f3) collapse(2)
		for(int a=0;a<N;++a){
			for(int b=0;b<N;++b){
				if(b<=a)continue;
				r.first = position[b].first - position[a].first;
				r.second = position[b].second - position[a].second;
				r.third = position[b].third - position[a].third;
				dist = sqrt(r.first*r.first+r.second*r.second+r.third*r.third);
				f = G*M*M/dist;
				f1 = f*r.first/dist;
				f2 = f*r.second/dist;
				f3 = f*r.third/dist;
				#pragma omp atomic
				force[a].first+=f1;
				#pragma omp atomic
				force[a].second+=f2;
				#pragma omp atomic
				force[a].third+=f3;
				#pragma omp atomic
				force[b].first+=-1*f1;
				#pragma omp atomic
				force[b].second+=-1*f2;
				#pragma omp atomic
				force[b].third+=-1*f3;
			}
		}
		#pragma omp parallel for schedule(guided) shared(velocity,force)
		for(int a=0;a<N;++a){
			velocity[a].first+=force[a].first*dt/(2*M);
			velocity[a].second+=force[a].second*dt/(2*M);
			velocity[a].third+=force[a].third*dt/(2*M);
		}
		ioctl(statFd,PERF_EVENT_IOC_DISABLE,PERF_IOC_FLAG_GROUP);	
		end = omp_get_wtime();
		read(statFd,buf,sizeof(buf));
		printStatToFile(rf,perfLog,id);
		cout<<"Percentage finished: "<<(run+1)*100.0/totalRun<<"("<<run+1<<"/"<<totalRun<<")\r";
		timeLog<<run+1<<","<<end-begin<<endl;
		cout.flush();
	}
	trajFile.close();		
	timeLog.close();
	perfLog.close();
	cout<<endl;
	return 0;
}
void inline printToFile(vector<triplet> &pos,ofstream &strm){
	for(int a=0;a<N-1;++a){
		strm<<pos[a]<<",";
	}
	strm<<pos[N-1]<<endl;
}
ostream& operator <<(ostream& strm,triplet &t){
	strm<<t.first<<","<<t.second<<","<<t.third;
	return strm;
}
void printStatToFile(struct read_format *rf,ofstream &perfLog,vector<uint64_t> &id){
	static int stamp=0;
	perfLog<<stamp<<",";
	for(unsigned int a=0;a<rf->nr-1;++a){
		perfLog<<rf->values[a].value<<",";
	}
	perfLog<<rf->values[rf->nr-1].value<<endl;
	stamp++;
}
int init_perf(vector<uint64_t> &id){
	struct perf_event_attr *pe = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe,0,sizeof(perf_event_attr));
	pe->type = PERF_TYPE_HARDWARE;
    	pe->size = sizeof(struct perf_event_attr);
	pe->config = PERF_COUNT_HW_CPU_CYCLES;
	pe->disabled = 1;
	pe->exclude_kernel = 1;
	pe->exclude_hv = 1;
	pe->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int lead_fd = perf_event_open(pe,0,-1,-1,0);
        if (lead_fd == -1) {
        	fprintf(stderr, "Error opening leader %llx\n", pe->config);
        	exit(EXIT_FAILURE);
        }
	
	struct perf_event_attr *pe1 = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe1,0,sizeof(perf_event_attr));
	pe1->type = PERF_TYPE_HARDWARE;
    	pe1->size = sizeof(struct perf_event_attr);
	pe1->config = PERF_COUNT_HW_CACHE_REFERENCES;
	pe1->disabled = 1;
	pe1->exclude_kernel = 1;
	pe1->exclude_hv = 1;
	pe1->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int fd1 = perf_event_open(pe1,0,-1,lead_fd,0);

	struct perf_event_attr *pe2 = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe2,0,sizeof(perf_event_attr));
	pe2->type = PERF_TYPE_HARDWARE;
    	pe2->size = sizeof(struct perf_event_attr);
	pe2->config = PERF_COUNT_HW_CACHE_MISSES;
	pe2->disabled = 1;
	pe2->exclude_kernel = 1;
	pe2->exclude_hv = 1;
	pe2->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int fd2 = perf_event_open(pe2,0,-1,lead_fd,0);

	struct perf_event_attr *pe3 = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe3,0,sizeof(perf_event_attr));
	pe3->type = PERF_TYPE_HW_CACHE;
    	pe3->size = sizeof(struct perf_event_attr);
	pe3->config = PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_READ<<8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS<<16;
	pe3->disabled = 1;
	pe3->exclude_kernel = 1;
	pe3->exclude_hv = 1;
	pe3->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int fd3 = perf_event_open(pe3,0,-1,lead_fd,0);
	
	struct perf_event_attr *pe4 = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe4,0,sizeof(perf_event_attr));
	pe4->type = PERF_TYPE_HW_CACHE;
    	pe4->size = sizeof(struct perf_event_attr);
	pe4->config = PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_READ<<8 | PERF_COUNT_HW_CACHE_RESULT_MISS<<16;
	pe4->disabled = 1;
	pe4->exclude_kernel = 1;
	pe4->exclude_hv = 1;
	pe4->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int fd4 = perf_event_open(pe4,0,-1,lead_fd,0);

	struct perf_event_attr *pe5 = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe5,0,sizeof(perf_event_attr));
	pe5->type = PERF_TYPE_HW_CACHE;
    	pe5->size = sizeof(struct perf_event_attr);
	pe5->config = PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_WRITE<<8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS<<16;
	pe5->disabled = 1;
	pe5->exclude_kernel = 1;
	pe5->exclude_hv = 1;
	pe5->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int fd5 = perf_event_open(pe5,0,-1,lead_fd,0);

	struct perf_event_attr *pe8 = (perf_event_attr*)malloc(sizeof(perf_event_attr));
	memset(pe8,0,sizeof(perf_event_attr));
	pe8->type = PERF_TYPE_HW_CACHE;
    	pe8->size = sizeof(struct perf_event_attr);
	pe8->config = PERF_COUNT_HW_CACHE_L1I | PERF_COUNT_HW_CACHE_OP_READ<<8 | PERF_COUNT_HW_CACHE_RESULT_MISS<<16;
	pe8->disabled = 1;
	pe8->exclude_kernel = 1;
	pe8->exclude_hv = 1;
	pe8->read_format = PERF_FORMAT_ID | PERF_FORMAT_GROUP;
	int fd8 = perf_event_open(pe8,0,-1,lead_fd,0);

	ioctl(lead_fd,PERF_EVENT_IOC_ID,&id[CYCLES]);
	ioctl(fd1,PERF_EVENT_IOC_ID,&id[CACHE_REF]);
	ioctl(fd2,PERF_EVENT_IOC_ID,&id[CACHE_MIS]);
	ioctl(fd3,PERF_EVENT_IOC_ID,&id[L1D_R_AC]);
	ioctl(fd4,PERF_EVENT_IOC_ID,&id[L1D_R_MIS]);
	ioctl(fd5,PERF_EVENT_IOC_ID,&id[L1D_W_AC]);
	ioctl(fd8,PERF_EVENT_IOC_ID,&id[L1I_R_MIS]);
	
	return lead_fd;
}
