#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#include <vector>
#include <iostream>

#include <SFML/System.hpp>

// Source: http://codereview.stackexchange.com/questions/84374/quadtree-implementation

const int CAPACITY = 4;

struct AABB
{
    sf::Vector2f centre;
    sf::Vector2f halfSize;

    AABB(sf::Vector2f centre = sf::Vector2f(), sf::Vector2f halfSize = sf::Vector2f()): centre(centre), halfSize(halfSize){};

    bool contains(sf::Vector2f& a)
    {
        if(a.x < centre.x + halfSize.x && a.x > centre.x - halfSize.x)
        {
            if(a.y < centre.y + halfSize.y && a.y > centre.y - halfSize.y)
            {
                return true;
            }
        }
        return false;
    }

    bool intersects(AABB other)
    {
        //this right > that left                                          this left <s that right
        if(centre.x + halfSize.x > other.centre.x - other.halfSize.x || centre.x - halfSize.x < other.centre.x + other.halfSize.x)
        {
        // This bottom > that top
            if(centre.y + halfSize.y > other.centre.y - other.halfSize.y || centre.y - halfSize.y < other.centre.y + other.halfSize.y)
            {
                return true;
            }
        }
        return false;
    }
};

template <typename T>
struct Data
{
    sf::Vector2f pos;
    T* load;

    Data(sf::Vector2f pos = sf::Vector2f(), T* data = nullptr): pos(pos), load(data){};
};


template <class T>
class Quadtree
{
    private:
        //4 children
        Quadtree* nw;
        Quadtree* ne;
        Quadtree* sw;
        Quadtree* se;



        std::vector< Data<T> > objects;


    public:
        AABB boundary; // Modified

        Quadtree<T>();
        Quadtree<T>(AABB boundary);

        ~Quadtree();

        bool insert(Data<T> d);
        void subdivide();
        void clear();
        std::vector< Data<T> > queryRange(AABB range);
};

template <class T>
Quadtree<T>::Quadtree():
    nw{nullptr},
    ne{nullptr},
    sw{nullptr},
    se{nullptr}
{
    boundary = AABB();
    objects = std::vector< Data<T> >();
}

template <class T>
Quadtree<T>::Quadtree(AABB boundary):
    nw{nullptr},
    ne{nullptr},
    sw{nullptr},
    se{nullptr}
{
    objects = std::vector< Data<T> >();
    this->boundary = boundary;
}

template <class T>
Quadtree<T>::~Quadtree()
{
    delete nw;
    delete sw;
    delete ne;
    delete se;
}

template <class T>
void Quadtree<T>::subdivide()
{
    sf::Vector2f qSize = {boundary.halfSize.x/2, boundary.halfSize.y/2};
    sf::Vector2f qCentre = {boundary.centre.x - qSize.x, boundary.centre.y - qSize.y};
    nw = new Quadtree(AABB(qCentre, qSize));

    qCentre = sf::Vector2f(boundary.centre.x + qSize.x, boundary.centre.y - qSize.y);
    ne = new Quadtree(AABB(qCentre, qSize));

    qCentre = sf::Vector2f(boundary.centre.x - qSize.x, boundary.centre.y + qSize.y);
    sw = new Quadtree(AABB(qCentre, qSize));

    qCentre = sf::Vector2f(boundary.centre.x + qSize.x, boundary.centre.y + qSize.y);
    se = new Quadtree(AABB(qCentre, qSize));
}

template <class T>
void Quadtree<T>::clear()
{
    objects.clear();
    delete nw;
    delete sw;
    delete ne;
    delete se;
    nw = nullptr;
    ne = nullptr;
    sw = nullptr;
    se = nullptr;
}

template <class T>
bool Quadtree<T>::insert(Data<T> d)
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

template <class T>
std::vector< Data<T> > Quadtree<T>::queryRange(AABB range) // TODO: Make a second query range, with a min and max range. Used for when you need to search outwards continuously until something is found.
{
    std::vector< Data<T> > pInRange = std::vector< Data<T> >();

    if(!boundary.intersects(range)) // For the todo, here is the 'max', do another as true for the 'min'
    {
        return pInRange;
    }

    for(auto&& object: objects)
    {
        if(range.contains(object.pos))
        {
            pInRange.push_back(object);
        }
    }

    if(nw == nullptr)
    {
        return pInRange;
   }

    std::vector< Data<T> > temp = nw->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    temp = ne->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    temp = sw->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    temp = se->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    return pInRange;
}





#endif // QUADTREE_H_INCLUDED
