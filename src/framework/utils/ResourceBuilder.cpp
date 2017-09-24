#include "stdafx.h"

#include "ResourceBuilder.h"
#include "AssetFactory.h"
#include "ResourceManager.h"

using namespace Grafkit;

IAssetRef IResourceBuilder::GetSourceAsset(IResourceManager * const & assman)
{
	if (assman && assman->GetAssetFactory()) 
		return assman->GetAssetFactory()->Get(m_srcName);
	else 
		return IAssetRef();
}

IAssetRef Grafkit::IResourceBuilder::GetAsset(IResourceManager * const & assman, std::string asset)
{
	if (assman && assman->GetAssetFactory())
		return assman->GetAssetFactory()->Get(asset);
	return IAssetRef();
}
