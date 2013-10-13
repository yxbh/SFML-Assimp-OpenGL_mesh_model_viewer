#pragma once
#include "../../../Engine/common.hpp"
#include "Maths.hpp"

namespace KG
{
	// TODO : OrientTo/LookAt method
	/*! \class Transform

		Transform is a representation/container of the scale, position and orientation of some arbitrary
		object. Internally, orientation is defined by a quaternion.

		Of all the methods. The "Strafe" methods are the most interseting. They will translate the
		Transform some given delta units in the Transform's orientation. For example: If
		a Transform is current oriented forward in the +ve Z direction with 15 degrees of angle
		of elevation. StrafeForward will translate the Transform delta units that direction.
		Accounting for the orientation. So besides moving forward in +ve Z direction. The Transform
		will also translate up sin(angle) units. There are also more general versions the strafe methods
		called StrafeRelativeTo which is used by the Strafe* methods internally. It works by Strafing relative
		to a given orientation matrix or Transform object.

		refer to 
		http://glm.g-truc.net/0.9.4/api/a00151.html#gaacb9cbe8f93a8fef9dc3e25559df19c0 
		and http://glm.g-truc.net/0.9.4/api/a00206.html#gad9b1d9ea9a64ba5e76dbe807abf8362c
		for glm functions used.

		reference on obtaining the forward, right and up vectors
		http://www.gamedev.net/topic/348394-quaternion---how-to-extact-at-and-up-vectors/
	*/
	class Transform
	{
	protected:		
		glm::dvec3			m_Scale;
		glm::dvec3			m_Position;
		glm::dquat			m_OrientationQuat;
		glm::dmat4			m_FinalTransformMatrix;
		bool				m_Evaluated;

	public:
		Transform(void);
		Transform(const glm::mat4 p_Mat4f);
		Transform(const glm::dmat4 p_Mat4d);
	
		Transform & SetScale(const double p_ScaleX, const double p_ScaleY, const double p_ScaleZ);
		Transform & SetPosition(const double p_PosX, const double p_PosY, const double p_PosZ);
		/*! Set orientation angles(degrees) in order X, Z, Y axis. */
		virtual Transform & SetOrientation(const double p_AngleX, const double p_AngleY, const double p_AngleZ);
		/*! Replace the current orientation quaternion with the given one. */
		Transform & SetOrientationQuat(const glm::dquat & p_rquat);
		/*! Angle around x axis. */
		virtual Transform & SetPitch(const double p_Angle);
		/*! Angle around y axis. */
		virtual Transform & SetYaw(const double p_Angle);
		/*! Angle around z axis. */
		virtual Transform & SetRoll(const double p_Angle);
		virtual Transform & SetOrientationToPosition(const double p_PosX, const double p_PosY, const double p_PosZ);
		virtual Transform & SetOrientationToPosition(const glm::dvec3 & p_Pos);

		Transform & OffsetScale(const double p_ScaleX, const double p_ScaleY, const double p_ScaleZ);
		Transform & OffsetPosition(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ);
		virtual Transform & OffsetOrientation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ);
		/*! Rotate along x axis. */
		virtual Transform & OffsetPitch(const double p_Angle);
		/*! Rotate along y axis. */
		virtual Transform & OffsetYaw(const double p_Angle);
		/*! Rotate along z axis. */
		virtual Transform & OffsetRoll(const double p_Angle);

		/*! Strafe delta units relative to the TransformMatrix's orientation. */
		virtual void StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, KG::Transform & p_rTransform);
		/*! Strafe delta units relative to given 4x4 matrix's rotation/orientation. */
		virtual void StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const glm::dmat4 & p_rOrientation);
		/*! Translate up using the TransformMatrix as point of reference. */
		/*	void RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const KG::TransformMatrix & p_rTransform);
		void RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const glm::dvec3 p_Axis);*/
		const glm::dquat GetOrientationQuat(void);

		virtual void StrafeUp(const double p_Delta);
		virtual void StrafeDown(const double p_Delta);
		virtual void StrafeLeft(const double p_Delta);
		virtual void StrafeRight(const double p_Delta);
		/*! 'Translate' forward in the TransformMatrix's direction. */
		virtual void StrafeForward(const double p_Delta);
		virtual void StrafeBackward(const double p_Delta);

		/*! Return position of camera. */
		const glm::dvec3 & GetPositionVec3(void) const;
		/*! Return unit forward/view vector. */
		const glm::dvec3 GetForwardVec3(void);
		/*! Return unit up vector. */
		const glm::dvec3 GetUpVec3(void);
		/*! Return unit right vector. */
		const glm::dvec3 GetRightVec3(void);
		/*! Get the translation matrix. */
		const glm::dmat4 GetPositionMat(void) const;
		/*! Return angles for Pitch, Yaw and Roll. Beware these angles are unreliable due to singularity issues. */
		const glm::dvec3 GetEulerAngles(void) const;
		/*! Return oritentation in 4x4 matrix form. */
		const glm::dmat4 GetOrientationMat(void);

		/*! Return the raw pointer to the matrix. Evaluates final transformation matrix if it hasn't already. */
		const GLfloat * const GetRawPtrF(void);
		/*! */
		const glm::dmat4 & GetGLMMatd(void);
		/*! */
		const glm::mat4 GetGLMMatf(void);

	protected:
		/*! Evaluate the final transformation matrix if it's not done already. */
		Transform & Evaluate(void);
		
	}; // Transform class

	Transform operator*(Transform & p_rTM, const glm::dmat4 p_GLMMat4);
	Transform operator*(const glm::dmat4 p_GLMMat4, Transform & p_rTM);
	Transform operator*(Transform & p_rTM, const glm::mat4 & p_rGLMMat4);
	Transform operator*(const glm::mat4 & p_rGLMMat4, Transform & p_rTM);
	Transform operator*(Transform & p_rTMLeft, Transform & p_rTMRight);

	const glm::dmat4 mat4_cast(KG::Transform & p_rTransformMatrix);

} // KG ns
