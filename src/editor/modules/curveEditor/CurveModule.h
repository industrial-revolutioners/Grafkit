#pragma once

#include "utils/ViewModule.h"

#include "utils/Command.h"

namespace  Idogep
{
	class CurveEditorView;
	class CurveSceneModule;

	// ========================================================================================================

	class CurveEditorModule : public Module, public EmitsCommandRole
	{
	public:
		explicit CurveEditorModule(const Ref<Module>& parent);
		~CurveEditorModule() override;

		void Initialize() override;

		Event<float*&, size_t&, size_t&, size_t&> onRequestWaveform; // ptr, samplecount, channelcount, samplePerSec

	private:
		Ref<CurveEditorView> m_myView;
		Ref<CurveSceneModule> m_curveScene;
        
	};

}