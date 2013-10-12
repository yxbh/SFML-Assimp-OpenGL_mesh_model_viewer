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
		return this->SearchAndAddUniform(p_rUniform);
	}

	const bool ShaderProgram::HasParameter(const std::string & p_rUniform)
	{
		return this->HasUniform(p_rUniform);
	}

	const bool ShaderProgram::HasAttribute(const std::string & p_rAttribute)
	{
		if (m_Attributes.find(p_rAttribute) != m_Attributes.end()) // in map
			return true;
		return this->SearchAndAddAttribute(p_rAttribute);
	}

	const GLint ShaderProgram::GetHandle(void) const
	{
		return m_ProgramID;
	}

	const GLint ShaderProgram::GetUniformLocation(const std::string & p_rUniform)
	{
		const auto it = m_Uniforms.find(p_rUniform); // look in hashmap first.
		if (it != m_Uniforms.end()) // found
			return it->second;
		this->SearchAndAddUniform(p_rUniform);
		return ShaderProgram::InvalidUniformLocation;
		//if (this->SearchAndAddUniform(p_rUniform)) // look for it in shader (last resort)
		//	return m_Uniforms.find(p_rUniform)->second;
		//else
		//	return ShaderProgram::InvalidUniformLocation;
	}

	const GLint ShaderProgram::GetAttributeLocation(const std::string & p_rAttribute)
	{
		const auto it = m_Attributes.find(p_rAttribute); // look in hashmap first.
		if (it != m_Attributes.end()) // found
			return it->second;
		if (this->SearchAndAddAttribute(p_rAttribute)) // look for it in shader (last resort)
			return m_Attributes.find(p_rAttribute)->second;
		else
			return ShaderProgram::InvalidAttributeLocation;
	}

	const bool ShaderProgram::SearchAndAddUniform(const char * const p_cstrUniform)
	{
		return this->SearchAndAddUniform(std::string(p_cstrUniform));
	}

	const bool ShaderProgram::SearchAndAddUniform(const std::string & p_rUniform)
	{
		const GLint location = glGetUniformLocation(m_ProgramID, p_rUniform.c_str()); // look in shader
		if (location == -1)
			return false;
		m_Uniforms.insert(std::make_pair(p_rUniform, location)); // add to shader if found
		return true;
	}

	const bool ShaderProgram::SearchAndAddAttribute(const char * const p_cstrAttribute)
	{
		return this->SearchAndAddAttribute(std::string(p_cstrAttribute));
	}

	const bool ShaderProgram::SearchAndAddAttribute(const std::string & p_rAttribute)
	{
		const GLint location = glGetAttribLocation(m_ProgramID, p_rAttribute.c_str()); // look in shader
		if (location == -1)
			return false;
		m_Attributes.insert(std::make_pair(p_rAttribute, location)); // add to shader if found
		return true;
	}

} // KG ns
