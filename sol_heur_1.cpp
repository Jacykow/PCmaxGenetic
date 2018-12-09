#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;

class P : public vector<int>{
	private:
		int s;
	public:
		P() : vector<int>(){
			s=0;
		}
		int sum(){
			if(s==0){
				for(int x=0;x<this->size();x++){
					s+=this->at(x);
				}
			}
			return s;
		}
		void add(int x){
			this->push_back(x);
			s+=x;
		}
		int remove(int i){
			int r=this->at(i);
			s-=r;
			this->erase(this->begin()+i);
			return r;
		}
		int remove(){
			return remove(rand()%this->size());
		}
		void add(P o){
			for(int x=0;x<o.size();x++){
				this->add(o[x]);
			}
		}
};

int findmax(P tab[], int size){
	int m=0;
	for(int x=0;x<size;x++){
		if(tab[x].sum()>tab[m].sum()){
			m=x;
		}
	}
	return m;
}

bool exchange(P& a, P& b){
	int d=abs(b.sum()-a.sum());
	int ai = rand()%a.size();
	int bi = rand()%b.size();
	P A,B;
	while(ai--){
		A.add(a.remove());
	}
	while(bi--){
		B.add(b.remove());
	}
	
	if(abs((a.sum()+B.sum())-(b.sum()+A.sum())) < d){
		a.add(B);
		b.add(A);
		return true;
	} else {
		a.add(A);
		b.add(B);
		return false;
	}
}

int main(){
	int n; //ile procesów
	int m; //ile procesorów
	srand(time(NULL));
	
	fstream in;
	in.open("dane.in", fstream::in);
	in>>n>>m;
	P p[m];
	int a;
	while(n--){
		in>>a;
		int min=0;
		for(int x=1;x<m;x++){
			if(p[x].sum() < p[min].sum()){
				min=x;
			}
		}
		p[min].add(a);
	}
	
	int max,k;
	string line;
	while(getline(cin,line)){
		if(line==""){
			for(int x=0;x<m;x++){
				cout<<x+1<<"\t["<<p[x].sum()<<"]: ";
				for(int y=0;y<p[x].size();y++){
					cout<<p[x][y]<<" ";
				}
				cout<<endl;
			}
			cout<<"Max time:\t"<<p[findmax(p,m)].sum()<<endl;
		}
		int a=rand()%m;
		int b=rand()%(m-1);
		if(b>=a)b++;
		cout<<"swapping "<<a+1<<" and "<<b+1<<endl;
		exchange(p[a],p[b]);
	}
	
	return 0;
}
