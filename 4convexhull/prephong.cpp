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
#include <tuple>
#include <cassert>
#include <ranges>

using std::cout;
using std::double_t;
using std::endl;
using std::string;
using std::vector;


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

std::tuple<size_t, size_t> base_case(vector<point> & points){
    sort (points.begin(), points.end(), [](auto p1, auto p2){return p1.x>p2.x || (p1.x == p2.x && p1.y>p2.y); } );
    size_t s = points.size();
    assert( points.size() >= 2);
    if (points.size() == 2) return {2,1};

    double_t k1 = k(points[0], points[1]);
    double_t k2 = k(points[0], points[2]);

    //double intersect_y = points[0].y + k1*points[1].x; // if (points[1].y > intersect_y)
     
    assert( points.size() == 3 );
    if ( k1<k2 ){ 
        std::swap(points[1], points[2]);
        return  { 3, 1 } ;
    } else {
        return  { 3, 2 };
    }

    
}

std::tuple<size_t, size_t>
dc(vector<point> &points) {

    if (points.size()<=3) return base_case(points);

    
    return {0,0};
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
