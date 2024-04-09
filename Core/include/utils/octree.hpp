#pragma once
#include <queue>

#include "scene/entity.hpp"

BEGIN_XNOR_CORE

template<class T,float_t _minSize>
class Octree
{
private:
    static std::queue<T> m_objectQueu; 
public:

    static bool_t m_TreeReady = false;
    static bool_t m_TreeBuilt = false;
    
    enum class Octans : uint8_t
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

    Octans activeOctans;
    
    Bound boundRegion;
    
    std::vector<T> objectsList;

    Octree* parent = nullptr;

    DEFAULT_COPY_MOVE_OPERATIONS(Octree)
    
    Octree() = default;

    Octree(const std::vector<T>& object,const Bound& boundingRegion);
    
    ~Octree() = default;

    void Update();

    
private:
    int32_t m_MaxLifespan = 8; // 
    int32_t m_CurLife = -1;

  
};

template <class T, float_t _minSize>
Octree<T, _minSize>::Octree(const std::vector<T>& objects, const Bound& bounding) : objectsList(objects) , boundRegion(bounding)
{
    
}

template <class T, float_t _minSize>
void Octree<T, _minSize>::Update()
{
    if (!m_TreeBuilt) 
    { 
        while (m_objectQueu.Count != 0) 
            objectsList.push_back(m_objectQueu.Dequeue()); 
        BuildTree(); 
    } 
    else
    { 
        while (m_objectQueu.Count != 0) 
            Insert(m_pendingInsertion.Dequeue()); 
    } 
    m_TreeReady = true;
}


END_XNOR_CORE
