//
//  main.cpp
//  V90X
//
//  Created by SIDIOUSVIC on 2019/12/15.
//  Copyright © 2019 SIDIOUSVIC. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <string.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>


// WINDOW DIMENSIONS
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// VERTEX SHADER
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
}";

// FRAGMENT SHADER
static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
out vec4 color;                                                 \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);                           \n\
}";

void createTriangle()
{
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    // create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    
};

void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = (int)strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("ERROR COMPILING THE %d SHADER: '%s'\n", shaderType, eLog);
        return;
    }
    
    glAttachShader(theProgram, theShader);
    
};

void compileShaders ()
{
    shader = glCreateProgram();
    
    // CATCH ERROR DURING CREATION
    if(!shader)
    {
        printf("ERROR CREATING SHADER PROGRAM");
        return;
    }
    
    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    // LINK PROGRAM
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("ERROR LINKING PROGRAM: '%s' \n", eLog);
        return;
    }
    // VALIDATE PROGRAM
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("ERROR VALIDATING PROGRAM: '%s' \n", eLog);
        return;
    }
}

int main()
{
    
    // initialize GLFW
    if(!glfwInit())
    {
        printf("GLFW INITIALIZATION FAILED");
        glfwTerminate();
        return 1;
    }
    
    // setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
    if(!mainWindow)
    {
        printf("GLFW WINDOW CREATION FAILED");
        glfwTerminate();
        return 1;
    }
    
    // get buffer size information
    int buffferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &buffferWidth, &bufferHeight);
    
    // set the context for GLEW to use
    glfwMakeContextCurrent(mainWindow);
    
    // allow modern extension features
    glewExperimental = GL_TRUE;
    
    // initialize GLEW
    if(glewInit() != GLEW_OK)
    {
        printf("GLEW INITIALIZATION FAILED");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    // setup viewport
    glViewport(0, 0, buffferWidth, bufferHeight);
    
    // create shapes
    createTriangle();
    compileShaders();
    
    
    // loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // get and handle user input events
        glfwPollEvents();
        // clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // draw shaders
        glUseProgram(shader);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        
        glUseProgram(0);
        // swap the buffers
        glfwSwapBuffers(mainWindow);
    }
    
    std::cout << &bufferHeight;
    
    return 0;
}


