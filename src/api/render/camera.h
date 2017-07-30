#pragma once

#include "Actor.h"

#include "../stdafx.h"
#include "../math/matrix.h"

#include "renderer.h"

namespace Grafkit {

	class Camera;
	// 

	/** Basic camera class */
	__declspec(align(16)) class Camera : public Entity3D, public AlignedNew<Camera>/*, public Persistent*/
	{
	public:
		Camera();
		~Camera();

		void SetPosition(float x, float y, float z);
		float3 &GetPosition() { return this->m_position; }
		
		/// Camera looka at a reference point 
		void SetLookAt(float x, float y, float z);
		void SetLookAt(float3 p) { SetLookAt(p.x, p.y, p.z); }

		/// Camera looks towards a given vector
		void SetLookTo(float x, float y, float z);
		void SetLookTo(float3 p) { SetLookTo(p.x, p.y, p.z); }

		float3 &GetLook() { return this->m_position; }

		void SetUp(float x, float y, float z);
		float3 &GetUp() { return this->m_up; }

		void SetRotation(float x, float y, float z);
		float3 &GetRotation() { return this->m_rotation; }

		void SetAspect(float aspect) { this->m_aspect = aspect; }
		/// @todo GetAspect()
		
		/// Horizontal FOV in radian
		void SetFOV(float fov) { this->m_hFov = fov; }
		/// @todo GetFOV()
		
		void SetClippingPlanes(float znear, float zfar) { this->m_znear = znear, this->m_zfar = zfar; }
		/// @todo void GetClippingPlanes(float &znear, float &zfar);

		void SetScreenMetrics(float width, float height) { this->m_screenWidth = width, this->m_screenHeight = height; }
		/// @todo void GetScreenMetrics(float width, float height) { this->m_screenWidth = width, this->m_screenHeight = height; }

		/** Generate matrices */
		void Calculate(Renderer& renderer);

		//skip render, nothing to do with it. 
		virtual void Render(Grafkit::Renderer& deviceContext, Scene* const & scene) {}
		virtual void Build(Grafkit::Renderer& deviceContext, Scene* const & scene){}

		Matrix& GetViewMatrix() { return m_viewMatrix; }
		Matrix& GetPerspectiveMatrix() { return m_perspectiveMatrix; }
		Matrix& GetOrthoMatrix() { return m_orthoMatrix; }

		virtual Matrix& ProjectionMatrix() {
			switch(m_type){
			case ORTHOGRAPHIC:
				return m_orthoMatrix;
			//case PERSPECTIVE:
			default:
				return m_perspectiveMatrix;
			}
		}

		enum camera_mode { LOOK_TO, LOOK_AT };

		enum camera_mode getMode() {
			return m_mode;
		}
		
		enum camera_type { PERSPECTIVE, ORTHOGRAPHIC };
		
		virtual void setType(enum camera_type t) {
			m_type = t;
		}

		virtual enum camera_type getType() {
			return m_type;
		}

	private:
		float3 m_position;
		float3 m_look;
		float3 m_up;
		float3 m_rotation;

		float m_znear, m_zfar;
		float m_aspect, m_screenWidth, m_screenHeight;
		float m_hFov;

		enum camera_type m_type; // hack: van, ahol kezzel kell a kamerat atvaltnai perpective/ortho kozott
		enum camera_mode m_mode;
	
	protected:
		Grafkit::Matrix m_viewMatrix;
		Grafkit::Matrix m_perspectiveMatrix;
		Grafkit::Matrix m_orthoMatrix;

		PERSISTENT_DECL(Grafkit::Camera, 1);
	protected:
		virtual void serialize(Archive& ar);
	};
}
