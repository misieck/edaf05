#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <cassert>
#include <ranges>

using std::cout;
using std::double_t;
using std::endl;
using std::string;
using std::vector;
using std::ranges::sort;

struct point {
    double_t x;
    double_t y;
};


int crossW(point o, point a, point b) {
  double_t w = (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
  if (w < 0)
    return -1;
  if (w > 0)
    return 1;
  return 0;
}

double_t dot(point o, point a, point b)
{
    return (a.x - o.x) * (b.x-o.x) + (a.y - o.y) * (b.y-o.y);
}

double_t k(point a, point b) {
    return (b.y-a.y) / (b.x-a.x);
}

template <template <class...> class array_t>
array_t<double_t> create_ks( const array_t<point>& p){
    array_t<double> ret;
    for (int i = 0; i<p.size(); i++){
        ret.push_back( k( p[i], p[(i+1) % p.size()] ) );
    }
    return ret;
}

 
bool line_segment(point r, point s, point t){
    auto w = crossW( r, s, t );
    auto dot1 = dot(s,r,t);
    auto dot2 = dot(s,t,t);
    return (w == 0) && dot1 >= 0 && dot1 <= dot2;
    //return w == 0 && (  r.x>=s.x && t.x>=r.x  || r.x>=t.x && s.x >= r.x  ) ;
}

template <typename ARRAY>
int include_points( ARRAY& to_array, const ARRAY& from_array, size_t from_idx, size_t size )
{
    int i = 0;
    for (int k = 0; k < size; k++)
    {
        //(size + x -1 )%size guarantees positive result
        int u = (size + k + from_idx - 1) % size;
        int v = (size + k + from_idx ) % size;
        int w = (size + k + from_idx + 1) % size;
        if ( !line_segment(from_array[v], from_array[u], from_array[w] ))
        {
          to_array[i++] = from_array[v];
        }
    }
    
    return i;
}

template<typename T>
size_t add(size_t dst_idx, size_t from_idx, size_t to_idx, T& to_array, const T& from_array, size_t size){
    //cout<<size<<"  "<<to_idx-from_idx<<endl;

    assert( size == from_array.size() );

    if (from_idx<=to_idx){
        assert( to_idx-from_idx < size);
    }
    else{
        //assert ( );
    }
    
    size_t j = from_idx;
    size_t i;
    size_t k = dst_idx;
    do{
        to_array[k++] = from_array[j];
        i = j;
        j = (j + 1) % size;
        
    } while(i != to_idx);
    return k;
}

template<typename array_t>
size_t leftmost(const array_t& p){
    auto n = p.size();
    size_t i = 0;
    for(size_t k = 0;k < n; k++){
        if(p[k].x < p[i].x){
            i = k;
        }
    }
    if(i == 0) i = 1;
    return i;
}


double_t neg_inf(double_t in){
    if (std::isinf(in)) return -std::numeric_limits<double_t>::infinity();
    return in;
}


double_t pos_inf(double_t in){
    if (std::isinf(in)) return std::numeric_limits<double_t>::infinity();
    return in;
}

template <typename array_t>
std::tuple<size_t, size_t> base_case(array_t & points){
    std::ranges::sort (points, [](auto p1, auto p2){ return p1.x>p2.x || p1.x == p2.x && p1.y>p2.y; } );
    size_t n = points.size();
    assert( n > 0 );
    if (n < 3) {
        return {n, n-1};
    }

    double_t k1 = pos_inf( k(points[0], points[1]) );
    double_t k2 = pos_inf( k(points[0], points[2]) );

    assert( n == 3 );
    if ( k1<k2 ){ 
        std::swap(points[1], points[2]);
        return  { n, n-2 } ;
    } else {
        return  { n, n-1 };
    }
}


#include "prephong-cases.cpp"

template <typename array_t>
std::tuple<size_t, size_t>
dc(array_t &points) {

    if (points.size()<=3) return base_case(points);

    size_t na = points.size() / 2;
    /*
    if (points[na].y == points[na-1].y)
    {
        while (points[na+1].y == points[na].y){
            na++;
        }
    }
    */
    size_t nb = points.size() - na;

    array_t a = array_t(points.begin(), points.begin()+na);
    array_t b = array_t(points.begin()+na, points.end() );
    auto ra = dc(a);
    auto rb = dc(b);
    na = std::get<0>(ra);
    nb = std::get<0>(rb);
    assert(na == a.size() );
    assert(nb == b.size() );
    size_t iL = std::get<1>(ra);
    size_t jL = std::get<1>(rb);

    auto alfas = create_ks(a);
    auto betas = create_ks(b);
    double_t La = a[iL].x;
    double_t Ra = a[0].x;
    double_t Lb = b[jL].x;
    double_t Rb = b[0].x;

    size_t j_i_stars[4];
    if (Ra < Rb){
        if (La < Lb){
            case1(a, na, b, nb, alfas, betas, iL, jL, j_i_stars);
        } else {
            case2(a, na, b, nb, alfas, betas, iL, jL, j_i_stars);
        }
    } else{
        if (La < Lb){
            case3(a, na, b, nb, alfas, betas, iL, jL, j_i_stars);
        } else {
            case4(a, na, b, nb, alfas, betas, iL, jL, j_i_stars);
        }
    }
    // [ iR*, iL*, jL*, jR* ]
    array_t q( points.size() );
    size_t n = add(0, j_i_stars[0], j_i_stars[1], q,a,na);
           n = add(n, j_i_stars[2], j_i_stars[3], q,b,nb);

    size_t j = 0;
    for(int k = 0; k < n; k++){
            if(q[k].x > q[j].x || q[k].x == q[j].x && q[k].y > q[j].y){
                j = k;
            }
    }
    
    n = include_points(points,q,j,n);
    size_t i = leftmost(points);
    points.resize(n);
    return{n,i};
}


#define NTESTS  8

point data[ NTESTS ][3] = {
    { {2, 1}, {1, 3}, {4, 2},},   
    { {4, 2}, {1, 3}, {2, 1} },

    { {4,1}, {1,2}, {2,3} },
    { {4,1}, {2,3}, {1,2} },

    { {0,1}, {4,3}, {1,1} },

    { {0,1}, {1,1}, {4,0} },
    
    { {2, -2}, {1, -1}, {-1,0} }, // data/3/3.0.in
    
    { {0,  7}, {8,  6}, { 8,4} } //from 10.12.in
};

point results[][3] = {
    { {4, 2}, {2, 1}, {1, 3} },
    { {4, 2}, {2, 1}, {1, 3} },

    { {4,1}, {1,2}, {2,3} },
    { {4,1}, {1,2}, {2,3} },

    { {4,3}, {1,1}, {0,1} },
    
    { {4,0}, {0,1}, {1,1} },

    { {2, -2}, {-1, 0}, {1, -1} },

    { {8,  6}, { 8, 4}, {0, 7} } //from 10.12.in
    
};

template<typename array_t>
bool test_basic(point data[][3], point results[][3]){

    for (int i = 0; i<NTESTS; i++){
        array_t points(data[i], data[i] + 3);
        auto ret = base_case( points );


        bool res = std::equal(points.begin(), points.end(), results[i], [](point a, point b){return a.x == b.x && a.y == b.y;} ) ;
        cout<<"case: " <<i<<": "<<(res?"ok":"bad")<<endl;
        
        
    }
    return true;
}


template <typename array_t>
void read( array_t & points){

    string line;
    //turn off sync with c stdio (printf, scanf) - it doubles the time to read in a file
    std::ios::sync_with_stdio(false); 
    std::getline(std::cin, line);
    std::stringstream ss(line);
    int dims, npoints;
    ss >> dims >> npoints;
    
    points.reserve(npoints);

    //read in the points and find the point with lowest y-coordinate
    for (int i = 0; i < npoints; ++i) {
        double x, y;
        std::getline(std::cin, line);
        sscanf(line.c_str(), "%la %la", &x, &y);    
        points.push_back({x, y});
    }
        
}

bool isFloat(double d) {
    double intpart;
    return std::modf(d, &intpart) != 0.0;
}


int main(){
    //test_basic<vector<point>>(data, results);
    vector<point> points;
    read(points);

    std::ranges::sort(points, [](point a, point b){return a.y<b.y || a.y==b.y && a.x<b.x;});

    auto r  = dc(points);

    //set up printing according to floatness
    cout << std::fixed;

    auto hasAtLeastOneFloatOp = [](bool res, point p) { return res || isFloat(p.x) || isFloat(p.y); };
    
    
    bool hasAtLeastOneFloat =
        std::accumulate(points.begin(), points.end(), false, hasAtLeastOneFloatOp);
    if (hasAtLeastOneFloat) {
        cout << std::setprecision(3);
    } else {
        cout << std::setprecision(0);
    }
    
    
    cout<<std::get<0>(r)<<endl;
    for (int i = 0; i< std::get<0>(r); i++){
        cout<<points[i].x <<" "<<points[i].y<<endl;
    }

}
