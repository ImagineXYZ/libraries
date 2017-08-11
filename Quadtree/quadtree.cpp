#include "quadtree.h"

namespace std {
  void __throw_length_error(char const*) {
  }
  void __throw_bad_alloc(){
  }
}

using namespace std;

Quadtree::Quadtree():
    nw{nullptr},
    ne{nullptr},
    sw{nullptr},
    se{nullptr}
{
    this->boundary = Boundary(Boundary(0.0f, 0.0f, 180.0f));
}


Quadtree::Quadtree(Boundary boundary):
    nw{nullptr},
    ne{nullptr},
    sw{nullptr},
    se{nullptr}
{
    this->boundary = boundary;
}


Quadtree::~Quadtree()
{
    delete nw;
    delete sw;
    delete ne;
    delete se;
}

void Quadtree::subdivide()
{
    float q_size = boundary.half_size;
    Point q_center = Point(boundary.centre.x - q_size, boundary.centre.y - q_size);

    nw = new Quadtree(Boundary(q_center, q_size));

    q_center = Point(boundary.centre.x + q_size, boundary.centre.y - q_size);
    ne = new Quadtree(Boundary(q_center, q_size));

    q_center = Point(boundary.centre.x - q_size, boundary.centre.y + q_size);
    sw = new Quadtree(Boundary(q_center, q_size));

    q_center = Point(boundary.centre.x + q_size, boundary.centre.y + q_size);
    se = new Quadtree(Boundary(q_center, q_size));
}

bool Quadtree::insert(Data d)
{
    if(!boundary.contains(d.pos))
    {
        return false;
    }

    if(objects.size() < CAPACITY)
    {
        objects.push_back(d);
        return true;
    }

    if(nw == nullptr)
    {
        subdivide();
    }

    if(nw->insert(d))
    {
        return true;
    }
    if(ne->insert(d))
    {
        return true;
    }
    if(sw->insert(d))
    {
        return true;
    }
    if(se->insert(d))
    {
        return true;
    }

    return false;   
}

bool Quadtree::insert(float pos_x, float pos_y, uint16_t area){
    return this->insert(Data(pos_x,pos_y,area));
}

vector<Data> Quadtree::query_range(Boundary range) const
{
    vector<Data> p_in_range = vector<Data>();

    if(!boundary.intersects(range))
    {
        return p_in_range;
    }

    for(int i = 0; i < objects.size(); i++)
    {
        if(range.contains(objects.at(i).pos))
        {
            p_in_range.push_back(objects.at(i));
        }
    }

    if(nw == nullptr)
    {
        return p_in_range;
   }

    vector<Data> temp = nw->query_range(range);
    p_in_range.insert(p_in_range.end(), temp.begin(), temp.end());

    temp = ne->query_range(range);
    p_in_range.insert(p_in_range.end(), temp.begin(), temp.end());

    temp = sw->query_range(range);
    p_in_range.insert(p_in_range.end(), temp.begin(), temp.end());

    temp = se->query_range(range);
    p_in_range.insert(p_in_range.end(), temp.begin(), temp.end());

    return p_in_range;
}

vector<Data> Quadtree::nearest_points(float lat, float lon, float los){
    return this->query_range(Boundary(lon, lat, los));
}