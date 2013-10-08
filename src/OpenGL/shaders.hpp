#pragma once

namespace KG
{
	// basic pass through vertex shader.
	static const GLchar * VertexShader1 =
	{
		"#version 150\n"
		"layout(location=0) in vec4 vPosition;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vPosition;\n"
		"}\n"
	};

	// basic pass through vertex shader. With support for color vertices.
	static const GLchar* VertexShader2 =
	{
		"#version 400 core\n" 
		"layout(location=0) in vec4 in_Position;\n"
		"layout(location=1) in vec4 in_Color;\n" // color vertex array
		"smooth out vec4 vColor;\n" 
		"void main(void)\n"
		"{\n"
		"   gl_Position = in_Position;\n"
		"   vColor = in_Color;\n"
		"}\n"
	};

	// move anticlockwise around the origin.
	static const GLchar* VertexShader3 =
	{
		"#version 330\n"
		"layout(location=0) in vec4 vPosition;\n"
		"layout(location=1) in vec4 in_color;\n"
		"smooth out vec4 fColor;\n"
		"uniform float delta_time;\n"
		"uniform float rotation_duration;\n" // in seconds
		"void main()\n"
		"{\n"
		"	float delta_time_new = delta_time;\n"
		"	float timeScale = 3.14159 * 2.0f / rotation_duration;\n"
		"	float currTime = mod(delta_time_new, rotation_duration);\n"
		"	vec4 offset = vec4(cos(currTime * timeScale) * 0.5f, sin(currTime * timeScale) * 0.5f, 0.0f, 0.0f);\n"
		"	gl_Position = offset + vPosition;\n"
		"	fColor = in_color;\n"
		"}\n"
	};

	static const GLchar* VertexShader4 =
	{
		"#version 330\n"
		"layout(location=0) in vec4 vPosition;\n"
		"layout(location=1) in vec4 vColor;\n"
		"smooth out vec4 fragColor;\n"
		"uniform mat4 offset;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = offset*vPosition;\n"
		"	fragColor = vColor;\n"
		"}\n"
	};

	static const GLchar * VertexShader5 =
	{
		"#version 330\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"smooth out vec4 vColor;\n"
		"uniform vec2 offset;\n"
		"uniform float zNear;\n"
		"uniform float zFar;\n"
		"uniform float frustumScale;\n"
		"void main()\n"
		"{\n"
		"	vec4 cameraPos = position + vec4(offset.x, offset.y, 0.0, 0.0);\n"
		"	vec4 clipPos;\n"
		"	clipPos.xy = cameraPos.xy * frustumScale;\n"
		"	clipPos.z = cameraPos.z * (zNear + zFar) / (zNear - zFar);\n"
		"	clipPos.z += 2 * zNear * zFar / (zNear - zFar);\n"
		"	clipPos.w = -cameraPos.z;\n"
		"	gl_Position = clipPos;\n"
		"	vColor = color;\n"
		"}\n"
	};

	//=====================================================================================

	static const GLchar * FragmentShader1 =
	{
		"#version 400\n"
		"out vec4 out_fragColor;\n"
		"void main()\n"
		"{\n"
		"	out_fragColor = vec4( 1.0, 0.0, 0.0, 1.0 );\n"
		"}\n"
	};

	static const GLchar* FragmentShader2 =
	{
		"#version 400\n"
		"smooth in vec4 fragColor;\n"
		"out vec4 out_fragColor;\n"

		"void main()\n"
		"{\n"
		"	out_fragColor = fragColor;\n"
		"}\n"
	};

	// change color depending on the time
	static const GLchar* FragmentShader3 =
	{
		"#version 400\n"

		"smooth in vec4 fColor;\n"
		"out vec4 out_Color;\n"
		"const vec4 firstColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"const vec4 secondColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
		"uniform float delta_time;\n"
		"uniform float rotation_duration;\n" // in seconds

		"void main()\n"
		"{\n"
		"	float delta_time_new = delta_time;\n"
		"	float timeScale = 3.14159 * 2.0f / rotation_duration;\n"
		"	float currTime = mod(delta_time_new, rotation_duration);\n"
		"	out_Color = vec4(\n"
		"		cos(currTime * timeScale) * 0.5f,\n"
		"		sin(currTime * timeScale) * 0.5f,\n"
		"		tan(currTime * timeScale) * 0.2f,\n"
		"		1.0f\n"
		"	);\n"
		"}\n"
	};

	static const GLchar* FragmentShader4 =
	{
		"#version 330\n"
		" "
		"void main()\n"
		"{\n"
		
		"}\n"
	};

	static const GLchar * FragmentShader5 =
	{
		"#version 330\n"

		"smooth in vec4 vColor;\n"
		"out vec4 fs5_output_color;\n"

		"void main()\n"
		"{\n"
		"	fs5_output_color = vColor;\n"
		"}\n"
	};

} // KG ns
