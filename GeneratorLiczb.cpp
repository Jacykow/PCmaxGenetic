#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

bool powtarzalnosc(int lista[],int los,int n){
	bool bezpowtorzen=true;
		for (int x=0;x<=n-1;x++)
				if (lista[x]==los)
				{
					return false;
				 } 
		if (bezpowtorzen==true) {
			return true;
			}
}

void generator(int n, int m, int wynik[]){

	int k = m; 
	n=n-1;
	int los, los_pos;
	int *lista = new int[n];
	fill_n(lista,n,-1);
	int *powtorzenia = new int [n];
	fill_n(powtorzenia,n,-1);
	
	for (int i = 0; i<=n-1; i++){
		bool ok=false;
		while(!ok){
			los=(rand()%(k-1)+1);
			ok = powtarzalnosc(powtorzenia,los,n);
			if (ok) powtorzenia[i]=los; 
		}
	/*	//losowanie miejsca na ktore wpisac liczbe aby byly potasowane:
		if (n!=1){
		ok=false;
		while (!ok){
			los_pos = (rand()%(n));
			ok = powtarzalnosc(lista,los_pos,n);
			if (ok) lista[i]=los_pos;
			}	
	}*/
		wynik[i]=los;
		}
	delete lista;
	delete powtorzenia;
}
	
int main(){
srand(time(NULL));

int n=8; //ilosc elementow
int m=20; //suma elementow
int r=30; //czas pracy procesora

fstream file("dane.in", ios::out);

if (file){
    file << n << " " << m <<endl;
    
cout <<"Ilosc procesorow: "<<n<<endl;
cout <<"Ilosc procesow dla wszystkich procesorow: "<<m <<endl;
cout <<"Ilosc czasu pracy procesora: "<< r<<endl;

int *procesy = new int[n];
int *tmp=new int[n];

generator(n,m,procesy);
sort(procesy, procesy+n-1);
cout <<"Ilosci zadan na kazdy procesor: "<<endl;
for (int i=0;i<=n-1;i++){
	if (i==0) tmp[i] = procesy[i];
	else if(i>=n-1) tmp[i] = m - procesy[i-1]; 
	else tmp[i] = (procesy[i]-procesy[i-1]);
}
for (int i=0;i<=n-1;i++)
	cout <<"<"<<tmp[i]<<">"<<endl;
	
for (int i=0;i<=n-1;i++){
	int *podzialki = new int[tmp[i]-1];
	int *tmp2=new int[tmp[i]-1];
	
	if(tmp[i]!=1){
		generator(tmp[i],r,podzialki);
		sort(podzialki, podzialki+tmp[i]-1);
	}
	cout <<"Wylosowane czasy procesow dla procesora "<<i<<endl;

	for (int j=0;j<=tmp[i]-1;j++){
	
		if (tmp[i]==1) tmp2[j]=r;
		else if (j==0) tmp2[j] = podzialki[j];
		else if (j==tmp[i]-1)tmp2[j] = r - podzialki[j-1]; 	
		else tmp2[j] = (podzialki[j]-podzialki[j-1]);
	}

	for (int j=0;j<=tmp[i]-1;j++){
		cout <<"<"<<tmp2[j]<<">"<<endl;
		file << tmp2[j] <<endl;
	}
	delete podzialki;
}

delete procesy;
delete tmp;
file<<r; //JEB£EM NA KONIEC WYNIK, ¯EBYM MÓG£ GO SPRAWDZIÆ W MIÊDZYCZASIE =============================================================================
file.close();
return 0;

}else exit(1);
}
