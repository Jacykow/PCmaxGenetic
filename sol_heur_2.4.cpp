#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <cmath>
using namespace std;

// SETTINGS
string inputFile="m50n1000.txt";
float mutationRate=0.001f;
int populationSize=1000;

int processes; //processes
int processors; //processors
int currentTarget;
int minimumTarget;

struct gene{
	vector<int> order;
	int lastMax;
	int r;
	int rSum;
};

int diff(int a, int b){
	return a>b ? a-b : b-a;
}

void show(vector<int>& tab, int max, bool save){
	string outputFile = "answer_to_";
	outputFile.append(inputFile);
	fstream out;
	if(save)out.open(outputFile, fstream::out);
	if(save)out<<max<<endl;
	int x, s=0;
	for(x=0;x<tab.size();x++){
		if(s+tab[x] <= max){
			s+=tab[x];
			if(save)out<<tab[x]<<" ";
			else cout<<tab[x]<<" ";
		}
		else{
			if(save)out<<0<<endl<<tab[x]<<" ";
			else cout<<" === "<<s<<" "<<0<<endl<<tab[x]<<" ";
			s=tab[x];
		}
	}
	if(save)out<<0;
	else cout<<" === "<<s<<" "<<0;
	if(save)out.close();
}

int fitness(vector<int>& tab, int max, int processors){
	int x, s=0;
	for(x=0;x<tab.size();x++){
		if(s+tab[x] <= max){
			s+=tab[x];
		}
		else{
			s=tab[x];
			processors--;
			if(processors <= 0){
				break;
			}
		}
	}
	return tab.size()-x;
}

int smallFitness(vector<int>& tab, int r){
	int s=0;
	for(int x=tab.size()-r;x<tab.size();x++){
		s+=tab[x];
	}
	return s;
}

void shuffle(vector<int>& tab){
	vector<int> temp;
	for(int x=0;x<tab.size();x++){
		temp.push_back(tab[x]);
	}
	temp.clear();
	while(temp.size() > 0){
		int r = rand()%temp.size();
		tab.push_back(temp[r]);
		temp.erase(temp.begin()+r);
	}
}

int randomFromDist(int max){
	return rand()%(rand()%max+1); 
}

void mutate(vector<int>& tab, float rate){
	int mutations = (int)(rate * (float)tab.size()) + 1;
	do{
		int a = tab.size() - randomFromDist(tab.size()) - 1;
		int b = tab.size() - randomFromDist(tab.size()-1) - 1;
		b -= (b <= a);
		swap(tab[a],tab[b]);
		mutations /= 2;
	} while(rand()%tab.size() < mutations + 1);
}

void show(gene& g, bool save){
	show(g.order, g.lastMax, save);
}

int calcFitness(gene& g, int max){
	if(g.lastMax != max){
		g.lastMax = max;
		g.r = fitness(g.order, max, processors);
		g.rSum = smallFitness(g.order, g.r);
	}
	return g.r;
}

void mutate(gene& g){
	g.lastMax=0;
	mutate(g.order, mutationRate);
}

bool comp(gene& a, gene& b){
	calcFitness(a,currentTarget);
	calcFitness(b,currentTarget);
	if(a.r==b.r){
		return a.rSum < b.rSum;
	}
	else{
		return a.r < b.r;
	}
}

int findBest(gene& a, int r){
	int l = minimumTarget - 1;
	int m = (l+r)/2;
	while(m>l){
		if(calcFitness(a,m) == 0){
			r=m;
		}
		else{
			l=m;
		}
		m = (l+r)/2;
	}
	return r;
}

int findBest(gene& a){
	int max = minimumTarget * 2;
	while(calcFitness(a, max) != 0){
		max *= 2;
	}
	return findBest(a, max);
}

vector<gene> nextGen(vector<gene>& pop){
	sort(pop.begin(),pop.end(),comp);
	int ps = pop.size();
	vector<gene> npop;
	npop.push_back(pop[0]);
	for(int x=1;x<pop.size();x++){
		if(calcFitness(pop[x],currentTarget)==0){
			npop.push_back(pop[x]);
		}
	}
	while(npop.size()<ps){
		gene g = pop[randomFromDist(pop.size())];
		mutate(g);
		npop.push_back(g);
	}
	if(calcFitness(npop[0], currentTarget) == 0){
		int fb = findBest(npop[0],currentTarget);
		if(currentTarget == fb){
			cout<<"Breakthrough:\t"<<fb<<" ||| "<<minimumTarget<<endl;
		}
		else{
			cout<<"Breakthrough:\t"<<currentTarget<<" -> "<<fb<<" ||| "<<minimumTarget<<endl;
		}
		show(npop[0].order,fb,true);
		currentTarget = fb - 1;
	}
	return npop;
}

void showAll(vector<gene>& pop){
	for(int x=0;x<pop.size();x++){
		cout<<calcFitness(pop[x],currentTarget)<<endl;
	}
}

int main(){
	srand(time(NULL));
	
	fstream in;
	in.open(inputFile, fstream::in);
	in>>processors>>processes;
	vector<int> tab;
	int a;
	minimumTarget=0;
	while(processes--){
		in>>a;
		minimumTarget+=a;
		tab.push_back(a);
	}
	minimumTarget=minimumTarget/processors-minimumTarget%processors;
	
	vector<gene> population;
	for(int x=0;x<populationSize;x++){
		gene g;
		g.lastMax=0;
		g.order = tab;
		shuffle(g.order);
		population.push_back(g);
	}
	
	currentTarget=findBest(population[0]);
	string line;
	int skip=-1;
	int g=0;
	int lr=-1;
	int lrs=-1;
	int lt = time(NULL);
	
	cout<<"Start"<<endl;
	while(1){
		if(skip > 0){
			population = nextGen(population);
			if(lr != calcFitness(population[0],currentTarget) || time(NULL) - lt >= 3 || population[0].rSum != lrs){
				lr = calcFitness(population[0],currentTarget);
				lrs = population[0].rSum;
				lt = time(NULL);
				cout<<"G: "<<g<<"\tR: "<<lr<<"\tRS: "<<lrs<<endl;
			}
			g++;
			skip--;
			continue;
		}
		cout<<"?- ";
		getline(cin,line);
		if(line == ""){
			population = nextGen(population);
			g++;
			cout<<"G: "<<g<<"\tT: "<<currentTarget<<" ||| "<<minimumTarget<<endl;
			cout<<"R: "<<calcFitness(population[0],currentTarget)<<endl;
		}
		if(line == "t"){
			cout<<"Target: ";
			cin>>currentTarget;
		}
		if(line == "s"){
			cout<<"Skip: ";
			cin>>skip;
		}
		if(line == "b"){
			show(population[0],false);
		}
		if(line == "sa"){
			showAll(population);
		}
	}
	
	return 0;
}



































