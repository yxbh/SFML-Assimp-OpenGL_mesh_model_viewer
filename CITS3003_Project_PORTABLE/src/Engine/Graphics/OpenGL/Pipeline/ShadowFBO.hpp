#include "FrameBuffer.hpp"

namespace KG
{

	/*! \class
	*/
	class ShadowFBO
		: public KG::FrameBuffer
	{
	private:

	public:
		ShadowFBO(const unsigned p_Width, const unsigned p_Height);
		virtual ~ShadowFBO(void);
	};

} // KG ns
