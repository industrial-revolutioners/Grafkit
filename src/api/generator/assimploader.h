#pragma once

#include "../utils/exceptions.h"

#include "../utils/asset.h"
#include "../utils/resource.h"
#include "../utils/ResourceBuilder.h"
			   
#include "../render/Scene.h"
#include "../render/model.h"
#include "../render/texture.h"

namespace Grafkit {

	class AssimpLoader : public Grafkit::IResourceBuilder
	{
	public:
		AssimpLoader(std::string source_name, Grafkit::SceneRef & scenegraph);
		~AssimpLoader();

		virtual void load(IResourceManager * const & assman);

	private:
		/*Grafkit::IAssetRef m_resource;
		Grafkit::Scene * const & m_scenegraph;*/
	};

}

// exceptions
DEFINE_EXCEPTION(AssimpParseException, 4000, "Error while parsing file via ASSIMP");
