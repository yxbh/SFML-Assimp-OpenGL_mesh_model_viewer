#pragma once
#include "../../../common.hpp"

namespace KG
{
	/** \class ShaderProgram
		Representation of a OpenGL shader program.
	*/
	class ShaderProgram
	{
	public:

	private:
		GLint m_ShaderProgram_id;
		bool m_Usable;
	
	public:
		ShaderProgram(void);
		ShaderProgram(const GLuint p_ShaderProgramID);
		~ShaderProgram(void);

		ShaderProgram & Use(void);
		ShaderProgram & Enable(void);
		void Disable(void) const;

		void SetID(const GLuint p_ShaderProgramID);

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

		const GLuint GetID(void) const;
		const GLuint GetAttribLocation(const GLchar * const p_AttribName) const;
	};

	typedef std::shared_ptr<KG::ShaderProgram>	ShaderProgram_SmartPtr;


} // KG ns

#include "ShaderProgram.inl"