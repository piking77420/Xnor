#pragma once
#include <queue>
#include <rendering/draw_gizmo.hpp>

#include "scene/entity.hpp"

BEGIN_XNOR_CORE

template<class T>
struct ObjectBounding
{
    T handle;
    Bound bound;

    DEFAULT_COPY_MOVE_OPERATIONS(ObjectBounding)

    bool operator==(const ObjectBounding& other) const
    {
        return bound == other.bound;
    }

    ObjectBounding() = default;

    ~ObjectBounding() = default;
};

template<class T,float_t MinSize>
class Octree
{
public:
   
    
private:
    static inline std::queue<ObjectBounding<T>> m_ObjectQueue;

public:
  
    
    static inline bool_t treeReady = false;
    static inline bool_t treeBuilt = false;
    
    enum Octans : uint8_t
    {
        Q1 = 0x01, // 0b00000001
        Q2 = 0x02, // 0b00000010
        Q3 = 0x04, // 0b00000100
        Q4 = 0x08, // 0b00001000
        Q5 = 0x10, // 0b00010000
        Q6 = 0x20, // 0b00100000
        Q7 = 0x40, // 0b01000000
        Q8 = 0x80, // 0b10000000
    };

    uint32_t activeOctans = 0;
    
    Bound boundRegion;

    bool_t draw;
    
    std::vector<ObjectBounding<T>> objectsList;

    std::array<Octree*,8> childNodes; 

    Octree* parent = nullptr;

    DEFAULT_COPY_MOVE_OPERATIONS(Octree)
    
    Octree();

    Octree(const std::vector<ObjectBounding<T>>& object,const Bound& boundingRegion);
    
    ~Octree();

    void Update();

    void FindEnclosingCube();
    
    void Build();

    void Draw();
    
private:
    int32_t m_MaxLifespan = 8; // 
    int32_t m_CurLife = -1;

    void CreateOctant(std::array<Bound,8>& bounds);
    
};

template <class T, float_t MinSize>
Octree<T, MinSize>::Octree()
{
   
}

template <class T, float_t MinSize>
Octree<T, MinSize>::Octree(const std::vector<ObjectBounding<T>>& objects, const Bound& bounding) : objectsList(objects) , boundRegion(bounding)
{
}

template <class T, float_t MinSize>
Octree<T, MinSize>::~Octree()
{
      for (uint32_t i = 0; i < 8; ++i)
      {
          if (activeOctans & (1 << i))
          {
               delete childNodes[i];
          }
      }
}

template <class T, float_t MinSize>
void Octree<T, MinSize>::   Update()
{
    if (!treeBuilt) 
    { 
        while (m_ObjectQueue.size() != 0)
        {
            objectsList.push_back(m_ObjectQueue.front());
            m_ObjectQueue.pop();
        }
            
        Build(); 
    } 
    else
    { 
        //while (m_ObjectQueue.Count != 0) 
            //Insert(m_pendingInsertion.Dequeue()); 
    } 
    treeReady = true;
}

template <class T, float_t MinSize>
void Octree<T, MinSize>::FindEnclosingCube()
{
	
    for (ObjectBounding<T>& obj : objectsList)  
    {
       boundRegion.Encapsulate(obj.bound);
    }
    
    const Vector3 previousSize = boundRegion.GetSize();
    const float_t maxSize = std::max( { previousSize.x , previousSize.y ,previousSize.z });
    const Vector3 size = Vector3(maxSize) * 0.5f;
    boundRegion.SetMinMax(boundRegion.center - size,boundRegion.center + size);

}

template <class T, float_t MinSize>
void Octree<T, MinSize>::Build()
{
    
    
    Vector3 dimensions =  boundRegion.GetSize();
    if (dimensions == Vector3::Zero())
    {
        FindEnclosingCube();
        dimensions = boundRegion.GetSize();
    }
	
    //Check to see if the dimensions of the box are greater than the minimum dimensions
    if (dimensions.x <= MinSize)
    {
        return;
    }
    
    std::array<Bound,8> octant;
    CreateOctant(octant);
    
    //This will contain all of our objects which fit within each respective octant.
    std::vector<ObjectBounding<T>> octList[8];
    
    //this list contains all of the objects which got moved down the tree and can be delisted from this node.
     std::vector<ObjectBounding<T>> delist;
	
    for (ObjectBounding<T>& obj : objectsList)  
    {
        for (int a = 0; a < 8; a++)
        {
            if (octant[a].Countains(obj.bound))
            {
                octList[a].push_back(obj);
                delist.push_back(obj);
                break;
            
            }
        }
    }
	
    //delist every moved object from this node.
    for (ObjectBounding<T>& deleteObjectBounding : delist)
    {
        std::erase(objectsList, deleteObjectBounding);
    }
		
    //Create child nodes where there are items contained in the bounding region
    for (int a = 0; a < 8; a++)
    {
        if (octList[a].size() != 0)
        {
            childNodes[a] = new Octree(octList[a],octant[a]);
            activeOctans = activeOctans | (1 << a);
            childNodes[a]->parent = this;
            childNodes[a]->Build();
        }
    }
	
    treeBuilt = true;
    treeReady = true;
    
}

template <class T, float_t MinSize>
void Octree<T, MinSize>::Draw()
{
    DrawGizmo::Rectangle(boundRegion.center, boundRegion.extents, Colorf::Green());
    
    for (uint32_t i = 0; i < 8; ++i)
    {
        if (activeOctans & (1 << i))
        {
            childNodes[i]->Draw();
        }
    }
}

template <class T, float_t MinSize>
void Octree<T, MinSize>::CreateOctant(std::array<Bound,8>& bounds)
{
    /*
    const float_t quarter = boundRegion.GetSize().x * 0.25f;
    const Vector3 childLength = boundRegion.GetSize() * 0.5f;
    const Vector3 childSize = Vector3(childLength);
    const Vector3 center = boundRegion.center;
    
    bounds[0] = Bound(center + Vector3(-quarter,quarter,-quarter), childSize ); 
    bounds[1] = Bound(center + Vector3(quarter,quarter,-quarter), childSize ); 
    bounds[2] = Bound(center + Vector3(-quarter,quarter,quarter), childSize ); 
    bounds[3] = Bound(center + Vector3(quarter,quarter,quarter), childSize ); 
    bounds[4] = Bound(center + Vector3(-quarter,-quarter,-quarter), childSize ); 
    bounds[5] = Bound(center + Vector3(quarter,-quarter,-quarter), childSize ); 
    bounds[6] = Bound(center + Vector3(-quarter,-quarter,quarter), childSize ); 
    bounds[7] = Bound(center + Vector3(quarter,-quarter,quarter), childSize );*/

    
    Vector3 min = boundRegion.GetMin();
    Vector3 max = boundRegion.GetMax();
    Vector3 center = boundRegion.center;

    bounds[0].SetMinMax(min, center);
    bounds[1].SetMinMax(Vector3(center.x, min.y, min.z), Vector3(max.x, center.y, center.z));
    bounds[2].SetMinMax(Vector3(center.x, min.y, center.z), Vector3(max.x, center.y, max.z));
    bounds[3].SetMinMax(Vector3(min.x, min.y, center.z), Vector3(center.x, center.y, max.z));
    bounds[4].SetMinMax(Vector3(min.x, center.y, min.z), Vector3(center.x, max.y, center.z));
    bounds[5].SetMinMax(Vector3(center.x, center.y, min.z), Vector3(max.x, max.y, center.z));
    bounds[6].SetMinMax(center, max);
    bounds[7].SetMinMax(Vector3(min.x, center.y, center.z), Vector3(center.x, max.y, max.z));
}


END_XNOR_CORE
