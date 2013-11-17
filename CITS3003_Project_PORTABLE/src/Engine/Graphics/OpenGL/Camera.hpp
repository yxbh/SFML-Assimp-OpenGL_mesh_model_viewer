#pragma once
#include "../../common.hpp"
#include "../../Graphics/OpenGL/SceneNode.hpp"

namespace KE
{
	class IEvent;
	typedef std::shared_ptr<IEvent> Event_SmartPtr;
}

namespace KG
{

	/*! \class Camera
		A basic 1st person camera.
		Target related functionalies are untested and most likely not working.

		The Camera class has a Field of View mode.
			- Fixedfovy mode: vertical field of view is fixed. Wider screen = More horizontal views.
			- Fixedfovx mode: horizontal field of view is fixed, higher vertical resolution = bigger vertical field.
			- FixedDynamic mode : works as Fixedfovy mode until aspect ratio goes below the limit, then switch to Fixedfovx mode. 
	
		Orientation related methods have to be overloaded and have parameters negated because we don't move the camera,
		but we move the world. The same does not have to be done for position related methods because this is done when
		we calculate the project (we used -m_Position).

		TODO : Overload SetOrientation() from Transform to set Yaw rotation properly.

	*/
	class Camera
		: public KG::SceneNode
	{
	public:
		enum class ProjectionMode {Perspective, Ortho}; // Ortho mode might not work correct.
		enum DefaultFov { Fovy = 43};
		enum class FOVType {Dynamic, FixedFovy, FixedFovx, FixedBoth};
		enum class CamType{FreeFlight, ThirdPerson};

	private:
		glm::dvec3			m_CameraTop;						// direction of the camera's top
		FOVType				m_FOVMode;
		double				m_AspectRatioLimit;					// CITS
		double				m_FOVY, m_FOYX, m_FOVDynamic;		// in degrees
		double				m_AspectRatio;
		double				m_ZFar, m_ZNear;
		ProjectionMode		m_ProjectionMode;
		double				m_DistanceToTarget;
		CamType				m_CameraType;						// defines rotation rules
		bool				m_CameraChanged;

	public:
		Camera(void);

		Camera & SetFOVType(const FOVType p_Type);
		Camera & SetAspectRatio(const double p_ARatio);
		Camera & SetAspectRatio(const double p_Width, const double p_Height);
		Camera & SetFieldOfViewY(const double p_Angle);
		Camera & SetFieldOfViewX(const double p_Angle);
		Camera & SetZFar(const double p_Val);
		Camera & SetZNear(const double p_Val);

		CamType GetCameraType(void);
		void SetCameraMode(CamType p_newType);
		void SetDistanceToTarget(double p_distance);
		void OffsetTargetDistance(double p_DeltaDistance);

		/*! Angle around x axis. */
		virtual Camera & SetPitch(const double p_Angle) override;
		/*! Angle around y axis. */
		virtual Camera & SetYaw(const double p_Angle) override;
		/*! Angle around z axis. */
		virtual Camera & SetRoll(const double p_Angle) override;
		/*! Rotate along y axis. Overloads to rotate along the global y-axis only. */
		virtual Camera & OffsetYaw(const double p_Angle) override;

		const glm::dmat4 GetViewMatrix(void);
		const glm::dmat4 GetProjectionMatrix(void);
		/*! Return camera's project matrix. Projext matrix * View matrix */
		const glm::dmat4 GetViewProjectionMatrixd(void);
		const glm::mat4 GetViewProjectionMatrixf(void);

		bool WindowResizeDelegate(KE::Event_SmartPtr p_spEvent);

	private:
		Camera(const Camera & p_rCamera);
	};

	typedef std::shared_ptr<KG::Camera> Camera_SmartPtr;
} // KE ns