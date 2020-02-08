#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
	window = glfwCreateWindow(1023, 768, "turorial", NULL, NULL);
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


	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do{
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

	return 0;
}
