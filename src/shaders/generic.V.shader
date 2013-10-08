#version 330

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec3 vNormal;
layout(location=3) in vec3 vTexCoord;

// model mesh related.


// model related.
smooth out vec3 fPosition;
smooth out vec3 fNormal;
smooth out vec4 fColor;
smooth out vec2 fTexCoord2D;

// project matrix
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(vPosition, 1.0f); // final position vertex.
	fPosition = vPosition; // position vertex pass through.
	fNormal = vNormal; // normal vec3 pass through

	fColor = vColor; // color v4 pass through
	fTexCoord2D = vTexCoord.xy; // tex coord concan 2D pass through.
}