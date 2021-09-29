/*
Type your name and student ID here
    - Name: YU, Yue
    - Student ID: 1155124490
*/
/*Declaration: This program is written by YU Yue individually and is shared with nobody. Some source code are from the tutorial online: https://learnopengl.com/ . So if there is anything similar to other's code, it mainly because that we learned the same thing. :)
*/
#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//VAOs, VBOs and EBOs
GLint programID;
GLuint VAO[7], VBO[7], EBO[7];

//camera views
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat fov = 45.0f;
glm::vec3 cameraPosBefore;

GLfloat scalesize = 1.0f;

GLfloat delta_x = 0.0f, delta_y = 0.0f;

bool keys[1024];

//Delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void get_OpenGL_info() {
    // OpenGL information
    const GLubyte* name = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glversion = glGetString(GL_VERSION);
    std::cout << "OpenGL company: " << name << std::endl;
    std::cout << "Renderer name: " << renderer << std::endl;
    std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
    GLuint objectID,
    PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
    PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
    GLenum statusType)
{
    GLint status;
    objectPropertyGetterFunc(objectID, statusType, &status);
    if (status != GL_TRUE)
    {
        GLint infoLogLength;
        objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];

        GLsizei bufferSize;
        getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
        std::cout << buffer << std::endl;

        delete[] buffer;
        return false;
    }
    return true;
}

bool checkShaderStatus(GLuint shaderID) {
    return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
    return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
    std::ifstream meInput(fileName);
    if (!meInput.good()) {
        std::cout << "File failed to load ... " << fileName << std::endl;
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(meInput),
        std::istreambuf_iterator<char>()
    );
}

void installShaders() {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* adapter[1];
    //adapter[0] = vertexShaderCode;
    std::string temp = readShaderCode("VertexShaderCode.glsl");
    adapter[0] = temp.c_str();
    glShaderSource(vertexShaderID, 1, adapter, 0);
    //adapter[0] = fragmentShaderCode;
    temp = readShaderCode("FragmentShaderCode.glsl");
    adapter[0] = temp.c_str();
    glShaderSource(fragmentShaderID, 1, adapter, 0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
        return;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    if (!checkProgramStatus(programID))
        return;

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glUseProgram(programID);
}

void sendDataToOpenGL() {
    // TODO:
    // create 2D objects and 3D objects and/or lines (points) here and bind to VAOs & VBOs

    //Plane vertices
    const GLfloat plane_vertices[] =
    {
        0.8f,  0.7f, -0.2f, 1.0f, 0.725f, 0.05f,
        0.8f, -0.7f, -0.2f, 0.5f, 0.5f, 0.0f,
        -0.8f, -0.7f, -0.2f, 0.5f, 0.5f, 0.0f,
        -0.8f,  0.7f, -0.2f, 0.5f, 0.5f, 0.0f,
    };
    GLuint plane_indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    //Words vertices
    const GLfloat word_vertices[] = {
        //O
        -0.6f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.6f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        //C
        -0.45f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.45f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.35f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.35f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        //E
        -0.3f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.3f, 0.7f, 0.225f, 1.0f, 1.0f, 1.0f,
        -0.3f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.2f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.2f, 0.7f, 0.225f, 1.0f, 1.0f, 1.0f,
        -0.2f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        //A
        -0.15f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.15f, 0.7f, 0.225f, 1.0f, 1.0f, 1.0f,
        -0.15f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.05f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        -0.05f, 0.7f, 0.225f, 1.0f, 1.0f, 1.0f,
        -0.05f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        //N
        0.0f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        0.1f, 0.7f, 0.3f, 1.0f, 1.0f, 1.0f,
        0.1f, 0.7f, 0.15f, 1.0f, 1.0f, 1.0f,
        //Y
        -0.6f, 0.7f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.55f, 0.7f, 0.425f, 1.0f, 1.0f, 1.0f,
        -0.55f, 0.7f, 0.35f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.7f, 0.5f, 1.0f, 1.0f, 1.0f,
        //U
        -0.45f, 0.7f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.45f, 0.7f, 0.35f, 1.0f, 1.0f, 1.0f,
        -0.35f, 0.7f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.35f, 0.7f, 0.35f, 1.0f, 1.0f, 1.0f,
        //'
        -0.325f, 0.7f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.325f, 0.7f, 0.475f, 1.0f, 1.0f, 1.0f,
        //s
        -0.3f, 0.7f, 0.425f, 1.0f, 1.0f, 1.0f,
        -0.3f, 0.7f, 0.3875f, 1.0f, 1.0f, 1.0f,
        -0.3f, 0.7f, 0.35f, 1.0f, 1.0f, 1.0f,
        -0.25f, 0.7f, 0.425f, 1.0f, 1.0f, 1.0f,
        -0.25f, 0.7f, 0.3875f, 1.0f, 1.0f, 1.0f,
        -0.25f, 0.7f, 0.35f, 1.0f, 1.0f, 1.0f,
    };
    GLuint word_indices[] = {
        2, 0, 0, 1, 1, 3, 2, 3,
        4, 5, 4, 6, 5, 7,
        8, 10, 8, 11, 9, 12, 10, 13,
        14, 16, 17, 19, 14, 17, 15, 18,
        20, 21, 20, 23, 22, 23,
        24, 25, 25, 26, 25, 27,
        28, 29, 30, 31, 29, 31,
        32, 33,
        34, 35, 34, 37, 35, 38, 38, 39, 36, 39
    };

    //mountain vertices
    const GLfloat mountain_vertices[] = {
        -0.75f, -0.65f, -0.2f, 0.0f, 0.4f, 0.0f,
        -0.75f, -0.25f, -0.2f, 0.0f, 0.4f, 0.0f,
        -0.6f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f,
        -0.6f, -0.4f, 0.0f, 0.5f, 1.0f, 0.5f,
        -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f,
        -0.5f, -0.4f, 0.0f, 0.5f, 1.0f, 0.5f,
        -0.35f, -0.65f, -0.2f, 0.0f, 0.4f, 0.0f,
        -0.35f, -0.25f, -0.2f, 0.0f, 0.4f, 0.0f,

        0.1f, 0.0f, -0.2f, 0.0f, 0.4f, 0.0f,
        0.1f, 0.7f, -0.2f, 0.0f, 0.4f, 0.0f,
        0.5f, 0.4f, 0.3f, 0.5f, 1.0f, 0.5f,
        0.5f, 0.5f, 0.3f, 0.5f, 1.0f, 0.5f,
        0.6f, 0.4f, 0.3f, 0.5f, 1.0f, 0.5f,
        0.6f, 0.5f, 0.3f, 0.5f, 1.0f, 0.5f,
        0.8f, 0.0f, -0.2f, 0.0f, 0.4f, 0.0f,
        0.8f, 0.7f, -0.2f, 0.0f, 0.4f, 0.0f,


    };
    GLuint mountain_indices[] = {
        0,2,6, 2,6,4, 0,1,2, 1,2,3, 4,6,7, 4,7,5, 1,3,5, 1,5,7, 2,3,4, 3,4,5,
        8,10,14, 10,14,12, 8,9,10, 9,10,11, 12,14,15, 12,15,13, 9,11,13, 9,13,15, 10,11,12, 11,12,13,
    };

    //Fish1 vertices
    const GLfloat fish1_vertices[] = {
        //tail
        -0.5f, 0.0f, 0.1f, 0.5f, 0.8f, 1.0f,
        -0.5f, 0.0f, 0.0f, 0.5f, 0.8f, 1.0f,
        -0.5f, 0.05f, 0.1f, 0.5f, 0.8f, 1.0f,
        -0.5f, 0.05f, 0.0f, 0.5f, 0.8f, 1.0f,
        -0.45f, 0.025f, 0.05f, 1.0f, 0.7f, 1.0f,
        //body
        -0.45f, -0.025f, 0.1f, 1.0f, 0.8f, 0.8f,
        -0.45f, -0.025f, 0.0f, 1.0f, 0.8f, 0.8f,
        -0.45f, 0.075f, 0.1f, 1.0f, 0.8f, 0.8f,
        -0.45f, 0.075f, 0.0f, 1.0f, 0.8f, 0.8f,
        -0.35f, -0.025f, 0.1f, 1.0f, 0.2f, 0.7f,
        -0.35f, -0.025f, 0.0f, 1.0f, 0.2f, 0.7f,
        -0.35f, 0.075f, 0.1f, 1.0f, 0.2f, 0.7f,
        -0.35f, 0.075f, 0.0f, 1.0f, 0.2f, 0.7f,
        //eyes
        -0.37f, -0.026f, 0.08f, 0.0f, 0.0f, 0.0f,
        -0.37f, -0.026f, 0.07f, 0.0f, 0.0f, 0.0f,
        -0.36f, -0.026f, 0.08f, 0.0f, 0.0f, 0.0f,
        -0.36f, -0.026f, 0.07f, 0.0f, 0.0f, 0.0f,

        -0.37f, 0.076f, 0.08f, 0.0f, 0.0f, 0.0f,
        -0.37f, 0.076f, 0.07f, 0.0f, 0.0f, 0.0f,
        -0.36f, 0.076f, 0.08f, 0.0f, 0.0f, 0.0f,
        -0.36f, 0.076f, 0.07f, 0.0f, 0.0f, 0.0f,

    };
    GLuint fish1_indices[] = {
        0,1,4, 0,2,4, 3,1,4, 3,2,4, 0,1,2, 1,2,3,
        5,6,7, 6,7,8, 9,10,11, 10,11,12, 5,6,9, 6,9,10, 7,8,11, 8,11,12, 5,7,9, 7,9,11, 6,8,10, 8,10,12,
        13,14,15, 14,15,16, 17,18,19, 18,19,20, 13,14,17, 14,17,18, 13,15,17, 15,17,19, 15,16,19, 16,19,20, 14,16,18, 16,18,20,
    };

    //Fish2 vertices
    const GLfloat fish2_vertices[] = {
        //tail
        0.4f, -0.6f, 0.2f, 0.5f, 0.8f, 1.0f,
        0.4f, -0.625f, 0.1f, 0.5f, 0.8f, 1.0f,
        0.4f, -0.575f, 0.1f, 0.5f, 0.8f, 1.0f,
        0.3f, -0.6f, 0.1f, 1.0f, 0.2f, 0.7f,

        //body
        0.1f, -0.625f, 0.2f,0.0f, 0.8f, 1.0f,
        0.1f, -0.625f, 0.1f,0.0f, 0.8f, 1.0f,
        0.1f, -0.575f, 0.2f,0.0f, 0.8f, 1.0f,
        0.1f, -0.575f, 0.1f,0.0f, 0.8f, 1.0f,
        0.3f, -0.625f, 0.2f,1.0f, 0.0f, 0.0f,
        0.3f, -0.625f, 0.1f,1.0f, 0.0f, 0.0f,
        0.3f, -0.575f, 0.2f,1.0f, 0.0f, 0.0f,
        0.3f, -0.575f, 0.1f,1.0f, 0.0f, 0.0f,

        //mouth
        0.0f, -0.6f, 0.15f, 1.0f, 0.75f, 0.75f,

        //fins
        0.32f, -0.6f, 0.35f,0.6f,0.2f,1.0f,
        0.32f, -0.6f, -0.05f,0.6f,0.2f,1.0f,

        //eyes
        0.13f, -0.626f, 0.17f, 0.0f, 0.0f, 0.0f,
        0.13f, -0.626f, 0.16f, 0.0f, 0.0f, 0.0f,
        0.14f, -0.626f, 0.17f, 0.0f, 0.0f, 0.0f,
        0.14f, -0.626f, 0.16f, 0.0f, 0.0f, 0.0f,

        0.13f, -0.574f, 0.17f, 0.0f, 0.0f, 0.0f,
        0.13f, -0.574f, 0.16f, 0.0f, 0.0f, 0.0f,
        0.14f, -0.574f, 0.17f, 0.0f, 0.0f, 0.0f,
        0.14f, -0.574f, 0.16f, 0.0f, 0.0f, 0.0f,

    };
    GLuint fish2_indices[] = {
        0,1,2, 0,1,3, 0,2,3, 1,2,3,
        8,9,10, 9,10,11, 4,5,8, 5,8,9, 6,7,11, 6,11,10, 4,5,12, 4,6,12, 6,7,12, 5,7,12, 4,6,13, 4,8,13, 8,10,13, 6,10,13, 5,9,14, 5,7,14, 7,11,14, 9,11,14,
        15,16,17, 16,17,18, 19,20,21, 20,21,22, 15,16,19, 16,19,20, 15,17,19, 17,19,21, 17,18,21, 18,21,22, 16,18,20, 18,20,22,
    };

    //stone vertices
    const GLfloat stone_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.3f, 0.3f,

    };
    GLuint stone_indices[] = {
        0
    };

    //Jellyfish vertices
    const GLfloat jellyfish_vertices[] = {
        -0.575f, -0.475f, 0.15f, 0.6f, 1.0f, 1.0f, 
        -0.575f, -0.475f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.575f, -0.425f, 0.15f, 0.6f, 1.0f, 1.0f,
        -0.575f, -0.425f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.475f, 0.15f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.475f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.425f, 0.15f,0.6f, 1.0f, 1.0f,
        -0.525f, -0.425f, 0.1f,0.6f, 1.0f, 1.0f,

        -0.550f, -0.450f, 0.175f, 0.6f, 1.0f, 1.0f,

        -0.575f, -0.475f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.575f, -0.47f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.57f, -0.475f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.575f, -0.47f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.57f, -0.475f, 0.1f, 0.6f, 1.0f, 1.0f,

        -0.575f, -0.425f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.575f, -0.430f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.57f, -0.425f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.575f, -0.430f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.57f, -0.425f, 0.1f, 0.6f, 1.0f, 1.0f,

        -0.525f, -0.475f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.53f, -0.475f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.47f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.53f, -0.475f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.47f, 0.1f, 0.6f, 1.0f, 1.0f,

        -0.525f, -0.425f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.53f, -0.425f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.43f, 0.07f, 0.6f, 1.0f, 1.0f,
        -0.53f, -0.425f, 0.1f, 0.6f, 1.0f, 1.0f,
        -0.525f, -0.43f, 0.1f, 0.6f, 1.0f, 1.0f,

    };

    GLuint jellyfish_indices[] = {
        0,1,3, 0,3,2, 5,4,7, 4,7,6, 0,1,4, 1,4,5, 2,3,6, 3,6,7, 1,3,5, 3,5,7,
        0,4,8, 4,6,8, 2,6,8, 0,2,8, 
        9,10,11, 0,9,11, 0,13,11, 0,9,10, 0,10,12, 10,11,12, 11,12,13,
        14,15,16, 3,17,14, 17,14,15, 3,14,18, 14,18,16, 15,16,17, 16,17,18, 
        19,20,21, 5,19,22, 19,22,20, 19,21,23, 19,23,5, 20,21,22, 21,22,23,
        24,25,26, 24,26,28, 24,28,7, 7,24,25, 7,25,27, 25,26,27, 26,27,28,
    };

    glGenVertexArrays(7, VAO);
    glGenBuffers(7, VBO);
    glGenBuffers(7, EBO);

    //--------------------------------------------------
    //Plane
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

    //--------------------------------------------------
    //Words
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(word_vertices), word_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(word_indices), word_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

    //--------------------------------------------------
    //Mountain
    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mountain_vertices), mountain_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mountain_indices), mountain_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

    //--------------------------------------------------
    //Fish1
    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fish1_vertices), fish1_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fish1_indices), fish1_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

    //--------------------------------------------------
    //Fish2
    glBindVertexArray(VAO[4]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fish2_vertices), fish2_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fish2_indices), fish2_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

    //stones
    glBindVertexArray(VAO[5]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stone_vertices), stone_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stone_indices), stone_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

    //--------------------------------------------------
    //jellyfish
    glBindVertexArray(VAO[6]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(jellyfish_vertices), jellyfish_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(jellyfish_indices), jellyfish_indices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glBindVertexArray(0); // Unbind VAO

}

void paintGL(void) {
    // always run
    // TODO:
    // render your objects and control the transformation here
    glClearColor(0.1f, 0.1f, 0.35f, 1.0f);  //specify the background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 20.0f);


    GLint modelLoc = glGetUniformLocation(programID, "model");
    GLint viewLoc = glGetUniformLocation(programID, "view");
    GLint projLoc = glGetUniformLocation(programID, "projection");
    GLint transLoc = glGetUniformLocation(programID, "transform");

    // Pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glDrawElements(GL_LINES, 60, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[2]);
    glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //fish1
    model = glm::translate(model, glm::vec3(-0.425f, 0.375f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
    model = glm::rotate(model, (GLfloat)glfwGetTime() * (glm::radians(50.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.425f, -0.375f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO[3]);
    glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    //fish1-1
    model = glm::translate(model, glm::vec3(-0.425f, 0.375f, 0.05f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    model = glm::rotate(model, (GLfloat)glfwGetTime() * (glm::radians(75.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.425f, -0.375f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO[3]);
    glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    //fish1-2
    model = glm::translate(model, glm::vec3(-0.425f, 0.375f, -0.07f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, (GLfloat)glfwGetTime() * (glm::radians(65.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.425f, -0.375f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO[3]);
    glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    //fish2
    model = glm::translate(model, glm::vec3(0.2f, -0.4f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
    model = glm::rotate(model, (GLfloat)glfwGetTime() * (glm::radians(-30.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.2f, 0.4f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO[4]);
    glDrawElements(GL_TRIANGLES, 102, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    //fish2-1
    model = glm::translate(model, glm::vec3(0.2f, -0.4f, 0.2f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    model = glm::rotate(model, (GLfloat)glfwGetTime() * (glm::radians(-70.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.2f, 0.4f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO[4]);
    glDrawElements(GL_TRIANGLES, 102, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    //stones
    glm::vec3 stonesPositions[] = {
        glm::vec3(0.23f, -0.44f, -0.2f),
        glm::vec3(0.31f, -0.66f, -0.2f),
        glm::vec3(-0.56f, 0.28f, -0.2f),
        glm::vec3(-0.44f, 0.10f, -0.2f),
        glm::vec3(-0.22f, 0.52f, -0.2f),
        glm::vec3(0.54f, -0.64f, -0.2f),
        glm::vec3(0.32f, -0.27f, -0.2f),
        glm::vec3(0.15f, -0.53f, -0.2f),
        glm::vec3(0.62f, -0.12f, -0.2f),
        glm::vec3(-0.45f, 0.26f, -0.2f),
        glm::vec3(0.66f, -0.24f, -0.2f),
        glm::vec3(0.38f, -0.33f, -0.2f),
        glm::vec3(-0.11f, 0.31f, -0.2f),
        glm::vec3(-0.40f, 0.13f, -0.2f),
        glm::vec3(-0.26f, 0.48f, -0.2f),
        glm::vec3(0.15f, -0.67f, -0.2f),
        glm::vec3(0.52f, -0.55f, -0.2f),
        glm::vec3(0.11f, -0.41f, -0.2f),
        glm::vec3(0.25f, -0.62f, -0.2f),
        glm::vec3(-0.34f, 0.36f, -0.2f),
    };
    
    glEnable(GL_POINT_SMOOTH);
    glPointSize(15.0f);
    glBindVertexArray(VAO[5]);
    for (GLuint i = 0; i < 20; i++) {
        model = glm::translate(model, stonesPositions[i]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    };
    glBindVertexArray(0);

    //jellyfish

    model = glm::translate(model, glm::vec3(-0.575f, -0.425f, 0.0f));
    model = glm::translate(model, glm::vec3(delta_x, delta_y, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.7f));
    model = glm::scale(model, glm::vec3(scalesize));
    model = glm::translate(model, glm::vec3(0.575f, 0.425f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    
    glBindVertexArray(VAO[6]);
    glDrawElements(GL_TRIANGLES, 126, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    

}

bool firstMouse = true;
bool firstClick = true;
bool mouseLeftDown = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // TODO:
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS){
            keys[key] = true;
            firstMouse = true;
            firstClick = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
            firstMouse = true;
            firstClick = true;
        }
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void initializedGL(void) {
    // run only once
    // TODO:
    sendDataToOpenGL();
    installShaders();
}

void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 1.0f * deltaTime;
    if (keys[GLFW_KEY_UP])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_DOWN])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        delta_x -= cameraSpeed * 0.5;
    if (keys[GLFW_KEY_D])
        delta_x += cameraSpeed * 0.5;
    if (keys[GLFW_KEY_W])
        delta_y += cameraSpeed * 0.5;
    if (keys[GLFW_KEY_S])
        delta_y -= cameraSpeed * 0.5;
    if (keys[GLFW_KEY_Q]) {
        if(scalesize >= 0.5f)
            scalesize -= cameraSpeed * 0.5;
    }
    if (keys[GLFW_KEY_E]) {
        if (scalesize <= 3.0f)
            scalesize += cameraSpeed * 0.5;
    }


}




void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!mouseLeftDown) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            cameraPosBefore = cameraPos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;

        GLfloat sensitivity = 0.01;    // Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        GLfloat cameraPosLen = glm::sqrt(cameraPosBefore.x * cameraPosBefore.x + cameraPosBefore.y * cameraPosBefore.y + cameraPosBefore.z * cameraPosBefore.z);

        cameraPos.x = (cos(glm::radians(-xoffset)) * cameraPosBefore.x + sin(glm::radians(-xoffset)) * cameraPosBefore.z);
        if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) > glm::radians(89.0f)) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(89.0f));
        }
        else if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) < glm::radians((-89.0f))) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(-89.0f));
        }
        else {
            cameraPos.y = cameraPosLen * glm::sin(glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset));
        }
        cameraPos.z = -sin(glm::radians(-xoffset)) * cameraPosBefore.x + cos(glm::radians(-xoffset)) * cameraPosBefore.z;

        cameraFront.x = -0.5 * cameraPos.x;
        cameraFront.y = -0.5 * cameraPos.y;
        cameraFront.z = -0.5 * cameraPos.z;
    }
    else {
        if (firstClick)
        {
            lastX = xpos;
            lastY = ypos;
            cameraPosBefore = cameraPos;
            firstClick = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;

        GLfloat sensitivity = 0.5;    // Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        GLfloat cameraPosLen = glm::sqrt(cameraPosBefore.x * cameraPosBefore.x + cameraPosBefore.y * cameraPosBefore.y + cameraPosBefore.z * cameraPosBefore.z);

        cameraPos.x = (cos(glm::radians(-xoffset)) * cameraPosBefore.x + sin(glm::radians(-xoffset)) * cameraPosBefore.z);
        if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) > glm::radians(89.0f)) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(89.0f));
        }
        else if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) < glm::radians((-89.0f))) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(-89.0f));
        }
        else {
            cameraPos.y = cameraPosLen * glm::sin(glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset));
        }
        cameraPos.z = -sin(glm::radians(-xoffset)) * cameraPosBefore.x + cos(glm::radians(-xoffset)) * cameraPosBefore.z;

        cameraFront.x = -0.5 * cameraPos.x;
        cameraFront.y = -0.5 * cameraPos.y;
        cameraFront.z = -0.5 * cameraPos.z;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            firstClick = true;
            mouseLeftDown = true;
        }
        else if (action == GLFW_RELEASE)
        {
            firstMouse = true;
            mouseLeftDown = false;
        }

    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}
int main(int argc, char* argv[]) {
    GLFWwindow* window;

    /* Initialize the glfw */
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    /* glfw: configure; necessary for MAC */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* do not allow resizing */
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback); // TODO
    // TODO: mouse callback, etc.
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Initialize the glew */
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    get_OpenGL_info();
    initializedGL();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        paintGL();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        do_movement();

    }

    glfwTerminate();
    return 0;
}

