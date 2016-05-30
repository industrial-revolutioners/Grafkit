#pragma once

#include "../utils/exceptions.h"

#include "../utils/asset.h"
#include "../utils/resource.h"
#include "../utils/ResourceBuilder.h"

namespace Grafkit {

	class MusicFmodLoader : public Grafkit::IResourceBuilder {

	public:
		MusicFmodLoader(std::string source_name);
		~MusicFmodLoader();

		virtual void Load(IResourceManager * const & resman, IResource * source);

		virtual IResource* NewResource();
	};

}

DEFINE_EXCEPTION(MusicLoadException, 0, "Could not load music from disk");