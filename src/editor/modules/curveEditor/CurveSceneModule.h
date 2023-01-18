#pragma once
#include "utils/ViewModule.h"


namespace Idogep
{
	class CurveSceneView;


	namespace Roles {

		// Delegate audiogram rendering functions 
		class ManageCurveAudiogramRole {
		public:
			ManageCurveAudiogramRole();
			virtual ~ManageCurveAudiogramRole();

			void GetAudiogram(QImage** image, float startTime, float endTime, int rectWidth, int rectHeight);
			void ClearAudiogram();

			Event<float*&, size_t&, size_t&, size_t&> onRequestWaveform;
			//virtual bool RequestWaveform(float*& p, size_t& sampleCount, size_t& channelCount, size_t& samplePerSec) = 0;

		private:
			float* m_audiogramBuffer;
			size_t m_audiogramSampleCount;

			size_t m_audiogramChannelCount;
			size_t m_audiogramSamplePerSec;
		};

	}


	class CurveSceneModule : public Module, EmitsCommandRole 
	{
	public:
	    explicit CurveSceneModule(const Ref<Module>&& parent);

	    ~CurveSceneModule() override;
	    void Initialize() override;

	private:
		Ref<CurveSceneView> m_myView;
		
	    Roles::ManageCurveAudiogramRole* m_manageAudiogram;
	};
}