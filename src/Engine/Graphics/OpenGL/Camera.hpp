#pragma once
#include "../../common.hpp"
#include "../../Graphics/OpenGL/SceneNode.hpp"
#include "../../../OpenGL/TransformMatrix.hpp"

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
		FOVType				m_FovMode;
		double				m_AspectRatioLimit;					// CITS
		double				m_FOVY, m_FOYX, m_FOVDynamic;		// in degrees
		double				m_AspectRatio;
		double				m_ZFar, m_ZNear;
		ProjectionMode		m_ProjectionMode;
		glm::dmat4			m_ProjectionMatrix;					//
		glm::dmat4			m_ViewMatrix;						// the camera view matrix
		double				m_DistanceToTarget;
		CamType				m_CameraType;						// defines rotation rules

	public:
		Camera(void);

		Camera & SetFOVType(const FOVType p_Type);
		Camera & SetAspectRatio(const double p_ARatio);
		Camera & SetAspectRatio(const double p_Width, const double p_Height);
		Camera & SetFieldOfViewY(const double p_Angle);
		Camera & SetFieldOfViewX(const double p_Angle);
		Camera & SetZFar(const double p_Val);
		Camera & SetZNear(const double p_Val);

		/*! Rotate along x axis. */
		virtual TransformMatrix & SetPitch(const double p_Angle) override;
		/*! Rotate along y axis. */
		virtual TransformMatrix & SetYaw(const double p_Angle) override;
		/*! Rotate along z axis. */
		virtual TransformMatrix & SetRoll(const double p_Angle) override;
		virtual TransformMatrix & SetOrientation(const double p_AngleX, const double p_AngleY, const double p_AngleZ) override;
		virtual TransformMatrix & OffsetOrientation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ) override;
		/*! Rotate along x axis. */
		virtual TransformMatrix & OffsetPitch(const double p_Angle) override;
		/*! Rotate along y axis. */
		virtual TransformMatrix & OffsetYaw(const double p_Angle) override;
		/*! Rotate along z axis. */
		virtual TransformMatrix & OffsetRoll(const double p_Angle) override;

		/*! Strafe delta units relative to the TransformMatrix's orientation. */
		virtual void StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const KG::TransformMatrix & p_rTransform) override;
		/*! Strafe delta units relative to given 4x4 matrix's rotation/orientation. */
		virtual void StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const glm::dmat4 & p_rOrientation) override;

		virtual const glm::dmat4 GetRotationMat(void) const override;
		virtual const glm::dmat4 GetOrientationMat(void) const override;

		const glm::dmat4 GetViewMatrix(void);
		const glm::dmat4 GetProjectionMatrix(void);
		/*! Return camera's project matrix. Projext matrix * View matrix */
		const glm::dmat4 GetViewProjectionMatrixd(void);
		const glm::mat4 GetViewProjectionMatrixf(void);

		bool WindowResizeDelegate(KE::Event_SmartPtr p_spEvent);

		CamType GetCameraType(void);
		void SetCameraMode(CamType p_newType);
		void SetDistanceToTarget(double p_distance);
		void OffsetTargetDistance(double p_DeltaDistance);

	private:
		Camera(const Camera & p_rCamera);
	};

	typedef std::shared_ptr<KG::Camera> Camera_SmartPtr;
} // KE ns