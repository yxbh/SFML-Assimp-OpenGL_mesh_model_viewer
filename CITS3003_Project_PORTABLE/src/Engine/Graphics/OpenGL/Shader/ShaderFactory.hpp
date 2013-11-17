#pragma once
#include "../../../common.hpp"

namespace KG
{
	typedef std::pair<GLenum, std::string>					ShaderResource;
	typedef std::vector<std::pair<GLenum, std::string>>		ShaderResourceList;
	typedef GLuint											ShaderID;
	typedef std::vector<GLenum>								ShaderIDList;
	
	class ShaderProgramFactory
	{
	public:
		/*! Create a shader program from a given list of GLSL source codes. Does clean ups. */
		static const GLuint MakeFromSources(ShaderResourceList & p_ShaderResourceList);
		/*! Create a shader program from a given list of file path to to GLSL source codes. Does clean up. */
		static const GLuint MakeFromFiles(ShaderResourceList & p_ShaderResourceList);
		/*! Create a shader program from a given list of shader id's. Shaders are not deattached but not deleted.*/
		static const GLuint MakeFromIDs(ShaderIDList p_ShaderIDs);

		/*! create a shader object. */
		static const GLuint MakeShaderFromSource(const GLenum p_ShaderType, const std::string & p_rShaderSource);
		static const GLuint MakeShaderFromSource(const GLenum p_ShaderType, const char * p_pShaderSource);
		static const GLuint MakeShaderFromFile(const GLenum p_ShaderType, const std::string & p_rShaderSource);

		static const std::string LoadSourceFromFile(const std::string & p_rFilePath);
	private:

		static bool CheckShaderSourceCompilation(const GLenum p_ShaderType, GLuint p_ShaderID);
		static bool CheckShaderProgramLink(const GLuint p_ShaderProgramID);

		
	};


} // KG ns
