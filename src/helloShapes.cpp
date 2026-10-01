#include <iostream>
#include "../include/glad.h"
#include <GLFW/glfw3.h>

unsigned int VBOTriangle, VBORect, ShaderProgram, VAO; //IDs

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

int checkSuccess(unsigned int object, int isProgram){
    int  success;
    char infoLog[512];
    if (isProgram){
        glGetProgramiv(object, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(object, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
                return -1;
            }   
    }
    else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return -1;
        }
    }

    return 1;
}

int initializeShapes(){

    unsigned int VertexShader, FragmentShader;
 
    const float triangle[] = {0.0f, 1.0f, 0.0f, //top
                          -0.5f, 0.5f, 0.0f, //left
                          0.5f,  0.5f, 0.0f}; //right

    const float rect[] = {
        // first triangle
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    }; 
    
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
    
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(VertexShader);   
    if (!checkSuccess(VertexShader, 0)) {
        return -1;
    }

    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    if (!checkSuccess(FragmentShader, 0)) {
        return -1;
    }    
    
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    if (!checkSuccess(FragmentShader, 1)) {
        return -1;
    }    
 
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    // VAOs and VBOS

    glGenVertexArrays(1, &VAO); 

    glBindVertexArray(VAO); //Store vbos efficently soe we need to initalize this only once    
    
    glGenBuffers(1, &VBOTriangle); 
    glBindBuffer(GL_ARRAY_BUFFER, VBOTriangle);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    //Linking vertex atrributes
    //Layout (location = 0), vec3 = 3, float = GL_FLOAT, Normalize = False, stride (x1,y1,z1, x2... = 3 * size(float)), void*(0) = offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbid VBOTrig

    glGenBuffers(1, &VBORect); 
    glBindBuffer(GL_ARRAY_BUFFER, VBORect);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

    //Linking vertex atrributes
    //Layout (location = 0), vec3 = 3, float = GL_FLOAT, Normalize = False, stride (x1,y1,z1, x2... = 3 * size(float)), void*(0) = offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbid VBORect
    glBindVertexArray(0); 


    return 0;
}

int main(void)
{
    GLFWwindow *window = createWindow(640,480, "Whats up bitches");
    glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
    initializeShapes();
  

    while (!glfwWindowShouldClose(window))
    {

        //Inputs
        //function

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); //Cleans buffer so we don't see the remnants of previous frame
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBOTriangle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindBuffer(GL_ARRAY_BUFFER, VBORect);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        

        glfwPollEvents();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}