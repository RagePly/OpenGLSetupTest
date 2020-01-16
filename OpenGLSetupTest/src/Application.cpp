#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


static unsigned int CompileShader( unsigned int type, const std::string& source ) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //make sure that the source string is still in scope while compiling
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //iv stands for integer vector, check documents
	if (result == GL_FALSE)
	{
		//The shader did not compile successfully
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char)); //you cant use a stack allocated int length (non-constant) to define char array. Therefore there is this macro to allocate the space on the stack
		//and then cast it to char*
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment ") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) { //vertexShader is the actual shader source code
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program); 
	glValidateProgram(program); //Actually validates the program, and returns a state sort of

	glDeleteShader(vs); //delete intermediate shaders, since we have now successfully linked the program we attached them to
	glDeleteShader(fs);

	return program;
}




int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	std::cout << "Initializing the GLFW library...";
	if (!glfwInit())
		return -1;
	std::cout << "Done" << std::endl;

	/* Create a windowed mode window and its OpenGL context */
	std::cout << "Creating Window...";
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	std::cout << "Done" << std::endl;

	/* Make the window's context current */
	std::cout << "Creating valid OpenGL rendering context...";
	glfwMakeContextCurrent(window);
	std::cout << "Done" << std::endl;

	/* The above created a valid openGL rendering contex, which allows us to initialize glew */
	std::cout << "Initializing GLEW...";
	if (glewInit() == GLEW_OK) {
		std::cout << "Done" << std::endl;
	}

	float positions[6] = { //the initialization is different to how i usually do it, but it is still a pointer
		-0.5f, -0.5f,
		 0.0f, 0.5f,
		 0.5f, -0.5f
	};


	unsigned int buffer; //the id of the buffer created. 



	//Defining vertex buffer
	glGenBuffers(1, &buffer); //one buffer, it writes the id to the integer address
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //it is now bound, we will now work on it (it is now the active buffer)
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),positions,GL_STATIC_DRAW); //the buffer is now given the data (it is copied over from the ram to the video-ram)
	//attributes means i.e position, color, normal...
	//index are to the vertecies
	//type is the type of data, ie float. 
	//stride could also be calculated from the offsetof macro
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //last pointer, number of bytes, if non-zero, cast the int to a (const void*)
	/*
	The way i understand it:
	index : when the shader is run and you access the array buffer (of vertecies), the index of a certain vertex's attribute is that parameter. so buffer[vertex index][attribute INDEX].
	size : the number of objects per vertex getting this attribute. In our case it is 2, because we want two floats to be an attribute (position). 
	type : the type of the objects. In our case it is a float. 
	normalized : i dont really get this
	stride : the distance between vertex attributes. It essentially translates to the distance between vertecies, because the next time this attribute needs to be 
	defined is when it is being defined in the next vertex
	offset : 
	easier with an example
	every vertex has three values (stride sizeof(values))
	the first two are one attribute (index 0) , the third is the second attribute(index 1)
	the offset for the first is 0, because it will start applying the attributes in steps of stride, starting att val1. The second attribute has offset
	sizeof(val1 + val2), because it starts at val3 and then after one stride ends up at val6 and so on
	val1, val2, val3,
	val4, val 5, val6,
	...
	
	*/

	std::string vertexShader = //run for each vertex
		"#version 330 core\n" //c++ concatonates if you put strings next (only whitespace) to each other 
		"\n"
		"layout(location = 0) in vec4 position;" //layout(location = 0) references the attribute at index 0, which in our case is 2d position. then setting it to vec4 position. in stands for input
		"void main()\n"
		"{\n"
		" gl_Position = position;\n" //gl_position is a vec4, thats why we converted it before since we in the attribute assignment defined the attributes as having two components (a vec2)
		"}\n";

	std::string fragmentShader = 
		"#version 330 core\n" //c++ concatonates if you put strings next (only whitespace) to each other 
		"\n"
		"layout(location = 0) out vec4 color;" //layout(location = 0) references the attribute at index 0, which in our case is 2d position. then setting it to vec4 position (so "attributes is in variable position which is of type vec4")
		"void main()\n"
		"{\n"
		" color = vec4(1.0,0.0,0.0,1.0);\n" //gl_position is a vec4, thats why we converted it before since we in the attribute assignment defined the attributes as having two components (a vec2)
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	std::cout << "Terminating Window...";
	glfwTerminate();
	std::cout << "Done" << std::endl;
	return 0;
}