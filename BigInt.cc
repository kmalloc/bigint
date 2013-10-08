#include <string>
#include <iostream>
#include <assert.h>
#include <iterator>
#include <algorithm>
#include <fstream>

#include "BigInt.h"

using namespace std;


BigInt::BigInt(long long value)
{
    vi.clear();
    m_IsNeg = value < 0;
    value = value < 0?-value:value;
    do
    {
        char c = value%10 + '0';
        vi.push_front(c);
        value = value/10;
    } while(value);
}

BigInt::BigInt(const BigInt& hi)
{
    vi.clear();
    vi.insert(vi.begin(),hi.vi.begin(),hi.vi.end());
    m_IsNeg = hi.m_IsNeg;
}

BigInt::BigInt(const string& str)
{
    vi.clear();
    m_IsNeg = str[0] == '-';
    vi.insert(vi.begin(),str.begin() + m_IsNeg,str.end());
}

BigInt::BigInt(bool isNeg,deque<char>& v)
{
    vi.swap(v);
    m_IsNeg = isNeg;
}


// simulate multiplication, si < 10
// warn: prod must not overlap vi.
static inline void SimCal(const deque<char>& vi, const int si, deque<char>& prod)
{
    deque<char>::const_reverse_iterator re = vi.rend(), ri = vi.rbegin();
    int carry = 0;

    if(si == 0)
    {
        prod.clear();
        prod.push_back('0');
        return ;
    }

    if(si < 0)
        throw "si shold greater than 0";

    prod.clear();

    for(; ri != re;++ri)
    {
        char v  = *ri;
        int iv  = v - '0';
        int pro = (iv * si) + carry;

        prod.push_front(pro%10+'0');
        carry = (pro)/10;
    }

    if(carry)
    {
        prod.push_front(carry+'0');
    }

    while(prod.size() > 1 && prod[0] == '0')
        prod.pop_front();
}



// lp == s is ok.
// warn, s should be resized to proper capacity before passed in as paramer.
// must assure there is no extra '0' in high position for lp and rp, like: 00234.
static inline void SimSum(const deque<char>& lp, const deque<char>& rp, deque<char>& s)
{
    deque<char>::const_reverse_iterator ril, rir;

    int carry = 0;
    int pos    = s.size() - 1;
    for(ril=lp.rbegin(), rir=rp.rbegin(); ril != lp.rend()&&rir != rp.rend(); ++ril, ++rir)
    {
        int vl  = *ril - '0';
        int vr  = *rir - '0';
        int sum = vl+vr+carry;
        if(pos < 0)
            s.push_front(sum%10 + '0');
        else
            s[pos--] = sum%10 + '0';

        carry= sum/10;
    }

    if(ril != lp.rend())
    {
        for(; ril != lp.rend(); ++ril)
        {
            int v1 = *ril - '0' + carry;
            if(pos < 0)
                s.push_front(v1%10 + '0');
            else
                s[pos--] = v1%10 + '0';

            carry = v1/10;
        }
    }

    if(rir != rp.rend())
    {
        for(; rir != rp.rend(); ++rir)
        {
            int vi = *rir - '0' + carry;
            if(pos < 0)
                s.push_front(vi%10+'0');
            else
                s[pos--] = vi%10+'0';
            carry = vi/10;
        }

    }	
    if(carry)
    {
        if(pos < 0)
            s.push_front(carry+'0');
        else
            s[pos--] = carry+'0';
    }

    while(pos-- >= 0)
    {
        s.pop_front();
    }
}

// return value indicates result of subtraction is negative or not.
static bool SimSubtract(const deque<char>& lp, const deque<char>& rp, deque<char>& s)
{
    if(lp.size() < rp.size())
    {
        SimSubtract(rp, lp, s);
        return true;
    }

    deque<char>::const_reverse_iterator ril, rir;

    int carry = 0;
    int pos    = s.size() - 1;
    for(ril = lp.rbegin(), rir = rp.rbegin(); ril != lp.rend() && rir != rp.rend(); ++ril, ++rir)
    {
        int vl  = *ril - '0';
        int vr  = *rir - '0';
        int sum = vl-vr+carry;

        if(sum < 0)
        {
            sum = 10 - vr + vl+carry;
            carry = -1;	  
        }
        else
            carry = 0;

        if(pos < 0)
            s.push_front(sum%10 + '0');
        else
            s[pos--] = sum%10 + '0';
    }

    if(ril != lp.rend())
    {//so, lp > rp.
        for(; ril != lp.rend(); ++ril)
        {
            int v1 = *ril - '0' + carry;

            if(v1 < 0)
            {
                carry = -1;
                v1 = 9;
            }
            else
            {
                carry = 0;
            }
            if(pos < 0)
                s.push_front(v1%10 + '0');			
            else
                s[pos--] = v1%10 + '0';
        }

        if(carry)
            throw "wrong left operand input.";

    }

    if(rir != rp.rend() || carry)
    {//rp > lp, so we switch left , right operands , and do the subtraction again.
        //s.clear();
        SimSubtract(rp, lp, s);
        return true;
    }	

    while(pos-- >= 0)
    {
        s.pop_front();
    }

    while(s.size() > 1 && s[0] == '0')
    {
        s.pop_front();
    }
    return false;
}

static inline void TrimZero(deque<char>& con)
{
    while(con.size() > 1 && con[0] == '0')
        con.pop_front();
}

static inline void SimDiv(const deque<char>& lp, const deque<char>& rp, deque<char>& quo, deque<char>& rem)
{
    int curPos = 0;
    int szl = lp.size(), szr = rp.size();
    deque<char> tmp, SubtRes, MultRes;

    quo.clear(); 
    rem.clear();
    quo.push_back('0');

    if(szr > szl)
    {
        rem.insert(rem.begin(), lp.begin(), lp.end());
        return;
    }

    tmp.insert(tmp.begin(), lp.begin(), lp.begin()+szr-1);
    curPos = szr-1;

    do
    {
        bool isNeg = false;

        tmp.push_back(lp[curPos++]);
        isNeg = SimSubtract(tmp, rp, SubtRes);

        while(isNeg && curPos < szl)
        {
            quo.push_back('0');
            tmp.push_back(lp[curPos++]);
            TrimZero(tmp);
            isNeg = SimSubtract(tmp, rp, SubtRes);
        }

        if(isNeg )
        {
            quo.push_back('0');
            break;
        }

        isNeg = true;

        int i = 0;
        while(isNeg)
        {
            ++i;
            MultRes.clear();
            SubtRes.clear();
            SimCal(rp, i, MultRes);
            isNeg = SimSubtract(MultRes, tmp, SubtRes);
        }

        bool isDiviable = (SubtRes.size() == 1 && SubtRes[0] == '0');
        MultRes.clear();
        SubtRes.clear();

        if(!isDiviable)
        {
            i--;
            SimCal(rp, i, MultRes);
            SimSubtract(tmp, MultRes, SubtRes);
            tmp.clear();
            tmp.insert(tmp.begin(), SubtRes.begin(), SubtRes.end());
        }
        else
        {
            tmp.clear();
        }

        quo.push_back(i+'0');

    }while(curPos < szl);

    if(tmp.size())
        rem.swap(tmp);
    else
        rem.push_back('0');

    while(quo.size() > 1 && quo[0] == '0')
        quo.pop_front();

}


BigInt BigInt::operator*(const int value)const
{
    if(value == 0 || IsZero())
        return BigInt(0);

    int le = value < 0?-value:value;
    deque<char> tmp1;
    deque<char> tmp2;
    //deque<char> tmp3;

    int v = le%10;

    SimCal(vi, v, tmp1);

    le = le/10;

    int i = 1;
    while(le)
    {
        v = le%10;
        tmp2.clear();	 
        if(v)
        {
            SimCal(vi, v, tmp2);

            for(int k = 0;k < i;++k)
                tmp2.push_back('0');

            // tmp3.clear();
            // SimSum(tmp1, tmp2, tmp3);
            // tmp1.swap(tmp3);
            SimSum(tmp1, tmp2, tmp1);
        }

        ++i;
        le= le/10;
    }

    while(tmp1.size() > 1 && tmp1[0] == '0')//trim '0' from high endian.
        tmp1.pop_front();

    BigInt res((value < 0)^m_IsNeg, tmp1);
    return res;
}

BigInt BigInt::operator*(const BigInt& value)const
{
    BigInt tb(0);
    BigInt res(0);

    int sz = value.vi.size();
    int i = 0;

    if(IsZero() || value.IsZero())
        return BigInt(0);

    while(i < sz)
    {
        //tb = operator*(value.vi[sz - i - 1]-'0');
        SimCal(vi, value.vi[sz - i - 1]-'0', tb.vi);
        if(!(tb.vi.size() == 1 && tb.vi[0] == '0'))
        {//tb != 0
            for(int j=0; j < i; j++)
                tb.vi.push_back('0');
        }
        ++i;
        res += tb;
    }

    res.m_IsNeg = m_IsNeg ^ value.m_IsNeg;

    return res;
}

BigInt operator*(int value, BigInt& hi)
{
    return (hi.operator*(value));
}

BigInt BigInt::operator+(const BigInt& value)const
{
    deque<char> sum;
    bool isNeg = value.m_IsNeg ^ m_IsNeg;

    if(!isNeg)//addition of two positive numbers or two negative numbers
    {
        SimSum(vi, value.vi, sum);
        isNeg = m_IsNeg;
    }
    else if(m_IsNeg)//negativ+positive, that is sutraction if we reverse left, right operand.
        isNeg = SimSubtract(value.vi, vi, sum);
    else//positive+negative, that is subtraction.
        isNeg = SimSubtract(vi, value.vi, sum);

    while(sum.size() > 1 && sum[0] == '0')
        sum.pop_front();

    return BigInt(isNeg, sum);
}

void BigInt::operator+=(const BigInt&value)
{
    //*this = *this+value;
    if(value == 0)
        return ;

    bool isNeg = value.m_IsNeg ^ m_IsNeg;

    if(!isNeg)//addition of two positive numbers or two negative numbers
    {
        SimSum(vi, value.vi, vi);
    }
    else if(m_IsNeg)//negativ+positive, that is sutraction if we reverse left, right operand.
        isNeg = SimSubtract(value.vi, vi, vi);
    else//positive+negative, that is subtraction.
        isNeg = SimSubtract(vi, value.vi, vi);

    while(vi.size() > 1 && vi[0] == '0')
        vi.pop_front();

    m_IsNeg = isNeg;
}
void BigInt::operator+=(const int&value)
{
    //*this = (BigInt)*this + value;
    if(value == 0)
        return;

    int v = value;
    bool isNeg = false;
    bool bNeg = v < 0;
    deque<char> tmp;

    while(v)
    {
        char c = v%10;
        v /= 10;
        tmp.push_front(c);
    }

    bNeg = bNeg ^ m_IsNeg;
    if(bNeg == false)
    {
        SimSum(vi, tmp, vi);
        isNeg = m_IsNeg;
    }
    else if(m_IsNeg)//negativ+positive, that is sutraction if we reverse left, right operand.
        isNeg = SimSubtract(tmp, vi, vi);
    else//positive+negative, that is subtraction.
        isNeg = SimSubtract(vi, tmp, vi);

    while(vi.size() > 1 && vi[0] == '0')
        vi.pop_front();

    m_IsNeg = isNeg;
}

BigInt BigInt::operator-(const BigInt&value)const
{
    deque<char> sum;
    bool isNeg = value.m_IsNeg ^ m_IsNeg;

    if(!isNeg)//subtraction of two positive numbers , or 
    {
        if(m_IsNeg == false)
            isNeg = SimSubtract(vi, value.vi, sum);
        else
            isNeg = SimSubtract(value.vi, vi, sum);
    }
    else 
    {
        SimSum(vi, value.vi, sum);
        isNeg = m_IsNeg;
    }

    while(sum.size() > 1 && sum[0] == '0')
        sum.pop_front();
    return BigInt(isNeg, sum);
}


BigInt BigInt::operator/(const BigInt&value)const
{
    bool isNeg = m_IsNeg ^ value.m_IsNeg;
    BigInt tmp, quo, remainder;

    if(IsZero())
        return BigInt(0);

    if(value.IsZero())
        throw "divided by zero.";

    SimDiv(vi, value.vi, quo.vi, remainder.vi);

    while(quo.vi.size()  > 1 && quo.vi[0] == '0')
        quo.vi.pop_front();

    if(quo.IsZero())
        quo.m_IsNeg = false;
    else
        quo.m_IsNeg = isNeg;

    return quo;
}

BigInt BigInt::operator%(const BigInt&value) const
{
    BigInt tmp, quo, remainder;

    SimDiv(vi, value.vi, quo.vi, remainder.vi);

    if(remainder.IsZero())
        remainder.m_IsNeg = false;
    else
        remainder.m_IsNeg = m_IsNeg;

    return remainder;
}

bool BigInt::operator==(const BigInt&value) const
{
    if(m_IsNeg ^ value.m_IsNeg)
        return false;

    int sz1 = vi.size();
    int sz2 = value.vi.size();

    if(sz1 != sz2 || sz1+sz2 == 0)
        return false;

    for(int i = 0; i < sz1; ++i)
    {
        if(vi[i] != value.vi[i])
            return false;
    }

    return true;
}

bool BigInt::operator!=(const BigInt&value)const
{
    return !(*this == value);
}


bool BigInt::operator<(const BigInt&v)const
{
    if(m_IsNeg&&!v.m_IsNeg)
        return true;
    if(!m_IsNeg && v.m_IsNeg)
        return false;
    BigInt res = *this - v;

    return res.m_IsNeg;
}

bool BigInt::operator>=(const BigInt&v)const
{
    return !(*this < v);
}

bool BigInt::operator>(const BigInt&v)const
{
    if(m_IsNeg&&!v.m_IsNeg)
        return false;
    if(!m_IsNeg && v.m_IsNeg)
        return true;

    BigInt res = v - *this;

    return res.m_IsNeg;
}

bool BigInt::operator<=(const BigInt&v)const
{
    return !(*this > v);
}


ostream& operator<<(ostream&os, const BigInt&value)
{
    value.Print(os);
    return os;
}

BigInt& BigInt::operator= (const BigInt& hi)
{
    vi.clear();
    vi.insert((vi.begin()),hi.vi.begin(),hi.vi.end());
    m_IsNeg = hi.m_IsNeg;
    return *this;
}

void BigInt::Print(ostream& os) const
{
    deque<char>::const_iterator it = vi.begin();

    if(m_IsNeg)
        os << '-';


    for(;it != vi.end();++it)
        os << *it;
}

