#pragma once
#include "../../../common.hpp"
#include <IL/il.h>
#include <IL/ilu.h>		// might not need this at all.
//#include <IL/ilut.h>	// might not need this at all.

namespace KG
{

	class TextureLoader;
	typedef std::shared_ptr<KG::TextureLoader>	TextureLoader_SmartPtr;

	/*! \class Texture
	*/
	class Texture
	{
		friend TextureLoader;

	public:
		enum class DType // dimension type
		{
			Null, Tex1D, Tex2D, Tex3D, Cube, Size
		};
	
	private:
		static KG::TextureLoader_SmartPtr 	s_spTextureLoader;

		KG::Texture::DType		m_Type;
		GLuint					m_GLTextureHandle;
		ILuint					m_DevILHandle;
		std::string				m_FilePath;
		GLenum					m_Target;

	public:
		static TextureLoader & GetLoader(void);

		Texture(const GLenum p_TextureTarget = GL_TEXTURE_2D, const DType p_DType = DType::Tex2D, const std::string & p_Path = "");
		virtual ~Texture(void);

		const KG::Texture::DType GetType(void) const;
		const GLuint GetGLHandle(void) const;

	private:
		void SetType(const KG::Texture::DType p_Type);
		void SetFilePath(const std::string & p_Path);
		void SetGLHandle(const GLuint p_Handle);
		void SetDevILHandle(const ILuint p_Handle);

	}; // class Texture

	typedef std::shared_ptr<KG::Texture>	Texture_SmartPtr;
	typedef std::weak_ptr<KG::Texture>		Texture_WeakPtr;

}
