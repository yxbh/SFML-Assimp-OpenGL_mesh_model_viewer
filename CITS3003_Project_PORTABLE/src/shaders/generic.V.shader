#version 330

layout(location=0) in vec3 	vPosition;
layout(location=1) in vec4 	vColor;
layout(location=2) in vec3	vNormal;
layout(location=3) in vec3 	vTexCoord;
layout(location=4) in ivec4 vBoneIDs;
layout(location=5) in vec4 	vBoneWeights;

// model mesh related.

// animation related
uniform	bool HasBones = false;
const int MaxNumBones = 128;
uniform mat4 BoneTranforms[MaxNumBones];

// model related.
smooth out vec3 fPosition;
smooth out vec3 fNormal;
smooth out vec4 fColor;
smooth out vec2 fTexCoord2D;

// project matrix
uniform mat4 mvpMatrix;

void main()
{
	vec3 v_pos = vPosition;
	vec3 v_normal = vNormal;
	if (HasBones)
	{
		mat4 bone_transform = BoneTranforms[vBoneIDs[0]] * vBoneWeights[0] 
							+ BoneTranforms[vBoneIDs[1]] * vBoneWeights[1]
							+ BoneTranforms[vBoneIDs[2]] * vBoneWeights[2]
							+ BoneTranforms[vBoneIDs[3]] * vBoneWeights[3];
		v_pos = vec3(bone_transform * vec4(v_pos, 1.0f));
		v_normal = vec3(bone_transform * vec4(v_normal, 0.0f));
		//v_pos = vec3(inverse(bone_transform) * bone_transform * vec4(v_pos, 1.0f));
		//v_normal = vec3(inverse(bone_transform) * bone_transform * vec4(v_normal, 0.0f));
	}

	gl_Position = mvpMatrix * vec4(v_pos, 1.0f); // final position vertex.
	fPosition = v_pos; // position vertex pass through.
	fNormal = v_normal; // normal vec3 pass through

	fColor = vColor; // color v4 pass through
	fTexCoord2D = vTexCoord.xy; // tex coord concan 2D pass through.
}