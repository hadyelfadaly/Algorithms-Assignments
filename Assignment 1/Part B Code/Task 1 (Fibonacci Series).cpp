/*
Name: Hady Hassan El Fadaly - ID: 20236113
Name: Mohamed Amr Ahmed - ID: 20236087
Name: Omar Waleed ElSobky - ID:20237008
*/

#include <iostream>

using namespace std;

int fib_rec(int n)
{

    if(n == 0) return 0;
    if(n == 1) return 1;

    return fib_rec(n - 1) + fib_rec(n - 2);

}

//to multiply 2 matrices (the matrix with the base matrix) and store the value in the main matrix we need.
void multiply(long long F[2][2], long long M[2][2])
{

    long long a = F[0][0]*M[0][0] + F[0][1]*M[1][0];
    long long b = F[0][0]*M[0][1] + F[0][1]*M[1][1];
    long long c = F[1][0]*M[0][0] + F[1][1]*M[1][0];
    long long d = F[1][0]*M[0][1] + F[1][1]*M[1][1];

    F[0][0] = a;
    F[0][1] = b;
    F[1][0] = c;
    F[1][1] = d;

}
void power(long long F [2][2], int n)
{

    if(n<=1) return;

    long long M[2][2]={{1,1},{1,0}};

    //we need to calculate n/2 using it in divide and conquer
    power(F,n/2);
    multiply(F,F);

    if(n % 2 == 1) multiply(F,M);

}
int fib_matrix(int n)
{

    if(n==0) return 0;
    if(n == 1) return 1;

    long long F[2][2] = {{1, 1}, {1, 0}};

    //calculate F^(n-1)              fib(5) -> so here we will need M^n-1 (M^4).
    power(F, n - 1);

    //answer is in top-left position
    return F[0][0];

}

int fibDP(int n)
{

    if(n==0) return 0;
    if(n == 1) return 1;

    //fib(0), fib(1)
    int prev = 0, curr = 1;

    for(int i=2; i<=n; i++)
    {

        int next = prev + curr;
        prev = curr;
        curr = next;

    }

    return curr;

}

int main()
{

    cout<<fib_rec(5)<<endl;
    cout<<fib_matrix(6)<<endl;
    cout<<fibDP(3)<<endl;

    return 0;

}