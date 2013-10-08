#include "ShadowFBO.hpp"

namespace KG
{

	ShadowFBO::ShadowFBO(const unsigned p_Width, const unsigned p_Height)
		: KG::FrameBuffer(p_Width, p_Height)
	{
		glGenTextures(1, &m_Texture2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, p_Width, p_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture2D, 0);

		glDrawBuffer(GL_NONE);	// disable write to color buffer.
		glReadBuffer(GL_NONE);	// disable reading color buffer.

		// check for errors.
		KE::Debug::check_for_GL_error();
		assert(this->CheckFrameBufferError());
	}

	ShadowFBO::~ShadowFBO(void)
	{}
} // KG ns
