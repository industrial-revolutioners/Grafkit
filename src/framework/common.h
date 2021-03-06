#pragma once 
#include <algorithm>
#include <string>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>

#include <cassert>
#include <memory>

#ifdef __cplusplus

//#include"core/reference.h"
#include"core/resource.h"
#include"core/asset.h"
#include"core/Object.h"

#include "render/renderer.h"
#include "render/dxtypes.h"

#include "utils/persistence/dynamics.h"

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T> using UniqueRef = std::unique_ptr<T>;

class Referencable{};

// predefine classes to reduce compile time 

namespace Grafkit {

	class Shader;

	class Vertexshader;
	class PixelShader;
	class GemoetryShader;
	class ComputeShader;

	typedef Ref<Shader> ShaderRef;
	typedef Resource<Shader> ShaderRes;
	typedef ShaderRes ShaderResRef; // !

	// --- 
	class Animation;
	typedef Ref<Animation> AnimationRef;

	// ---
	class SceneGraph;
	typedef Ref<SceneGraph> SceneGraphRef;

	class Mesh;
	typedef Ref<Mesh> MeshRef;

	class Material;
	typedef Ref<Material> MaterialRef;

	class Model;
	typedef Ref<Model> ModelRef;

	class Actor;
	typedef Ref<Actor> ActorRef;

	class Entity3D;
	typedef Ref<Entity3D> Entity3DRef;

	class Camera;
	typedef Ref<Camera> CameraRef;

	class Light;
	typedef Ref<Light> LightRef;

    // ---

    class EffectComposer;
    typedef Ref<EffectComposer> EffectComposerRef;

	// ---
	class Bitmap;
	class Texture1D;
	class Texture2D;
	class TextureCube;

	typedef Ref<Texture1D> Texture1DRef;
	typedef Resource<Texture1D> Texture1DRes;
	typedef Texture1DRes Texture1DResRef; // !

	typedef Ref<Texture2D> Texture2DRef;
	typedef Resource<Texture2D> Texture2DRes;
	typedef Texture2DRes Texture2DResRef; // !

	typedef Ref<Texture2D> TextureRef;
	typedef Texture2DRes TextureResRef; // !

	typedef Ref<TextureCube> TextureCubeRef;
	typedef Resource<TextureCube> TextureCubeRes;
	typedef TextureCubeRes TextureCubeResRef; // !

	class TextureSampler;

	typedef Ref<TextureSampler> TextureSamplerRef;
	typedef Resource<TextureSampler> TextureSamplerRes;
	typedef TextureSamplerRes TextureSamplerResRef; // !

	// ---

	class Timer;

	typedef Ref<Timer> TimerRef;

	class Music;
	typedef Ref<Music> MusicRef;
	typedef Resource<Music> MusicRes;
	//typedef Ref<MusicRes> MusicResRef;

    // --- 
    class IStream;
    typedef std::unique_ptr<IStream> StreamRef;
    typedef std::unique_ptr<uint8_t> StreamDataPtr;
}

#endif //__cplusplus
