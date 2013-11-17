#pragma once
#include "../../../common.hpp"
#include "../../../Entity/EntityID.hpp"

namespace KG
{
	class Light;
	typedef std::shared_ptr<Light>							Light_SmartPtr;
	typedef std::weak_ptr<KG::Light>						Light_WeakPtr;
	typedef std::map<KE::EntityID, KG::Light_WeakPtr>		LightMap;
	class ShaderProgram;
	typedef std::shared_ptr<KG::ShaderProgram>				ShaderProgram_SmartPtr;

	/*! \class LightManager
	*/
	class LightManager
	{
	public:
		static const int sk_MaxNumLights = 8;
	private:
		KG::LightMap	m_LightMap;

	public:
		LightManager(void);

		void Update(const KE::Duration p_Duration);

		/*! SmartPtr is convereted to WeakPtr interally for storage. */
		const bool AddLight(KG::Light_SmartPtr p_spLight);
		const bool RemoveLight(const KE::EntityID p_EntityID);
		const unsigned GetNumLights(void) const;

		void SetupLights(ShaderProgram_SmartPtr p_spShaderProgram) const;

	}; // LightManager class

} // KG ns
