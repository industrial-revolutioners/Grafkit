#include "actoranimation.h"

using namespace Grafkit;

#include "animation.h"


Grafkit::ActorAnimation::ActorAnimation() : Animation()
{
	CreateTrack();
}

Grafkit::ActorAnimation::ActorAnimation(ActorRef actor) : Animation(), m_actor(actor)
{
	CreateTrack();
}

void Grafkit::ActorAnimation::Update(double time)
{
	// ... TBD
}

void Grafkit::ActorAnimation::CreateTrack()
{
	m_position = new Animation::Track("Position", "xyz");
	AddTrack(m_position);

	m_rotation = new Animation::Track("Rotation", "rpy");
	AddTrack(m_rotation);

	m_scale = new Animation::Track("Scaling", "xyz");
	AddTrack(m_scale);
}
