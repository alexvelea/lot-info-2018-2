#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define f first
#define s second


#include "anagrame.h"

int query( vector< int > &v, int *a )
{
    for( int i = 1 ; i < v.size() ; i++ )
    {
        a[ i - 1 ] = v[ i ];
    }
    int x = Ask( a );
    return x;
}

void print( vector< int > &v, int *a )
{
    for( int i = 1 ; i < v.size() ; i++ )
    {
        a[ i - 1 ] = v[ i ];
    }
    GiveSolution( a );
}

void rotate_elements( vector< int > &v )
{
    int x = v[ 1 ];
    for( int i = 1 ; i < v.size() - 1 ; i++ )
    {
        v[ i ] = v[ i + 1 ];
    }
    v[ v.size() - 1 ] = x;
}

int FirstPoz( vector< int > &v, int val )
{
    for( int i = 1 ; i < v.size() ; i++ )
    {
        if( v[ i ] != val )
            return i;
    }
    return v.size() - 1;
}

int LastPoz( vector < int > &v, int val )
{
    for( int i = v.size() - 1 ; i >= 1 ; i-- )
        if( v[ i ] != val )
            return i + 1;
    return 1;
}

void Solve(int, int n, int *vec )
{
    int totalSum = 0;
    vector< int > v,ans,sol;
    int *a = (int *)malloc( sizeof( int ) * n );
    v.resize( n + 1 );
    ans.resize( n + 1 );
    sol.resize( n + 1 );
    for( int i = 1 ; i <= n ; i++ )
    {
        v[ i ] = vec[ i - 1 ];
        totalSum += v[ i ];
    }

    int prev,curr,sum=0;
    prev = query( v , a );
    int currentElement = v[1];
    if( prev == 0 )
    {
        print( v , a );
    }

    for( int i = 1 ; i < n ; i++ )
    {
        rotate_elements( v );
        curr = query( v , a );

        if( curr == 0 )
        {
            print( v , a );
        }

        pair< int , int > p1 = { FirstPoz( v , currentElement ) , prev }, p2 = { LastPoz( v , currentElement ) , curr };


        if( p1.s > p2.s )
        {
            int dif = p1.s - p2.s;
            p1 = { p1.f + dif , p2.s };
        }
        else
        {
            int dif = p2.s - p1.s;
            p2 = { p2.f - dif , p1.s };
        }
        
        

        sol[ ( p1.f + p2.f ) / 2 ] = currentElement;
        currentElement = v[ 1 ];
        prev = curr;
    }

    int fixBug = 0;
    for( int i = 1 ; i <= n ; i++ )
        fixBug += sol[ i ];


    for( int i = 1 ; i <= n ; i++ )
    {
        if( sol[ i ] == 0 )
        sol[ i ] = totalSum - fixBug;
    }

    print( sol , a );
}