#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility

#include "keyboard.h"
#include "mouse.h"

#include "light.h"
#include "planets.h"

//*******************************************************************
// include stb_image with the implementation preprocessor definition
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//*******************************************************************
// global constants
static const char*	window_name = "T1 - Team 4";
static const char*	vert_shader_path = "../bin/shaders/circ.vert";
static const char*	frag_shader_path = "../bin/shaders/circ.frag";

//*******************************************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = ivec2(1024, 576);	// initial window size

//*******************************************************************
// OpenGL objects
GLuint	program = 0;					// ID holder for GPU program
GLuint	sphere_vertex_buffer = 0;		// ID holder for sphere vertex buffer
GLuint	sphere_index_buffer = 0;		// ID holder for sphere index buffer
GLuint	ring_vertex_buffer = 0;			// ID holder for ring vertex buffer
GLuint	ring_index_buffer = 0;			// ID holder for ring index buffer

//*******************************************************************
// global variables
float	initAngle = 260.0f;
int		frame = 0;				// index of rendering frames
float	radius = 1.0f;
bool	bWireframe = false;
double	oldTime;
float	lastAngle;

GLuint	texture_planet[10];
GLuint	texture_ring[2];

//*******************************************************************
// objects
mesh*		pMesh = nullptr;
camera		cam;
keypress	pkey;

//*******************************************************************
// holder of vertices and indices
std::vector<vertex> sphere_vertex_list;
std::vector<uint> sphere_index_list;
std::vector<vertex> ring_vertex_list;
std::vector<uint> ring_index_list;

//*******************************************************************
void update()
{
	// move camera as WASD moving
	if (pkey.isKeyPressed())
	{
		vec3 diff = pkey.calculateDifference(cam.eye, cam.at);
		cam.eye += diff;
		cam.at += diff;
	}

	// update view matrix
	cam.view_matrix = mat4::lookAt(cam.eye, cam.at, cam.up);

	// update projection matrix
	cam.aspect_ratio = window_size.x / float(window_size.y);
	cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect_ratio, cam.dNear, cam.dFar);

	// update uniform variables in vertex/fragment shaders
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_TRUE, cam.view_matrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1, GL_TRUE, cam.projection_matrix);

	// enable texture manager
	glUniform1i(glGetUniformLocation(program, "TEX1"), 0); // GL_TEXTURE0

	// update shading variables
	update_light(program);
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// notify GL that we use our own program
	glUseProgram(program);

	// variables
	const char*	vertex_attrib[] = {"position", "normal", "texcoord"};
	size_t		attrib_size[] = {sizeof(vertex().pos), sizeof(vertex().norm), sizeof(vertex().tex)};

	//------------------------------
	// draw spheres & dwarfs

	// bind vertex attributes to your shader program
	for(size_t k = 0, kn = std::extent<decltype(vertex_attrib)>::value, byte_offset = 0; k<kn; k++, byte_offset += attrib_size[k - 1])
	{
		GLuint loc = glGetAttribLocation(program, vertex_attrib[k]); if(loc >= kn) continue;
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_vertex_buffer);
		glVertexAttribPointer(loc, attrib_size[k] / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)byte_offset);
	}

	// render vertices: trigger shader programs to process vertex data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_index_buffer);

	// draw planets
	mat4 model_matrix;
	float t = float(glfwGetTime()) * 0.5f;

	for(uint k = 0; k < 9; k++)
	{
		model_matrix = mat4::scale(planets[k].radius, planets[k].radius, planets[k].radius);
		model_matrix = mat4::rotate(vec3(0, 0, 1), t * planets[k].rotate) * model_matrix;
		model_matrix = mat4::translate(planets[k].distance, 0, 0) * model_matrix;
		model_matrix = mat4::rotate(vec3(0, 0, 1), t * planets[k].revolve) * model_matrix;

		// especially for the sun
		if(k == 0)
			glUniform1i(glGetUniformLocation(program, "blinnEnabled"), 0);
		else
			glUniform1i(glGetUniformLocation(program, "blinnEnabled"), 1);

		// bind texture
		glBindTexture(GL_TEXTURE_2D, texture_planet[k]);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, sphere_index_list.size(), GL_UNSIGNED_INT, nullptr);
	}

	// draw dwarfs
	glBindTexture(GL_TEXTURE_2D, texture_planet[9]); // moon texture
	for(uint k = 0; k < 12; k++)
	{
		model_matrix = mat4::scale(dwarfs[k].info.radius, dwarfs[k].info.radius, dwarfs[k].info.radius);
		model_matrix = mat4::rotate(vec3(0, 0, 1), t * dwarfs[k].info.rotate) * model_matrix;
		model_matrix = mat4::translate(dwarfs[k].info.distance, 0, 0) * model_matrix;
		model_matrix = mat4::rotate(vec3(0, 0, 1), t * dwarfs[k].info.revolve) * model_matrix;
		model_matrix = mat4::translate(planets[dwarfs[k].planet].distance, 0, 0) * model_matrix;
		model_matrix = mat4::rotate(vec3(0, 0, 1), t * planets[dwarfs[k].planet].revolve) * model_matrix;

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, sphere_index_list.size(), GL_UNSIGNED_INT, nullptr);
	}

	//------------------------------
	// draw rings

	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniform1i(glGetUniformLocation(program, "blendEnabled"), 1);

	// bind vertex attributes to your shader program
	for(size_t k = 0, kn = std::extent<decltype(vertex_attrib)>::value, byte_offset = 0; k<kn; k++, byte_offset += attrib_size[k - 1])
	{
		GLuint loc = glGetAttribLocation(program, vertex_attrib[k]); if(loc >= kn) continue;
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, ring_vertex_buffer);
		glVertexAttribPointer(loc, attrib_size[k] / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)byte_offset);
	}

	// render vertices: trigger shader programs to process vertex data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ring_index_buffer);

	for(uint k = 0; k < 2; k++)
	{
		model_matrix = mat4::scale(rings[k].scale, rings[k].scale, rings[k].scale);
		model_matrix = mat4::translate(planets[rings[k].planet].distance, 0, 0) * model_matrix;
		model_matrix = mat4::rotate(vec3(0, 0, 1), t * planets[rings[k].planet].revolve) * model_matrix;

		// bind texture
		glBindTexture(GL_TEXTURE_2D, texture_ring[k]);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, sphere_index_list.size(), GL_UNSIGNED_INT, nullptr);
	}

	// disable alpha blending
	glDisable(GL_BLEND);
	glUniform1i(glGetUniformLocation(program, "blendEnabled"), 0);

	//------------------------------
	// swap front and back buffers, and display to screen
	glfwSwapBuffers(window);
}

void update_and_render()
{
	update();
	render();
}

void reshape(GLFWwindow* window, int width, int height)
{
	window_size = ivec2(width, height);
	glViewport(0, 0, width, height);
	update_and_render();
}

//*******************************************************************
void print_help()
{
	printf("[help]\n");
	printf("- press ESC or 'q' to terminate the program\n");
	printf("- press F1 or 'h' to see help\n");
	printf("- press 'w' to toggle wireframe\n");
	printf("- press Home to reset camera\n");
	printf("- press Pause to pause the simulation");
	printf("\n");
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_W)		pkey.setKeyPress(0, true);
		else if (key == GLFW_KEY_A)	pkey.setKeyPress(1, true);
		else if (key == GLFW_KEY_S)	pkey.setKeyPress(2, true);
		else if (key == GLFW_KEY_D)	pkey.setKeyPress(3, true);
	}
	else if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W)		pkey.setKeyPress(0, false);
		else if (key == GLFW_KEY_A)	pkey.setKeyPress(1, false);
		else if (key == GLFW_KEY_S)	pkey.setKeyPress(2, false);
		else if (key == GLFW_KEY_D)	pkey.setKeyPress(3, false);
	}

	if(action == GLFW_PRESS)
	{
		if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)	glfwSetWindowShouldClose(window, GL_TRUE);
		else if(key == GLFW_KEY_H || key == GLFW_KEY_F1) print_help();
		else if(key == GLFW_KEY_E)
		{
			bWireframe = !bWireframe;
			glPolygonMode(GL_FRONT_AND_BACK, bWireframe ? GL_LINE : GL_FILL);
			printf("> using %s mode\n", bWireframe ? "wireframe" : "solid");
			update_and_render();
		}
		else if (key == GLFW_KEY_HOME)
		{
			cam.resetCamera();
			memcpy(&cam, &camera(), sizeof(camera));
			update_and_render();
		}
	}

	/*
	if(action == GLFW_RELEASE)
	if(action == GLFW_PRESS)

	if(key == GLFW_KEY_LEFT_SHIFT)
	if(key == GLFW_KEY_LEFT_CONTROL)
	*/
}

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	/*
	// 마우스 위치 구하기
	dvec2 pos;
	glfwGetCursorPos(window, &pos.x, &pos.y);

	// 좌표를 [0,1]로 노멀라이즈
	vec2 npos = vec2(float(pos.x) / float(window_size.x - 1), float(pos.y) / float(window_size.y - 1));

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	
	if (action == GLFW_PRESS)
	if (action == GLFW_RELEASE)
	*/
}


void motion(GLFWwindow* window, double x, double y)
{
	ivec2 window_center = window_size / 2;
	ivec2 diff = ivec2((int)x, (int)y) - window_center;

	// 마우스가 움직이지 않으면 연산 생략
	if (diff == ivec2(0, 0))
		return;

	// 1인칭 시점 카메라 업데이트
	cam.applyFirstPerson(window_size, x, y);

	// 마우스 커서를 중앙으로
	glfwSetCursorPos(window, window_center.x, window_center.y);
}

//*******************************************************************
void create_vertex_buffer()
{
	// sphere
	for(uint k = 0; k < 36; k++)
		for(uint l = 0; l < 72; l++)
		{
			float t = PI / 35 * k;
			float e = PI*2.0f / 71 * l;
			sphere_vertex_list.push_back({
				vec3(radius * sin(t) * cos(e), radius * sin(t) * sin(e), radius * cos(t)),	// vertex position
				vec3(sin(t) * cos(e), sin(t) * sin(e), cos(t)),								// normal vector
				vec2(e / (PI*2.0f), 1 - (t / PI))											// texture coordinate in ([0,1], [0,1])
			});
		}

	// ring
	float radius;
	for(uint k = 0; k < 2; k++)
		for(uint l = 0; l < 32; l++)
		{
			float t = PI * 2.0f / float(32) * float(l);
			float c = cos(t), s = sin(t);
			if(k == 0) radius = 1.0f;
			else		radius = 1.8f;
			ring_vertex_list.push_back({
				vec3(radius * c, radius * s, 0.0f),	// vertex position
				vec3(radius * c, radius * s, 2.0f),	// normal vector (z값을 바꾸면 쉐이딩 반사값이 변함)
				vec2((float)k, (float)1 - k)		// texture coordinate in ([0,1], [0,1])
			});
		}
}

void create_index_buffer()
{
	// sphere
	for(uint k = 0; k < 35; k++)
		for(uint l = 0; l < 72; l++)
		{
			sphere_index_list.push_back(k * 72 + l % 72);
			sphere_index_list.push_back((k + 1) * 72 + l % 72);
			sphere_index_list.push_back(k * 72 + (l + 1) % 72);

			sphere_index_list.push_back(k * 72 + (l + 1) % 72);
			sphere_index_list.push_back((k + 1) * 72 + l % 72);
			sphere_index_list.push_back((k + 1) * 72 + (l + 1) % 72);
		}

	// generation of vertex buffer
	glGenBuffers(1, &sphere_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*sphere_vertex_list.size(), &sphere_vertex_list[0], GL_STATIC_DRAW);

	// geneation of index buffer
	glGenBuffers(1, &sphere_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*sphere_index_list.size(), &sphere_index_list[0], GL_STATIC_DRAW);

	//-----

	// ring
	for(uint k = 0; k < 32; k++)
	{
		ring_index_list.push_back(k);
		ring_index_list.push_back(32 + k);
		ring_index_list.push_back((k + 1) % 32);

		ring_index_list.push_back((k + 1) % 32);
		ring_index_list.push_back(32 + k);
		ring_index_list.push_back(32 + (k + 1) % 32);
	}

	// generation of vertex buffer
	glGenBuffers(1, &ring_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, ring_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*ring_vertex_list.size(), &ring_vertex_list[0], GL_STATIC_DRAW);

	// geneation of index buffer
	glGenBuffers(1, &ring_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ring_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*ring_index_list.size(), &ring_index_list[0], GL_STATIC_DRAW);
}

//*******************************************************************
bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
	glEnable(GL_DEPTH_TEST);		// turn on depth tests
	glEnable(GL_TEXTURE_2D);		// enable texturing for 2D
	glActiveTexture(GL_TEXTURE0);	// active texture manager 0

	// hide mouse cursor and set position to center
	glfwSetCursorPos(window, window_size.x / 2, window_size.y / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// create vertex buffer and index buffer
	create_vertex_buffer();
	create_index_buffer();

	// texture processing
	int width, height, comp = 3;
	unsigned char* pimage0;
	unsigned char* pimage;
	int stride0, stride1;
	for(int i = 0; i < 10; i++) // planets
	{
		// load and flip an image
		pimage0 = stbi_load(texture_planet_path[i], &width, &height, &comp, 3); if(comp == 1) comp = 3; /* convert 1-channel to 3-channel image */
		stride0 = width*comp;
		stride1 = (stride0 + 3)&(~3);	// 4-byte aligned stride
		pimage = (unsigned char*)malloc(sizeof(unsigned char)*stride1*height);
		for(int y = 0; y < height; y++) memcpy(pimage + (height - 1 - y)*stride1, pimage0 + y*stride0, stride0); // vertical flip

		// create textures
		glGenTextures(1, &texture_planet[i]);
		glBindTexture(GL_TEXTURE_2D, texture_planet[i]);

		// allocate and create mipmap
		int mip_levels = get_mip_levels(window_size.x, window_size.y);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8 /* GL_RGB for legacy GL */, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pimage);
		for(int k = 1, w = width >> 1, h = height >> 1; k<mip_levels; k++, w = max(1, w >> 1), h = max(1, h >> 1))
			glTexImage2D(GL_TEXTURE_2D, k, GL_RGB8 /* GL_RGB for legacy GL */, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);

		// configure texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// release the new image
		free(pimage);
	}
	for(int i = 0; i < 2; i++) // rings
	{
		// load and flip an image
		pimage0 = stbi_load(texture_ring_path[i], &width, &height, &comp, 3); if(comp == 1) comp = 3; /* convert 1-channel to 3-channel image */
		stride0 = width*comp;
		stride1 = (stride0 + 3)&(~3);	// 4-byte aligned stride
		pimage = (unsigned char*)malloc(sizeof(unsigned char)*stride1*height);
		for(int y = 0; y < height; y++) memcpy(pimage + (height - 1 - y)*stride1, pimage0 + y*stride0, stride0); // vertical flip

		// create textures
		glGenTextures(1, &texture_ring[i]);
		glBindTexture(GL_TEXTURE_2D, texture_ring[i]);

		// allocate and create mipmap
		int mip_levels = get_mip_levels(window_size.x, window_size.y);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8 /* GL_RGB for legacy GL */, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pimage);
		for(int k = 1, w = width >> 1, h = height >> 1; k<mip_levels; k++, w = max(1, w >> 1), h = max(1, h >> 1))
			glTexImage2D(GL_TEXTURE_2D, k, GL_RGB8 /* GL_RGB for legacy GL */, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);

		// configure texture parameters	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// release the new image
		free(pimage);
	}

	return true;
}

void user_finalize()
{
}

//*******************************************************************
void main(int argc, char* argv[])
{
	// initialization
	if(!glfwInit()){ printf("1[error] failed in glfwInit()\n"); return; }

	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window(window_name, window_size.x, window_size.y))){ glfwTerminate(); return; }
	if(!cg_init_extensions(window)){ glfwTerminate(); return; }	// init OpenGL extensions

	// initializations and validations of GLSL program
	if(!(program = cg_create_program(vert_shader_path, frag_shader_path))){ glfwTerminate(); return; }	// create and compile shaders/program
	if(!user_init()){ printf("Failed to user_init()\n"); glfwTerminate(); return; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback(window, reshape);		// callback for window resizing events
	glfwSetKeyCallback(window, keyboard);			// callback for keyboard events
	glfwSetMouseButtonCallback(window, mouse);		// callback for mouse click inputs
	glfwSetCursorPosCallback(window, motion);		// callback for mouse movements

	// enters rendering/event loop
	for(frame = 0; !glfwWindowShouldClose(window); frame++)
	{
		glfwPollEvents();		// polling and processing of events
		update_and_render();	// per-frame update/render
	}

	// normal termination
	user_finalize();
	glfwDestroyWindow(window);
	glfwTerminate();
}