#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

GLFWwindow* createWindow(int width, int height, char *title){
    if (!glfwInit()){
        return NULL;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *win = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!win){
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }   

    //Set viewport for OpenGL
    glViewport(0,0, width, height);
    return win;
}



int main(void)
{
    GLFWwindow *window = createWindow(640,480, "Whats up bitches");
    glClearColor(0.3f, 0.3f, 0.8f, 1.0f);

    float triangle[] = {
        0.0f, 1.0f, 0.0f,
        0.5f, -0.3f, 0.0f
        -0.5f, -0.3f, 0.0f
    };

        // Setup our vertex data
    GLfloat vertices[] = {-0.5f, -0.5f, 0.0f,
                          0.5f, -0.5f, 0.0f,
                          0.0f,  0.5f, 0.0f};


    float rect[] = {
        // first triangle
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    }; 


    //Triangle

    unsigned int VBO, VertexShader, FragmentShader, ShaderProgram, VAO; //IDs

    const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 tPos;\n"
    "void main(){\n"
        "gl_Position = vec4(tPos.x, tPos.y, tPos.z, 1.0f); "
    "}\0";

    const char* fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main(){\n"
        "fragColor = vec4(1.0f,0.0f, 0.0f, 1.0f); "
    "}\0";

         //Shaders
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(VertexShader);


        //Check Success
    int  success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

        //Fragment Shader
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(FragmentShader);

        //Check Success
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

        //Shader Program
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }   

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    // VAOs and VBOS

    glGenVertexArrays(1, &VAO); 

    glBindVertexArray(VAO); //Store vbos efficently soe we need to initalize this only once    
    
    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);    

    //Linking vertex atrributes
    //Layout (location = 0), vec3 = 3, float = GL_FLOAT, Normalize = False, stride (x1,y1,z1, x2... = 3 * size(float)), void*(0) = offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);      

    while (!glfwWindowShouldClose(window))
    {

        //Inputs
        //function

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); //Cleans buffer so we don't see the remnants of previous frame
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
        

        glfwPollEvents();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}