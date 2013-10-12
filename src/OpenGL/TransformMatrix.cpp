#include "TransformMatrix.hpp"

namespace KG
{
	TransformMatrix::TransformMatrix(void)
		: m_Scale(1.0, 1.0, 1.0)
		, m_Angles(0.0, 0.0, 0.0)
		, m_Position(0.0, 0.0, 0.0)
		, m_FinalTransformMatrix(1.0)
		, m_Evaluated(true)
	{}

	TransformMatrix::TransformMatrix(const glm::mat4 p_Mat4f)
		: m_Scale(1.0)
		, m_Position(0.0)
		, m_FinalTransformMatrix(p_Mat4f)
		, m_Evaluated(true)
	{}

	TransformMatrix::TransformMatrix(const glm::dmat4 p_Mat4d)
		: m_Scale(1.0)
		, m_Position(0.0)
		, m_FinalTransformMatrix(p_Mat4d)
		, m_Evaluated(true)
	{}

	TransformMatrix & TransformMatrix::SetPosition(const double p_PosX, const double p_PosY, const double p_PosZ)
	{
		m_Position.x = p_PosX; m_Position.y = p_PosY; m_Position.z = p_PosZ;
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::SetTarget(const double p_ValX, const double p_ValY, const double p_ValZ)
	{
		m_Target.x = p_ValX; m_Target.y = p_ValY; m_Target.z = p_ValZ;
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::SetScale(const double p_ScaleX, const double p_ScaleY, const double p_ScaleZ)
	{
		m_Scale.x = p_ScaleX; m_Scale.y = p_ScaleY; m_Scale.z = p_ScaleZ;
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::SetRotation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Angles.x = p_DeltaX; m_Angles.y = p_DeltaY; m_Angles.z = p_DeltaZ;
		if (m_Angles.x < -180.0)	m_Angles.x += 360.0;
		if (m_Angles.x > 180.0)		m_Angles.x -= 360.0;
		if (m_Angles.y < -180.0)	m_Angles.y += 360.0;
		if (m_Angles.y > 180.0)		m_Angles.y -= 360.0;
		if (m_Angles.z < -180.0)	m_Angles.z += 360.0;
		if (m_Angles.z > 180.0)		m_Angles.z -= 360.0;
		UpdateOrientation();
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::SetOrientation(const double p_AngleX, const double p_AngleY, const double p_AngleZ)
	{
		return this->SetRotation(p_AngleX, p_AngleY, p_AngleZ);
	}

	TransformMatrix & TransformMatrix::SetOrientationQuat(const glm::dquat & p_rquat)
	{
		m_OrientationQuat = p_rquat;
		m_Evaluated = false;
		return *this;
	}

	TransformMatrix & TransformMatrix::SetPitch(const double p_Angle)
	{
		m_Angles.x = p_Angle;
		this->UpdateOrientation();
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::SetYaw(const double p_Angle)
	{
		m_Angles.y = p_Angle;
		this->UpdateOrientation();
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::SetRoll(const double p_Angle)
	{
		m_Angles.z = p_Angle;
		this->UpdateOrientation();
		m_Evaluated = false;return *this;
	}

	TransformMatrix & TransformMatrix::SetOrientationToPosition(const double p_PosX, const double p_PosY, const double p_PosZ)
	{
		return this->SetOrientationToPosition(glm::dvec3(p_PosX, p_PosY, p_PosZ));
	}

	TransformMatrix & TransformMatrix::SetOrientationToPosition(const glm::dvec3 & p_Pos)
	{
		// untested
		assert(p_Pos != m_Position);
		glm::dvec3 direction = glm::normalize(p_Pos - m_Position);
		m_Angles.y = KE::Math::RadianToDegree(asin(-direction.y));
		m_Angles.x = KE::Math::RadianToDegree(atan2(-direction.x, -direction.z));
		this->UpdateOrientation();
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::OffsetScale(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Scale.x += p_DeltaX; m_Scale.y += p_DeltaY; m_Scale.z += p_DeltaZ;
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::OffsetPosition(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Position.x += p_DeltaX; m_Position.y += p_DeltaY;	m_Position.z += p_DeltaZ;
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::OffsetTarget(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Target.x += p_DeltaX; m_Target.y += p_DeltaY; m_Target.z += p_DeltaZ;
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::OffsetRotation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		return this->OffsetOrientation(p_DeltaX, p_DeltaY, p_DeltaZ);
	}

	TransformMatrix & TransformMatrix::OffsetOrientation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Angles.x += p_DeltaX; m_Angles.y += p_DeltaY; m_Angles.z += p_DeltaZ;
		this->UpdateOrientation();
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::OffsetPitch(const double p_Angle)
	{
		m_Angles.x += p_Angle;
		this->UpdateOrientation();
		m_Evaluated = false; return *this;
	}

	TransformMatrix & TransformMatrix::OffsetYaw(const double p_Angle)
	{
		m_Angles.y += p_Angle;
		this->UpdateOrientation();
		m_Evaluated = false;return *this;
	}

	TransformMatrix & TransformMatrix::OffsetRoll(const double p_Angle)
	{
		m_Angles.z += p_Angle;
		this->UpdateOrientation();
		m_Evaluated = false;return *this;
	}

	void TransformMatrix::StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const KG::TransformMatrix & p_rTransform)
	{
		this->StrafeRelativeTo(p_DeltaX, p_DeltaY, p_DeltaZ, p_rTransform.GetOrientationMat());
	}

	void TransformMatrix::StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const glm::dmat4 & p_rOrientation)
	{
		using namespace glm;
		const dvec4 result = p_rOrientation * dvec4(p_DeltaX, p_DeltaY, p_DeltaZ, 1.0);
		m_Position.x += result.x;	m_Position.y += result.y;	m_Position.z +=	result.z;
		m_Evaluated = false;
	}

	//void TransformMatrix::RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const KG::TransformMatrix & p_rTransform)
	//{

	//}

	void TransformMatrix::StrafeUp(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0,p_Delta, 0.0, this->GetOrientationMat());
	}

	void TransformMatrix::StrafeDown(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0,-p_Delta, 0.0, this->GetOrientationMat());
	}

	void TransformMatrix::StrafeLeft(const double p_Delta)
	{
		this->StrafeRelativeTo(-p_Delta, 0.0, 0.0, this->GetOrientationMat());
	}

	void TransformMatrix::StrafeRight(const double p_Delta)
	{		
		this->StrafeRelativeTo(p_Delta, 0.0, 0.0, this->GetOrientationMat());			
	}

	void TransformMatrix::StrafeForward(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0, 0.0, -p_Delta, this->GetOrientationMat());
	}

	void TransformMatrix::StrafeBackward(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0, 0.0, p_Delta, this->GetOrientationMat());
	}

	const glm::dvec3 & TransformMatrix::GetPositionVec(void) const
	{
		return m_Position;
	}

	const glm::dvec3 TransformMatrix::GetDirectionVec(void) const
	{
		const glm::dmat4 rotation_mat(this->GetOrientationMat());
		const glm::dvec4 direction = rotation_mat * glm::dvec4(0.0, 0.0, -1.0, 0.0);
		return glm::normalize(glm::dvec3(direction));
	}

	const glm::dvec3 TransformMatrix::GetTargetVec(void) const
	{
		return m_Target;
	}

	const glm::dmat4 TransformMatrix::GetPositionMat(void)
	{
		return glm::translate(m_Position);
	}

	//void TransformMatrix::RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const glm::dvec3 p_Axis)
	//{
	//	glm::dquat rotation = glm::normalize(glm::angleAxis(p_AngleY, p_Axis) * glm::dquat());
	//	rotation = glm::normalize(glm::angleAxis(p_AngleX, p_Axis) * rotation); // X come after Y
	//	rotation = glm::normalize(glm::angleAxis(p_AngleZ, p_Axis) * rotation);
	//	m_Orientation = rotation * m_Orientation;
	//	m_Angles = glm::eulerAngles(m_Orientation) + m_Angles;
	//}

	glm::dquat TransformMatrix::GetOrientationQuat()
	{
		return m_OrientationQuat;
	}

	const glm::dvec3 & TransformMatrix::GetRotationAngles(void) const
	{
		return m_Angles;
	}

	const glm::dmat4 TransformMatrix::GetRotationMat(void) const
	{
		return glm::mat4_cast(m_OrientationQuat);
	}

	const glm::dmat4 TransformMatrix::GetOrientationMat(void) const
	{
		return this->GetRotationMat();
	}

	void TransformMatrix::UpdateOrientation()
	{
		m_OrientationQuat = glm::normalize(glm::angleAxis(m_Angles.y, glm::dvec3(0.0, 1.0, 0.0)) * glm::dquat());
		m_OrientationQuat = glm::normalize(glm::angleAxis(m_Angles.x, glm::dvec3(1.0, 0.0, 0.0)) * m_OrientationQuat); // X come after Y
		m_OrientationQuat = glm::normalize(glm::angleAxis(m_Angles.z, glm::dvec3(0.0, 0.0, 1.0)) * m_OrientationQuat);
	}

	const GLfloat * const TransformMatrix::GetRawPtrF(void)
	{
		this->Evaluate();
		return glm::value_ptr(glm::fmat4(m_FinalTransformMatrix));
	}

	const glm::dmat4 & TransformMatrix::GetGLMMatd(void)
	{
		this->Evaluate();
		return m_FinalTransformMatrix;
	}

	const glm::mat4 TransformMatrix::GetGLMMatf(void)
	{
		return glm::fmat4(this->GetGLMMatd());
	}

	TransformMatrix & TransformMatrix::Evaluate(void)
	{
		if (!m_Evaluated)
		{
			m_FinalTransformMatrix = glm::translate(m_Position) * this->GetRotationMat() * glm::scale(m_Scale);
			m_Evaluated = true;
		}
		return *this;
	}

	TransformMatrix operator*(TransformMatrix & p_rTM, const glm::dmat4 p_GLMMat4)
	{
		return p_rTM.GetGLMMatd() * p_GLMMat4;
	}
	
	TransformMatrix operator*(const glm::dmat4 p_GLMMat4, TransformMatrix & p_rTM)
	{
		return p_GLMMat4 * p_rTM.GetGLMMatd();
	}

	TransformMatrix operator*(TransformMatrix & p_TM,const glm::mat4 & p_rGLMMat4)
	{
		return p_TM * glm::dmat4(p_rGLMMat4);
	}

	TransformMatrix operator*(const glm::mat4 p_GLMMat4, TransformMatrix & p_rTM)
	{
		return glm::dmat4(p_GLMMat4) * p_rTM.GetGLMMatd();
	}

	TransformMatrix operator*(TransformMatrix & p_rTMLeft, TransformMatrix & p_rTMRight)
	{
		return p_rTMLeft.GetGLMMatd() * p_rTMRight.GetGLMMatd();
	}

	const glm::dmat4 mat4_cast(KG::TransformMatrix & p_rTransformMatrix)
	{
		return glm::dmat4(p_rTransformMatrix.GetGLMMatd());
	}

} // KG ns
