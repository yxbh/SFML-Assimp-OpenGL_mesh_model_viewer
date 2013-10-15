#include "MatrixStack.hpp"

namespace KG
{

	MatrixStack::MatrixStack(void)
		: m_TopMatrix(1.0f)
	{}

	void MatrixStack::ApplyMatrix(const glm::dmat4 & p_rMat)
	{
		m_TopMatrix = p_rMat * m_TopMatrix;
	}

	void MatrixStack::Push(void)
	{
		m_MatrixStack.push(m_TopMatrix);
		m_TopMatrix = std::move(glm::dmat4(1.0));
	}

	void MatrixStack::Pop(void)
	{
		if (m_MatrixStack.empty()) KE::Debug::print(KE::Debug::DBG_ERROR, "MatrixStack : stack empty.");
		m_TopMatrix = m_MatrixStack.top();
		m_MatrixStack.pop();
	}

	const glm::dmat4 & MatrixStack::GetTop(void) const
	{
		return m_TopMatrix;
	}

	void MatrixStack::SetMatrix(const glm::dmat4 & p_Mat4)
	{
		m_TopMatrix = p_Mat4;
	}

	void MatrixStack::SetIdentity(void)
	{
		m_TopMatrix = std::move(glm::dmat4());
	}


	PushMatrixStack::PushMatrixStack(KG::MatrixStack & p_rMatrixStack)
		: m_rMatrixStack(p_rMatrixStack)
	{
		m_rMatrixStack.Push();
	}

	PushMatrixStack::~PushMatrixStack(void)
	{
		m_rMatrixStack.Pop();
	}

} // KG ns
