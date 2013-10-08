#pragma once
#include "../../common.hpp"

namespace KG
{


	/*! \class MatrixStack
	*/
	class MatrixStack
	{
	private:
		static const glm::dmat4		sk_IdentityMatrix;
		std::stack<glm::dmat4>	m_MatrixStack;
		bool					m_PushEngaged;
		glm::dmat4				m_TopMatrix;

	public:
		MatrixStack(void);

		/*! Save the current top matrix into the stack. */
		void Push(void);

		void Pop(void);
		const glm::dmat4 & Top(void) const;

		/*! Set given matrix as top matrix. */
		void SetMatrix(const glm::dmat4 & p_Mat4);
		/*! Set top matrix to identity matrix. */
		void SetIdentity(void);
	};

	/*! \class PushMatrixStack
		Using RAII to push a MatrixStack. When object is destroyed, MatrixStack is popped.
		Construct one of these objects before using the MatrixStack in a local scope.
	*/
	class PushMatrixStack
	{
	private:
		KG::MatrixStack & m_rMatrixStack;
	public:
		PushMatrixStack(KG::MatrixStack & p_rMatrixStack);
		~PushMatrixStack(void);

	private:
		PushMatrixStack(const KG::PushMatrixStack & p_rPMS);
		void operator=(const KG::PushMatrixStack & p_rPMS);
	}; // PushMatrixStack

} // KG ns
