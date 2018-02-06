#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<cmath>
#include<omp.h>
using namespace std;
class badFileException:public exception{
	virtual const char* what() const throw(){
		return "Cannot read file";
	}
};
void readMat(vector<vector<float> >&,string);
double mulGuided(vector<vector<float> >&,vector<vector<float> >&,vector<vector<float> >&,int);
double mulGuidedRed(vector<vector<float> >&,vector<vector<float> >&,vector<vector<float> >&,int);
inline void setZero(vector<vector<float> >&);
int main(int argc,char *argv[]){
	if(argc<4){
		cout<<"usage: "<<argv[0]<<" <size> <file1> <file2>"<<endl;
		return 0;
	}
	int size=0;
	try{
		size = stoi(string(argv[1]));
	}catch(exception &e){
		cout<<e.what()<<endl;
		return 0;
	}
	string Atxt = string(argv[2]);
	string Btxt = string(argv[3]);
	vector<vector<float> > A(size,vector<float>(size,0.0));
	vector<vector<float> > B(size,vector<float>(size,0.0));
	vector<vector<float> > C(size,vector<float>(size,0.0));
	try{
    		readMat(A,Atxt);
		readMat(B,Btxt);
	}catch(exception &e){
		cout<<e.what()<<endl;
		return 0;
	}
	int numt;
	ofstream res("Result11.txt");
	int chunk;
	res<<"Num_threads,Guided,Guided Reduction"<<endl;
	for(numt=1;numt<=size;numt++){
		omp_set_num_threads(numt*numt);
		setZero(C);
		double r1 = mulGuided(A,B,C,(int)floor((float)size/(numt*numt)));
		double r2 = mulGuidedRed(A,B,C,(int)floor((float)size/(numt*numt)));
		res<<numt<<","<<r1<<","<<r2<<endl;	
	}
	res.close();
	return 0;
}
void readMat(vector<vector<float> > &A,string name){
	ifstream file(name.c_str());
	string line;
	stringstream stream;
	badFileException badfile;
	for(int a=0;a<A.size();a++){
		if(!getline(file,line))throw badfile;
		stream<<stringstream(line).str();
		for(int b=0;b<A[a].size();b++){
			if(!stream || stream.eof())throw badfile;
			stream>>A[a][b];
		}
	}
	file.close();
}
double mulGuided(vector<vector<float> > &A,vector<vector<float> > &B,vector<vector<float> > &C,int chunk){
	//Guided Scheduler
	double st=omp_get_wtime();
	int n = A.size();
	#pragma omp parallel for shared(A,B,C,n) schedule(guided,chunk)
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			float temp=0;			
			for(int k=0;k<n;k++)temp+=A[i][k]*B[k][j];
			C[i][j]=temp;
		}
	}
	double en=omp_get_wtime();
	return en-st;
}
double mulGuidedRed(vector<vector<float> > &A,vector<vector<float> > &B,vector<vector<float> > &C,int chunk){
	//Guided Scheduler
	double st=omp_get_wtime();
	int n = A.size();
	#pragma omp parallel for shared(A,B,C,n) schedule(guided,chunk)
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			float temp=0;			
			#pragma omp parallel for reduction(+:temp)
			for(int k=0;k<n;k++)temp+=A[i][k]*B[k][j];
			C[i][j]=temp;
		}
	}
	double en=omp_get_wtime();
	return en-st;
}
inline void setZero(vector<vector<float> > &A){
	for_each(A.begin(),A.end(), [](vector<float> &r){fill(r.begin(),r.end(),0.0);});
}

