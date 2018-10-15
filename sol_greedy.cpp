#include <iostream>
#include <fstream>
using namespace std;
int main(){
	int n; //ile procesów
	int m; //ile procesorów
	
	fstream in;
	in.open("test.in", fstream::in);
	in>>n>>m;
	int t[n]; //procesy
	for(int x=0;x<n;x++){
		in>>t[x];
	}
	return 0;
}
