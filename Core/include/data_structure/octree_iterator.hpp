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

    // return true if we iterate to a children
    // return false if all the octan has been iterated
    bool_t DownTree() const
    {
        uint32_t i = GetNextOctanToIterate();
        if (i == AllOctanHasBeenIterated)
        {
            return false;
        }
        
        OctansState& octanState = GetCurrentOctanState();
        // we update the stae octan
        octanState = static_cast<OctansState>((octanState | 1 << i));

        m_Ptr->GetChildNode(i, m_Ptr);
        PushOctanState(OctansStateZero);

        return true;
    }

    bool_t ClimbTree() const
    {
        // ptr point to the mother node so we end the iteration
        if (m_Ptr->parent == nullptr)
        {
            m_OctanState.pop();
            return false;
        }

        // pop the state of the actual node
        PopOctanState();
        m_Ptr = m_Ptr->parent;
        return true;
    }
    
    Bound GetBound() const;

    void GetHandles(std::vector<typename T::Type*>** handles);

    void GetHandles(std::vector<const typename T::Type*>** handles) const;

    
private:
    mutable PtrType m_Ptr;

    mutable std::stack<XnorCore::OctansState> m_OctanState;

    void PushOctanState(OctansState octancState) const;
    
    OctansState& GetCurrentOctanState() const;
    
    void PopOctanState() const;

    int32_t GetNextOctanToIterate() const;

  
};

template <typename T>
Bound OctreeIterator<T>::GetBound() const
{
    return m_Ptr->boudingBox;
}

template <typename T>
void OctreeIterator<T>::GetHandles(std::vector<typename T::Type*>** handles)
{
    *handles = &m_Ptr->handles;
}

template <typename T>
void OctreeIterator<T>::GetHandles(std::vector<const typename T::Type*>** handles) const
{
    *handles = &m_Ptr->handles;
}

template <typename T>
void OctreeIterator<T>::PushOctanState(OctansState octancState) const
{
    m_OctanState.push(octancState);
}

template <typename T>
OctansState& OctreeIterator<T>::GetCurrentOctanState() const
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
    OctansState currentOctanState = GetCurrentOctanState();
    
    for (int32_t i = 0; i < 8; i++)
    {
        // if 0 so has not been updated
        if ((currentOctanState & (1 << i)) == 0)
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
