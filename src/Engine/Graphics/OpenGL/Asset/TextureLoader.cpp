#include "TextureLoader.hpp"
#include "Texture.hpp"

namespace KG
{

	TextureLoader::TextureLoader(void)
	{
		ilInit();
		iluInit();
		//ilutInit();
	}

	TextureLoader::~TextureLoader(void)
	{
		ilShutDown(); // won't exit properly on multi-threaded code if this is not called.
	}

	KG::Texture_SmartPtr TextureLoader::LoadFromFile(const std::string & p_Path)
	{
		KG::Texture_SmartPtr texture_ptr(new KG::Texture);

		if (p_Path.length() == 0)
			KE::Debug::print(KE::Debug::DBG_ERROR, "TextureLoader : LoadFromFile(string) given empty string.");
		texture_ptr->SetFilePath(p_Path);

		KG::Texture::GetLoader();
		ILuint il_handle(0);
		ilGenImages(1, &il_handle);
		ilBindImage(il_handle);
		texture_ptr->SetGLHandle(il_handle);
		if (!ilLoadImage(p_Path.c_str())) // load and if fails
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "TextureLoader : LoadFromFile(string), DevIL ilLoadImage(string) loading failed.");
			KE::Debug::print(KE::Debug::DBG_ERROR, " - > File path = " + p_Path);
			KG::check_for_DevIL_error();
			return nullptr;
		}
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "TextureLoader : LoadFromFile(string), DevIL ilConvertImage(,) failed.");
			return nullptr;
		}

		GLuint gl_handle(0);
		glGenTextures(1, &gl_handle);
		texture_ptr->SetGLHandle(gl_handle);
		switch (texture_ptr->GetType())
		{
		case KG::Texture::Type::Tex2D:
			glBindTexture(GL_TEXTURE_2D, gl_handle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO : provide option to change this.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D
			(
				GL_TEXTURE_2D								// Texture target.
				, 0										
				, GL_SRGB_ALPHA //ilGetInteger(IL_IMAGE_FORMAT)
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

		return texture_ptr;
	}

	const bool check_for_DevIL_error(void)
	{
		bool result = true;
		ILenum	error_code = IL_NO_ERROR;
	
		while ((error_code = ilGetError()) != IL_NO_ERROR)
		{
			result = false;
			std::string error_str = "DevIL has encountered an error : ";
			switch (error_code)
			{
				case IL_INVALID_ENUM:
					error_str += "IL_INVALID_ENUM = " + std::string(iluErrorString(IL_INVALID_ENUM));	break;
				case IL_OUT_OF_MEMORY:
					error_str += "IL_OUT_OF_MEMORY = " + std::string(iluErrorString(IL_OUT_OF_MEMORY)); break;
				case IL_FORMAT_NOT_SUPPORTED:
					error_str += "IL_FORMAT_NOT_SUPPORTED = " + std::string(iluErrorString(IL_FORMAT_NOT_SUPPORTED));	break;
				case IL_INTERNAL_ERROR:
					error_str += "IL_INTERNAL_ERROR = " + std::string(iluErrorString(IL_INTERNAL_ERROR)); break;
				case IL_INVALID_VALUE:
					error_str += "IL_INVALID_VALUE = " + std::string(iluErrorString(IL_INVALID_VALUE)); break;
				case IL_ILLEGAL_OPERATION:
					error_str += "IL_ILLEGAL_OPERATION = " + std::string(iluErrorString(IL_ILLEGAL_OPERATION)); break;
				case IL_ILLEGAL_FILE_VALUE:
					error_str += "IL_ILLEGAL_FILE_VALUE = " + std::string(iluErrorString(IL_ILLEGAL_FILE_VALUE)); break;
				case IL_INVALID_FILE_HEADER:
					error_str += "IL_INVALID_FILE_HEADER = " + std::string(iluErrorString(IL_INVALID_FILE_HEADER)); break;
				case IL_INVALID_PARAM:
					error_str += "IL_INVALID_PARAM = " + std::string(iluErrorString(IL_INVALID_PARAM)); break;
				case IL_COULD_NOT_OPEN_FILE:
					error_str += "IL_COULD_NOT_OPEN_FILE = " + std::string(iluErrorString(IL_COULD_NOT_OPEN_FILE)); break;
				case IL_INVALID_EXTENSION:
					error_str += "IL_INVALID_EXTENSION = " + std::string(iluErrorString(IL_INVALID_EXTENSION)); break;
				case IL_FILE_ALREADY_EXISTS:
					error_str += "IL_FILE_ALREADY_EXISTS = " + std::string(iluErrorString(IL_FILE_ALREADY_EXISTS)); break;
				case IL_OUT_FORMAT_SAME:
					error_str += "IL_OUT_FORMAT_SAME = " + std::string(iluErrorString(IL_OUT_FORMAT_SAME)); break;
				case IL_STACK_OVERFLOW:
					error_str += "IL_STACK_OVERFLOW = " + std::string(iluErrorString(IL_STACK_OVERFLOW)); break;
				case IL_STACK_UNDERFLOW:
					error_str += "IL_STACK_UNDERFLOW = " + std::string(iluErrorString(IL_STACK_UNDERFLOW)); break;
				case IL_INVALID_CONVERSION:
					error_str += "IL_INVALID_CONVERSION = " + std::string(iluErrorString(IL_INVALID_CONVERSION)); break;
				case IL_BAD_DIMENSIONS:
					error_str += "IL_BAD_DIMENSIONS = " + std::string(iluErrorString(IL_BAD_DIMENSIONS)); break;
				case IL_FILE_READ_ERROR:
					error_str += "IL_FILE_READ_ERROR = " + std::string(iluErrorString(IL_FILE_READ_ERROR)); break;
				case IL_LIB_JPEG_ERROR:
					error_str += "IL_LIB_JPEG_ERROR = " + std::string(iluErrorString(IL_LIB_JPEG_ERROR)); break;
				case IL_LIB_PNG_ERROR:
					error_str += "IL_LIB_PNG_ERROR = " + std::string(iluErrorString(IL_LIB_PNG_ERROR)); break;
				case IL_LIB_TIFF_ERROR:
					error_str += "IL_LIB_TIFF_ERROR = " + std::string(iluErrorString(IL_LIB_TIFF_ERROR)); break;
				case IL_LIB_MNG_ERROR:
					error_str += "IL_LIB_MNG_ERROR = " + std::string(iluErrorString(IL_LIB_MNG_ERROR)); break;
				case IL_LIB_JP2_ERROR:
					error_str += "IL_LIB_JP2_ERROR = " + std::string(iluErrorString(IL_LIB_JP2_ERROR)); break;
				case IL_LIB_EXR_ERROR:
					error_str += "IL_LIB_EXR_ERROR = " + std::string(iluErrorString(IL_LIB_EXR_ERROR)); break;
				case IL_UNKNOWN_ERROR:
					error_str += "IL_UNKNOWN_ERROR = " + std::string(iluErrorString(IL_UNKNOWN_ERROR)); break;
			}
			KE::Debug::print(KE::Debug::DBG_ERROR, error_str);
		}
		return result;
	}
} // KG ns

