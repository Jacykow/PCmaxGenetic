#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
int sum(vector<int> v){
	int s=0;
	for(int x=0;x<v.size();x++){
		s+=v[x];
	}
	return s;
}
int main(){
	int n; //ile procesów
	int m; //ile procesorów
	
	fstream in;
	in.open("test.in", fstream::in);
	in>>n>>m;
	vector<int> p[m];
	int s=0;
	int a;
	while(n--){
		in>>a;
		s+=a;
		int min=0;
		for(int x=1;x<m;x++){
			if(sum(p[x]) < sum(p[min])){
				min=x;
			}
		}
		p[min].push_back(a);
	}
	cout<<"Working time:\t"<<s<<endl;
	
	int max=0;
	for(int x=1;x<m;x++){
		if(sum(p[x]) > sum(p[max])){
			max=x;
		}
	}
	cout<<"Idle time:\t"<<sum(p[max])*m-s<<endl;
	
	for(int x=0;x<m;x++){
		cout<<x+1<<":";
		for(int y=0;y<p[x].size();y++){
			cout<<" "<<p[x][y];
		}
		cout<<endl;
	}
	
	return 0;
}
