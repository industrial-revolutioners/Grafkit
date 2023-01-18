#pragma once

#include "core/Music.h"
#include "utils/Event.h"

namespace Idogep
{
    class Editor;

    class MusicProxy : public Grafkit::Music
    {
        friend class Editor;
    public:
        void Play() override;
        void Stop() override;
        void Pause(int e) override;
        void Update() override;

        uint64_t GetSampleCount() override;
        uint64_t GetSampleCountPerSec() override;

        uint64_t GetTimeSample() override;
        void SetTimeSample(uint64_t t) override;
        void SetLoop(int e) override;
        int IsPlaying() override;

        bool GetFFT(float* ptr, int segcount) override;
        bool GetWaveform(float*& ptr, size_t& length, size_t& channelCount, size_t& samplePerSec) override;

        Event<> onMusicChanged;

        bool HasMusic() const;

    protected:
        Grafkit::MusicResRef m_music;
    };
}
