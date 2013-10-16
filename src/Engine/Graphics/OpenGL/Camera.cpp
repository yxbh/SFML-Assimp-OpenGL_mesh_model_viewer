#include "Camera.hpp"
#include "../../Event/EventManager.hpp"
#include "Events.hpp"

namespace KG
{

	Camera::Camera(void)
		: m_CameraTop(0.0, 1.0, 0.0)
		, m_AspectRatio(1.77777777778)
		, m_FOVMode(FOVType::Dynamic), m_AspectRatioLimit(1.0)
		, m_FOVY(60), m_FOYX(0 /*below*/), m_FOVDynamic(DefaultFov::Fovy)
		, m_CameraType(CamType::FreeFlight)
		, m_DistanceToTarget( 0.0 )
		, m_ZNear(0.1), m_ZFar(10000.0)
		, m_ProjectionMode(ProjectionMode::Perspective)
		, m_CameraChanged(false)
	{
		this->SetPosition(0.0, 0.0, 5.0);
		this->SetOrientation(0.0, 0.0, 0.0);
		m_FOYX = KE::Math::RadianToDegree(2*std::atan(std::tan(KE::Math::DegreeToRadian(m_FOVY)/2)*m_AspectRatio));
		KE::Event::Get().AddListener
			(this->GetEntityID(), std::bind(&KG::Camera::WindowResizeDelegate, this, std::placeholders::_1), KG::Event::Resize);

		KE::Debug::print("Camera : Camera created.");
	}

	Camera & Camera::SetFOVType(const FOVType p_Type)
	{
		m_FOVMode = p_Type; return *this;
	}

	Camera & Camera::SetAspectRatio(const double p_ARatio)
	{
		using namespace KE::Math;
		assert(p_ARatio > 0);
		switch (m_FOVMode)
		{
		case FOVType::Dynamic:
			{
				m_AspectRatio = p_ARatio;
				if (m_AspectRatio >= m_AspectRatioLimit)
				{
					m_FOYX = KE::Math::RadianToDegree(2*std::atan(std::tan(KE::Math::DegreeToRadian(m_FOVY)/2)*m_AspectRatio));
					m_FOVDynamic = DefaultFov::Fovy;
				}
				else
					m_FOVDynamic = RadianToDegree(2.0 * std::atan(tan(DegreeToRadian(m_FOYX) / 2.0) / m_AspectRatio));
				break;
			}
		case FOVType::FixedFovy: // wider screen, more view.
			m_AspectRatio = p_ARatio;
			break;
		case FOVType::FixedFovx: // higher screen, more view.
			m_AspectRatio = p_ARatio;
			m_FOVY = RadianToDegree(2.0 * std::atan(tan(DegreeToRadian(m_FOYX) / 2.0) / m_AspectRatio));
			break;
		case FOVType::FixedBoth:
			m_AspectRatio = p_ARatio;
			break;
		}	
		return *this;
	}

	Camera & Camera::SetAspectRatio(const double p_Width, const double p_Height)
	{
		return this->SetAspectRatio(p_Width/p_Height);
	}

	Camera & Camera::SetFieldOfViewY(const double p_Angle)
	{
		m_FOVY = p_Angle;
		m_FOYX = KE::Math::RadianToDegree(2*std::atan(std::tan(KE::Math::DegreeToRadian(m_FOVY)/2)*m_AspectRatio)); // multiply aspect ratio
		return *this;
	}

	Camera & Camera::SetFieldOfViewX(const double p_Angle)
	{
		m_FOYX = p_Angle;
		m_FOVY = KE::Math::RadianToDegree(2*std::atan(std::tan(KE::Math::DegreeToRadian(m_FOVY)/2)/m_AspectRatio)); // divide aspect ratio
		return *this;
	}

	Camera & Camera::SetZFar(const double p_Val)
	{
		m_ZFar = p_Val; return *this;
	}

	Camera & Camera::SetZNear(const double p_Val)
	{
		m_ZNear = p_Val; return *this;
	}

	Camera::CamType Camera::GetCameraType()
	{
		return m_CameraType;
	}

	void Camera::SetCameraMode(Camera::CamType p_newType)
	{
		m_CameraType = p_newType;
	}

	void Camera::SetDistanceToTarget(double p_distance)
	{
		m_DistanceToTarget = p_distance;
	}
	
	void Camera::OffsetTargetDistance(double p_DeltaDistance)
	{
		m_DistanceToTarget += p_DeltaDistance;
		if(m_DistanceToTarget < 0) m_DistanceToTarget = 0;		
	}

	Camera & Camera::SetPitch(const double p_Angle)
	{ // these are actually quite FPS camera specific at the moment.
		m_OrientationQuat.x = 0.0;
		m_OrientationQuat.z = 0.0;
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		this->OffsetPitch(p_Angle);
		return *this;
	}

	Camera & Camera::SetYaw(const double p_Angle)
	{ // these are actually quite FPS camera specific at the moment.
		m_OrientationQuat.y = 0.0;
		m_OrientationQuat.z = 0.0;
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		this->OffsetYaw(p_Angle);
		return *this;
	}

	Camera & Camera::SetRoll(const double p_Angle)
	{ // these are actually quite FPS camera specific at the moment.
		m_OrientationQuat.z = 0.0;
		//m_OrientationQuat.x = 0.0;
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		this->OffsetRoll(p_Angle);
		return *this;
	}

	Camera & Camera::OffsetYaw(const double p_Angle)
	{
		if (p_Angle == 0.0) return *this;
		m_OrientationQuat = glm::angleAxis(p_Angle, glm::dvec3(0.0, 1.0, 0.0)) * m_OrientationQuat;
		m_Evaluated = false;return *this;
	}

	const glm::dmat4 Camera::GetViewMatrix(void)
	{
		const glm::dmat4 orientation_mat(glm::mat4_cast(glm::conjugate(this->GetOrientationQuat())));
		const glm::dmat4 translation_mat(glm::translate(glm::dmat4(1.0), -this->GetPositionVec3()));
		if(m_CameraType == CamType::FreeFlight)
			return orientation_mat * translation_mat;
		else
		{
			const glm::dmat4 offset(glm::translate(glm::dmat4(),glm::dvec3(0,0,-m_DistanceToTarget)));
			return offset * orientation_mat * translation_mat;
		}
	}

	const glm::dmat4 Camera::GetProjectionMatrix(void) // TODO : put back on const
	{
		if (m_ProjectionMode == ProjectionMode::Perspective)
		{
			switch (m_FOVMode)
			{
			case FOVType::FixedFovy:
				return glm::perspective(m_FOVY, m_AspectRatio, m_ZNear, m_ZFar);
			case FOVType::FixedFovx:
				{
					const double fov = 2.0 * std::atan(tan(KE::Math::DegreeToRadian(m_FOYX) / 2.0) / m_AspectRatio); 
					return glm::perspective(KE::Math::RadianToDegree(fov), m_AspectRatio, m_ZNear, m_ZFar);
				}
			case FOVType::Dynamic:
				return glm::perspective(m_FOVDynamic, m_AspectRatio, m_ZNear, m_ZFar);
			}
			assert(false);
			return glm::perspective(m_FOVY, m_AspectRatio, m_ZNear, m_ZFar);
		}
		else
			return glm::ortho(0.0, 200.0, 0.0, 200.0, m_ZNear, m_ZFar); // need to fix fixed values.
	}

	const glm::dmat4 Camera::GetViewProjectionMatrixd(void)
	{
		return this->GetProjectionMatrix() * this->GetViewMatrix();
	}

	const glm::mat4 Camera::GetViewProjectionMatrixf(void)
	{
		return glm::mat4(this->GetViewProjectionMatrixd());
	}

	bool Camera::WindowResizeDelegate(KE::Event_SmartPtr p_spEvent)
	{
		if (p_spEvent->GetType() != KG::Event::Type::Resize)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Camera : window resize delegate given event with wrong type.");
			return false;
		}
		std::shared_ptr<KG::Event::WindowResize_Event> event_ptr = std::static_pointer_cast<KG::Event::WindowResize_Event>(p_spEvent);
		this->SetAspectRatio(double(event_ptr->GetSize().x), double(event_ptr->GetSize().y));
		return true;
	}

} // KE ns
