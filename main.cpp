#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>//glm::make_mat4()
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265

#include "shader.h"

int main(){
	glewExperimental = true;
	if( !glfwInit() ){
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	int width = 640;
	int height = 480;
	window = glfwCreateWindow(width, height, "turorial", NULL, NULL);
	if( window == NULL ){
		std::cout << "Failed to open GLFW window, if you have an Intel GPU they are not 3.3 compatible. try another version" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if( glewInit() != GLEW_OK ) {
		std::cout << "failed to initialize GLEW\n";
		return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLfloat g_color_buffer_data[12*3*3];
	srand(time(NULL));
	for(int i = 0; i < 12*3*3; ++i){
		g_color_buffer_data[i] = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
	}

	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


	GLuint programID = loadShaders( "shader/vertex_shader.vertexshader", "shader/fragment_shader.fragmentshader");
	
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
	// glm::mat4 Projection = glm::ortho(-2.0f, (float)width/100, (float)height/100, -2.0f, 0.0f, 10.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(8,6,6), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

	// float aaa[16] = {
	// 	1, 0, 0, 0,
	// 	0, 1, 0, 0,
	// 	0, 0, 1, 0,
	// 	2, 1, 0, 1
	// };

	// glm::mat4 Model = glm::make_mat4(aaa);//glm::mat4(1.0f);
	// glm::mat4 mvp = Projection * View * Model;

	int t = 0;
	float theta = 0;
	do{
		theta = (float)t*PI/180;
		float aaa[16] = {
			cos(theta), 	0, 	-sin(theta), 	0,
			0, 				1, 	0, 				0,
			sin(theta), 	0, 	cos(theta), 	0,
			(float)t/60, 	1, 	0, 				1
		};
		if(t == 360)
			t = -1;
		t++;

		glm::mat4 Model = glm::make_mat4(aaa);//glm::mat4(1.0f);
		glm::mat4 mvp = Projection * View * Model;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
		);
		
		glDrawArrays(GL_TRIANGLES, 0, 12*3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		glfwSwapBuffers(window);
		glfwPollEvents();
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 &&
			glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS
			);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
