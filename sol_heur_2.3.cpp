#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <cmath>
using namespace std;

// SETTINGS
string inputFile="m50n200.txt";
float avgMutations=6;
int populationSize=50;

int n;
int m;
int currentTarget;
int minimumTarget;

class Gene{
	public:
		vector<int> order;
		int rV;
		int cT;
		
		Gene(vector<int> o){
			order=o;
			cT=0;
			rV=-1;
		}
		
		int eval(int l, int r){
			while(1){
				int m=(l+r)/2;
				if(m==l){
					return r;
				}
				if(eval(m)==0){
					r=m;
				}
				else{
					l=m;
				}
			}
		}
		
		int eval(){
			int r=minimumTarget, l;
			while(eval(r)!=0){
				l=r;
				r*=2;
			}
			return eval(l,r);
		}
		
		void show(int max){
			int s=0;
			int k=0;
			cout<<k<<":\t";
			for(int x=0;x<order.size();x++){
				if(s+order[x]<=max){
					cout<<order[x]<<" ";
					s+=order[x];
				}
				else{
					if(k>=m){
						return;
					}
					if(order[x]>max){
						return;
					}
					cout<<"=== "<<s<<endl;
					s=order[x];
					k++;
					cout<<k<<":\t"<<s<<" ";
				}
			}
			cout<<"=== "<<s<<endl;
			cout<<"MAX: "<<max<<endl;
		}
		
		void save(){
			int max=getBestVal();
			fstream out;
			string f="answer_to_";
			f.append(inputFile);
			out.open(f, fstream::out);
			
			out<<"MAX: "<<max<<endl;
			
			int s=0;
			int k=0;
			out<<k<<":\t";
			for(int x=0;x<order.size();x++){
				if(s+order[x]<=max){
					out<<order[x]<<" ";
					s+=order[x];
				}
				else{
					if(k>=m){
						return;
					}
					if(order[x]>max){
						return;
					}
					out<<"=== "<<s<<endl;
					s=order[x];
					k++;
					out<<k<<":\t"<<s<<" ";
				}
			}
			out<<"=== "<<s<<endl;
			
			out.close();
		}
		
		void show(){
			show(getBestVal());
		}
		
		int eval(int max){
			int s=0;
			int k=0;
			int x;
			for(x=0;x<order.size();x++){
				if(s+order[x]<=max){
					s+=order[x];
				}
				else{
					if(k>=m){
						break;
					}
					if(order[x]>max){
						break;
					}
					s=order[x];
					k++;
				}
			}
			s=0;
			for(;x<order.size();x++){
				s+=order[x];
			}
			return s;
		}
		
		void mutate(float avgTimes){
			avgTimes /= order.size();
			vector<int> t;
			for(int x=0;x<order.size();){
				if((float)rand()/(float)RAND_MAX < avgTimes){
					t.push_back(order[x]);
					order.erase(order.begin()+x);
				}
				else{
					x++;
				}
			}
			while(!t.empty()){
				order.insert(order.begin()+rand()%order.size(),t[t.size()-1]);
				t.pop_back();
			}
			cT=0;
			rV=-1;
		}
		
		int getR(){
			if(rV==-1 || currentTarget!=cT){
				rV=eval(currentTarget);
				cT=currentTarget;
			}
			return rV;
		}
		
		int getBestVal(){
			return eval(minimumTarget-1,currentTarget+2);
		}
};

bool geneComp(Gene& a, Gene& b){
	return a.getR() < b.getR();
}

class Population{
	public:
		float mut;
		vector<Gene> g;
		int s;
		int gi;
		
		Population(Gene starter, float mutation, int size){
			mut=mutation;
			s=sqrt(size);
			while(size--){
				Gene k=starter;
				k.mutate(mutation);
				g.push_back(k);
			}
			currentTarget=getBest().getBestVal()-1;
			gi=0;
		}
		
		void makeNextGen(){
			sort(g.begin(),g.end(),geneComp);
			vector<Gene> ng;
			for(int x=0;x<s;x++){
				ng.push_back(g[x]);
			}
			while(1){
				Gene k=g[g.size()-sqrt(rand()%(g.size()*g.size()))-1];
				k.mutate(mut);
				ng.push_back(k);
				if(ng.size()==g.size()){
					break;
				}
			}
			g=ng;
			gi++;
		}
		
		Gene& getBest(){
			return g[0];
		}
		
		void showAllValues(){
			for(int x=0;x<g.size();x++){
				g[x].eval();
			}
			for(int x=0;x<g.size();x++){
				cout<<x<<": "<<g[x].getBestVal()<<" --- "<<g[x].getR()<<endl;
			}
		}
};

int main(){
	srand(time(NULL));
	
	fstream in;
	in.open(inputFile, fstream::in);
	in>>m>>n;
	vector<int> tab;
	int a;
	minimumTarget=0;
	while(n--){
		in>>a;
		minimumTarget+=a;
		tab.push_back(a);
	}
	minimumTarget=minimumTarget/m-minimumTarget%m;
	
	Gene g(tab);
	currentTarget=g.eval()-1;
	string line;
	int skip=-1;
	int lr=-1;
	
	Population p(g,avgMutations,populationSize);
	
	while(1){
		Gene& best=p.getBest();
		if(line=="" && p.gi!=skip){
			p.makeNextGen();
			if(lr!=-1){
				if(best.getR()!=lr){
					cout<<"Best R: "<<best.getR()<<endl;
					lr=best.getR();
				}
			}
			if(best.getR()==0 && best.getBestVal()<=currentTarget){
				currentTarget=best.getBestVal()-1;
				best.save();
			}
			else{
				continue;
			}
		}
		
		if(p.gi<skip){
			continue;
		}
		skip=-1;
		lr=-1;
		
		
		if(line=="s"){
			p.getBest().show();
		}
		else if(line=="v"){
			p.showAllValues();
		}
		else if(line=="c"){
			break;
		}
		else if(line=="t"){
			cout<<"target: ";
			cin>>skip;
			currentTarget=skip;
			skip=-1;
			lr=0;
			cout<<endl;
		}
		else if(line=="j"){
			cout<<"skip: ";
			cin>>skip;
			skip+=p.gi;
			cout<<endl;
		}
		else{
			cout<<"Generation "<<p.gi<<endl;
			cout<<"Target "<<currentTarget<<" >>> "<<minimumTarget<<endl;
			cout<<"Best MAX: "<<best.getBestVal()<<endl;
		}
		
		getline(cin,line);
	}
	
	return 0;
}



































