#pragma once
#include "../../../common.hpp"
#include "../SceneNode.hpp"

namespace KG
{

	/*! \class Light
	*/
	class Light
		: public KG::SceneNode
	{
	public:
		enum Type
		{
			Directional = 0,
			PointLight = 1,
			SpotLight = 2,
		};

	private:
		/* type */
		Type		m_LightType;
		/* Properties. */
		glm::dvec3 	m_Ambient;			// ambient intensity
		glm::dvec3 	m_Color;			// light color. AKA: diffuse, Intensity
		/* antenuation related */
		float		m_AttenuationConstantCoeff;
		float		m_AttenuationLinearCoeff;
		float		m_AttenuationQuadraticCoeff;
		/* spotlight related. (this is a dual cone spot light: 1 inner cone and 1 outer cone). */
		float		m_OuterConeAngle, m_OuterConeCosAngle;
		float		m_InnerConeAngle, m_InnerConeCosAngle;
		bool		m_ConeAnglesEvaluated;
		/**/
		bool	m_On;

	public:
		Light(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const RenderPass p_RenderPass = RenderPass::NotRendered);
		~Light(void);

		const bool IsOn(void) const;
		const bool IsOff(void) const;
		void TurnOn(void);
		void TurnOff(void);

		const Light::Type GetType(void) const;
		const glm::dvec3 GetAmbient(void) const;
		const glm::dvec3 GetColor(void) const;
		const float GetAttenuationConstant(void) const;
		const float GetAttenuationLinear(void) const;
		const float GetAttenuationQuadratic(void) const;
		const float GetOuterConeAngle(void) const;
		const float GetInnerConeAngle(void) const;
		const float GetOuterConeCosAngle(void) const;
		const float GetInnerConeCosAngle(void) const;
		const bool GetConeAngleEvaluated(void) const;

		Light & SetType(const Light::Type p_LightType);
		Light & SetAmbient(const glm::dvec3 p_Vec3);
		Light & SetColor(const glm::dvec3 p_Vec3);
		Light & SetAttenuationConstant(const float p_Coefficient);
		Light & SetAttenuationLinear(const float p_Coefficient);
		Light & SetAttenuationQuadratic(const float p_Coefficient);
		Light & SetOuterConeAngle(const float p_Angle);
		Light & SetInnerConeAngle(const float p_Angle);
		Light & SetOuterConeCosAngle(const float p_CosAngle);
		Light & SetInnerConeCosAngle(const float p_CosAngle);
		Light & SetConeAngleEvaluated(const bool p_Evaluated);

	}; // Light class
	typedef std::shared_ptr<Light> Light_SmartPtr;
	typedef std::weak_ptr<KG::Light> Light_WeakPtr;

} // KG ns
