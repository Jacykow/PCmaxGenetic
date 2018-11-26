#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

/*
v f(ilosc wartosci, przedzial)

*/

using namespace std;
int main() {
	srand(time(NULL));
	
	// n - iloœæ procesów
	// m - iloœæ procesorów
	// d - dlugosc procesora stala
	/*
	
	n-m - tyle przedzia³ek
	
	vec{
	rozmiar - iloœæ procesów dla aktualnego procesora - 1
	elementy - losowe wartoœci z przedzia³u (0,10000)
	}
	
	
	
	
	==========|===========|==========
	===============|===========|=====
	====|====================|=======
	=======|=======================|=
	*/
	
	int n=5; //ilosc procesorow
    int m=100; //czas pracy jednego procesora

	int sum=0;
	int cnt=0;

	fstream file("dane.in", ios::out);

	if (file){
    file << n << " " << m <<endl;

	while(cnt<n){

        int los=((rand()%(m))+1);
        sum+=los;
       // cout <<"Suma wyniosla "<<sum<<endl;
       // cout <<"Los wyniosl "<<los<<endl;
        if(((sum<=m)&&(cnt==0))||(sum<m)){
            file << los <<endl;
       //     cout <<"Wiec do tablicy wpisano "<<los<<endl;
            if (sum==m) {
                cnt+=1;
                sum=0;
                }
            }
        else
        {los=(m-(sum-los));
        file << los <<endl;
     //   cout <<"Wiec do tablicy wpisano "<<los<<endl;
        sum=0;
        cnt+=1;
        }
    // cout <<"Cnt wynioslo "<<cnt<<endl<<endl;
	}

	file.close();
	} else exit(1);
}
