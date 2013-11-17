#pragma once
#include "../../../common.hpp"

namespace KG
{

	/*! \class FrameBuffer

		An OpenGL context must exist before constructing an object from this class and calling any of it's methods.

	*/
	class FrameBuffer
	{
	private:
		glm::uvec2	m_Size;

	protected:
		GLuint		m_FBO;				// OpenGL FBO handle.
		GLuint		m_Texture2D;		// OpenGL 2D texture handle.

	public:
		FrameBuffer(const unsigned p_Widht, const unsigned p_Height);
		~FrameBuffer(void);

		const glm::uvec2 & GetSize(void) const;
		virtual void BindForRead(GLenum p_TextureUnit) const;
		virtual void BindForWrite(void) const;

		const bool CheckFrameBufferError(void) const;
	};


} // KG ns
