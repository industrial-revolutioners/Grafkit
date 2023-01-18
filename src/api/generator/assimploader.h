#pragma once

#include "exceptions.h"

#include "../core/asset.h"
#include "../core/resource.h"
#include "../core/ResourceBuilder.h"

#include "../render/Scene.h"
#include "../render/model.h"
#include "../render/texture.h"

namespace Grafkit {

	class AssimpLoader : public Grafkit::IResourceBuilder
	{
	public:
		AssimpLoader(Grafkit::IAssetRef resource, Grafkit::Scene * const & scenegraph);
		~AssimpLoader();

		virtual void operator () (Grafkit::IResourceManager * const &assman);

	private:
		Grafkit::IAssetRef m_resource;
		Grafkit::Scene * const & m_scenegraph;
	};

}

// exceptions
DEFINE_EXCEPTION(AssimpParseException, 4000, "Error while parsing file via ASSIMP");
