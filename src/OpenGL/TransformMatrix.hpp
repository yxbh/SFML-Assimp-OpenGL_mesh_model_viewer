#pragma once
#include "../Engine/common.hpp"
#include "../Engine/Graphics/OpenGL/Maths.hpp"

namespace KG
{

	/*! \class TransformMatrix
		Following the general OpenGL and glm convention. Matrices are done in column major form.
		This means that right multiplication is used.

		Of all the methods. The "Strafe" methods are the most interseting. They will translate the
		TransformMatrix some given delta units in the TransformMatrix's orientation. For example: If
		a TransformMatrix is current oriented forward in the +ve Z direction with 15 degrees of angle
		of elevation. StrafeForward will translate the TransformMatrix delta units that direction.
		Accounting for the orientation. So besides moving forward in +ve Z direction. The TransformMatrix
		will also translate up sin(angle) units.

		refer to 
		http://glm.g-truc.net/0.9.4/api/a00151.html#gaacb9cbe8f93a8fef9dc3e25559df19c0 
		and http://glm.g-truc.net/0.9.4/api/a00206.html#gad9b1d9ea9a64ba5e76dbe807abf8362c
		for glm functions used.
	*/
	class TransformMatrix
	{
	private:		
		glm::dvec3			m_Scale;
		glm::dvec3			m_Angles;
		glm::dvec3			m_Position;			// camera location.
		glm::dvec3			m_Target;			// target of the camera.(where it's pointing)
		glm::dmat4			m_FinalTransformMatrix;
		glm::dquat			m_OrientationQuat;

		bool				m_Evaluated;

	public:
		TransformMatrix(void);
		TransformMatrix(const glm::mat4 p_Mat4f);
		TransformMatrix(const glm::dmat4 p_Mat4d);
	
		TransformMatrix & SetPosition(const double p_PosX, const double p_PosY, const double p_PosZ);
		TransformMatrix & SetTarget(const double p_ValX, const double p_ValY, const double p_ValZ);
		TransformMatrix & SetScale(const double p_ScaleX, const double p_ScaleY, const double p_ScaleZ);
		/*! Rotate angles in degrees. In order X, Z, Y axis. */
		virtual TransformMatrix & SetRotation(const double p_AngleX, const double p_AngleY, const double p_AngleZ);
		/*! Set orientation. Wrapper over SetRotation. */
		virtual TransformMatrix & SetOrientation(const double p_AngleX, const double p_AngleY, const double p_AngleZ);
		/*! */
		TransformMatrix & SetOrientationQuat(const glm::dquat & p_rquat);
		/*! Rotate along x axis. */
		virtual TransformMatrix & SetPitch(const double p_Angle);
		/*! Rotate along y axis. */
		virtual TransformMatrix & SetYaw(const double p_Angle);
		/*! Rotate along z axis. */
		virtual TransformMatrix & SetRoll(const double p_Angle);
		virtual TransformMatrix & SetOrientationToPosition(const double p_PosX, const double p_PosY, const double p_PosZ);
		virtual TransformMatrix & SetOrientationToPosition(const glm::dvec3 & p_Pos);

		TransformMatrix & OffsetScale(const double p_ScaleX, const double p_ScaleY, const double p_ScaleZ);
		TransformMatrix & OffsetPosition(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ);
		TransformMatrix & OffsetTarget(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ);
		virtual TransformMatrix & OffsetRotation(const double p_AngleX, const double p_AngleY, const double p_AngleZ);
		virtual TransformMatrix & OffsetOrientation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ);
		/*! Rotate along x axis. */
		virtual TransformMatrix & OffsetPitch(const double p_Angle);
		/*! Rotate along y axis. */
		virtual TransformMatrix & OffsetYaw(const double p_Angle);
		/*! Rotate along z axis. */
		virtual TransformMatrix & OffsetRoll(const double p_Angle);

		/*! Strafe delta units relative to the TransformMatrix's orientation. */
		virtual void StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const KG::TransformMatrix & p_rTransform);
		/*! Strafe delta units relative to given 4x4 matrix's rotation/orientation. */
		virtual void StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const glm::dmat4 & p_rOrientation);
		/*! Translate up using the TransformMatrix as point of reference. */
	/*	void RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const KG::TransformMatrix & p_rTransform);
		void RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const glm::dvec3 p_Axis);*/
		glm::dquat GetOrientationQuat(void);

		virtual void StrafeUp(const double p_Delta);
		virtual void StrafeDown(const double p_Delta);
		virtual void StrafeLeft(const double p_Delta);
		virtual void StrafeRight(const double p_Delta);
		/*! 'Translate' forward in the TransformMatrix's direction. */
		virtual void StrafeForward(const double p_Delta);
		virtual void StrafeBackward(const double p_Delta);

		/*! Return position of camera. */
		const glm::dvec3 & GetPositionVec(void) const;
		/*! Return normalized direction vector of camera. */
		const glm::dvec3 GetDirectionVec(void) const;
		/*! Return direction of camera. */
		const glm::dvec3 GetTargetVec(void) const;
		/*! Get the translation matrix. */
		const glm::dmat4 GetPositionMat(void);
		/*! */
		const glm::dvec3 & GetRotationAngles(void) const;
		/*! Get the rotation matrix. */
		virtual const glm::dmat4 GetRotationMat(void) const;
		/*! Get */
		virtual const glm::dmat4 GetOrientationMat(void) const;

		void UpdateOrientation(void);
		/*! Return the raw pointer to the matrix. Evaluates final transformation matrix if it hasn't already. */
		const GLfloat * const GetRawPtrF(void);
		/*! */
		const glm::dmat4 & GetGLMMatd(void);
		/*! */
		const glm::mat4 GetGLMMatf(void);		
		/*! Evaluate the final transformation matrix if it's not done already. */
		TransformMatrix & Evaluate(void);
		
	}; // TransformationMatrix class

	TransformMatrix operator*(TransformMatrix & p_rTM, const glm::dmat4 p_GLMMat4);
	TransformMatrix operator*(const glm::dmat4 p_GLMMat4, TransformMatrix & p_rTM);
	TransformMatrix operator*(TransformMatrix & p_rTM, const glm::mat4 & p_rGLMMat4);
	TransformMatrix operator*(const glm::mat4 & p_rGLMMat4, TransformMatrix & p_rTM);
	TransformMatrix operator*(TransformMatrix & p_rTMLeft, TransformMatrix & p_rTMRight);

	const glm::dmat4 mat4_cast(KG::TransformMatrix & p_rTransformMatrix);

} // KG ns
