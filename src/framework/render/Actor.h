#pragma once

#include <vector>

#include "../utils/reference.h"

#include "../math/matrix.h"

#include "renderer.h"


#include "../utils/persistence/persistence.h"

namespace Grafkit {

	class SceneGraph;

	class Actor;

	class Entity3D : public Persistent, virtual public Referencable
	{
	protected:
		void _serialize(Archive& ar);

		friend class Actor;
	public:
		Entity3D();
		virtual ~Entity3D();

		virtual void Calculate(Renderer& renderer, ActorRef parent = nullptr) = 0;
		virtual void Render(Grafkit::Renderer& deviceContext, SceneGraph * const & scene) = 0;
		virtual void Build(Grafkit::Renderer& deviceContext, SceneGraph * const & scene) = 0;

	    void serialize(Archive& ar) override = 0;

	public:
		std::string GetName() { return this->m_name; }
		void SetName(std::string name) { m_name = name; }

	protected:
		std::string m_name;
	};

	// Probaly we don!t need this thing below:

	//class Actor;
	//typedef Ref<Actor> ActorRef;

	// an event handler to do some special before and after entering each node in the graph
	// it helps to interchange shaders on the fly
	//__declspec(align(16))
	//	class ActorEventHandler : public AlignedNew<ActorEventHandler>, public Referencable
	//{
	//public:
	//	ActorEventHandler();
	//	virtual ~ActorEventHandler();

	//	virtual void OnBeforeRender(Grafkit::Renderer& render, SceneGraph * const & scene) { PushShader(scene); }
	//	virtual void OnAfterRender(Grafkit::Renderer& render, SceneGraph * const & scene) { PopShader(scene); }

	//	void SetPShader(ShaderResRef & ps) { myPShader = ps; }
	//	void SetVshader(ShaderResRef & vs) { myVShader = vs; }

	//protected:
	//	void PushShader(SceneGraph *const & scene);
	//	void PopShader(SceneGraph *const & scene);

	//protected:
	//	ShaderResRef myPShader;
	//	ShaderResRef myVShader;

	//private:
	//	ShaderResRef otherPShader;
	//	ShaderResRef otherVShader;
	//};

	/**
	An actor node - ez a scenegraph es a nodeja
	*/
	__declspec(align(16))
		class Actor : public AlignedNew<Actor>, public Persistent, virtual public Referencable
	{
		friend class SceneGraph;
	public:
		Actor();
		Actor(Ref<Entity3D> entity);
		~Actor();

		// kisse kulonos megoldas ... 
		void Matrix(const Grafkit::Matrix &mat) { m_viewMatrix = mat; }
		void Transform(const Grafkit::Matrix &mat) { m_transformMatrix = mat; }

		Grafkit::Matrix& Matrix() { return m_viewMatrix; }
		Grafkit::Matrix& Transform() { return m_transformMatrix; }

		virtual void Render(Grafkit::Renderer &render, SceneGraph* scene);

		/// @todo igazi ListTree-t hasznaljon, ha lehet, es majd mukodik
		Ref<Actor> GetParent() { return m_pParent; }

		void AddChild(Actor* child);
		ActorRef GetChild(int i = 0) { return this->m_pChildren[i]; }
		size_t GetChildrenCount() { return this->m_pChildren.size(); }

		void AddEntity(Ref<Entity3D> entity) { m_pEntities.push_back(entity); }
		Entity3DRef GetEntity(int id = 0) { return m_pEntities[id]; }
		size_t GetEntityCount() { return m_pEntities.size(); }

		//void SetEventHandler(Ref<ActorEventHandler> handler) { myEvtHandler = handler; }

		void Hide() { m_ishidden = 1; }
		void Show() { m_ishidden = 0; }

		int IsHidden() { return (m_pParent.Invalid() ? 0 : m_pParent->IsHidden()) || m_ishidden; }

	public:
		// calculated matrix for internal use
		Grafkit::Matrix WorldMatrix() { return m_worldMatrix; }

	public:
		std::string GetName() { return this->m_name; }
		void SetName(std::string name) { m_name = name; }

	protected:
		std::string m_name;

	protected:
		void WorldMatrix(const Grafkit::Matrix &mat) { m_worldMatrix = mat; }

		//// Probaly we don!t need this thing 
		//void DispatchBeforeRender(Grafkit::Renderer& render, SceneGraph * const & scene) {
		//	if (m_pParent.Valid())
		//		m_pParent->DispatchBeforeRender(render, scene);
		//	if (myEvtHandler.Valid())
		//		myEvtHandler->OnBeforeRender(render, scene);
		//}
		//void DispatchAfterRender(Grafkit::Renderer& render, SceneGraph * const & scene) {
		//	if (m_pParent.Valid())
		//		m_pParent->DispatchAfterRender(render, scene);
		//	if (myEvtHandler.Valid())
		//		myEvtHandler->OnAfterRender(render, scene);
		//}

		Grafkit::Matrix m_viewMatrix;			///< Node tranyyformacioja
		Grafkit::Matrix m_transformMatrix;		///< Kulon transzformacio a node tetejen (hogy ne legyen szukseg az eredeti matrixra)
		Grafkit::Matrix m_worldMatrix;			///< Szarmaztatott matrix

		Ref<Actor> m_pParent;
		std::vector<Ref<Actor>> m_pChildren;
		std::vector<Ref<Entity3D>> m_pEntities;

		//Ref<ActorEventHandler> myEvtHandler;

		int m_ishidden;

	protected:
		    void serialize(Archive& ar) override;
		PERSISTENT_DECL(Grafkit::Actor, 1);
	};

}

