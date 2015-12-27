// Exponentiation by squaring

#include <iostream>

int exponent(int x, int n)
{
    if(n == 0)return 1;
    if(n == 1)return x;

    if(n&1)
        return x*exponent(x*x, (n-1)>>1);
    else
        return exponent(x*x, n>>1);
}

int main(int argc, char *argv[])
{
    int x, n;
    while(1)
    {
        std::cin>>x>>n;
        std::cout<<exponent(x, n)<<std::endl;
    }
    return 0;
}
