#pragma once
#include "../../common.hpp"
#include "Transform.hpp"

namespace KG
{

	/*! \class TransformStack
		
		Similar to the MatrixStack class. This is a stack structure for the KG::Transform class.
	*/
	class TransformStack
	{
	private:
		std::stack<KG::Transform>	m_TransformStack;
		KG::Transform				m_CurrentTransform;

	public:
		/*! Ctor. */
		TransformStack(void);

		void ApplyTransform(KG::Transform & p_rTransform);
		void Push(void);
		void Pop(void);
		Transform & GetTop(void);

		void SetTransform(const KG::Transform & p_rTransform);
		void SetIdentity(void);

	};

} // KG ns
