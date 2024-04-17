#pragma once

#include <stack>

#include "core.hpp"
#include "utils/concepts.hpp"
#include "octree_node.hpp"

BEGIN_XNOR_CORE
template <class T, class U>
concept ObjectBoundingT = Concepts::IsTSameAsU<OctreeNode<T>,U>;

template <ObjectBoundingT T>
class OctreeIterator
{
public:
    using Type = T;
    using PtrType = T*;
    using RefType = T&;

    OctreeIterator() = default;
    
    OctreeIterator(PtrType ptr) : m_Ptr(ptr)
    {
        m_NodesActiveOctans.push(ptr->GetActiveOctans());
    } 

    ~OctreeIterator() = default;

    Bound operator++() const
    {
        Bound returnBoud = m_Ptr.GetBound();
        return returnBoud;
    }

    
private:
    PtrType m_Ptr;
    std::stack<XnorCore::Octans> m_NodesActiveOctans;
};

END_XNOR_CORE