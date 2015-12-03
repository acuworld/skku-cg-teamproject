#pragma once

struct light_t
{
	vec4 position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct material_t
{
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 1000.0f;
};

struct texmaterial_t : public material_t
{
	GLuint diffuseTexture;
	GLuint bumpTexture;
	GLuint cubemapTexture;
};

light_t		light;
material_t	material;

void update_light(GLuint program)
{
	// setup light properties
	glUniform4fv(glGetUniformLocation(program, "light_position"), 1, light.position);
	glUniform4fv(glGetUniformLocation(program, "Ia"), 1, light.ambient);
	glUniform4fv(glGetUniformLocation(program, "Id"), 1, light.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Is"), 1, light.specular);

	// setup material properties
	glUniform4fv(glGetUniformLocation(program, "Ka"), 1, material.ambient);
	glUniform4fv(glGetUniformLocation(program, "Kd"), 1, material.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Ks"), 1, material.specular);
	glUniform1f(glGetUniformLocation(program, "shininess"), material.shininess);
}