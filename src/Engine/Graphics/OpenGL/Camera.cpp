#include "Camera.hpp"
#include "../../Event/EventManager.hpp"
#include "Events.hpp"

namespace KG
{

	Camera::Camera(void)
		: m_CameraTop(0.0, 1.0, 0.0)
		, m_AspectRatio(1.77777777778)
		, m_FovMode(FOVType::Dynamic), m_AspectRatioLimit(1.0)
		, m_FOVY(60), m_FOYX(0 /*below*/), m_FOVDynamic(DefaultFov::Fovy)
		, m_CameraType(CamType::FreeFlight)
		, m_DistanceToTarget( 0.0 )
		, m_ZNear(0.1), m_ZFar(10000.0)
		, m_ProjectionMode(ProjectionMode::Perspective)
		, m_ProjectionMatrix(1.0) , m_ViewMatrix(1.0)
		, m_CameraChanged(false)
	{
		this->SetPosition(0.0, 0.0, 5.0);
		this->SetOrientation(0.0, 0.0, 0.0);
		m_FOYX = KE::Math::RadianToDegree(2*std::atan(std::tan(KE::Math::DegreeToRadian(m_FOVY)/2)*m_AspectRatio));
		m_ProjectionMatrix = GetProjectionMatrix();
		m_ViewMatrix = GetViewMatrix();
		KE::Event::Get().AddListener
			(this->GetEntityID(), std::bind(&KG::Camera::WindowResizeDelegate, this, std::placeholders::_1), KG::Event::Resize);

		KE::Debug::print("Camera : Camera created.");
	}

	Camera & Camera::SetFOVType(const FOVType p_Type)
	{
		m_FovMode = p_Type; return *this;
	}

	Camera & Camera::SetAspectRatio(const double p_ARatio)
	{
		using namespace KE::Math;
		assert(p_ARatio > 0);
		switch (m_FovMode)
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

	/*TransformMatrix & Camera::SetPitch(const double p_Angle)
	{
		return this->SetOrientation(p_Angle, 0.0, 0.0);
	}

	TransformMatrix & Camera::SetYaw(const double p_Angle)
	{
		return this->SetOrientation(0.0, p_Angle, 0.0);
	}

	TransformMatrix & Camera::SetRoll(const double p_Angle)
	{
		return this->SetOrientation(0.0, 0.0, p_Angle);
	}*/

	//TransformMatrix & Camera::SetOrientation(const double p_AngleX, const double p_AngleY, const double p_AngleZ)
	//{
	//	double angle_x(p_AngleX);	double angle_y(p_AngleY);	double angle_z(p_AngleZ);
	//	if (angle_x <= -90.0)	angle_x = -89.5;
	//	if (angle_x >= 90.0)	angle_x = 89.5;
	//	return this->TransformMatrix::SetOrientation(angle_x, angle_y, angle_z);
	//}

	//TransformMatrix & Camera::OffsetOrientation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	//{
	//	double angle_x(p_DeltaX);	double angle_y(p_DeltaY);	double angle_z(p_DeltaZ);
	//	glm::dvec3 current_rotation = this->GetRotationAngles();
	//	angle_x += current_rotation.x; angle_y += current_rotation.y; angle_z += current_rotation.z;
	//	if (angle_x <= -90.0)	angle_x = -89.5;
	//	if (angle_x >= 90.0)	angle_x = 89.5;
	//	return this->TransformMatrix::SetOrientation(angle_x, angle_y, angle_z);
	//}

	/*TransformMatrix & Camera::OffsetPitch(const double p_Angle)
	{
		return this->TransformMatrix::OffsetPitch(p_Angle);
	}*/

	Transform & Camera::OffsetYaw(const double p_Angle)
	{
		if (p_Angle == 0.0) return *this;
		m_OrientationQuat = glm::angleAxis(p_Angle, glm::dvec3(0.0, 1.0, 0.0)) * m_OrientationQuat;
		m_Evaluated = false;return *this;
	}

	/*TransformMatrix & Camera::OffsetRoll(const double p_Angle)
	{
		return this->TransformMatrix::OffsetRoll(p_Angle);
	}*/

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
			switch (m_FovMode)
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
		m_ProjectionMatrix = this->GetProjectionMatrix();
		m_ViewMatrix = this->GetViewMatrix();
		return m_ProjectionMatrix * m_ViewMatrix;
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
} // KE ns
