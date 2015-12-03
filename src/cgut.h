#pragma once
#ifndef __CGUT_H__
#define __CGUT_H__

// minimum standard headers
#include <stdio.h>
#include <stdlib.h>

// enforce not to use /MD or /MDd flag
#ifdef _DLL
	#error Use /MT (or /MTd for DEBUG) at Configuration -> C/C++ -> Code Generation -> Run-time Library
#endif

#define __GLU_H__		// enforce not to use <glu.h>
#include "GL/glad.h"	// https://github.com/Dav1dde/glad
						// visit http://glad.dav1d.de/ to generate your own glad.h/glad.c of a different version
						// suggested profile: OpenGL, gl Version 4.5, core profile
#include "GL/glfw3.h"	// http://www.glfw.org

// explicitly link libraries
#pragma comment( lib, "OpenGL32.lib" )		// link OpenGL32 library
#pragma comment( lib, "glfw3.lib" )			// static version (currently, VC 2013)
//#pragma comment( lib, "glfw3dll.lib" )	// dynamic version for other VC version

//*******************************************************************
// common structures
struct mem_t
{
	char*	ptr = nullptr;
	size_t	size = 0;
};

struct vertex // will be used for all the course examples
{
    vec3 pos;	// position
	vec3 norm;	// normal vector; we will use this for vertex color for this example
    vec2 tex;	// texture coordinate; ignore this for the moment
};

struct mesh
{
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	GLuint				vertex_buffer = 0;
	GLuint				index_buffer = 0;
	GLuint				texture = 0;
};

//*******************************************************************
// utility functions
inline mem_t cg_read_binary( const char* file_path )
{
	FILE* fp = fopen( file_path, "rb" ); if(fp==nullptr){ printf( "[error] Unable to open %s\n", file_path ); return mem_t(); }
	fseek( fp, 0L, SEEK_END);
	mem_t m; m.size = ftell(fp);
	fseek( fp, 0L, SEEK_SET );
	m.ptr = (char*) malloc(m.size+1);		// +1 for string
	fread( m.ptr, m.size, 1, fp );
	memset(m.ptr+m.size,0,sizeof(char));	// for string
	fclose(fp);
	return m;
}

inline char* cg_read_shader( const char* file_path )
{
	// get the full path of shader file
	char module_file_path[_MAX_PATH]; GetModuleFileNameA( 0, module_file_path, _MAX_PATH );
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_splitpath_s( module_file_path, drive,_MAX_DRIVE,dir,_MAX_DIR,fname,_MAX_FNAME,ext,_MAX_EXT);
	char shader_file_path[_MAX_PATH]; sprintf_s( shader_file_path, "%s%s%s", drive, dir, file_path );
	
	// get the full path of a shader file
	return cg_read_binary( file_path ).ptr;
}

inline bool cg_validate_shader( GLuint shaderID, const char* shaderName )
{
	const int MAX_LOG_LENGTH=4096;
	static char msg[MAX_LOG_LENGTH] = {NULL};
	GLint shaderInfoLogLength;

	glGetShaderInfoLog( shaderID, MAX_LOG_LENGTH, &shaderInfoLogLength, msg );
	if( shaderInfoLogLength>1 && shaderInfoLogLength<=MAX_LOG_LENGTH )
		printf( "[Shader Log: %s]\n%s\n", shaderName, msg );

	GLint shaderCompileStatus; glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompileStatus);
	if(shaderCompileStatus==GL_TRUE) return true;
	
	glDeleteShader( shaderID );
	return false;
}

inline bool cg_validate_program( GLuint programID, const char* programName )
{
	const int MAX_LOG_LENGTH=4096;
	static char msg[MAX_LOG_LENGTH] = {NULL};
	GLint programInfoLogLength;

	glGetProgramInfoLog( programID, MAX_LOG_LENGTH, &programInfoLogLength, msg );
	if( programInfoLogLength>1 && programInfoLogLength<=MAX_LOG_LENGTH )
		printf( "[Program Log: %s]\n%s\n", programName, msg );

	GLint programLinkStatus; glGetProgramiv( programID, GL_LINK_STATUS, &programLinkStatus);
	if(programLinkStatus!=GL_TRUE){ glDeleteProgram(programID); return false; }

	glValidateProgram( programID );
	glGetProgramInfoLog( programID, MAX_LOG_LENGTH, &programInfoLogLength, msg );
	if( programInfoLogLength>1 && programInfoLogLength<=MAX_LOG_LENGTH )
		printf( "[Program Log: %s]\n%s\n", programName, msg );

	GLint programValidateStatus; glGetProgramiv( programID, GL_VALIDATE_STATUS, &programValidateStatus);
	if( programValidateStatus!=GL_TRUE ){ glDeleteProgram(programID); return false; }

	return true;
}

inline GLFWwindow* cg_create_window( const char* name, int width, int height, bool show_window=true )
{
	// give GLFW hints for window and OpenGL context
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );				// minimum requirement for modern-style programming
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );				// minimum requirement for modern-style programming
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE );			// for legacy GPUs common in students
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE );	// for legacy GPUs common in students
	glfwWindowHint( GLFW_RESIZABLE, GL_TRUE );
	glfwWindowHint( GLFW_VISIBLE, GL_FALSE );

	// create a windowed mode window and its OpenGL context
	GLFWwindow* win = glfwCreateWindow( width, height, name, nullptr, nullptr ); if(!win){ printf( "Failed to create GLFW window.\n" ); glfwTerminate(); return nullptr; }

	// get the screen size and locate the window in the center
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	ivec2 screen_size = ivec2( mode->width, mode->height );
	glfwSetWindowPos( win, (screen_size.x-width)/2, (screen_size.y-height)/2 );
	
	// make context and show window
	glfwMakeContextCurrent(win);
	if(show_window) glfwShowWindow( win );

	return win;
}

inline bool cg_init_extensions( GLFWwindow* window )
{
	glfwMakeContextCurrent(window);	// make sure the current context again

#ifdef __glad_h_
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ printf( "init_extensions(): Failed in gladLoadGLLoader()\n" ); glfwTerminate(); return false; }
#endif

	// check GL version
	int verMajor; glGetIntegerv(GL_MAJOR_VERSION, &verMajor); // major = 3
	int verMinor; glGetIntegerv(GL_MINOR_VERSION, &verMinor); // minor = 2
	while(verMinor>10) verMinor/=10;
	printf( "Using OpenGL %d.%d", verMajor, verMinor );
	int version = verMajor*10+verMinor;

	// check GLSL version
	const char* strGLSLver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	if(strGLSLver==NULL) printf( "Warning: Unable to get glGetString(GL_SHADING_LANGUAGE_VERSION)\n" );
	float GLSLversion = strGLSLver?float(atof(strGLSLver)):0.0f;
	if( GLSLversion < 1.3f ){ printf( "init_extensions(): GLSL %.1f may not support shader programs. Please update your platform\n", GLSLversion ); return false; }
	printf( " and GLSL %.1f", GLSLversion );

	const char* renderer = (char*)glGetString(GL_RENDERER);
	const char* vendor = (char*)glGetString(GL_VENDOR);
	printf( " on %s, %s\n\n", renderer, vendor );

	// check supported extensions
#ifdef __glad_h_
	#define VALIDIDATE_GLAD_EXT(ext) if(!GLAD_GL_ARB_##ext){ printf( "init_extensions(): GLAD: GL_ARB_" #ext " not supported.\n" ); return false; }
	VALIDIDATE_GLAD_EXT( shading_language_100 );	// check your platform supports GLSL
	VALIDIDATE_GLAD_EXT( vertex_buffer_object );	// BindBuffers, DeleteBuffers, GenBuffers, IsBuffer, BufferData, BufferSubData, GenBufferSubData, ...
	VALIDIDATE_GLAD_EXT( vertex_shader );			// functions related to vertex shaders
	VALIDIDATE_GLAD_EXT( fragment_shader );			// functions related to fragment shaders
	VALIDIDATE_GLAD_EXT( shader_objects );			// functions related to program and shaders
#endif

	return true;
}

inline GLuint cg_create_program_from_string( const char* vertex_shader_source, const char* fragment_shader_source )
{
	// create a program before linking shaders
	GLuint program = glCreateProgram();
	glUseProgram( program );

	// compile shader sources
	GLuint vertex_shader = glCreateShader( GL_VERTEX_SHADER );
	GLint vertex_shader_length = strlen(vertex_shader_source);
	glShaderSource( vertex_shader, 1, &vertex_shader_source, &vertex_shader_length );
	glCompileShader( vertex_shader );
	if(!cg_validate_shader( vertex_shader, "vertex_shader" )){ printf( "Unable to compile vertex shader\n" ); return 0; }

	GLuint fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
	GLint fragment_shader_length = strlen(fragment_shader_source);
	glShaderSource( fragment_shader, 1, &fragment_shader_source, &fragment_shader_length );
	glCompileShader( fragment_shader );
	if(!cg_validate_shader( fragment_shader, "fragment_shader" )){ printf( "Unable to compile fragment shader\n" ); return 0; }

	// attach vertex/fragments shaders and link program
	glAttachShader( program, vertex_shader );
	glAttachShader( program, fragment_shader );
	glLinkProgram( program );
	if(!cg_validate_program( program, "program" )){ printf( "Unable to link program\n" ); return 0; }

	return program;
}

inline GLuint cg_create_program( const char* vert_path, const char* frag_path )
{
	const char* vertex_shader_source = cg_read_shader( vert_path ); if(vertex_shader_source==NULL) return 0;
	const char* fragment_shader_source = cg_read_shader( frag_path ); if(fragment_shader_source==NULL) return 0;
	
	// try to create a program
	GLuint program = cg_create_program_from_string( vertex_shader_source, fragment_shader_source );

	// deallocate string
	free((void*)vertex_shader_source);
	free((void*)fragment_shader_source);
	return program;
}

//*******************************************************************
inline mesh* cg_load_mesh( const char* vert_binary_path, const char* index_binary_path )
{
	mesh* new_mesh = new mesh();
	
	// load vertex buffer
	mem_t v = cg_read_binary(vert_binary_path);
	if(v.size%sizeof(vertex)){ printf( "%s is not a valid vertex binary file\n", vert_binary_path ); return nullptr; }
	new_mesh->vertex_list.resize( v.size/sizeof(vertex) );
	memcpy( &new_mesh->vertex_list[0], v.ptr, v.size );

	// load index buffer
	mem_t i = cg_read_binary(index_binary_path);
	if(i.size%sizeof(uint)){ printf( "%s is not a valid index binary file\n", index_binary_path ); return nullptr; }
	new_mesh->index_list.resize( v.size/sizeof(uint) );
	memcpy( &new_mesh->index_list[0], i.ptr, i.size );

	// release memory
	if(v.ptr) free(v.ptr);
	if(i.ptr) free(i.ptr);

	// create a vertex buffer
	glGenBuffers( 1, &new_mesh->vertex_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, new_mesh->vertex_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertex)*new_mesh->vertex_list.size(), &new_mesh->vertex_list[0], GL_STATIC_DRAW );

	// create a index buffer
	glGenBuffers( 1, &new_mesh->index_buffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, new_mesh->index_buffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*new_mesh->index_list.size(), &new_mesh->index_list[0], GL_STATIC_DRAW );

	return new_mesh;
}

#endif // __CGUT_H__