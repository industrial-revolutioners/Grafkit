#pragma once


#include "renderer.h"
#include "dxtypes.h"
#include "reference.h"
#include "shader.h"
#include "Material.h"
#include "texture.h"

#include "../core/renderassets.h"

#include <vector>

namespace FWrender {

	class Entity3DEvents;
	class Actor;

	class Entity3D : public virtual Referencable, virtual public FWassets::IRenderAsset
	{
		friend class Actor;
		public:
			Entity3D();
			virtual ~Entity3D();

			ShaderRef &GetVertexShader() { return this->m_vertexShader; }
			void SetVertexShader(ShaderRef shader) { this->m_vertexShader = shader; }
			
			MaterialRef &GetMaterial() { return this->m_material; }
			void SetMaterial(MaterialRef material) { this->m_material = material; }

			///@{
			///Ezek a materialbol veszik ki a shadert, ha megosztott material van, akkor mindenkiet modositja
			///Kulonben ha zero a material, akkor gaz van
			ShaderRef GetFragmentShader() { return this->m_material.Valid()?this->m_material->GetShader():ShaderRef(); }
			void SetFragmentShader(ShaderRef shader) { this->m_vertexShader = shader; }
			///@}

			Actor * const & GetParent() { return m_parent; }

			virtual void Render(FWrender::Renderer& deviceContext) = 0;

		protected:	
			Actor* m_parent;
			ShaderRef m_vertexShader;
			MaterialRef m_material;

			/// @todo + transformation matrix
			/// @todo + bounding box, ha kell 1
	};


	class Entity3DEvents {
		friend class Entity3D;
		protected:
			virtual void onUpdateShaderParams(ID3D11DeviceContext* deviceContext, Shader*& shader) {}
			virtual void onRender(ID3D11DeviceContext* deviceContext, Actor*& actor) {}
	};


	/**
	An actor node 
	Todo: advanced architectural design goez here 
	*/
	class Actor : public virtual Referencable
	{

		public:
			Actor();
			Actor(const Actor&);
			~Actor();

			virtual void Render(ID3D11DeviceContext* deviceContext) = 0;

		protected:
			//void updateShader();	///@todo implement
			//void callDraw();		///@todo implement

			//ActorEvents* m_events;

			/// @todo + transformation

			std::vector<Ref<Entity3D>> entities;
	};

	typedef Ref<Actor> ActorRef;
}