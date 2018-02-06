#include<iostream>
#include<omp.h>
using namespace std;
int main(){
	#pragma omp parallel
	{
		cout<<"Mayank\n";
	}
	return 0;
}
	
