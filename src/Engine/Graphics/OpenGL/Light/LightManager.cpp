#include "LightManager.hpp"
#include "Light.hpp"
#include "../Shader/ShaderProgram.hpp"

namespace KG
{

	LightManager::LightManager(void)
	{}

	void LightManager::Update(const KE::Duration p_Duration)
	{
		KE_UNREFERENCED_PARAMETER(p_Duration);
		for (LightMap::const_iterator it = m_LightMap.begin(); it != m_LightMap.end(); ++it)
		{
			auto to_remove = it;
			if (to_remove->second.expired())
			{
				m_LightMap.erase(to_remove);
				++it;
			}
		}
	}

	const bool LightManager::AddLight(KG::Light_SmartPtr p_spLight)
	{
		if (m_LightMap.size() >= sk_MaxNumLights)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "LightManager : maximum number of light already.");
			assert(false);
		}
		if (!p_spLight)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "LightManager : Light point to null.");
			return false;
		}
		KG::Light_WeakPtr light_wkptr(p_spLight);
		m_LightMap.insert(std::make_pair(p_spLight->GetEntityID(), light_wkptr));
		return true;
	}

	const bool LightManager::RemoveLight(const KE::EntityID p_EntityID)
	{
		std::size_t result = m_LightMap.erase(p_EntityID);
		if (result != 1)
			return false;
		return true;
	}

	const unsigned LightManager::GetNumLights(void) const
	{
		return m_LightMap.size();
	}

	void LightManager::SetupLights(ShaderProgram_SmartPtr p_spShaderProgram) const
	{
		p_spShaderProgram->Enable();

		static const char * array_name					= "Lights[";
		static const char * position					= "].Position";
		static const char * direction					= "].Direction";
		static const char * ambient						= "].Ambient";
		static const char * color						= "].Color";
		static const char * type						= "].Type";
		static const char * attenuateConstantCoeff		= "].AttenuateConstantCoeff";	
		static const char * attenuateLinearCoeff		= "].AttenuateLinearCoeff";
		static const char * attenuateQuadraticCoeff		= "].AttenuateQuadraticCoeff";
		static const char * outerConeCosAngle			= "].OuterConeCosAngle";
		static const char * innerConeCosAngle			= "].InnerConeCosAngle";
		
		std::string uniform; uniform.reserve(70);
		std::string array_and_index; array_and_index.reserve(20);
		int i = -1;
		for (const auto & light_pair : m_LightMap)
		{
			KG::Light_SmartPtr light_ptr(light_pair.second.lock());
			if (!light_ptr)
				continue;
			array_and_index.clear();
			array_and_index.append(array_name); array_and_index.append(std::to_string(++i));

			uniform.clear(); uniform.append(array_and_index); uniform.append(position);
			p_spShaderProgram->SetParameter<glm::vec3>(uniform.c_str(), glm::vec3(light_ptr->GetPositionVec()));

			uniform.clear(); uniform += array_and_index; uniform += direction;
			p_spShaderProgram->SetParameter<glm::vec3>(uniform.c_str(), glm::vec3(light_ptr->GetDirectionVec()));

			uniform.clear(); uniform += array_and_index; uniform += ambient;
			(p_spShaderProgram->SetParameter<glm::vec3>(uniform.c_str(), glm::vec3(light_ptr->GetAmbient())));

			uniform.clear(); uniform += array_and_index; uniform += color;
			(p_spShaderProgram->SetParameter<glm::vec3>(uniform.c_str(), glm::vec3(light_ptr->GetColor())));

			uniform.clear(); uniform += array_and_index; uniform += type;
			(p_spShaderProgram->SetParameter<GLint>(uniform.c_str(), light_ptr->GetType()));

			uniform.clear(); uniform += array_and_index; uniform += attenuateConstantCoeff;
			(p_spShaderProgram->SetParameter<GLfloat>(uniform.c_str(), light_ptr->GetAttenuationConstant()));
			uniform.clear(); uniform += array_and_index; uniform += attenuateLinearCoeff;
			(p_spShaderProgram->SetParameter<GLfloat>(uniform.c_str(), light_ptr->GetAttenuationLinear()));
			uniform.clear(); uniform += array_and_index; uniform += attenuateQuadraticCoeff;
			(p_spShaderProgram->SetParameter<GLfloat>(uniform.c_str(), light_ptr->GetAttenuationQuadratic()));

			uniform.clear(); uniform += array_and_index; uniform += outerConeCosAngle;
			(p_spShaderProgram->SetParameter<GLfloat>(uniform.c_str(), light_ptr->GetOuterConeCosAngle()));
			uniform.clear(); uniform += array_and_index; uniform += innerConeCosAngle;
			(p_spShaderProgram->SetParameter<GLfloat>(uniform.c_str(), light_ptr->GetInnerConeCosAngle()));
		}
		(p_spShaderProgram->SetParameter<GLint>("NumLights", i+1));
	}

} // KG ns
