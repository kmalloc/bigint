#include "BigInt.h"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main()
{
    BigInt lp(24333), rp(27);

    cout<<lp/rp<<endl;

    lp = 5124;rp = 28114;

    cout<<lp-rp<<endl;

    srand((unsigned int)time(NULL));
    for(int i = 0;i < 100000;i++)
    {
        long long a, b, f1, f2;
        a = rand();
        b = rand();
        f1 = rand()%2;
        f2 = rand()%2;

        if(f1)
            a = -a;
        if(f2)
            b = -b;

        BigInt aa(a), bb(b);

        if(b)
        {
            if(aa/bb != a/b)
                cout<<a<<"/"<<b<<"="<<aa/bb<<", ans:"<<a/b<<endl;
        }

        if(aa*bb != a*b)
            cout<<a<<"*"<<b<<"="<<aa*bb<<", ans:"<<a*b<<endl;

        if(f2)
            swap(a, b);

        BigInt cc(a), dd(b);

        if(cc - dd != a - b)
            cout<<a<<"-"<<b<<"="<<cc-dd<<", ans:"<<a-b<<endl;
    }

    BigInt pos(12004), pos1(12000), pos2(22004), pos3(3), pos4(13), pos5(121034), pos6(12012), pos7(240);
    BigInt neg(-24), neg2(-4), neg3(-12024);

    BigInt posLar("8557299059167389608190279930");
    BigInt posLar2("7540113804746346429");


    cout << posLar/posLar2 << endl;

    BigInt one1(1), one2(1);
    BigInt one3(3736), one4(37), one5(3750), one6(37);

    long long intarrdiv[][2] = 
    {//a, b, a/b, a%b.
        3736, 37,
        3750, 37,
        1, 1,
        12004, -4, /**/
        12004, -24,
        121034, -24,
        12012, 240,
    };

    long long intarrmul[][2] = 
    {
        3736, 37,
        3750, 37,
        1, 1, 1, 0,
        12004, -4,
        12004, -24,
        121034, -24,
        12012, 240,
    };

    BigInt res;

    int na = -4;
    int na2= -123;
    int bb = na2%4;
    int aa = (123)%na;

    aa = 123/na;

    int sz1 = sizeof(intarrdiv)/sizeof(intarrdiv[0]);
    for(int i = 0;i < sz1;i++)
    {
        BigInt lp(intarrdiv[i][0]);
        BigInt rp(intarrdiv[i][1]);

        cout<<endl;
        cout<<intarrdiv[i][0]<<"/"<<intarrdiv[i][1]<<"=";
        cout<<lp/rp;
        cout<<", ans:"<<intarrdiv[i][0]/intarrdiv[i][1];
        cout<<endl;
        cout<<intarrdiv[i][0]<<"%"<<intarrdiv[i][1]<<"=";
        cout<<lp%rp;
        cout<<", ans:"<<intarrdiv[i][0]%intarrdiv[i][1];
    }

    int sz2 = sizeof(intarrmul)/sizeof(intarrmul[0]);
    for(int i = 0;i < sz1;i++)
    {
        BigInt lp(intarrmul[i][0]);
        BigInt rp(intarrmul[i][1]);
        cout<<endl;
        cout<<intarrmul[i][0]<<"*"<<intarrmul[i][1]<<"=";
        cout<<lp*rp;
        cout<<", ans:"<<intarrmul[i][0]*intarrmul[i][1];
    }

    res = pos+neg3;
    res = pos+neg;
    res = pos - pos1;
    res = pos - pos2;
    res = pos - neg;
    return 0;
}

