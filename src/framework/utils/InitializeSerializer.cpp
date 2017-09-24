#include "stdafx.h"

#include "persistence/dynamics.h"
#include "persistence/persistence.h"
#include "InitializeSerializer.h"

#include "render/predefs.h"

#include "render/mesh.h"
#include "render/model.h"

#include "render/Light.h"
#include "render/camera.h"

#include "render/animation.h"

#define ADD_CLONABLE(_name, _clazzname)\
	Grafkit::AddClonable _name(#_clazzname, new _clazzname::Factory())

Grafkit::ClonableInitializer::ClonableInitializer()
{
	ADD_CLONABLE(mesh, Grafkit::Mesh);
	ADD_CLONABLE(model, Grafkit::Model);
	ADD_CLONABLE(material, Grafkit::Material);
	ADD_CLONABLE(camera, Grafkit::Camera);
	ADD_CLONABLE(light, Grafkit::Light);
	ADD_CLONABLE(actorAnimation, Grafkit::ActorAnimation);
}