#pragma once

#include <stack>

#include "../utils/tree.h"
#include "../utils/memory_align.h"

#include "../math/matrix.h"

#include "renderer.h"

#include "Actor.h"

#include "camera.h"
#include "Light.h"

namespace Grafkit {

	class Scene;
	typedef Ref<Scene> SceneRef;

#define SCENE_BUCKET "scene"

	__declspec(align(16)) class Scene : virtual public Referencable, public AlignedNew<Scene>
	{
	public:
		Scene();
		~Scene();

		void PreRender(Grafkit::Renderer & render);
		void Render(Grafkit::Renderer & render);

		void SetRootNode(ActorRef &root) { m_pScenegraph = root; }

		// --- 

		CameraRef GetCamera() { 
			return (this->m_pCameraNode.Valid() && !this->m_pCameraNode->GetEntities().empty()) ? dynamic_cast<Camera*>(this->m_pCameraNode->GetEntities()[0].Get()) : nullptr;
		}

		void SetCameraNode(ActorRef &camera);
		void AddLightNode(ActorRef &light);

		LightRef GetLight(int n) { return dynamic_cast<BaseLight*>(this->m_pvLightNodes[n]->GetEntities()[0].Get()); }
		size_t GetLightCount() { return this->m_pvLightNodes.size(); }

		Grafkit::Matrix& GetWorldMatrix() { return this->m_currentWorldMatrix; }

		ShaderRef &GetVShader() { return this->m_vertexShader; }
		ShaderRef &GetFShader() { return this->m_fragmentShader; }

		void SetVShader(ShaderRef &VS) {this->m_vertexShader = VS; }
		void SetFShader(ShaderRef &FS) {this->m_fragmentShader = FS; }

		virtual const char* GetBucketID() { return SCENE_BUCKET; }

	protected:
		ActorRef m_pScenegraph;
		// fenyek + camera

		ActorRef m_pCameraNode;
		std::vector<ActorRef> m_pvLightNodes;
		
		// std::vector<LightRef> m_rvLights;
		// CameraRef m_rCamera;

		//shader_pair_t m_shader;
		ShaderRef m_vertexShader;
		ShaderRef m_fragmentShader;

	private:
		void RenderNode(Grafkit::Renderer & render, Actor* actor, int maxdepth = TREE_MAXDEPTH);
		void push();
		void pop();

		Grafkit::Matrix m_cameraMatrix;

		Grafkit::Matrix m_currentWorldMatrix;
		std::stack<Grafkit::Matrix> m_worldMatrixStack; 

	private:
			
	};

	typedef Ref<Scene> SceneRef;
	typedef Resource<Scene> SceneRes;
	typedef Ref<SceneRes> SceneResRef;
}
