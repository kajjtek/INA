#ifndef PRODUCT_ORDER_HPP
#define PRODUCT_ORDER_HPP

#include <vector>
#include <array>
#include <algorithm>

template <std::size_t N>
struct Point {
    std::array<long long, N> coords;

    Point() { coords.fill(0); }
    Point(std::initializer_list<long long> l) {
        std::copy(l.begin(), l.end(), coords.begin());
    }

    bool product_less_equal(const Point<N>& other) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (coords[i] > other.coords[i]) return false;
        }
        return true;
    }

    bool operator<(const Point<N>& other) const {
        return coords < other.coords;
    }

    bool operator==(const Point<N>& other) const {
        return coords == other.coords;
    }
};

template <std::size_t N>
std::vector<Point<N>> findMinimals(std::vector<Point<N>> points) {
    if (points.empty()) return {};

    std::sort(points.begin(), points.end());

    std::vector<Point<N>> minimals;

    for (const auto& p : points) {
        bool is_dominated = false;
        for (const auto& m : minimals) {
            if (m.product_less_equal(p)) {
                is_dominated = true;
                break;
            }
        }
        if (!is_dominated) {
            minimals.push_back(p);
        }
    }
    return minimals;
}

#endif