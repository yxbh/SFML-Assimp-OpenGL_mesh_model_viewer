#include "Light.hpp"

namespace KG
{

	Light::Light(const KE::EntityID p_EntityID, const RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_LightType(Type::Directional)
		, m_Ambient(0.01, 0.01, 0.01), m_Color(0.5, 0.5, 0.5)
		, m_AttenuationConstantCoeff(1.0f), m_AttenuationLinearCoeff(0.0f), m_AttenuationQuadraticCoeff(1.0f)
		, m_OuterConeAngle(30.0f), m_OuterConeCosAngle(std::cos(KE::Math::DegreeToRadian(m_OuterConeAngle)))
		, m_InnerConeAngle(20.0f), m_InnerConeCosAngle(std::cos(KE::Math::DegreeToRadian(m_InnerConeAngle)))
		, m_ConeAnglesEvaluated(true)
		, m_On(true)
	{
		this->SetOrientation(-90.0, 0.0, 0.0);
	}

	Light::~Light(void)
	{}

	const bool Light::IsOn(void) const
	{
		return m_On;
	}

	const bool Light::IsOff(void) const
	{
		return !m_On;
	}

	void Light::TurnOn(void)
	{
		m_On = true;
	}

	void Light::TurnOff(void)
	{
		m_On = false;
	}

	const Light::Type Light::GetType(void) const
	{
		return m_LightType;
	}

	const glm::dvec3 Light::GetAmbient(void) const
	{
		return m_Ambient;
	}

	const glm::dvec3 Light::GetColor(void) const
	{
		return m_Color;
	}

	const float Light::GetAttenuationConstant(void) const
	{
		return m_AttenuationConstantCoeff;
	}

	const float Light::GetAttenuationLinear(void) const
	{
		return m_AttenuationLinearCoeff;
	}

	const float Light::GetAttenuationQuadratic(void) const
	{
		return m_AttenuationQuadraticCoeff;
	}

	const float Light::GetOuterConeAngle(void) const
	{
		return m_OuterConeAngle;
	}

	const float Light::GetInnerConeAngle(void) const
	{
		return m_InnerConeAngle;
	}

	const float Light::GetOuterConeCosAngle(void) const
	{
		return m_OuterConeCosAngle;
	}

	const float Light::GetInnerConeCosAngle(void) const
	{
		return m_InnerConeCosAngle;
	}

	const bool Light::GetConeAngleEvaluated(void) const
	{
		return m_ConeAnglesEvaluated;
	}

	Light & Light::SetType(const Light::Type p_LightType)
	{
		if (p_LightType < Type::Directional || p_LightType > Type::SpotLight)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Light : type set to invalid type.");
			assert(false);
		}
		m_LightType = p_LightType; return *this;
	}

	Light & Light::SetAmbient(const glm::dvec3 p_Vec3)
	{
		m_Ambient = p_Vec3; return *this;
	}

	Light & Light::SetColor(const glm::dvec3 p_Vec3)
	{
		m_Color = p_Vec3; return *this;
	}

	Light & Light::SetAttenuationConstant(const float p_Coefficient)
	{
		m_AttenuationConstantCoeff = p_Coefficient; return *this;
	}

	Light & Light::SetAttenuationLinear(const float p_Coefficient)
	{
		m_AttenuationLinearCoeff = p_Coefficient; return *this;
	}

	Light & Light::SetAttenuationQuadratic(const float p_Coefficient)
	{
		m_AttenuationQuadraticCoeff = p_Coefficient; return *this;
	}

	Light & Light::SetOuterConeAngle(const float p_Angle)
	{
		m_OuterConeAngle = p_Angle;
		m_OuterConeCosAngle = static_cast<float>(std::cos(KE::Math::DegreeToRadian(m_OuterConeAngle)));
		return *this;
	}

	Light & Light::SetInnerConeAngle(const float p_Angle)
	{
		m_InnerConeAngle = p_Angle;
		m_InnerConeCosAngle = static_cast<float>(std::cos(KE::Math::DegreeToRadian(m_InnerConeAngle)));
		return *this;
	}

	Light & Light::SetOuterConeCosAngle(const float p_CosAngle)
	{
		m_OuterConeCosAngle = p_CosAngle; return *this;
	}

	Light & Light::SetInnerConeCosAngle(const float p_CosAngle)
	{
		m_InnerConeCosAngle = p_CosAngle; return *this;
	}

	Light & Light::SetConeAngleEvaluated(const bool p_Evaluated)
	{
		m_ConeAnglesEvaluated = p_Evaluated; return *this;
	}


} // KG ns
