#include "../include/GaussNum.hpp"
#include <cmath>
#include <set>

using DivisionResult = std::vector<std::pair<GaussNum, GaussNum>>;

std::vector<std::pair<GaussNum, GaussNum>> GaussNum::division(GaussNum one, GaussNum two) {
    double denominator_ = static_cast<double>(two.norm());
    std::vector<std::pair<GaussNum, GaussNum>> result;

    double real_ = (one.real*two.real + one.im*two.im) / denominator_;
    double im_ = (one.im*two.real - one.real*two.im) / denominator_;

    std::set<long long> potentialReals = {(long long)std::floor(real_), (long long)std::ceil(real_)};
    std::set<long long> potentialIms = {(long long)std::floor(im_), (long long)std::ceil(im_)};

    for(long long r : potentialReals) {
        for(long long i: potentialIms) {
            GaussNum q(r, i);
            GaussNum r2 = one - q*two;
            if(r2.norm() < two.norm()) {
                result.push_back({q, r2});
            }
        }
    }

    return result;
}

GaussNum GaussNum::gdc(GaussNum num1, GaussNum num2) {
    GaussNum zero(0,0);
    while(num2!=zero) {
        std::pair<GaussNum, GaussNum> res = division(num1, num2).front();
        num1 = num2;
        num2 = res.second;
    }
    return num1;
}

GaussNum GaussNum::lcd(GaussNum num1, GaussNum num2) {
    return division( num1 * num2 ,gdc(num1, num2)).front().first; 
}