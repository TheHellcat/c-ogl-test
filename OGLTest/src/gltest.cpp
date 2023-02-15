#include <cstdio>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include "gltest.h"

#include "math.h"
#include "shader.h"

#define ASSERT(x) if( !(x) ) { __debugbreak(); }
#define GlCall(x) \
	GlClearError();\
	x;\
	ASSERT(GlCheckError(#x, __FILE__, __LINE__))

int main()
{
	GLFWwindow* window;
	unsigned int vertex_buffer;
	unsigned int index_buffer;
	unsigned int normal_buffer;
	unsigned int shaderProgram;
	float m_projection[4*4];
	float m_look_at[4*4];

	float model_triangle[6] = {
		-0.5f, -0.5f,
        0.0f, 0.0f,
        0.5f, -0.5f
    };

	float model_cube[] = {
		// x     y      z      nx,    ny,    nz,   u     v       idx

		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.5f, //  0
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //  1
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.5f, 0.0f, //  2

		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.5f, 0.5f, //  3

		// right
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.5f, //  4
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, //  5
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //  6

		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.5f, //  7

		// back
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f, 0.5f, //  8
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f, 0.0f, //  9
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 10

		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.5f, // 11

		// left
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.5f, // 12
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 13
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.5f, 0.0f, // 14

		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.5f, 0.5f, // 15

		// top
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 16
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, // 17
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.5f, 1.0f, // 18

		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.5f, 0.5f, // 19

		// bottom
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.5f, // 20
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 21
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.5f, 0.5f, // 22

		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.5f, 1.0f  // 23
	};

	unsigned int model_cube_idx[] = {
		// front
		0, 1, 2,
		0, 2, 3,

		// right
		4, 5, 6,
		4, 6, 7,

		// back
		8, 10, 9,
		8, 11, 10,

		// left
		12, 14, 13,
		12, 15, 14,

		// top
		16, 17, 18,
		18, 17, 19,

		// bottom
		22, 21, 20,
		22, 23, 21
	};

	float* model_cube_normaldata;

	printf( "\n\n    HELLCATs next pity attempt at doing bare OpenGL\n\n" );

	printf( "          ____\n" );
	printf( "         /\\   \\\n" );
	printf( "        /  \\___\\\n" );
	printf( "        \\  /   /\n" );
	printf( "         \\/___/\n" );

	printf( "\n\n" );

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(1600, 900, "Hello OpenGL World", NULL, NULL);
	window = glfwCreateWindow(768, 768, "Hello OpenGL World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	printf("created window and GL context.\n");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		printf("Failed to init GL: %s\n", (char*)glewGetErrorString(err) );
		return -1;
	}

	int model_cube_normaldata_sz = generateNormalData( model_cube, 8, model_cube_idx, sizeof(model_cube_idx)/sizeof(int), model_cube_normaldata );
	printf("Generated additional normal data - %d bytes / %llu tan/bitan pairs.\n", model_cube_normaldata_sz, model_cube_normaldata_sz/sizeof(float)/6);

	std::streampos size;
	char* colormap;
	char* normalmap;
	char* specularmap;

	std::ifstream file ("colormap.bin", std::ios::in|std::ios::binary|std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		colormap = new char [size];
		file.seekg (0, std::ios::beg);
		file.read (colormap, size);
		file.close();

		printf("Loaded color texture.\n");
	}
	else
	{
		colormap = new char [1];
		printf("ERROR loading color texture.\n");
	}

	std::ifstream file2 ("normalmap.bin", std::ios::in|std::ios::binary|std::ios::ate);
	if (file2.is_open())
	{
		size = file2.tellg();
		normalmap = new char [size];
		file2.seekg (0, std::ios::beg);
		file2.read (normalmap, size);
		file2.close();

		printf("Loaded normal texture.\n");
	}
	else
	{
		normalmap = new char [1];
		printf("ERROR loading normal texture.\n");
	}

	std::ifstream file3 ("specularmap.bin", std::ios::in|std::ios::binary|std::ios::ate);
	if (file3.is_open())
	{
		size = file3.tellg();
		specularmap = new char[size];
		file3.seekg (0, std::ios::beg);
		file3.read (specularmap, size);
		file3.close();

		printf("Loaded specular texture.\n");
	}
	else
	{
		specularmap = new char [1];
		printf("ERROR loading specular texture.\n");
	}

	printf( "Initialised OpenGL, we got version %s\n", (char*)glGetString( GL_VERSION ) );

	glFrontFace(GL_CW);
	GlCall(glEnable(GL_CULL_FACE));
	GlCall(glCullFace(GL_BACK));
	GlCall(glEnable(GL_DEPTH_TEST));
	GlCall(glDepthFunc(GL_LESS));
	GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model_cube), model_cube, GL_STATIC_DRAW);
	printf("%llu bytes / %llu items // %llu vertices of vertex data.\n", sizeof(model_cube), sizeof(model_cube)/sizeof(float), sizeof(model_cube)/sizeof(float)/8);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model_cube_idx), model_cube_idx, GL_STATIC_DRAW);
	printf("%llu bytes / %llu items of index data.\n", sizeof(model_cube_idx), sizeof(model_cube_idx)/sizeof(int));

	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// vertex UVs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, model_cube_normaldata_sz, model_cube_normaldata, GL_STATIC_DRAW);
	printf("%d bytes / %llu items // %llu additional normal for vertex data.\n", model_cube_normaldata_sz, model_cube_normaldata_sz/sizeof(float), model_cube_normaldata_sz/sizeof(float)/6);

	// tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(3);

	// bitangents
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(4);

	unsigned int tex1;
	unsigned int tex2;
	unsigned int tex3;
	GlCall(glGenTextures(1, &tex1));
	GlCall(glBindTexture(GL_TEXTURE_2D, tex1));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_BGR, GL_UNSIGNED_BYTE, colormap));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
	// don't forget "glDeleteTextures(1, &tex1);" if/once we don't need the texture data anymore - omitted for this experiment

	GlCall(glGenTextures(1, &tex2));
	GlCall(glBindTexture(GL_TEXTURE_2D, tex2));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_BGR, GL_UNSIGNED_BYTE, normalmap));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	GlCall(glGenTextures(1, &tex3));
	GlCall(glBindTexture(GL_TEXTURE_2D, tex3));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_BGR, GL_UNSIGNED_BYTE, specularmap));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	GlCall(glActiveTexture(GL_TEXTURE0));
	GlCall(glBindTexture(GL_TEXTURE_2D, tex1));
	GlCall(glActiveTexture(GL_TEXTURE1));
	GlCall(glBindTexture(GL_TEXTURE_2D, tex2));
	GlCall(glActiveTexture(GL_TEXTURE2));
	GlCall(glBindTexture(GL_TEXTURE_2D, tex3));

	shaderProgram = CreateShader( vertexShaderSource, fragmentShaderSource );
	glUseProgram(shaderProgram);

	//gl_perspective( 30.0f, 16.0f/9.0f, 0.001f, 100.0f, m_projection );
	gl_perspective( 25.0f, 1.0f, 0.001f, 100.0f, m_projection );

	vector3 look_eye;
	vector3 look_at;
	vector3 look_up;
	look_eye.x = -0.5f;
	look_eye.y = 0.0f;
	look_eye.z = 0.5f;
	look_at.x = -0.32f;
	look_at.y = 0.11; //0.17;
	look_at.z = 0.0f;
	look_up.x = 0.0f;
	look_up.y = 1.0f;
	look_up.z = 0.0f;
	gl_look_at( look_eye, look_at, look_up, m_look_at );

	printf( "\n\nrunning...." );

	/* Loop until the user closes the window */
	float rota1 = 0.0f;
	float rota2 = 0.0f;
	float cam_dir = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		look_eye.x += (0.005f * cam_dir);
		if( look_eye.x > 1.5f)
		{
			cam_dir = -1.0f;
		}
		else if( look_eye.x < -1.5f)
		{
			cam_dir = 1.0f;
		}
		gl_look_at( look_eye, look_at, look_up, m_look_at );

		rota1 += 0.25f;
		if( rota1 > 180 )
		{
			rota1 = -180;
		}
		quaternion q = euler_to_quaternion(deg_to_rad(rota1), 0.0f, 0.0f);
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_translate_pos" ), 0.0f, 0.5f, -1.0f ));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_translate_scale" ), 0.25f, 0.25f, 0.25f ));
		GlCall(glUniform4f( glGetUniformLocation( shaderProgram, "u_translate_rot_quat" ), q.x, q.y, q.z, q.w ));
		//GlCall(
			glUniformMatrix4fv(glGetUniformLocation( shaderProgram, "u_projection" ), 1, false, m_projection);
			//);
		GlCall(glUniformMatrix4fv(glGetUniformLocation( shaderProgram, "u_look_at" ), 1, false, m_look_at));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_eye_pos" ), look_eye.x, look_eye.y, look_eye.z ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_color" ), 0 ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_normal" ), 1 ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_specular" ), 2 ));

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//GlCall(glDrawElements(GL_TRIANGLES, sizeof(model_cube_idx)/sizeof(float), GL_UNSIGNED_INT, 0));
		GlCall(glDrawElements(GL_TRIANGLES, sizeof(model_cube_idx)/sizeof(float), GL_UNSIGNED_INT, 0));


		rota2 += 0.25f;
		if( rota2 > 180 )
		{
			rota2 = -180;
		}
		//rota2 = 90;
		q = euler_to_quaternion(0.0f, deg_to_rad(rota2), 0.0f);
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_translate_pos" ), 0.0f, 0.15f, -1.0f ));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_translate_scale" ), 0.25f, 0.25f, 0.25f ));
		GlCall(glUniform4f( glGetUniformLocation( shaderProgram, "u_translate_rot_quat" ), q.x, q.y, q.z, q.w ));
		GlCall(glUniformMatrix4fv(glGetUniformLocation( shaderProgram, "u_projection" ), 1, false, m_projection));
		GlCall(glUniformMatrix4fv(glGetUniformLocation( shaderProgram, "u_look_at" ), 1, false, m_look_at));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_eye_pos" ), look_eye.x, look_eye.y, look_eye.z ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_color" ), 0 ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_normal" ), 1 ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_specular" ), 2 ));
		GlCall(glDrawElements(GL_TRIANGLES, sizeof(model_cube_idx)/sizeof(float), GL_UNSIGNED_INT, 0));

		q = euler_to_quaternion(0.0f, 0.0f, deg_to_rad(45));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_translate_pos" ), 0.0f, -0.5f, -1.0f ));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_translate_scale" ), 0.25f, 0.25f, 0.25f ));
		GlCall(glUniform4f( glGetUniformLocation( shaderProgram, "u_translate_rot_quat" ), q.x, q.y, q.z, q.w ));
		GlCall(glUniformMatrix4fv(glGetUniformLocation( shaderProgram, "u_projection" ), 1, false, m_projection));
		GlCall(glUniformMatrix4fv(glGetUniformLocation( shaderProgram, "u_look_at" ), 1, false, m_look_at));
		GlCall(glUniform3f( glGetUniformLocation( shaderProgram, "u_eye_pos" ), look_eye.x, look_eye.y, look_eye.z ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_color" ), 0 ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_normal" ), 1 ));
		GlCall(glUniform1i( glGetUniformLocation( shaderProgram, "u_tex_specular" ), 2 ));
		GlCall(glDrawElements(GL_TRIANGLES, sizeof(model_cube_idx)/sizeof(float), GL_UNSIGNED_INT, 0));


		/* Swap front and back buffers */
		GlCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	printf( " not, anymore." );
	printf( "\n\n  That's it - so long, and thanks for all the fish....\n\n" );

	return 0;
}

unsigned int CreateShader( std::string &vertexShader, std::string &fragmentShader )
{
	unsigned int shaderProgram;
	unsigned int vs;
	unsigned int fs;

	printf( "Compiling shaders.... " );

	shaderProgram = glCreateProgram();
	vs = CompileShader( vertexShader, GL_VERTEX_SHADER );
	fs = CompileShader( fragmentShader, GL_FRAGMENT_SHADER );

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glDeleteShader(vs);
	glDeleteShader(fs);

	printf("DONE\n");

	return shaderProgram;
}

unsigned int CompileShader( std::string &source, unsigned int type )
{
	unsigned int id;
	char* src;
	int compileResult;
	char msg[512];
	int msgLen;

	id = glCreateShader( type );
	src = &source[0];
	glShaderSource( id, 1, &src, nullptr );
	glCompileShader( id );

	glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
	if( compileResult == GL_FALSE )
	{
		memset(msg, 0, 512);
		glGetShaderInfoLog(id, 512, &msgLen, msg);
		printf( "/!\\ ERROR compiling shader %d: %s\n", type, msg );

		glDeleteShader(id);
		return 0;
	}

	return id;
}

void GlClearError()
{
	while(glGetError() != GL_NO_ERROR)
	{
		// NOP
	}
}

bool GlCheckError(const char* func, const char* file, int line)
{
	GLenum glError;
	bool r;

	r = true;
	while((glError = glGetError()) != GL_NO_ERROR)
	{
		printf("! GL ERROR: %d - at %s #%d {%s}\n", glError, file, line, func);
		r = false;
	}

	return r;
}


int generateNormalData( float* vertices, int vertex_stride, unsigned int* indices, int count, float* &normal_data )
{
	int sz;
	vector3 v1;
	vector3 v2;
	vector3 v3;
	vector2 uv1;
	vector2 uv2;
	vector2 uv3;
	vector3 delta1;
	vector3 delta2;
	vector2 deltaUv1;
	vector2 deltaUv2;
	int i;
	float r;
	vector3 tangent;
	vector3 bitangent;

	sz = count * sizeof(float) * 6;
	normal_data = new float[count*6];

	for( i=0; i<count; i+=3 )
	{
		v1.x = vertices[ (indices[i] * vertex_stride) ];
		v1.y = vertices[ (indices[i] * vertex_stride) + 1 ];
		v1.z = vertices[ (indices[i] * vertex_stride) + 2 ];
		uv1.x = vertices[ (indices[i] * vertex_stride) + 6 ];
		uv1.y = vertices[ (indices[i] * vertex_stride) + 7 ];

		v2.x = vertices[ (indices[i+1] * vertex_stride) ];
		v2.y = vertices[ (indices[i+1] * vertex_stride) + 1 ];
		v2.z = vertices[ (indices[i+1] * vertex_stride) + 2 ];
		uv2.x = vertices[ (indices[i+1] * vertex_stride) + 6 ];
		uv2.y = vertices[ (indices[i+1] * vertex_stride) + 7 ];

		v3.x = vertices[ indices[i+2] * vertex_stride ];
		v3.y = vertices[ (indices[i+2] * vertex_stride) + 1 ];
		v3.z = vertices[ (indices[i+2] * vertex_stride) + 2 ];
		uv3.x = vertices[ (indices[i+2] * vertex_stride) + 6 ];
		uv3.y = vertices[ (indices[i+2] * vertex_stride) + 7 ];

		delta1 = v2 - v1;
		delta2 = v3 - v1;
		deltaUv1 = uv2 - uv1;
		deltaUv2 = uv3 - uv1;

		r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
		tangent = ((delta1 * deltaUv2.y) - (delta2 * deltaUv1.y))*r;
		bitangent = ((delta2 * deltaUv1.x) - (delta1 * deltaUv2.x))*r;

		//printf("i: %d - T: %s - B: %s - N: -\n", i, tangent.to_string(), bitangent.to_string());
		//printf("i: %d - T: %f, %f, %f - B: %f, %f, %f - N: -\n", i, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z);

		// normal_data[i * 6] = tangent.x;
		// normal_data[(i * 6) + 1] = tangent.y;
		// normal_data[(i * 6) + 2] = tangent.z;
		// normal_data[(i * 6) + 3] = bitangent.x;
		// normal_data[(i * 6) + 4] = bitangent.y;
		// normal_data[(i * 6) + 5] = bitangent.z;
		// normal_data[(i + 1) * 6] = tangent.x;
		// normal_data[((i + 1) * 6) + 1] = tangent.y;
		// normal_data[((i + 1) * 6) + 2] = tangent.z;
		// normal_data[((i + 1) * 6) + 3] = bitangent.x;
		// normal_data[((i + 1) * 6) + 4] = bitangent.y;
		// normal_data[((i + 1) * 6) + 5] = bitangent.z;
		// normal_data[(i + 2) * 6] = tangent.x;
		// normal_data[((i + 2) * 6) + 1] = tangent.y;
		// normal_data[((i + 2) * 6) + 2] = tangent.z;
		// normal_data[((i + 2) * 6) + 3] = bitangent.x;
		// normal_data[((i + 2) * 6) + 4] = bitangent.y;
		// normal_data[((i + 2) * 6) + 5] = bitangent.z;

		normal_data[indices[i] * 6] = tangent.x;
		normal_data[(indices[i] * 6) + 1] = tangent.y;
		normal_data[(indices[i] * 6) + 2] = tangent.z;
		normal_data[(indices[i] * 6) + 3] = bitangent.x;
		normal_data[(indices[i] * 6) + 4] = bitangent.y;
		normal_data[(indices[i] * 6) + 5] = bitangent.z;
		normal_data[indices[i + 1] * 6] = tangent.x;
		normal_data[((indices[i + 1]) * 6) + 1] = tangent.y;
		normal_data[((indices[i + 1]) * 6) + 2] = tangent.z;
		normal_data[((indices[i + 1]) * 6) + 3] = bitangent.x;
		normal_data[((indices[i + 1]) * 6) + 4] = bitangent.y;
		normal_data[((indices[i + 1]) * 6) + 5] = bitangent.z;
		normal_data[indices[i + 2] * 6] = tangent.x;
		normal_data[((indices[i + 2]) * 6) + 1] = tangent.y;
		normal_data[((indices[i + 2]) * 6) + 2] = tangent.z;
		normal_data[((indices[i + 2]) * 6) + 3] = bitangent.x;
		normal_data[((indices[i + 2]) * 6) + 4] = bitangent.y;
		normal_data[((indices[i + 2]) * 6) + 5] = bitangent.z;
	}

	return sz;
}
