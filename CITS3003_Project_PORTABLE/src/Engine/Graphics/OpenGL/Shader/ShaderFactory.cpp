#include "ShaderFactory.hpp"

namespace KG
{
	const GLuint ShaderProgramFactory::MakeFromSources(ShaderResourceList & p_rShaderResourceList)
	{
		assert(p_rShaderResourceList.size() > 0);

		GLuint new_shader_program_id = glCreateProgram();
		GLuint new_shader = GL_ZERO;
		std::vector<GLuint> temp_id_holder; temp_id_holder.reserve(p_rShaderResourceList.size());
		for ( auto shader_resource : p_rShaderResourceList)
		{
			new_shader = ShaderProgramFactory::MakeShaderFromSource(shader_resource.first, shader_resource.second);
			temp_id_holder.push_back(new_shader);
			glAttachShader(new_shader_program_id, new_shader);
		}

		// link and check for failure
		glLinkProgram(new_shader_program_id);
		assert(CheckShaderProgramLink(new_shader_program_id));
		KE::Debug::check_for_GL_error();
		// clean up
		for ( auto shader_id : temp_id_holder)
		{
			glDetachShader(new_shader_program_id, shader_id); //KE::Debug::print(std::to_string(new_shader_program_id) + " " + std::to_string(shader_id));
			glDeleteShader(shader_id);
		}

		return new_shader_program_id;
	} // MakeShaderProgramFromSources()

	const GLuint ShaderProgramFactory::MakeFromFiles(ShaderResourceList & p_rShaderResourceList)
	{
		assert(p_rShaderResourceList.size() > 0);

		GLuint new_shader_program_id = glCreateProgram();
		GLuint new_shader = GL_ZERO;
		std::vector<GLuint> temp_id_holder; temp_id_holder.reserve(p_rShaderResourceList.size());
		for ( auto shader_resource : p_rShaderResourceList)
		{
			new_shader = ShaderProgramFactory::MakeShaderFromSource(shader_resource.first, LoadSourceFromFile(shader_resource.second));
			temp_id_holder.push_back(new_shader);
			glAttachShader(new_shader_program_id, new_shader);
		}

		// link and check for failure
		glLinkProgram(new_shader_program_id);
		assert(CheckShaderProgramLink(new_shader_program_id));
		KE::Debug::check_for_GL_error();
		// clean up
		for ( auto shader_id : temp_id_holder)
		{
			glDetachShader(new_shader_program_id, shader_id); //KE::Debug::print(std::to_string(new_shader_program_id) + " " + std::to_string(shader_id));
			glDeleteShader(shader_id);
		}		

		return new_shader_program_id;
	}

	const GLuint ShaderProgramFactory::MakeFromIDs(ShaderIDList p_ShaderIDs)
	{
		assert(p_ShaderIDs.size() > 0);

		GLuint new_shader_program_id = glCreateProgram();
		for ( auto shader_id : p_ShaderIDs)
			glAttachShader(new_shader_program_id, shader_id);
		glLinkProgram(new_shader_program_id);

		// check for failure
		assert(CheckShaderProgramLink(new_shader_program_id));

		for ( auto shader_id : p_ShaderIDs)
			glDetachShader(new_shader_program_id, shader_id);

		return new_shader_program_id;
	}

	const GLuint ShaderProgramFactory::MakeShaderFromSource(const GLenum p_ShaderType, const std::string & p_rShaderSource)
	{
		return ShaderProgramFactory::MakeShaderFromSource(p_ShaderType, p_rShaderSource.c_str());
	}

	const GLuint ShaderProgramFactory::MakeShaderFromSource(const GLenum p_ShaderType, const char * p_pShaderSource)
	{
		GLuint new_shader_id= glCreateShader(p_ShaderType);
		if (new_shader_id == 0) KE::Debug::print(KE::Debug::msg_type::DBG_ERROR, "error creating shader object.");
		glShaderSource(new_shader_id, 1, &p_pShaderSource, NULL);
		glCompileShader(new_shader_id);

		// check compilation.
		assert(CheckShaderSourceCompilation(p_ShaderType, new_shader_id));

		return new_shader_id;
	} // MakeShader()

	bool ShaderProgramFactory::CheckShaderSourceCompilation(const GLenum p_ShaderType, GLuint p_ShaderID)
	{
		GLint compiler_status;
		glGetShaderiv(p_ShaderID, GL_COMPILE_STATUS, &compiler_status);
		if (GL_FALSE == compiler_status)
		{
			GLint str_len;
			glGetShaderiv(p_ShaderID, GL_INFO_LOG_LENGTH, &str_len);
			GLchar * compiler_log = new GLchar[str_len+1];
			glGetShaderInfoLog(p_ShaderID, str_len, NULL, compiler_log);
			std::string error_str = "ShaderProgramFactory : ";
			switch (p_ShaderType)
			{
			case GL_VERTEX_SHADER:
				error_str += "vertex shader"; break;
			case GL_GEOMETRY_SHADER:
				error_str += "geometry shader"; break;
			case GL_FRAGMENT_SHADER:
				error_str += "fragment shader"; break;
			}
			error_str += " GL error: " + std::string(compiler_log);
			KE::Debug::print(KE::Debug::DBG_ERROR, error_str);
			delete [] compiler_log;
			return false;
		}
		return true;
	}

	bool ShaderProgramFactory::CheckShaderProgramLink(const GLuint p_ShaderProgramID)
	{
		GLint link_status;
		glGetProgramiv(p_ShaderProgramID, GL_LINK_STATUS, &link_status);
		if (GL_FALSE == link_status)
		{
			GLint str_len;
			glGetProgramiv(p_ShaderProgramID, GL_INFO_LOG_LENGTH, &str_len);
			GLchar * linking_log = new GLchar[str_len+1];
			glGetProgramInfoLog(p_ShaderProgramID, str_len, NULL, linking_log);
			KE::Debug::print(KE::Debug::DBG_ERROR, "ShaderProgramFactory : " + std::string(linking_log));
			KE::Debug::check_for_GL_error();
			delete [] linking_log;
			return false;
		}
		return true;
	}

	const std::string ShaderProgramFactory::LoadSourceFromFile(const std::string & p_rFilePath)
	{
		std::ifstream fin;
		fin.open(p_rFilePath, std::ios_base::in);
		if (!fin)
		{
			if ((fin.rdstate() & std::ifstream::eofbit) != 0)
				KE::Debug::print(KE::Debug::DBG_ERROR, "ShaderProgramFactory : unexpected EOF.");
			if ((fin.rdstate() & std::ifstream::failbit) != 0)
				KE::Debug::print(KE::Debug::DBG_ERROR, "ShaderProgramFactory : logic error on input operation.");
			if ((fin.rdstate() & std::ifstream::badbit) != 0)
				KE::Debug::print(KE::Debug::DBG_ERROR, "ShaderProgramFactory : read error on input operation.");
			throw std::exception("failure to open shader source file.");
		}

		std::string shader_source;
		char c_buffer;
		while (fin.get(c_buffer))
			shader_source += c_buffer;

		return shader_source;
	}

} // KG ns
