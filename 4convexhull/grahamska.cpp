#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <stack>

using std::cout;
using std::endl;
using std::string;

template<typename T>
struct point{
  T x;
  T y;
};

template<typename T>
int crossW(point<T> o, point<T> a, point<T> b){
  int w = (a.x - o.x)*(b.y-o.y) - (a.y-o.y)*(b.x-o.x);
  if (w<0) return -1;
  if (w>0) return 1;
  return 0;
}

template<typename T>
T abs(point<T> p){
  return p.x*p.x + p.y*p.y;
}

template<typename T>
int compare(point<T> u, point<T> v){
  auto w = crossW({0,0}, u, v);
  if (w == 0){
    return abs(u)<abs(v) ? -1 : 1;
  } else {
    return -w;
  }
  
}

template<typename T, typename C>
point<T> next_point(std::vector< point<T> > & heap, C comp ){
  auto ret = heap[0];
  std::pop_heap(heap.begin(), heap.end(), comp);
  heap.pop_back();
  return ret;  
}


int main(){
  string line;
  std::getline(std::cin, line); 
  std::stringstream ss (line);
  int dims, npoints;
  ss>>dims>>npoints;

  std::vector< point<double> > points;

  size_t least_idx;
  point<double> least_y = {0, std::numeric_limits<double>::max()};
  
  for (int i = 0; i<npoints; ++i){
    double x,y;
    string line;
    std::getline(std::cin, line);

    //std::stringstream ss (line);
    //ss>> std::hexfloat >>x>>y;

    sscanf(line.c_str(), "%la %la", &x, &y);  

    if (y < least_y.y){
      least_y = {x,y};
      least_idx = i;
    }
    
    points.push_back( {x,y} );
     
    cout<<"point: "<<x<<", "<<y<<endl;
  }

  std::for_each(points.begin(), points.end(), [least_y](point<auto> &p) { p.y -= least_y.y; p.x -= least_y.x; });  


  auto comp = [](auto &a, auto &b){ return compare(a,b)>0?true:false;  };
  std::make_heap(points.begin(), points.end(), comp);

  std::for_each(points.begin(), points.end(), [](auto p){cout<<"newpoint: "<<p.x<<", "<<p.y<<endl;});


  std::stack<point<double>> H;

  H.push(next_point<>(points, comp));
  H.push(next_point<>(points, comp));
  H.push(next_point<>(points, comp));



  
  cout<<"Alama "<<npoints<<endl;
  return 0;
}
