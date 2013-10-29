#pragma once
#include "../Engine/common.hpp"
#include "../Engine/Event/IEvent.hpp"
#include "../Engine/Entity/EntityID.hpp"
#include "../Engine/Graphics/OpenGL/OGLEntityRenderComponent.hpp"

namespace CITS
{

	enum EventType
	{
		LookUp				= 2001,
		LookDown			= 2002,
		LookLeft			= 2003,
		LookRight			= 2004,
		TiltLeft			= 2005,
		TiltRight			= 2006,
		TranslateDeltaX		= 2007,
		TranslateDeltaY		= 2008,
		TranslateDeltaZ		= 2009,	
		StrafeForward		= 2010,
		StrafeBackward		= 2011,
		StrafeLeft			= 2012,
		StrafeRight			= 2013,
		StrafeUp			= 2028,
		StrafeDown			= 2029,
		StrafeForeRelCam	= 2014,
		StrafeBackRelCam	= 2015,
		StrafeLeftRelCam	= 2016,
		StrafeRightRelCam	= 2017,
		StrafeUpRelCam		= 2018,
		StrafeDownRelCam	= 2019,
		TranslateUp			= 2020,
		TranslateDown		= 2021,
		UniformScale		= 2022,
		ScaleDeltaX			= 2023,
		ScaleDeltaY			= 2024,
		ScaleDeltaZ			= 2025,
		RollLeft			= 2024, //Think of rolling something from a bird's eye point of view., or on a table.
		RollRight			= 2025,
		RollUp				= 2026,
		RollDown			= 2027,
		IncreaseDistance    = 2030,
		DecreaseDistance    = 2031,
		NewObject						= 100001,
		NewObjectRequest				= 100002,
		NewGUICreated					= 100003,
		ChangeTextureRequest			= 100004,
		ToggleControlPanelVisibility 	= 100005,
		SetMatEmissive					= 100006,
		SetMatAmbient					= 100007,
		SetMatDiffuse					= 100008,
		SetMatSpecular					= 100009,
		SetMatShininess					= 100010,
		SetFreeFlightMode				= 100011,
		SetThirdPersonMode				= 100012,
		SetObjectManMode				= 100013,
		SetObjectMatMode				= 100014,
		SetTexCoordMultiplier           = 100015,
		SetMeshStrafeDistance			= 100016,
		SetMeshStrafeSpeed				= 100017,
	};


	class Control_Event
		: public KE::IEvent
	{
	private:
		double	m_Delta;

	public:
		explicit Control_Event(const double p_Delta) : m_Delta(p_Delta) {}

		const double GetDelta(void) const
		{
			return m_Delta;
		}

		void SetDelta(const double p_Delta)
		{
			m_Delta = p_Delta;
		}

	};

	class LookUp_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::LookUp;	

	public:
		explicit LookUp_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class LookDown_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::LookDown;	

	public:
		explicit LookDown_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class LookLeft_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::LookLeft;	

	public:
		explicit LookLeft_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class LookRight_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::LookRight;	

	public:
		explicit LookRight_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class TiltLeft_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::TiltLeft;	

	public:
		explicit TiltLeft_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class TiltRight_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::TiltRight;	

	public:
		explicit TiltRight_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class TranslateDeltaY_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::TranslateDeltaY;	

	public:
		explicit TranslateDeltaY_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeForward_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeForward;	

	public:
		explicit StrafeForward_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeBackward_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeBackward;	

	public:
		explicit StrafeBackward_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeLeft_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeLeft;	

	public:
		explicit StrafeLeft_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeRight_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeRight;	

	public:
		explicit StrafeRight_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class TranslateUp_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::TranslateUp;	

	public:
		explicit TranslateUp_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeUp_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeUp;	

	public:
		explicit StrafeUp_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeDown_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeDown;	

	public:
		explicit StrafeDown_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class TranslateDown_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::TranslateDown;	

	public:
		explicit TranslateDown_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class UniformScale_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::UniformScale;	

	public:
		explicit UniformScale_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeForeRelCam_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeForeRelCam;	

	public:
		explicit StrafeForeRelCam_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeBackRelCam_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeBackRelCam;	

	public:
		explicit StrafeBackRelCam_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeLeftRelCam_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeLeftRelCam;	

	public:
		explicit StrafeLeftRelCam_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class StrafeRightRelCam_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::StrafeRightRelCam;	

	public:
		explicit StrafeRightRelCam_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class RollRight_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::RollRight;	

	public:
		explicit RollRight_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class RollUp_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::RollUp;	

	public:
		explicit RollUp_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class IncreaseDistance_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::IncreaseDistance;	

	public:
		explicit IncreaseDistance_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class DecreaseDistance_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::DecreaseDistance;	

	public:
		explicit DecreaseDistance_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class NewObject_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::NewObject;
		KE::EntityID	m_EntityID;

	public:
		explicit NewObject_Event(const KE::EntityID p_EntityID)	: m_EntityID(p_EntityID) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}

		const KE::EntityID GetEntityID(void) const
		{
			return m_EntityID;
		}
	};

	class NewObjectRequest_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::NewObjectRequest;
		std::string	m_TextureFileName;
		std::string m_MeshFileName;

	public:
		NewObjectRequest_Event(const std::string p_MeshFileName = "DEFAULT", const std::string p_TextureFileName = "NONE")
			: m_TextureFileName(p_TextureFileName), m_MeshFileName(p_MeshFileName) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}

		const std::string & GetMeshFileName(void) const
		{
			return m_MeshFileName;
		}

		const std::string & GetTextureFileName(void) const
		{
			return m_TextureFileName;
		}
	};

	class NewGUICreated_Event final
		: public KE::IEvent
	{
		typedef std::shared_ptr<KG::OGLEntityRenderComponent> RenderComponent_SmartPtr;
	private:
		static const KE::EventType sk_EventTypeId = EventType::NewGUICreated;
		RenderComponent_SmartPtr		m_spGUI;

	public:
		NewGUICreated_Event(RenderComponent_SmartPtr p_spGUI) : m_spGUI(p_spGUI) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}

		RenderComponent_SmartPtr GetGUI(void) const
		{
			return m_spGUI;
		}
	};

	class ChangeTextureRequest_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::ChangeTextureRequest;
		std::string	m_TextureFileName;
		std::string m_MeshFileName;

	public:
		ChangeTextureRequest_Event(const std::string p_MeshFileName = "DEFAULT", const std::string p_TextureFileName = "NONE")
			: m_TextureFileName(p_TextureFileName), m_MeshFileName(p_MeshFileName) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}

		const std::string & GetMeshFileName(void) const
		{
			return m_MeshFileName;
		}

		const std::string & GetTextureFileName(void) const
		{
			return m_TextureFileName;
		}
	};

	class ToggleControlPanelVisibility_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::ToggleControlPanelVisibility;

	public:
		ToggleControlPanelVisibility_Event(void) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}
	};

	class SetVec3f_Event
		: public KE::IEvent
	{
	private:
		glm::fvec3	m_Vec3f;

	public:
		SetVec3f_Event(const float p_X, const float p_Y, const float p_Z)
			: m_Vec3f(p_X, p_Y, p_Z)
		{}

		const glm::fvec3 & GetValues(void) const
		{
			return m_Vec3f;
		}

	};

	class SetMatEmissive_Event final
		: public CITS::SetVec3f_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMatEmissive;

	public:
		explicit SetMatEmissive_Event(const float p_X, const float p_Y, const float p_Z)
			: SetVec3f_Event(p_X, p_Y, p_Z) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}

	};

	class SetMatAmbient_Event final
		: public CITS::SetVec3f_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMatAmbient;

	public:
		explicit SetMatAmbient_Event(const float p_X, const float p_Y, const float p_Z)
			: SetVec3f_Event(p_X, p_Y, p_Z) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class SetMatDiffuse_Event final
		: public CITS::SetVec3f_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMatDiffuse;

	public:
		explicit SetMatDiffuse_Event(const float p_X, const float p_Y, const float p_Z)
			: SetVec3f_Event(p_X, p_Y, p_Z) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class SetMatSpecular_Event final
		: public CITS::SetVec3f_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMatSpecular;

	public:
		explicit SetMatSpecular_Event(const float p_X, const float p_Y, const float p_Z)
			: SetVec3f_Event(p_X, p_Y, p_Z) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class SetMatShininess_Event final
		: public CITS::SetVec3f_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMatShininess;

	public:
		explicit SetMatShininess_Event(const double p_Value) : SetVec3f_Event(float(p_Value), 1.0f, 1.0f) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class SetFreeFlightMode_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::SetFreeFlightMode;

	public:
		SetFreeFlightMode_Event(void) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}
	};

	class SetThirdPersonMode_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::SetThirdPersonMode;

	public:
		SetThirdPersonMode_Event(void) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}
	};

	class SetObjectManMode_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::SetObjectManMode;

	public:
		SetObjectManMode_Event(void) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}
	};

	class SetObjectMatMode_Event final
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventTypeId = EventType::SetObjectMatMode;

	public:
		SetObjectMatMode_Event(void) {}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventTypeId;
		}
	};

	class SetTexCoordMutiplier_Event final
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetTexCoordMultiplier;

	public:
		explicit SetTexCoordMutiplier_Event(const double p_Value) : CITS::Control_Event(p_Value) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class SetMeshStrafeDistance_Event
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMeshStrafeDistance;

	public:
		explicit SetMeshStrafeDistance_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

	class SetMeshStrafeSpeed_Event
		: public CITS::Control_Event
	{
	private:
		static const KE::EventType sk_EventType = EventType::SetMeshStrafeSpeed;

	public:
		explicit SetMeshStrafeSpeed_Event(const double p_Delta) : Control_Event(p_Delta) {}

		virtual const KE::EventType GetType(void) const final
		{
			return sk_EventType;
		}
	};

} // CITS ns
