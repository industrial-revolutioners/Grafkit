#pragma once
// A qnd framework to create demos more easily

#include <list>

#include "common.h"

#include "core/system.h"
#include "core/Music.h"

#include "render/renderer.h"

#include "utils/ResourceManager.h"
#include "utils/ResourcePreloader.h"

#include "utils/InitializeSerializer.h"

#include "builtin_data/loaderbar.h"

namespace GKDemo {

	class SceneElem;

	class DemoApplication :
		public virtual Grafkit::System,
		protected virtual Grafkit::ResourcePreloader,
		protected virtual Grafkit::ClonableInitializer,
		protected virtual GrafkitData::LoaderBar
	{
	public:
		DemoApplication();
		~DemoApplication();

	protected:
		virtual void OnBeforePreload() {}
		virtual void OnAfterPreload() {}
		virtual void OnDelegateTracks() {}
		virtual void OnSetupResources() {}

		virtual void Shutdown(){}

	protected:
		virtual int init();
		virtual void release();

	protected:
		std::list<Ref<SceneElem>> m_scenes;

	protected:
		Grafkit::Renderer m_render;

	protected:
		void UpdateLoaderBar(float p);

	protected:
		Grafkit::IAssetFactory *m_loader;

	public:
		Grafkit::IAssetFactory *GetAssetFactory() { return m_loader; }
		Grafkit::Renderer & GetDeviceContext() { return m_render; }

	private:
		// qnd enhance loader bar
		enum LoadPhase_e {
			Phase_BeforePreload,
			Phase_AfterPreload,
			Phase_COUNT
		};

		enum LoadPhase_e m_activeLoadPhase;

	};
}