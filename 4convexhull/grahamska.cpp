#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::string;

enum class Direction { left, right };

template <typename T> struct point {
  T x;
  T y;
};

template <typename T> int crossW(point<T> o, point<T> a, point<T> b) {
  T w = (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
  if (w < 0)
    return -1;
  if (w > 0)
    return 1;
  return 0;
}

template <typename T> Direction direction(point<T> o, point<T> a, point<T> b) {
    int w = crossW(o, a, b);
    if (w <= 0)
        return Direction::right;
    else
        return Direction::left;
}

template <typename T> T abs(point<T> p) { return p.x * p.x + p.y * p.y; }

template <typename T> int compare(point<T> u, point<T> v) {
    auto w = crossW({0, 0}, u, v);
    if (w == 0) {
        return abs(u) < abs(v) ? -1 : 1;
    } else {
        return -w;
    }
}

template <typename T, typename C>
typename T::value_type next_point(T begin, T &end, C comp) {
    auto ret = *begin;
    std::pop_heap(begin, end, comp);
    //after pop_heap, the heap is shrunk by 1 from the end, but only "virtually"
    //- the last element is now not part of the heap structure
    // we shall modify the end iterator to reflect this, instead of removing the last element
    end--;
    return ret;
}

bool isFloat(double d) {
    double intpart;
    return std::modf(d, &intpart) != 0.0;
}

template <typename T> bool hasAtLeastOneFloatOp(bool res, point<T> p) {
    return res || isFloat(p.x) || isFloat(p.y);
}

int main() {
    string line;
    //turn off sync with c stdio (printf, scanf) - it doubles the time to read in a file
    std::ios::sync_with_stdio(false); 
    std::getline(std::cin, line);
    std::stringstream ss(line);
    int dims, npoints;
    ss >> dims >> npoints;
    
    std::vector<point<double>> points;
    
    size_t least_idx;
    point<double> least_y = {0.0, std::numeric_limits<double>::max()};
    points.reserve(npoints);

    //read in the points and find the point with lowest y-coordinate
    for (int i = 0; i < npoints; ++i) {
        double x, y;
        std::getline(std::cin, line);
        sscanf(line.c_str(), "%la %la", &x, &y);    
        if (y < least_y.y || (y == least_y.y && x < least_y.x)) {
            least_y = {x, y};
            least_idx = i;
        }
        points.push_back({x, y});
    }

    //move origo to the point with lowest y-coordinate
    for (auto &p : points) {
        p.y -= least_y.y;
        p.x -= least_y.x;
    }
    
    auto comp = [](auto &a, auto &b) { return compare(a, b) > 0 ? true : false; };
    std::make_heap(points.begin(), points.end(), comp);
    
    std::vector<point<double>> H;


    //the main part of the graham scanner
    auto end = points.end();
    H.push_back(next_point<>(points.begin(), end, comp));
    H.push_back(next_point<>(points.begin(), end, comp));
    
    while (points.begin() != end) {
        
        auto p = next_point(points.begin(), end, comp);
        Direction d;
        do {
            size_t last = H.size() - 1;
            auto ps = H[last];
            auto pr = H[last - 1];
            d = direction(pr, ps, p);
            if (d == Direction::right) {
                H.pop_back();
            }
        } while (d == Direction::right && H.size() > 2);
        
        H.push_back(p);
    }
    
    // now check if the last two points are in line with the first one
    size_t last = H.size() - 1;
    auto ps = H[last];
    auto pr = H[last - 1];
    auto p = H[0];
    Direction d = direction(pr, ps, p);
    if (d == Direction::right) {
        H.pop_back();
    }


    //restore the coordinates
    for (auto &p : H) {
        p.y += least_y.y;
        p.x += least_y.x;
    }
    
    cout << H.size() << endl;
    
    // find rightmost element:
    size_t idx = 0;
    for (; idx < H.size() - 1; idx++) {
        if (H[idx + 1].x < H[idx].x) {
            break;
        }
    }

    //set up printing according to floatness
    cout << std::fixed;
    bool hasAtLeastOneFloat =
        std::accumulate(H.begin(), H.end(), false, hasAtLeastOneFloatOp<double>);
    if (hasAtLeastOneFloat) {
        cout << std::setprecision(3);
    } else {
        cout << std::setprecision(0);
    }
    
    // iterate backwards from the rightmost element
    int i = idx;
    do {
        cout << H[i].x << " " << H[i].y << endl;
        i--;
        if (i < 0)  i = H.size() - 1;
    } while (i != idx);
    
    return 0;
}
