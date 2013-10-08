#ifndef __BIG_INT_H_
#define __BIG_INT_H_

#include <deque>

class BigInt
{
    public:
        BigInt(long long value);

        BigInt():m_IsNeg(false){}

        ~BigInt(){}

        BigInt& operator= (const BigInt& hi);
        BigInt(const BigInt& hi);
        BigInt(const std::string& str);
        BigInt(bool isNeg, std::deque<char>& v);

        bool operator> (const BigInt&v) const;
        bool operator< (const BigInt&v) const;

        bool operator>= (const BigInt&v) const;
        bool operator<= (const BigInt&v) const;
        bool operator!= (const BigInt&v) const;
        bool operator== (const BigInt&v) const;
        void operator+= (const BigInt&v);//note, += is much more efficient than + by reducing unnecessary copying.
        void operator+= (const int&v);
        void operator-= (const BigInt&v);
        void operator*= (const BigInt&v);
        void operator/= (const BigInt&v);

        BigInt operator+ (const BigInt& v) const;
        BigInt operator- (const BigInt& v) const;
        BigInt operator* (int value) const;//multiplication and division are costly operation.
        BigInt operator* (const BigInt&v) const;
        BigInt operator/ (const BigInt&v) const;
        BigInt operator% (const BigInt&v) const;

        friend BigInt operator* (int value, const BigInt& hi);

        bool IsHasValue() { return !vi.empty(); }
        void Print(std::ostream& os) const;
        friend std::ostream& operator<< (std::ostream&os, const BigInt& value);
        inline bool IsZero() const { return vi.size() == 1 && vi[0] == '0'; }

    private:

        bool m_IsNeg;
        std::deque<char> vi;
};

#endif

