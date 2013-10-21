#include "Texture.hpp"
#include "TextureLoader.hpp"


namespace KG
{
	KG::TextureLoader_SmartPtr	Texture::s_spTextureLoader(nullptr);

	TextureLoader & Texture::GetLoader(void)
	{
		if (s_spTextureLoader == nullptr)
			s_spTextureLoader = KG::TextureLoader_SmartPtr(new KG::TextureLoader());
		check_for_DevIL_error();
		return *s_spTextureLoader;
	}

	Texture::Texture(const GLenum p_TextureTarget, const DType p_DType, const std::string & p_Path)
		: m_Type(DType::Tex2D), m_GLTextureHandle(0), m_DevILHandle(0)
		, m_Target(p_TextureTarget)
		, m_FilePath(p_Path)
	{
		if (p_Path.length() == 0)
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : ctor given empty string.");
		m_FilePath = p_Path;

		KG::Texture::GetLoader();
		ilGenImages(1, &m_DevILHandle);
		ilBindImage(m_DevILHandle);
		if (!ilLoadImage(p_Path.c_str())) // load and if fails
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : DevIL ilLoadImage(string) loading failed.");
			KE::Debug::print(KE::Debug::DBG_ERROR, " - > File path = " + p_Path);
			KG::check_for_DevIL_error();
			return;
		}
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : DevIL ilConvertImage(,) failed.");
			return;
		}

		glGenTextures(1, &m_GLTextureHandle);
		switch (m_Type)
		{
		case KG::Texture::DType::Tex2D:
			glBindTexture(GL_TEXTURE_2D, m_GLTextureHandle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO : provide option to change this.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D
			(
				GL_TEXTURE_2D								// Texture target.
				, 0										
				, GL_SRGB_ALPHA //ilGetInteger(IL_IMAGE_FORMAT)  # Use GL_SRGB_ALPHA to accomdate Gamma correction in shader. This converts image to linear space?
				, ilGetInteger(IL_IMAGE_WIDTH)
				, ilGetInteger(IL_IMAGE_HEIGHT)
				, 0
				, GL_RGBA //ilGetInteger(IL_IMAGE_FORMAT)
				, ilGetInteger(IL_IMAGE_TYPE)
				, ilGetData()
			);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
		default:
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : unsupported Texture type.");
		}
		KE::Debug::check_for_GL_error();
		ilBindImage(0);
		ilDeleteImage(m_DevILHandle);
	}
	Texture::~Texture(void)
	{
		glDeleteTextures(1, &m_GLTextureHandle);
	}

	const KG::Texture::DType Texture::GetType(void) const
	{
		return m_Type;
	}

	const GLuint Texture::GetGLHandle(void) const
	{
		return m_GLTextureHandle;
	}

	void Texture::SetType(const KG::Texture::DType p_Type)
	{
		m_Type = p_Type;
	}

	void Texture::SetFilePath(const std::string & p_Path)
	{
		m_FilePath = p_Path;
	}

	void Texture::SetGLHandle(const GLuint p_Handle)
	{
		m_GLTextureHandle = p_Handle;
	}

	void Texture::SetDevILHandle(const ILuint p_Handle)
	{
		m_DevILHandle = p_Handle;
	}


}