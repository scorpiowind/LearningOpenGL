#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(GLenum shaderType, const std::string& source)
{
    unsigned int shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult != GL_TRUE)
    {
        GLsizei log_length = 0;
        char message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        std::cout << "shader compile failed!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int ps = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, ps);
    glLinkProgram(program);

    //int linkResult;
    //glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    //if (linkResult != GL_TRUE)
    //{
    //    GLsizei log_length = 0;
    //    char message[1024];
    //    glGetProgramInfoLog(program, 1024, &log_length, message);
    //    std::cout << "program link failed!" << std::endl;
    //    std::cout << message << std::endl;
    //}

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(ps);
    
    return program;
}

// Vertex Attributes and Layout in OpenGL
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
 
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f
    };
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexBuffer);
    glVertexAttribPointer(vertexBuffer, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    int program = CreateShader(vertexShader, fragmentShader);
    glUseProgram(program);
   
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

    glfwTerminate();
    return 0;
}