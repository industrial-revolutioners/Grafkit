#include "core/system.h"

#include "render/renderer.h"
#include "render/camera.h"
#include "render/Material.h"
#include "render/shader.h"

#include "generator/assimploader.h"
#include "render/Scenegraph.h"

#include "math/matrix.h"

#include "core/renderassets.h"
#include "core/assets.h"

using namespace FWrender;
using FWmodel::AssimpLoader;
using FWmath::Matrix;

class Application : public FWcore::System, public FWassets::IRenderAssetManager
{
public:
	Application() : FWcore::System(),
		m_file_loader(nullptr)
	{
		int screenWidth, screenHeight;

		screenWidth = 800;
		screenHeight = 600;

		// initwindowot is ertelmesebb helyre kell rakni
		InitializeWindows(screenWidth, screenHeight);
	}

	~Application() {
	}

protected:
	Renderer render;
	CameraRef camera;
	
	Scenegraph * scene;

	float t;

	// TextureShaderClass *shader_texture;
	ShaderRef shader_vs;
	ShaderRef shader_fs;

	int init() {
		//this->render = new Renderer();

		// --- ezeket kell osszeszedni egy initwindowban
		const int screenWidth = m_window.getRealWidth(), screenHeight = m_window.getRealHeight();
		const int VSYNC_ENABLED = 1, FULL_SCREEN = 0;
		const float SCREEN_DEPTH = 0.1f, SCREEN_NEAR = 1000.f;

		int result = 0;

		result = this->render.Initialize(screenWidth, screenHeight, VSYNC_ENABLED, this->m_window.getHWnd(), FULL_SCREEN);

		// init file loader
		this->m_file_loader = new FWassets::FileResourceManager("./");

		// -- camera
		camera = new Camera();
		camera->SetPosition(0.0f, 0.0f, -5.0f);

		// -- load shaderZ
		shader_vs = new Shader();
		shader_vs->LoadFromFile(render, "TextureVertexShader", L"./texture.hlsl", ST_Vertex);

		shader_fs = new Shader();
		shader_fs->LoadFromFile(render, "TexturePixelShader", L"./texture.hlsl", ST_Pixel);

		// -- model 
		scene = new Scenegraph();
		AssimpLoader loader(this->m_file_loader->GetResourceByName("tegla.3ds"), scene);
		loader(this);

		this->t = 0;

		return 0;
	};

	void release() {
		this->render.Shutdown();
	};

	int mainloop() {
		this->render.BeginScene();
		{
			struct {
				matrix worldMatrix;
				matrix viewMatrix;
				matrix projectionMatrix;
			} matbuff;

			camera->Render(render);

			//render.GetWorldMatrix(matbuff.worldMatrix);
			camera->GetViewMatrix(matbuff.viewMatrix);
			camera->GetProjectionMatrix(matbuff.projectionMatrix);

			//matbuff.worldMatrix = DirectX::XMMatrixIdentity(); 
			matbuff.worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(t * 10, t * 15, t * 20);

			// --- ez a legfontosabb dolog, amit meg meg kell itt tenni mielott atadod a cbuffernek:
			matbuff.worldMatrix = XMMatrixTranspose(matbuff.worldMatrix);
			matbuff.viewMatrix = XMMatrixTranspose(matbuff.viewMatrix);
			matbuff.projectionMatrix = XMMatrixTranspose(matbuff.projectionMatrix);
			// ---

			shader_vs["MatrixBuffer"].Set(&matbuff);

			shader_vs->Render(render);
			shader_fs->Render(render);

			// model->Render(render);
			// render of scenegraph goez here 

			this->t += 0.001;
		}

		this->render.EndScene();
		return 0;
	};

private:
	FWassets::FileResourceManager *m_file_loader;
public:
	FWassets::IResourceFactory* GetResourceFactory() { return m_file_loader; }
	FWrender::Renderer & GetDeviceContext() { return render; }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Application app;
	return app.execute();
}
