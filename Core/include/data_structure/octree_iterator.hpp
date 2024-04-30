﻿#pragma once

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
     OctansStateFill = 0xFF
};



template <typename  T>
class OctreeIterator
{
public:
    using Type = T;
    using PtrType = T*;
    using RefType = T&;
    
    
    OctreeIterator() = default;
    
    OctreeIterator(PtrType ptr) : m_Ptr(ptr)
    {
        PushOctanState(OctansStateZero);
    } 

    ~OctreeIterator() = default;

    bool_t Iterate() const
    {
        OctansState& state = GetCurrentOctanState();
        int32_t indexBit = GetNextOctanToIterate(state);


        if (state != OctansStateFill)
        {
            m_Ptr->SetPtrToChildNode(indexBit,&m_Ptr);
            DownTree(indexBit);
            
            return true;
        }
        
        // found a valid octant in parent 
        while (true)
        {
            if (!ClimbTree())
                return false;
            
            state = GetCurrentOctanState();
            indexBit = GetNextOctanToIterate(state);
            
            if (state != OctansStateFill)
            {
                m_Ptr->SetPtrToChildNode(indexBit, &m_Ptr);
                DownTree(indexBit);
            
                return true;
            }

        
        }
        
        // trying climbing but ptr is mother node
        return false;
    }
    
    Bound GetBound() const;

    void GetHandles(std::vector<typename T::Type*>** handles);

    void GetHandles(std::vector<const typename T::Type*>** handles) const;

    
private:
    // return true if we iterate to a children
    // return false if all the octan has been iterated
    void DownTree(uint32_t childindex) const
    {
        PushOctanState(OctansState::OctansStateZero);
        m_Ptr->SetPtrToChildNode(static_cast<size_t>(childindex),&m_Ptr);
    }

    // Return false if has no parent iterator = mother node
    bool_t ClimbTree() const
    {
        if (m_Ptr->parent == nullptr)
        {
            m_OctanState.pop();
            return false;
        }
        m_Ptr = m_Ptr->parent;
        m_OctanState.pop();
        
        return true;
    }
    

    
    mutable PtrType m_Ptr;

    mutable std::stack<XnorCore::OctansState> m_OctanState;

    void PushOctanState(OctansState octancState) const;
    
    OctansState& GetCurrentOctanState() const;
    
    
    void PopOctanState() const;

    int32_t GetNextOctanToIterate(OctansState& state) const;

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
int32_t OctreeIterator<T>::GetNextOctanToIterate(OctansState& state) const
{
    for (int32_t i = 0; i < 8; i++)
    {
        if ((state & (1 << i)))
            continue;
        
        state = static_cast<OctansState>((state | 1 << i));
        // Check if is valid if true return the octan else set the bit has been iterate but continue to iterate in order to found a valid octan
        if (T::IsOctanValid(m_Ptr->GetActiveOctans(),i))
        {
            return i;
        }
        
    }
    
    return -1;
}


END_XNOR_CORE
