#include "FrameBuffer.hpp"

namespace KG
{
	FrameBuffer::FrameBuffer(const unsigned p_Widht, const unsigned p_Height)
		: m_Size(p_Widht, p_Height), m_FBO(0), m_Texture2D(0)
	{
		glGenFramebuffers(1, &m_FBO);
	}

	FrameBuffer::~FrameBuffer(void)
	{
		if (m_FBO != 0)
			glDeleteFramebuffers(1, &m_FBO);
		if (m_Texture2D != 0)
			glDeleteTextures(1, &m_Texture2D);
	}

	const glm::uvec2 & FrameBuffer::GetSize(void) const
	{
		return m_Size;
	}

	void FrameBuffer::BindForRead(GLenum p_TextureUnit) const
	{
		if (m_FBO != 0 && m_Texture2D != 0)
		{
			glActiveTexture(p_TextureUnit);
			glBindTexture(GL_TEXTURE_2D, m_Texture2D);
		}
		else
			KE::Debug::print(KE::Debug::DBG_ERROR, "FrameBuffer : BindForRead() error. ");
	}

	void FrameBuffer::BindForWrite(void) const
	{
		if (m_FBO != 0)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
		else
			KE::Debug::print(KE::Debug::DBG_ERROR, "FrameBuffer : BindForWrite() error. ");
	}

	const bool FrameBuffer::CheckFrameBufferError(void) const
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "FrameBuffer : error, code = 0x%x" + std::to_string(status));
			return false;
		}
		return true;
	}


} // KG ns
