#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<sstream>
#include<omp.h>
using namespace std;
void checkIntensity(vector<vector<unsigned char> >&,int,int,int,float);
int main(int argc,char *argv[]){
	if(argc<4){
		cout<<"usage: "<<argv[0]<<" <image_file> <filter size> <minimum intensity>"<<endl;
		return 1;
	}
	ifstream image(argv[1]);
	if(!image.is_open()){
		cout<<"Cannot open image file."<<endl;
		return 1;
	}
	int window=stoi(argv[2]);
	float intensity=stof(argv[3]);
	string line;
	//version:
	getline(image,line);
	string version=line;
	cout<<"Version: "<<line<<endl;
	//getline(image,line); No comments
	//dimensions:
	getline(image,line);
	cout<<"Dimensions: "<<line<<endl;
	stringstream dim(line);
	int r,c;
	dim>>r>>c;
	//level:
	getline(image,line);
	string level = line;
	cout<<"Gray level: "<<line<<endl;
	//matrix init
	vector<vector<unsigned char> > mat(r,vector<unsigned char>(c));
	char temp;
	for(int a=0;a<r;a++){
		for(int b=0;b<c;b++){
			image.read(&temp,1);
			mat[a][b]=temp;
		}
		image.read(&temp,1);
	}	
	image.close();
	omp_set_num_threads(10);//Set by examining graphs in part1
	int chunk = mat.size()*mat[0].size()/10;
	/**
 	 * Image filtering 
 	 **/
	#pragma omp parallel for shared(mat) schedule(guided,chunk) collapse(2)
	for(int a=0;a<r-window+1;a+=10){
		for(int b=0;b<c-window+1;b+=10){
			checkIntensity(mat,a,b,window,intensity);
		}
	}
	//Frequency matrix
	vector<float> freq(256,0);
	/**
 	 * Frequency counting.
 	 **/
	#pragma omp parallel for shared(mat,freq) schedule(guided,chunk) collapse(2)
	for(int a=0;a<r;a++){
		for(int b=0;b<c;b++){	
			#pragma omp atomic
			freq[mat[a][b]]++;
		}
	}
	//Writing Histogram
	ofstream hist("Histogram.txt");
	float total=accumulate(freq.begin(),freq.end(),0.0f);
	for_each(freq.begin(),freq.end(),[total](float &r){r/=total;});
	for(int a=0;a<256;a++)hist<<freq[a]<<endl;
	hist.close();
	//Writing new PGM
	ofstream output("Output.pgm");
	output<<version<<endl;
	output<<"#Modified pgm file"<<endl;
	output<<mat.size()<<" "<<mat[0].size()<<endl;
	output<<level<<endl;
	for(int a=0;a<mat.size();a++){
		for(int b=0;b<mat[a].size();b++){
			temp = mat[a][b];
			output.write(&temp,1);
		}
		output<<endl;
	}
	output.close();
	return 0;
}
void checkIntensity(vector<vector<unsigned char> > &mat,int r,int c,int window,float intensity){
	float sum=0;
	for(int a=r;a<r+window;a++){
		for(int b=c;b<c+window;b++){
			sum+=mat[a][b];
		}
	}
	sum/=100;
	if(sum>=intensity)return;
	for(int a=r;a<r+window;a++){
		for(int b=c;b<c+window;b++){
			mat[a][b]=0;
		}
	}
}
			
			
	
		

