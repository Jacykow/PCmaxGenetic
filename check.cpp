#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <cmath>
using namespace std;

string inputFile="m50n200.txt";

int main(){
	fstream in;
	in.open(inputFile, fstream::in);
	int m,n;
	in>>m>>n;
	vector<int> p1,p2;
	for(int x=0;x<n;x++){
		int a;
		in>>a;
		p1.push_back(a);
	}
	in.close();
	
	cout<<"Done 1\n";
	
	string testFile = "answer_to_";
	testFile.append(inputFile);
	in.open(testFile, fstream::in);
	int max;
	in>>max;
	int s;
	for(int x=0;x<m;x++){
		cout<<x+1<<": ";
		int a=-1;
		s=0;
		while(1){
			in>>a;
			if(a==0){
				break;
			}
			s+=a;
			cout<<a<<" ";
			p2.push_back(a);
			if(s>max){
				cout<<"\n\t!!!OVERFLOW!!!\n";
			}
		}
		cout<<endl;
	}
	sort(p1.begin(),p1.end());
	sort(p2.begin(),p2.end());
	int y=0;
	for(int x=0;x<p2.size();x++){
		if(p1[x+y]<p2[x]){
			cout<<"missing "<<p1[x+y]<<endl;
			y++;
			continue;
		}
		//cout<<p1[x+y]<<"\t=\t"<<p2[x]<<endl;
	}
	if(p2.size()==p1.size()){
		cout<<"OK"<<endl;
	}
	system("pause");
	in.close();
	
	return 0;
}
