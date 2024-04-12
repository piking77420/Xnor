#pragma once

#include <stack>

#include "core.hpp"
#include "utils/concepts.hpp"
#include "octree_node.hpp"

BEGIN_XNOR_CORE



enum class State
{
    Iteration,
    EndTree,
};

// if byte Zero Has not been iterated
// if byte at 1 then has been iterated
enum OctansState : uint8_t
{
     OctansStateZero = 0x00,// 0b00000000
     OctansStateQ1 = 0x01, // 0b00000001
     OctansStateQ2 = 0x02, // 0b00000010
     OctansStateQ3 = 0x04, // 0b00000100
     OctansStateQ4 = 0x08, // 0b00001000
     OctansStateQ5 = 0x10, // 0b00010000
     OctansStateQ6 = 0x20, // 0b00100000
     OctansStateQ7 = 0x40, // 0b01000000
     OctansStateQ8 = 0x80, // 0b10000000
};



template <typename  T>
class OctreeIterator
{
public:
    using Type = T;
    using PtrType = T*;
    using RefType = T&;

    static constexpr int32_t AllOctanHasBeenIterated = 8;
    
    
    OctreeIterator() = default;
    
    OctreeIterator(PtrType ptr) : m_Ptr(ptr)
    {
        PushOctanState(OctansStateZero);
    } 

    ~OctreeIterator() = default;

    // Return true if Pass to the end
    bool_t operator++() const
    {
        if (!m_Ptr)
            throw std::exception("Ptr is null");

        
        // 1. Check the next octan to iterate
        // 2. if the current octant must be iterate Check if the current Element octan is valid
            // if true push octan repeat
            // if else try next octan
            // if end then go back to parent
        uint32_t i = GetNextOctanToIterate();

        // We Iterate in each octant
        if (i == AllOctanHasBeenIterated)
        {
            if (m_Ptr->m_Parent == nullptr)
            {
               // ptr is the mother node so we check all the tree Return End
               return true;
            }
            m_Ptr = m_Ptr->m_Parent; 
            PopOctanState();
            
        }
        else
        {
            m_Ptr->GetChildNode(i, m_Ptr);
            return true;
        }
        
        
        return false;
    }

    Bound GetBound() const;

    void GetHandles(std::vector<typename T::Type>* handles);
    
private:
    PtrType m_Ptr;

    mutable std::stack<XnorCore::OctansState> m_OctanState;

    void PushOctanState(OctansState octancState) const;
    
    OctansState GetOctanState() const;
    
    void PopOctanState() const;

    int32_t GetNextOctanToIterate() const; 
};

template <typename T>
Bound OctreeIterator<T>::GetBound() const
{
    return m_Ptr->boudingBox;
}

template <typename T>
void OctreeIterator<T>::GetHandles(std::vector<typename T::Type>* handles)
{
    *handles = m_Ptr.handles;
}

template <typename T>
void OctreeIterator<T>::PushOctanState(OctansState octancState) const
{
    m_OctanState.push(octancState);
}

template <typename T>
OctansState OctreeIterator<T>::GetOctanState() const
{
   return m_OctanState.top();
}

template <typename T>
void OctreeIterator<T>::PopOctanState() const
{
    m_OctanState.pop();
}

template <typename T>
int32_t OctreeIterator<T>::GetNextOctanToIterate() const
{
    OctansState currentOctanState = GetOctanState();
    
    for (int32_t i = 0; i < 8; i++)
    {
        // if 0 so has not been updated
        if (!(currentOctanState & (1 << i)))
        {
            // Check if is valid
            if (T::IsOctanValid(m_Ptr->GetActiveOctans(),i))
            {
                return i;
            }
            
        }
    }
    
    return AllOctanHasBeenIterated;
}


END_XNOR_CORE
