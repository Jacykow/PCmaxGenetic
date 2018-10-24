#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;
int main() {
	int n= ((rand()%100)+1);
    int m= ((rand()%10)+1);
	srand(time(NULL));
	fstream file("dane.in", ios::out);

	if (file){
    file << n << " " << m <<endl;
	for (int i=0;i<n;i++) file << ((rand()%10)+40) <<" ";
	file.close();
	} else exit(1);
}
