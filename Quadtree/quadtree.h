#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <Arduino.h>

#undef min
#undef max
#undef test


#include <vector>
#include <stdint.h>

#define CAPACITY 4

using namespace std;

struct Point
{
    float x, y;

    Point(float pos_x = 0.0f, float pos_y = 0.0f){
        x = pos_x;
        y = pos_y;
    };
};

struct Boundary
{
    Point centre;
    float half_size;

    bool contains(const Point& a) const
    {
        if(a.x < centre.x + half_size && a.x > centre.x - half_size)
        {
            if(a.y < centre.y + half_size && a.y > centre.y - half_size)
            {
                return true;
            }
        }
        return false;
    }

    bool intersects(const Boundary& other) const
    {
        //this right > that left                                          this left <s that right
        if(centre.x + half_size > other.centre.x - other.half_size || centre.x - half_size < other.centre.x + other.half_size)
        {
        // This bottom > that top
            if(centre.y + half_size > other.centre.y - other.half_size || centre.y - half_size < other.centre.y + other.half_size)
            {
                return true;
            }
        }
        return false;
    }

    Boundary(float pos_x = 0.0f, float pos_y = 0.0f, float h_size = 0.0f){
        centre = Point(pos_x, pos_y);
        half_size = h_size;
    }

    Boundary(Point p_point, float h_size = 0.0f){
        centre = p_point;
        half_size = h_size;
    }
};

struct Data
{
    Point pos;
    uint16_t range;

    Data(float pos_x = 0.0f, float pos_y = 0.0f, uint16_t area = 0x0){
        pos = Point(pos_x, pos_y);
        range = area;
    }

    bool point_in_area(float lat, float lon) const
    {

        uint8_t ret_val = 0;

        float area_size = (float)((range >> 4)*0.0001f);

        //cout << "Tamaño de area: " << area_size << "\n";

        uint8_t a_flags = range & 0xF;

        //cout << "Banderas: " << (int)(a_flags) << "\n";

        if (lon <= pos.x && lon >= pos.x - area_size){
            ret_val |= 0x6;
            //cout << "1, 2\n";
            //cout << "ret_val: " << (int)(ret_val) << "\n";
        } else if (lon > pos.x && lon <= pos.x + area_size){
            ret_val |= 0x9;
            //cout << "0, 3\n";
            //cout << "ret_val: " << (int)(ret_val) << "\n";
        }

        if (lat <= pos.y && lat > pos.y - area_size){
            ret_val &= 0xC;
            //cout << "2, 3\n";
            //cout << "ret_val: " << (int)(ret_val) << "\n";
        } else if (lat > pos.y && lat < pos.y + area_size){
            ret_val &= 0x3;
            //cout << "0, 1\n";
            //cout << "ret_val: " << (int)(ret_val) << "\n";
        }

        //cout << "Areas evaluadas: " << (int)(ret_val) << "\n";

        return ((ret_val & a_flags) != 0);

    }
};

class Quadtree
{
    private:
        //4 children
        Quadtree* nw;
        Quadtree* ne;
        Quadtree* sw;
        Quadtree* se;

        Boundary boundary;

        vector<Data> objects;

    public:

        Quadtree();
        Quadtree(Boundary boundary);

        ~Quadtree();

        bool insert(Data d);
        bool insert(float pos_x, float pos_y, uint16_t area);
        void subdivide();
        vector<Data> query_range(Boundary range) const;
        vector<Data> nearest_points(float lat, float lon, float los);

};

#endif
