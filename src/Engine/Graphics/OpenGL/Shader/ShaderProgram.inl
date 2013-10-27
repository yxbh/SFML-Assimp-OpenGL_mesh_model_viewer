
namespace KG
{
	template<typename T>
	const bool ShaderProgram::SetParameter
		(const GLchar * const p_UniformVariable, const T & p_Value)
	{
		assert(false);
	}

	template<>
	const bool ShaderProgram::SetParameter<GLfloat>
		(const GLchar * const p_UniformVariable, const GLfloat & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform1f(location, p_Value);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<GLdouble>
		(const GLchar * const p_UniformVariable, const GLdouble & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform1d(location, p_Value);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<GLint>
		(const GLchar * const p_UniformVariable, const GLint & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform1i(location, p_Value);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<GLuint>
		(const GLchar * const p_UniformVariable, const GLuint & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform1ui(location, p_Value);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<GLboolean>
		(const GLchar * const p_UniformVariable, const GLboolean & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform1i(location, p_Value);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<glm::vec3>
		(const GLchar * const p_UniformVariable, const glm::vec3 & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform3fv(location, 1, glm::value_ptr(p_Value));
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<glm::vec4>
		(const GLchar * const p_UniformVariable, const glm::vec4 & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform4fv(location, 1, glm::value_ptr(p_Value));
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<glm::mat3>
		(const GLchar * const p_UniformVariable, const glm::mat3 & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(p_Value));
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<glm::mat4>
		(const GLchar * const p_UniformVariable, const glm::mat4 & p_Value)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(p_Value));
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<typename T> const bool ShaderProgram::SetParameter
		(const GLchar * const p_UniformVariable, const T & p_Value, const T & p_Value2)
	{
		assert(false);
	}

	template<> const bool ShaderProgram::SetParameter<GLfloat>
		(const GLchar * const p_UniformVariable, const GLfloat & p_Value, const GLfloat & p_Value2)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform2f(location, p_Value, p_Value2);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<GLint>
		(const GLchar * const p_UniformVariable, const GLint & p_Value, const GLint & p_Value2)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform2i(location, p_Value, p_Value2);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<typename T> const bool ShaderProgram::SetParameter
		(const GLchar * const p_UniformVariable, const T & p_Value, const T & p_Value2, const T & p_Value3, const T & p_Value4)
	{
		assert(false);
	}

	template<> const bool ShaderProgram::SetParameter<GLfloat>
		(const GLchar * const p_UniformVariable, const GLfloat & p_Value, const GLfloat & p_Value2, const GLfloat & p_Value3, const GLfloat & p_Value4)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform4f(location, p_Value, p_Value2, p_Value3, p_Value4);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<> const bool ShaderProgram::SetParameter<GLdouble>
		(const GLchar * const p_UniformVariable, const GLdouble & p_Value, const GLdouble & p_Value2, const GLdouble & p_Value3, const GLdouble & p_Value4)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform4d(location, p_Value, p_Value2, p_Value3, p_Value4);
		KE::Debug::check_for_GL_error();
		return true;
	}

	template<>	const bool ShaderProgram::SetParameter<GLint>
		(const GLchar * const p_UniformVariable, const GLint & p_Value, const GLint & p_Value2, const GLint & p_Value3, const GLint & p_Value4)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniform4i(location, p_Value, p_Value2, p_Value3, p_Value4);
		KE::Debug::check_for_GL_error();
		return true;
	}

	const bool ShaderProgram::SetParameter(const GLchar * const p_UniformVariable, const std::vector<glm::mat4> & p_rValue1)
	{
		GLint location;
		if (!this->SearchAndGetUniform(location, p_UniformVariable))
			return false;
		glUniformMatrix4fv(location, p_rValue1.size(), GL_FALSE, glm::value_ptr(p_rValue1[0]));
		KE::Debug::check_for_GL_error();
		return true;
	}

} // KG ns
