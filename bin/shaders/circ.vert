#version 130

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec4 epos;	// eye-coordinate position
out vec3 norm;	// per-vertex normal before interpolation
out vec2 tc;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 wpos = model_matrix * vec4(position, 1.0);
	epos = view_matrix * wpos;
	norm = normalize(mat3(view_matrix*model_matrix)*normal);
	tc = texcoord;
	gl_Position = projection_matrix * epos;
}