#pragma once
#include "../../../common.hpp"
#include <unordered_map>

namespace KG
{
	typedef std::unordered_map<std::string, GLuint>	GLSLVariableHashMap;

	/** \class ShaderProgram
		Representation of a OpenGL shader program.
	*/
	class ShaderProgram
	{
	public:
		static const GLint InvalidAttributeLocation = -1;
		static const GLint InvalidUniformLocation = -1;

	private:
		GLint						m_ProgramID;
		bool						m_Usable;
		KG::GLSLVariableHashMap		m_Uniforms;
		KG::GLSLVariableHashMap		m_Attributes;
	
	public:
		ShaderProgram(void);
		ShaderProgram(const GLuint p_ShaderProgramID);
		~ShaderProgram(void);

		ShaderProgram & Use(void);
		ShaderProgram & Enable(void);
		void Disable(void) const;
		void SetGLHandle(const GLuint p_ShaderProgramID);

		/* all has methods look in internal hash map first. If fails look in shader. */
		const bool HasUniform(const std::string & p_rUniform);
		const bool HasParameter(const std::string & p_rUniform);
		const bool HasAttribute(const std::string & p_rAttribute);

		const GLint GetHandle(void) const;
		/*! Return Uniform location. Returns InvalidUniformLocation if uniform does not exist. */
		const GLint GetUniformLocation(const std::string & p_rUniform);
		/*! Return attribute location. Returns InvalidAttributeLocation if attribute does not exist. */
		const GLint GetAttributeLocation(const std::string & p_rAttribute);

	private:
		/*! Search for uniform in shader. add shader into map is found and return true. return false otherwise. */
		const bool SearchAndAddUniform(const char * const p_cstrUniform);
		const bool SearchAndAddUniform(const std::string & p_rUniform);
		/*! Similar to the uniform version. */
		const bool SearchAndAddAttribute(const char * const p_cstrAttribute);
		const bool SearchAndAddAttribute(const std::string & p_rAttribute);
		/*! */
		const bool SearchAndGetUniform(GLint & p_rLocation, const char * const p_cstrUniform);

	public:
		/* single variables. */
		template<typename T>
		const bool SetParameter(const GLchar * const p_UniformVariable, const T & p_Value);
		/*! GLfloat. */
		template<> const bool SetParameter<GLfloat>
		(const GLchar * const p_UniformVarible, const GLfloat & p_Value);
		/*! GLdouble. */
		template<> const bool SetParameter<GLdouble>
		(const GLchar * const p_UniformVarible, const GLdouble & p_Value);
		/*! GLint. */
		template<> const bool SetParameter<GLint>
		(const GLchar * const p_UniformVarible, const GLint & p_Value);
		/*! GLuint. */
		template<> const bool SetParameter<GLuint>
		(const GLchar * const p_UniformVarible, const GLuint & p_Value);
		/*! GLboolean. */
		template<> const bool SetParameter<GLboolean>
		(const GLchar * const p_UniformVariable, const GLboolean & p_Value);
		/*! glm::vec3f */
		template<> const bool SetParameter<glm::vec3>
		(const GLchar * const p_UniformVariable, const glm::vec3 & p_Value);
		/*! glm::vec4f */
		template<> const bool SetParameter<glm::vec4>
		(const GLchar * const p_UniformVariable, const glm::vec4 & p_Value);
		/*! glm::mat4. */
		template<> const bool SetParameter<glm::mat3>
		(const GLchar * const p_UniformVariable, const glm::mat3 & p_Value);
		/*! glm::mat4. */
		template<> const bool SetParameter<glm::mat4>
		(const GLchar * const p_UniformVariable, const glm::mat4 & p_Value);

		/* 2 variables. */
		template<typename T>
		const bool SetParameter(const GLchar * const p_UniformVariable, const T & p_Value, const T & p_Value2);
		/*! GLfloat. */
		template<> const bool SetParameter<GLfloat>
		(const GLchar * const p_UniformVariable, const GLfloat & p_Value, const GLfloat & p_Value2);
		/*! GLint. */
		template<> const bool SetParameter<GLint>
		(const GLchar * const p_UniformVariable, const GLint & p_Value, const GLint & p_Value2);

		/*! 4 variables. */
		template<typename T> const bool SetParameter
		(const GLchar * const p_UniformVariable, const T & p_Value, const T & p_Value2, const T & p_Value3, const T & p_Value4);
		/*! GLfloat */
		template<> const bool SetParameter<GLfloat>
		(const GLchar * const p_UniformVariable, const GLfloat & p_Value, const GLfloat & p_Value2, const GLfloat & p_Value3, const GLfloat & p_Value4);
		/*! GLdouble. */
		template<> const bool SetParameter<GLdouble>
		(const GLchar * const p_UniformVariable, const GLdouble & p_Value, const GLdouble & p_Value2, const GLdouble & p_Value3, const GLdouble & p_Value4);
		/*! GLint */
		template<> const bool SetParameter<GLint>
		(const GLchar * const p_UniformVariable, const GLint & p_Value, const GLint & p_Value2, const GLint & p_Value3, const GLint & p_Value4);

	}; // ShaderProgram

	typedef std::shared_ptr<KG::ShaderProgram>	ShaderProgram_SmartPtr;


} // KG ns

#include "ShaderProgram.inl"