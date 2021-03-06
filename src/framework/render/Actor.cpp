#include "stdafx.h"
#include "Actor.h"
#include "SceneGraph.h"

PERSISTENT_IMPL(Grafkit::Actor);

using namespace Grafkit;

// =================================================================

Actor::Actor() : Object()
    , m_isHidden(false) 
{
}

Actor::Actor(const Entity3DRef & entity) : Object()
    , m_isHidden(false)
{
    AddEntity(entity);
}

Actor::~Actor() {
}

void Actor::Render(Renderer& render, SceneGraph& scene) { for (size_t i = 0; i < this->m_pEntities.size(); i++) { m_pEntities[i]->Render(render, scene); } }

void Actor::AddChild(const ActorRef & child)
{
    m_pChildren.push_back(child);
    //child->m_pParent = this;
}