#pragma once

#include "../stdafx.h"

#include "Actor.h"

#include "renderer.h"
#include "shader.h"

///@todo minden cuccot egy namespace-be kellene bedobni; 
///@todo valahogy a materialba kellene belejatszani mindezt
///@todo aligned new-t mindenre
namespace Grafkit{

	class Light;
	typedef Ref<Light> LightRef; 

	class Light : virtual public Referencable, public Entity3D
	{
	public:
		Light();
		~Light();

		/// @todo ez csak egy QnD Hack, ki kell majd javitani a jovoben
		float4 &Position() { return m_light.position; }
		void Position(float4 p) {m_light.position = p; }

		float4 &Direction() { return m_light.direction; }

		float4 &Ambient() { return m_light.ambient; }
		float4 &Diffuse() { return m_light.diffuse; }
		float4 &Specular() { return m_light.specular; }

		float &ConstantAttenuation() { return m_light.ca; }
		float &LinearAttenuation() { return m_light.la; }
		float &QuardicAttenuation() { return m_light.qa; }

		float& Angle() { return m_light.angle; }
		float& Falloff() { return m_light.falloff; }

		/// Setup the corresponding constant buffer inside the shader
		//void SetShaderCB(ShaderRef &rPShader);

		void Render(Grafkit::Renderer& deviceContext, Scene* scene);
		void Build(Grafkit::Renderer& deviceContext, Scene* scene) {}

	protected:

		///@todo ez egy kurvaszar megoldas, es nem kene hasznalni a feny tipusanak meghatarozasara; helyette virtualis fuggvennyekkel kellene~
		enum type_t {
			INVALID = 0, T_point=1, T_directional=2, T_spot=3, T_ambient=4, COUNT
		};

		virtual enum type_t GetLightType() = 0;

		struct light_t {
			int type;
			float4 position;
			float4 direction;

			float4 ambient;
			float4 diffuse;
			float4 specular;

			float ca, la, qa;

			float angle, falloff;
		};

		struct light_t m_light;

	};

	// ============================================================================================================
	__declspec(align(16)) class PointLight : public Light, public AlignedNew<PointLight>
	// class PointLight : public Light 
	{
	public:
		PointLight() : Light () {}
		~PointLight() {}

	private:
		virtual enum type_t GetLightType() { return T_point; }
	};

	// ============================================================================================================
	__declspec(align(16)) class DirectionalLight : public Light, public AlignedNew<DirectionalLight>
	// class DirectionalLight : public Light
	{
		public:
		DirectionalLight() : Light() {}
		~DirectionalLight() {}

	private:
		virtual enum type_t GetLightType() { return T_directional; }

	};

	// ============================================================================================================
	__declspec(align(16)) class SpotLight : public Light, public AlignedNew<SpotLight>
	// class SpotLight : public Light
	{
	public:
		SpotLight() : Light() {}
		~SpotLight() {}

	private:
		virtual enum type_t GetLightType() { return T_spot; }
	};

	// ============================================================================================================
	__declspec(align(16)) class AmbientLight : public Light, public AlignedNew<AmbientLight>
	// class AmbientLight : public Light
	{
	public:
		AmbientLight() : Light() {}
		~AmbientLight() {}

	private:
		virtual enum type_t GetLightType() { return T_ambient; }
	};
}
