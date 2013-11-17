#pragma once
#include "../../common.hpp"

namespace KG
{


	/*! \class MatrixStack

		
	*/
	class MatrixStack
	{
	private:
		std::stack<glm::dmat4>		m_MatrixStack;
		glm::dmat4					m_TopMatrix;

	public:
		/*! Ctor. */
		MatrixStack(void);

		/*! Right multiple the current matrix with the given one. Result matrix becomes the current one. */
		void ApplyMatrix(const glm::dmat4 & p_rMat);
		/*! Save/perserve the current top matrix into the stack. */
		void Push(void);
		/*! Remove the top matrix. Unicorn will poop atomic bombs if there isn't matrix already on the stack. */
		void Pop(void);
		/*! Get const reference to top matrix. */
		const glm::dmat4 & GetTop(void) const;

		/*! Set the current/top matrix to the given matrix. */
		void SetMatrix(const glm::dmat4 & p_Mat4);
		/*! Set current/top matrix to identity matrix. */
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
