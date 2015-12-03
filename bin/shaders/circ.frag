#version 130

in vec4 epos;
in vec3 norm;
in vec2 tc;

out vec4 fragColor;

uniform mat4	view_matrix;
uniform vec4	light_position, Ia, Id, Is;	// light
uniform vec4	Ka, Kd, Ks;					// material properties
uniform float	shininess;
uniform bool	blinnEnabled;
uniform bool	blendEnabled;

uniform sampler2D TEX1;

void main()
{
	if(blinnEnabled)
	{
		// light position in the eye-space coordinate
		vec4 lpos = view_matrix*light_position;

		vec3 n = normalize(norm);	// norm interpolated via rasterizer should be normalized again here
		vec3 p = epos.xyz;			// 3D position of this fragment
		vec3 l = normalize(lpos.xyz-(lpos.a==0.0?vec3(0):p));	// lpos.a==0 means directional light
		vec3 v = normalize(-p);		// eye-epos = vec3(0)-epos
		vec3 h = normalize(l+v);	// the halfway vector

		vec4 Ira = Ka*Ia;									// ambient reflection
		vec4 Ird = max(Kd*dot(l,n)*Id,0.0);					// diffuse reflection
		vec4 Irs = max(Ks*pow(dot(h,n),shininess)*Is,0.0);	// specular reflection

		fragColor = texture2D(TEX1, tc) * (Ira + Ird + Irs);
	} else {
		fragColor = texture2D(TEX1, tc);
	}
	if(blendEnabled)
		fragColor.a = 0.5;
}