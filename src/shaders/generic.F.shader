/*
	Author: YanXiang Huang

	References:
	 	- http://www.arcsynthesis.org/gltut/   (light chapters)
	 	- http://ogldev.atspace.co.uk/index.html (light sections)
		- http://tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
		- http://tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
		- https://developer.nvidia.com/content/cg-tutorial-chapter-5-lighting
		- http://msdn.microsoft.com/en-us/library/windows/desktop/bb147399(v=vs.85).aspx
*/

#version 330

/* camera */ 
uniform vec3 CameraPosition;

/*  model related */
smooth in 	vec3 	fPosition;
uniform 	mat4 	modelMatrix;
uniform 	bool	HasNormals 		= false;
uniform 	mat3	ModelRotationMat3;

// animation related
const int MaxNumBones = 64;
uniform mat4 BoneTranforms[MaxNumBones];

// texture & color related.
smooth in 	vec4		fColor;
smooth in 	vec2		fTexCoord2D;
uniform		bool		HasColorVertices	= bool(false);
uniform		bool 		HasTexCoord2D 		= bool(false);
uniform		bool		HasMaterial			= bool(false);
uniform 	sampler2D	Tex2D;
uniform		float		TexCoordMultiplier	= 1.0f;

/* light related */
smooth in 	vec3		fNormal;
uniform		bool		LightBackFace		= bool(false);	// mesh specific.
struct Light // global light
{
	vec3 	Position;					// location light. TODO : change it to just a direction. (its global light/illumination.)
	vec3	Direction;					// Light's direction vector towards all things. We should calculate and normalize on CPU for this.
	vec3 	Ambient;					// ambient intensity
	vec3 	Color;						// light color. AKA: diffuse, Intensity
	int  	Type;	 					// 0: directional, 1: point light, 2: spot light.
	/* attenuation related. */
	float	AttenuateConstantCoeff;		// normally 1.
	float	AttenuateLinearCoeff;
	float	AttenuateQuadraticCoeff;
	/* spotlight related. */
	float 	OuterConeCosAngle;
	float	InnerConeCosAngle;
};
const int MaxNumLights = 8;
uniform Light Lights[MaxNumLights]; // light array.
uniform int NumLights = 1;

/* A model/object's material. */
struct Material
{
	vec3	Emissive;	// self glow (emissive color).
	vec3	Ambient;	// ambient term.  Affected by the global ambient lighting. final_ambient = Material.Ambient * Global.Ambient
	vec3	Diffuse;	// diffuse
	vec3	Specular;	// specular
	float	Shininess;
	// surface color = Emissive + Ambient + Diffuse + Specular
};
// current model/object material. (set by uniform).
uniform Material material;
//Material material = Material(vec3(0, 0, 0), vec3(0.05, 0.05, 0.05), vec3(0.8, 0.8, 0.8), vec3(1, 1, 1), 100);

// final output
out vec4 out_fColor;


/*
	Function declarations.
*/
vec4 ComputeSurfaceColor(void);
vec4 ComputeGamma(vec4 p_LinearColor);
// compute both the specular and diffuse additive components.
void ComputeLightTerms(Light p_Light, vec3 p_FragPos, float p_Shininess, vec3 p_Normal, vec3 p_CameraPos, inout vec3 p_AmbientTerm, inout vec3 p_DiffuseTerm, inout vec3 p_SpecularTerm);
// compute an attenuation factor if the given light is a point light or spot light.
float ComputeAttenuation(Light p_Light, vec3 p_FragPos);
// compute the spot light factor if the given light is a spot light.
float ComputeDualConeSpotLight(Light p_Light, vec3 p_FragPos);

/*
	main
		- Fragment shader entry point.
*/
void main()
{
	// fix normal if normal not provided. TODO : move this into the vertex shader. It will get interpolated from VShader->FShader.
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	if (HasNormals)
		normal = fNormal; // might not need to normalize.
	normal = normalize(ModelRotationMat3*(normal)); // adjust orientation of normal to model's

	// calculate location of fragment/pixel in world space. (AKA : surface position.)
	vec3 frag_pos = vec3(modelMatrix * vec4(fPosition, 1.0f));

	/*
		Calculate light/s
	*/
	//Lights[0] =	GlobalLight;
	vec3 diffuse_term, specular_term, ambient_term;
	vec3 diffuse_sum, specular_sum, ambient_sum;
	diffuse_term = specular_term = diffuse_sum = specular_sum = ambient_sum = vec3(0,0,0);
	for (int i = 0; i < NumLights; ++i)
	{
		ComputeLightTerms(Lights[i], frag_pos, material.Shininess, normal, CameraPosition, ambient_term, diffuse_term, specular_term);
		diffuse_sum += diffuse_term;
		specular_sum += specular_term;
		ambient_sum += ambient_term;
	}
	vec4 surface_color = ComputeSurfaceColor();
	vec4 emissive = vec4(material.Emissive, 0);
	//vec4 ambient = vec4(material.Ambient * GlobalLight.Ambient, 0);
	vec3 diffuse = material.Diffuse * diffuse_sum;
	vec3 specular = material.Specular * specular_sum;
	vec4 ambient = vec4(ambient_sum, 1) * surface_color;
	vec4 light = vec4((diffuse + specular), 1) * surface_color;

	
	vec4 linear_color = emissive + ambient + light;

	out_fColor = ComputeGamma(linear_color);
}

vec4 ComputeSurfaceColor(void)
{
	vec4 color;
	if (HasTexCoord2D)			// has 2D texture
		color = texture(Tex2D, fTexCoord2D * TexCoordMultiplier);
	else if (HasColorVertices)	// has color vertices
		color = fColor;
	else						// use default color
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f); // white
	return color;
}

vec4 ComputeGamma(vec4 p_LinearColor)
{
	const float gamma_factor = 1.0/2.2; // TODO : move this into a uniform. Make it configurable from application.
	return pow(p_LinearColor.rgba, vec4(gamma_factor));
}

void ComputeLightTerms
	(Light p_Light
	, vec3 p_FragPos
	, float p_Shininess
	, vec3 p_Normal
	, vec3 p_CameraPos
	, inout vec3 p_AmbientTerm, inout vec3 p_rDiffuseTerm, inout vec3 p_rSpecularTerm)
{
	p_rDiffuseTerm = vec3(0,0,0);
	p_rSpecularTerm = vec3(0,0,0);
	if (!gl_FrontFacing && !LightBackFace)  // back face of mesh.
		return; // back face light turned off. Only calculate ambient.

	// calculate vector (fragment surface to light source)
	vec3 surface_to_light;
	if (p_Light.Type == 0) // directional. Use direction instead of position.
		surface_to_light = -p_Light.Direction; // assume normalized. Should have been done on CPU anyway.
	else
		surface_to_light = normalize(p_Light.Position - p_FragPos); // normalize.
	

	/*
		# Compute attenuation
	*/
	float attenuation_factor = ComputeAttenuation(p_Light, p_FragPos);

	/*
		# Compute Spot light.
	*/
	float spotlight_factor = ComputeDualConeSpotLight(p_Light, p_FragPos);

	/*
		# Compute Ambient.
	*/
	p_AmbientTerm = material.Ambient * p_Light.Ambient;
	if (p_Light.Type != 0) // not global. then ambient is attenuated and has spot light effect.
		p_AmbientTerm *= attenuation_factor * spotlight_factor;

	/*
		# Compute diffuse
			formula: diffuse = Kd x lightColor x max(N · L, 0)
			 - Kd is the material's diffuse color,
			 - lightColor is the color of the incoming diffuse light,
			 - N is the normalized surface normal,
			 - L is the normalized vector toward the light source, and
			 - P is the point being shaded.
	*/
	float diffuse_coefficient = max(0, dot(p_Normal, surface_to_light)); // calculate cos(angle) of incidence (brightness according to angle)
	p_rDiffuseTerm = diffuse_coefficient * p_Light.Color; // diffuse.
	p_rDiffuseTerm *= attenuation_factor;
	p_rDiffuseTerm *= spotlight_factor;

	/*
		Compute specular
			formula: specular = Ks x lightColor x facing x (max(N · H, 0)) shininess
			 - Ks is the material's specular color,
			 - lightColor is the color of the incoming specular light,
			 - N is the normalized surface normal,
			 - V is the normalized vector toward the viewpoint (eye/camera position),
			 - L is the normalized vector toward the light source,
			 - H is the normalized vector that is halfway between V and L,
			 - P is the point being shaded, and
			 - facing is 1 if N · L is greater than 0, and 0 otherwise.
	*/
	
		vec3 surface_to_camera = normalize(CameraPosition - p_FragPos/*P*/);	// V
		vec3 mid_vec = normalize(surface_to_camera + surface_to_light); // H
		p_rSpecularTerm = p_Light.Color * pow(max(dot(p_Normal, mid_vec), 0), p_Shininess); // maybe have GlobalLight.Specular?
		//p_rSpecularTerm = p_Light.Color * pow(clamp(dot(p_Normal, mid_vec), 0, 1), p_Shininess);
		p_rSpecularTerm *= attenuation_factor;
		p_rSpecularTerm *= spotlight_factor;
		if (diffuse_coefficient/*facing*/ > 0)
	{
	}

}

float ComputeAttenuation(Light p_Light, vec3 p_FragPos)
{	
	float attenuation_factor;
	if (p_Light.Type == 0) // directional
		attenuation_factor = 1;
	else // point light.
	{
		float distance_to_light = distance(p_Light.Position, p_FragPos);
		attenuation_factor
			= 1/
			(
				p_Light.AttenuateConstantCoeff
				+ p_Light.AttenuateLinearCoeff * distance_to_light
				+ p_Light.AttenuateQuadraticCoeff * distance_to_light * distance_to_light
			);
	}
	return attenuation_factor;
}

float ComputeDualConeSpotLight(Light p_Light, vec3 p_FragPos)
{
	if (p_Light.Type == 2) // if light is spot light.
	{
		vec3 light_to_frag = normalize(p_FragPos - p_Light.Position);
		float cos_direction = dot(light_to_frag, p_Light.Direction); // cos_direction = angle between light normal and vector to fragment.
		return smoothstep(p_Light.OuterConeCosAngle, p_Light.InnerConeCosAngle, cos_direction);
	}
	return 1.0;
}