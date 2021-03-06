#define _USE_MATH_DEFINES
#include <cmath>

#include "core/system.h"

#include "render/renderer.h"
#include "render/camera.h"
#include "render/Material.h"
#include "render/shader.h"
#include "render/light.h"

#include "generator/ShaderLoader.h"
#include "generator/SceneLoader.h"
#include "generator/TextureLoader.h"

#include "render/Scene.h"
#include "render/effect.h"
#include "render/camera.h"

#include "math/matrix.h"

#include "builtin_data/loaderbar.h"

#include "utils/AssetFile.h"
#include "utils/ResourceManager.h"
#include "utils/ResourcePreloader.h"

#include "utils/InitializeSerializer.h"

using namespace Grafkit;

#include "builtin_data/cube.h"

class Application : 
	public Grafkit::System, 
	protected Grafkit::ResourcePreloader,
	private Grafkit::ClonableInitializer, 
	protected GrafkitData::LoaderBar
{
public:
	Application() : ClonableInitializer(), Grafkit::System(), ResourcePreloader(this), LoaderBar(),
		m_file_loader(nullptr)
	{
		int screenWidth, screenHeight;

		screenWidth = 800;
		screenHeight = 600;

		t = 0;

		// initwindowot is ertelmesebb helyre kell rakni
		InitializeWindows(screenWidth, screenHeight);

		// --- ezeket kell osszeszedni egy initwindowban
		screenWidth = m_window.getRealWidth(), screenHeight = m_window.getRealHeight();
		const int VSYNC_ENABLED = 1, FULL_SCREEN = 0;

		int result = 0;

		result = this->render.Initialize(screenWidth, screenHeight, VSYNC_ENABLED, this->m_window.getHWnd(), FULL_SCREEN);
		this->m_file_loader = new FileAssetFactory("./../assets/");
	}

	~Application() {
		delete m_file_loader;	///TODO: shutdown elotte
		this->render.Shutdown();
	}

protected:
	Renderer render;
	SceneResRef scene;

	TextureSamplerRef sampler;

	TextureCubeResRef envmap;
	TextureCubeResRef irradiancemap;

	EffectComposerRef drawCubemap;
	EffectComposerRef postfx;

	TextureSamplerRef texSampler;

	LightRef light;
	ActorRef lightActor;

	ActorRef m_rootActor;
	ActorRef m_cameraActor;

	ActorRef cameraActor;
	CameraRef camera;

	float t;

	ShaderResRef vs;
	ShaderResRef fs;
	ShaderResRef cubemapShader;


	int init() {
		InitializeLoaderBar(render);

		LoadCache();

		// -- load shader
		vs = Load<ShaderRes>(new VertexShaderLoader("vShader", "shaders/vertex.hlsl", ""));
		//fs = Load<ShaderRes>(new PixelShaderLoader("pShader", "shaders/lightmaterial.hlsl", "phongBlinn"));
		fs = Load<ShaderRes>(new PixelShaderLoader("pShader", "shaders/lightpbr.hlsl", ""));

		cubemapShader = Load<ShaderRes>(new PixelShaderLoader("cubemapShader", "shaders/cubemap.hlsl", ""));

		envmap = Load<TextureCubeRes>(new TextureCubemapFromBitmap("envmap",
			"textures/yoko_posx.jpg",
			"textures/yoko_negx.jpg",
			"textures/yoko_posy.jpg",
			"textures/yoko_negy.jpg",
			"textures/yoko_posz.jpg",
			"textures/yoko_negz.jpg"
		));

		irradiancemap = Load<TextureCubeRes>(new TextureCubemapFromBitmap("irradiance",
			"textures/yoko_low_posx.jpg",
			"textures/yoko_low_negx.jpg",
			"textures/yoko_low_posy.jpg",
			"textures/yoko_low_negy.jpg",
			"textures/yoko_low_posz.jpg",
			"textures/yoko_low_negz.jpg"
		));

		// -- model 
		scene = this->Load<SceneRes>(new SceneLoader("scene", "sphere_pbr.scene"));

		DoPrecalc();

		drawCubemap = new EffectComposer();
		drawCubemap->AddPass(new EffectPass(cubemapShader));
		drawCubemap->Initialize(render, true);

		(*scene)->BuildScene(render, vs, fs);
		(*scene)->SetActiveCamera(0);

		cameraActor = (*scene)->GetActiveCamera();
		camera = dynamic_cast<Camera*>(cameraActor.Get());

		// -- add lights
		/*light = new PointLight();
		lightActor = new Actor(light);*/

		//(*scene)->GetRootNode()->AddChild(lightActor);

		sampler = new TextureSampler();
		sampler->Initialize(render);

		// --- serialize && deserialize

		this->t = 0;

		return 0;
	};

	// ==================================================================================================================

	void release() {
		this->render.Shutdown();
	};

	// ==================================================================================================================
	int mainloop() {

		struct {
			float2 res;
			float ar;
			float fov;
		} resprops;

		Scene::WorldMatrices_t worldMatrices;

		this->render.BeginScene();
		{
			t += .01;

			this->scene->Get()->UpdateAnimation(fmod(t, 10.5));
			this->scene->Get()->PreRender(render);

			resprops.ar = render.GetAspectRatio();
			resprops.fov = camera->GetFOV();

			worldMatrices = (*scene)->GetWorldMatrices();
			
			(*cubemapShader)->SetParam(render, "ResolutionBuffer", &resprops);
			(*cubemapShader)->SetParam(render, "MatrixBuffer", &worldMatrices);
			(*cubemapShader)->SetShaderResourceView(render, "skybox", (*envmap)->GetShaderResourceView());
			(*cubemapShader)->SetSamplerSatate(render, "SampleType", sampler->GetSamplerState());
			
			render.ToggleDepthWrite(false);

			drawCubemap->Render(render);

			render.ToggleDepthWrite(true);

			(*fs)->SetShaderResourceView(render, "c_specular", (*envmap)->GetShaderResourceView());
			(*fs)->SetShaderResourceView(render, "c_irradiance", (*irradiancemap)->GetShaderResourceView());
			(*fs)->SetSamplerSatate(render, "SampleType", sampler->GetSamplerState());

			this->scene->Get()->Render(render);
		}

		this->render.EndScene();

		m_file_loader->PollEvents(this);

		return isEscPressed();
	};

private:
	FileAssetFactory *m_file_loader;

	protected:
		void UpdateLoaderBar(float p) {
			DrawLoaderBar(render, p);
		}

public:
	IAssetFactory* GetAssetFactory() { return m_file_loader; };
	Renderer & GetDeviceContext() { return this->render; };
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Application app;
	return app.execute();
}
