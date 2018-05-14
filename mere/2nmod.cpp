#include <fstream>
#include <iostream>

using namespace std;

ifstream fin("mere.in");
ofstream fout("mere.out");

int main()
{
	long long int n,mod,ans = 1;
	mod = 1e9 + 7;
	fin>>n;
	for( int i = 1 ; i <= n ; i++ )
	{
		ans = ( ans * 2 ) % mod;
	}
	fout<<ans;
return 0;
}
