#include "ShaderProgram.hpp"

namespace KG
{
	ShaderProgram::ShaderProgram(void)
		: m_Usable(false)
	{}

	ShaderProgram::ShaderProgram(const GLuint p_ShaderProgramID)
		: m_ShaderProgram_id(p_ShaderProgramID)
		, m_Usable(true)
	{}

	ShaderProgram::~ShaderProgram(void)
	{
		glDeleteProgram(m_ShaderProgram_id);
	}

	ShaderProgram & ShaderProgram::Use(void)
	{
		if (m_Usable)
			glUseProgram(m_ShaderProgram_id);
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

	void ShaderProgram::SetID(const GLuint p_ShaderProgramID)
	{
		if (m_Usable) glDeleteProgram(m_ShaderProgram_id);
		m_ShaderProgram_id = p_ShaderProgramID;
		m_Usable = true;
	}

	const GLuint ShaderProgram::GetID(void) const
	{
		return m_ShaderProgram_id;
	}

	const GLuint ShaderProgram::GetAttribLocation(const GLchar * const p_AttribName) const
	{
		GLuint location = glGetAttribLocation(m_ShaderProgram_id, p_AttribName);
		if (location == -1)
			KE::Debug::print(KE::Debug::msg_type::DBG_WARNING, "ShaderProgram : incorrect attrib name = " + std::string(p_AttribName));
		KE::Debug::check_for_GL_error();
		return location;
	}

} // KG ns
