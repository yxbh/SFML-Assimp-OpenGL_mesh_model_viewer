#include "ShaderProgram.hpp"

namespace KG
{
	ShaderProgram::ShaderProgram(void)
		: m_Usable(false)
	{}

	ShaderProgram::ShaderProgram(const GLuint p_ShaderProgramID)
		: m_ProgramID(p_ShaderProgramID)
		, m_Usable(true)
	{}

	ShaderProgram::~ShaderProgram(void)
	{
		glDeleteProgram(m_ProgramID);
	}

	ShaderProgram & ShaderProgram::Use(void)
	{
		if (m_Usable)
			glUseProgram(m_ProgramID);
		else
			KE::Debug::print(KE::Debug::msg_type::DBG_WARNING, "ShaderProgram : attempted to used unusable shader program.");
		return *this;
	}

	ShaderProgram & ShaderProgram::Enable(void)
	{
		return this->Use();
	}

	void ShaderProgram::Disable(void) const
	{
		glUseProgram(0);
	}

	void ShaderProgram::SetGLHandle(const GLuint p_ShaderProgramID)
	{
		if (m_Usable) glDeleteProgram(m_ProgramID);
		m_ProgramID = p_ShaderProgramID;
		m_Usable = true;
	}

	const bool ShaderProgram::HasUniform(const std::string & p_rUniform)
	{
		if (m_Uniforms.find(p_rUniform) != m_Uniforms.end()) // in map
			return true;
		const GLint location = glGetUniformLocation(m_ProgramID, p_rUniform.c_str()); // look in shader
		if (location == -1)
			return false;
		m_Uniforms.insert(std::make_pair(p_rUniform, location)); // add to shader if found
		return true;
	}

	const bool ShaderProgram::HasParameter(const std::string & p_rUniform)
	{
		return this->HasUniform(p_rUniform);
	}

	const bool ShaderProgram::HasAttribute(const std::string & p_rAttribute)
	{
		if (m_Attributes.find(p_rAttribute) != m_Attributes.end()) // in map
			return true;
		const GLint location = glGetAttribLocation(m_ProgramID, p_rAttribute.c_str()); // look in shader
		if (location == -1)
			return false;
		m_Attributes.insert(std::make_pair(p_rAttribute, location)); // add to shader if found
		return true;
	}

	const GLint ShaderProgram::GetHandle(void) const
	{
		return m_ProgramID;
	}

	const GLint ShaderProgram::GetAttributeLocation(const GLchar * const p_AttribName) const
	{
		GLint location = glGetAttribLocation(m_ProgramID, p_AttribName);
		if (location == -1)
			KE::Debug::print(KE::Debug::msg_type::DBG_WARNING, "ShaderProgram : incorrect attrib name = " + std::string(p_AttribName));
		KE::Debug::check_for_GL_error();
		return location;
	}

} // KG ns
