#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
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

vector<double_t> create_ks( vector<point> p){
    vector<double_t> ret;
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

int include_points( vector<point>& to_array, vector<point> from_array, size_t from_idx, size_t size )
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

template<typename T>
size_t leftmost(const T& p){
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

std::tuple<size_t, size_t> base_case(vector<point> & points){
    std::ranges::sort (points, [](auto p1, auto p2){ return p1.x>p2.x || p1.x == p2.x && p1.y>p2.y; } );
    size_t n = points.size();
    assert( n > 0 );
    if (n < 3) {
        return {n, n-1};
    }

    double_t k1 = k(points[0], points[1]);
    double_t k2 = k(points[0], points[2]);

    assert( n == 3 );
    if ( k1<k2 ){ 
        std::swap(points[1], points[2]);
        return  { n, n-2 } ;
    } else {
        return  { n, n-1 };
    }
}


// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case1(VP a, size_t na, VP b, size_t nb, VD alfas, VD betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;
    while (true){
        double_t gamma = k(a[i], b[j]);
        if ( ( alfas[i] > gamma || std::isinf(alfas[i]) ) && i<iL  ){
            i++;
        } else if ( ( betas[j] > gamma || std::isinf(betas[j]) ) && j<jL  ){
            j++;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;

    while (true){
        double_t gamma = k(a[i], b[j]);
        if (  betas[j] > gamma  && j!=0  ){
            j = (j+1) % nb;
        } else if ( alfas[i] > gamma  && i != 0  ){
            i = (i+1) % na;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}


// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case2(VP a, size_t na, VP b, size_t nb, VD alfas, VD betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;
    while (true){
        double_t gamma = k(a[i], b[j]);
        if ( ( alfas[i] > gamma || std::isinf(alfas[i]) ) && i<iL  ){
            i++;
        } else if ( ( betas[j] > gamma || std::isinf(betas[j]) ) && j<jL  ){
            j++;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;

    while (true){
        double_t gamma = k(a[i], b[j]);
        size_t ak = (na+i-1) % na;
        size_t bk = (nb+i-1) % nb;
        
        if (  std::isfinite(alfas[ak]) && alfas[ak] < gamma && i != 0  ){
            i = ak;
        } else if ( betas[bk] < gamma  && j != 0  ){
            j = bk;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}

// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case3(VP a, size_t na, VP b, size_t nb, VD alfas, VD betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;

    while (true){
        double_t gamma = k(a[i], b[j]);
        size_t ak = (na+i-1) % na;
        size_t bk = (nb+i-1) % nb;
        
        if ( betas[bk] < gamma  && j != jL  ) {
            j = bk;
        } else if (alfas[ak] < gamma  && i != iL  ){
            i = ak;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;
    
    
    while (true){
        double_t gamma = k(a[i], b[j]);
        if ( betas[j] > gamma && j!=0  ){
            j = (j+1) % nb;
        } else if ( alfas[i] > gamma && i!=0  ){
            i = (i+1) % na;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}


// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case4(VP a, size_t na, VP b, size_t nb, VD alfas, VD betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;
    while (true){
        double_t gamma = k(a[i], b[j]);
        size_t ak = (na+i-1) % na;
        size_t bk = (nb+i-1) % nb;

        if ( betas[bk] < gamma && j!=jL   ){
            j = bk;
        } else if ( alfas[ak] < gamma && i!=iL  ){
            i = ak;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;

    while (true){
        double_t gamma = k(a[i], b[j]);
        size_t ak = (na+i-1) % na;
        size_t bk = (nb+i-1) % nb;
        
        if (  std::isfinite(alfas[ak]) && alfas[ak] < gamma && i != 0  ){
            i = ak;
        } else if ( std::isfinite(betas[bk]) && betas[bk] < gamma && j != 0  ){
            j = bk;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}



std::tuple<size_t, size_t>
dc(vector<point> &points) {

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

    vector<point> a = vector<point>(points.begin(), points.begin()+na);
    vector<point> b = vector<point>(points.begin()+na, points.end() );
    auto ra = dc(a);
    auto rb = dc(b);
    na = std::get<0>(ra);
    nb = std::get<0>(rb);
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

    vector<point> q;
    size_t n = add(0,j_i_stars[0],j_i_stars[2],q,a,na);
           n = add(n,j_i_stars[1],j_i_stars[2],q,b,nb);
    size_t j = 0;
    for(int k = 0; k < n; k++){
            if(q[k].x > q[j].x || q[k].x == q[j].x && q[k].y > q[j].y){
                j = k;
            }
    }
    n = include_points(points,q,j,n);
    size_t i = leftmost(points);
    return{n,i};
}


const int NTESTS = 7;

point data[NTESTS][3] = {
    { {2, 1}, {1, 3}, {4, 2},},   
    { {4, 2}, {1, 3}, {2, 1} },

    { {4,1}, {1,2}, {2,3} },
    { {4,1}, {2,3}, {1,2} },

    { {0,1}, {4,3}, {1,1} },

    { {0,1}, {1,1}, {4,0} },
    
    { {2, -2}, {1, -1}, {-1,0} }, // data/3/3.0.in
};

point results[][3] = {
    { {4, 2}, {2, 1}, {1, 3} },
    { {4, 2}, {2, 1}, {1, 3} },

    { {4,1}, {1,2}, {2,3} },
    { {4,1}, {1,2}, {2,3} },

    { {4,3}, {1,1}, {0,1} },
    
    { {4,0}, {0,1}, {1,1} },

    { {2, -2}, {-1, 0}, {1, -1} },
};


bool test_basic(point data[][3], point results[][3]){

    for (int i = 0; i<NTESTS; i++){
        vector<point> points(data[i], data[i] + 3);
        auto ret = base_case( points );


        bool res = std::equal(points.begin(), points.end(), results[i], [](point a, point b){return a.x == b.x && a.y == b.y;} ) ;
        cout<<"case: " <<i<<": "<<(res?"ok":"bad")<<endl;
        
        
    }
    return true;
}


template <typename T>
void read(std::vector<T>& points){

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


int main(){
    test_basic(data, results);
    vector<point> points;
    read(points);

    std::ranges::sort(points, [](point a, point b){return a.y<b.y;});

    auto r  = dc(points);
    
    cout<<std::get<0>(r)<<endl;
    for (int i = 0; i< std::get<0>(r); i++){
        cout<<points[i].x <<" "<<points[i].y<<endl;
    }

}
