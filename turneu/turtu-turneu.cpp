#include <bits/stdc++.h>

using namespace std;

#define NMAX 17
#define inf 10000000

#define fin cin
#define fout cout

int n,k,KMAX,zero[ (1<<NMAX) + 5 ];
int *D[NMAX + 2];

vector< int > v[ (1 << NMAX) + 5 ],dst;

bool PoateNivel( int concurent, int blaturi, int nivel )
{

    int z = 1 << ( nivel - 1 );

    if( z > concurent )
        return false;

    if( z <= blaturi )
        return true;

    if( D[ nivel ][ z - blaturi ] <= concurent )
        return true;

return false;
}

int maxBlaturi( int concurent )
{
    int ans = 0;
    while( concurent != 0 )
    {
        concurent /= 2;
        ans++;
    }
    return ans;
}

/// Concurentul x are dreptul la y blaturi
int solve( int concurent, int blaturi )
{
    if( blaturi == 0 )
        return zero[ concurent ];

    for( int i = n + 1 ; i > zero[ concurent ] ; i-- )
    {
        if( PoateNivel( concurent , blaturi , i ) )
        {
            return i;
        }
    }
    return zero[ concurent ];
}

void Merge( int x, int y )
{
    dst.resize( v[ x ].size() + v[ y ].size() );
    int i1=0,i2=0,idx=0;

    int n1=v[x].size();
    int n2=v[y].size();
    while( i1 < n1 || i2 < n2 )
    {
        if( i1 == n1 )
        {
            dst[ idx++ ] = v[ y ][ i2 ];
            i2++;
        }
        else if( i2 == n2 )
        {
            dst[ idx++ ] = v[ x ][ i1 ];
            i1++;
        }
        else
        {
            if( v[ x ][ i1 ] < v[ y ][ i2 ] )
            {
                dst[ idx++ ] = v[ x ][ i1 ];
                i1++;
            }
            else
            {
                dst[ idx++ ] = v[ y ][ i2 ];
                i2++;
            }
        }
    }
}

vector< int > precomputare( int nivel, int x, int y )
{
    vector< int > rasp( y - x + 1 );
    if( x == y )
    {
        rasp[ 0 ] = D[ 1 ][ x ];
        return rasp;
    }

    vector< int > a = precomputare( nivel - 1 , x , ( x + y ) / 2 );

    vector< int > b = precomputare( nivel - 1 , ( x + y ) / 2 + 1 , y );

    rasp.resize( a.size() + b.size() );

    int i=0,j=0,idx = 0;
    while( i < a.size() || j < b.size() )
    {
        if( i == a.size() )
        {
            rasp[ idx++ ] = b[ j ];
            j++;
        }
        else if( j == b.size() )
        {
            rasp[ idx++ ] = a[ i ];
            i++;
        }
        else
        {
            if( a[ i ] < b[ j ] )
            {
                rasp[ idx++ ] = a[ i ];
                i++;
            }
            else
            {
                rasp[ idx++ ] = b[ j ];
                j++;
            }
        }
        int dbg = D[ nivel + 1 ][ idx ];
        D[ nivel + 1 ][ idx ] = min( D[ nivel + 1 ][ idx ] , rasp[ idx - 1 ] );

        dbg = D[ nivel + 1 ][ idx ];
    }
    zero[ rasp[ idx - 1 ] ] = nivel + 1;


    return rasp;

}

int main()
{
    fin>>n;
    n = maxBlaturi( n ) - 1;
    k = 4;
    D[ 1 ] = new int[ (1<<n) + 5 ];
    for( int i = 2 ; i <= n + 1 ; i++ )
    {
        D[ i ] = new int[ k + 5 ];
        for( int j = 0 ; j < k + 5 ; j++ )
            D[ i ][ j ] = inf;
        k *= 2;
    }

    KMAX = 1 << n;
    for( int i = 1 ; i <= KMAX ; i++ )
    {
        int x;
        fin>>x;
        x++;
        zero[ x ] = 1;
        D[ 1 ][ i ] = x;
    }

    /*
    k = KMAX;
    for( int i = 2 ; i <= n + 1 ; i++ )
    {

        for( int j = 1 ; j <= k ; j += 2 )
        {
            Merge( j , j + 1 );
            v[ j ].resize( 0 );
            v[ j + 1 ].resize( 0 );
            v[ ( j + 1 ) / 2 ].resize( dst.size() );
            int idx = 0;
            for( auto it : dst )
                v[ ( j + 1 ) / 2 ][ idx++ ] = it;
            zero[ dst[ dst.size() - 1 ] ] = i;
        }

        k /= 2;
        for( int j2 = 0 ; j2 < KMAX / k ; j2++ )
        {
            int ans = inf;
            for( int j = 1 ; j <= k ; j++ )
            {
                ans = min( ans , v[ j ][ j2 ] );
            }
            D[ i ][ j2 + 1 ] = ans;
        }
    }
    return 0;*/
    precomputare( n , 1 , ( 1 << n ) );

    int Qn;
    fin>>Qn;

    while( Qn-- )
    {
        int x,y;
        fin>>x>>y;
        x++;
        y = min( y , 1 << ( maxBlaturi( x ) - 1 ) );

        fout<<solve( x , y ) - 1<<'\n';
    }

    return 0;
}
