#include "scene/component/test_component_animation.hpp"

#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

TestComponentAnimation::TestComponentAnimation()
    : m_Renderer(nullptr), m_Montage(new AnimationMontage())
{
}

TestComponentAnimation::~TestComponentAnimation()
{
    delete m_Montage;
}

void TestComponentAnimation::Begin()
{
}

void TestComponentAnimation::Update()
{
}
