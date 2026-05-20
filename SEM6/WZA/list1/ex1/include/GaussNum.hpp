#ifndef GAUSSNum_H
#define GAUSSNum_H

#include <utility>
#include <vector>

class GaussNum {
    using DivisionResult = std::vector<std::pair<GaussNum, GaussNum>>;
    public:
    long long real;
    long long im;
    unsigned long long norm() const {return real*real + im*im;}
    static std::vector<std::pair<GaussNum, GaussNum>> division(GaussNum num1, GaussNum num2);
    static GaussNum gdc(GaussNum num1, GaussNum num2);
    static GaussNum lcd(GaussNum num1, GaussNum num2);
    GaussNum(long long r, long long i) : real(r), im(i) {}

    GaussNum &operator+=(const GaussNum &o) { real += o.real; im += o.im; return *this; }
    GaussNum &operator-=(const GaussNum &o) { real -= o.real; im -= o.im; return *this; }
    GaussNum &operator*=(const GaussNum &o) { long long oldreal = real; real = oldreal*o.real - im*o.im; im = im*o.real + oldreal*o.im; return *this; }

    GaussNum operator+(const GaussNum &o) const { GaussNum r(*this); r += o; return r; }
    GaussNum operator-(const GaussNum &o) const { GaussNum r(*this); r -= o; return r; }
    GaussNum operator*(const GaussNum &o) const { GaussNum r(*this); r *= o; return r; }

    bool operator==(const GaussNum &o) const { return real == o.real && im == o.im; }
    bool operator!=(const GaussNum &o) const { return !(*this == o); }
    bool operator< (const GaussNum &o) const { return this->norm() <  o.norm(); }
    bool operator<=(const GaussNum &o) const { return this->norm() <=  o.norm(); }
    bool operator> (const GaussNum &o) const { return this->norm() >  o.norm(); }
    bool operator>=(const GaussNum &o) const { return this->norm() >=  o.norm(); }

};

#endif