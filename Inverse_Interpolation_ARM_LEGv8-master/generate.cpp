#include <bits/stdc++.h>
using namespace std;

int main(){
	ofstream file("samples.txt");
	double a = -3.142/2;
	double b = 3.142/2;
	int n = 10;
	double d = (b-a)/(double)n;
	double x = a;
	int i=0;
	for(;i<n;i++){
		file<<i<<" "<<x<<endl;
		x += d;
	}
	x = a;
	for(int j=0;j<n;j++,i++){
		file<<i<<" "<<sin(x)<<endl;
		x += d;
	}
	file.close();
	return 0;
}