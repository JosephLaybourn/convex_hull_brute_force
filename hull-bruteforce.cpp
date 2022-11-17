#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream>

bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

// returns a set of indices of points that form convex hull
// for each pair points determine whether all other points are one side of the
// line formed by the pair of points. If it does - add the points (or rather 
// indices to the hull). Since hull is represented by a std::set, you do not have 
// to worry about duplicates.
std::set<int> hullBruteForce ( std::vector< Point > const& points ) 
{
	int num_points = points.size();
	//std::cout << "number of points " << num_points << std::endl;
	if ( num_points < 3 ) throw "bad number of points";

	std::set<int> hull_indices;

  for(int i = 0; i < num_points; ++i)
  {
    for(int j = 0; j < num_points; ++j)
    {
      //std::cout << "Testing points " << points[i] << points[j] << std::endl;
      int k;
      for(k = 0; k < num_points; ++k)
      {
        //std::cout << "Against point " << points[k] << std::endl;
        if(points[i] == points[j])
        {
          //std::cout << "FAIL" << std::endl;
          break;
        }
        if(get_location(points[i].x, points[i].y, points[j].x, points[j].y, points[k].x, points[k].y).first)
        {
          //std::cout << "FAIL" << std::endl;
          break;
        }
      }
      if(k == num_points)
      {
        //std::cout << "SUCCESS" << std::endl;
        hull_indices.insert(i);
        hull_indices.insert(j);
      }
    }
  }
		
	return hull_indices;
}

// find the first point that is in the hull (smallest or biggest x or y coordinate),
// then find the next vertex of the hull in counter-clockwise order by considering all lines through the
// previous vertex and requiring that there are no points to the right of it.
std::vector<int> hullBruteForce2 ( std::vector< Point > const& points ) 
{
	int num_points = points.size();
	if ( num_points < 3 ) throw "bad number of points";
	std::vector<int> hull_indices;

  int originIndex = 0;
  
  // finds smallest x
  for(int i = 0; i < num_points; ++i)
  {
    if(points[i].x < points[originIndex].x)
    {
      originIndex = i;
    }
  }
  //hull_indices.push_back(originIndex);

  int point1 = originIndex;
  int point2;

  do
  {
    
    for(point2 = 0; point2 < num_points; ++point2)
    {
      //std::cout << "Testing Line: " << points[point1] << points[point2] << std::endl;
      if(point2 == point1)
      {
        continue;
      }
      int i;
      for(i = 0; i < num_points; ++i)
      {
        //std::cout << "Against: " << points[i] << std::endl;
        if(point1 == i || point2 == i)
        {
          continue;
        }
        
        if(get_location(points[point1].x, points[point1].y, points[point2].x, points[point2].y, points[i].x, points[i].y).second)
        {
          //std::cout << "FAIL" << std::endl;
          break;
        }
      }
      if(i == num_points)
      {
        //std::cout << "SUCCESS" << std::endl;
        // if(point2 == originIndex)
        // {
        //   break;
        // }
        hull_indices.push_back(point2);
        //int originSaved = originIndex;
        point1 = point2;
        //i = 0;
        break;
      }
    }
    //std::cout << "===========================================================================================" << std::endl;
    //std::cout << "point1: " << points[point1] << std::endl;
  }
  while(point2 != originIndex);

	return hull_indices;
}
