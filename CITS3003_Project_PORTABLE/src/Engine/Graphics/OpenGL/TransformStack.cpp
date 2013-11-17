#include "TransformStack.hpp"

namespace KG
{

	TransformStack::TransformStack(void)
	{}

	void TransformStack::ApplyTransform(KG::Transform & p_rTransform)
	{
		m_CurrentTransform = p_rTransform * m_CurrentTransform;
	}

	void TransformStack::Push(void)
	{
		m_TransformStack.push(m_CurrentTransform);
	}

	void TransformStack::Pop(void)
	{
		m_TransformStack.pop();
	}

	Transform & TransformStack::GetTop(void)
	{
		return m_CurrentTransform;
	}

	void TransformStack::SetTransform(const KG::Transform & p_rTransform)
	{
		m_CurrentTransform = p_rTransform;
	}

	void TransformStack::SetIdentity(void)
	{
		m_CurrentTransform = KG::Transform();
	}

} // KG ns
