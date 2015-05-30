//
//  main.cpp
//  SDL2 Open GL Tutorial 02
//  Port of tutorials from http://ogldev.atspace.co.uk/
//  Created by Paul Hindt on 3/5/15.

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

SDL_Window *gWindow;
SDL_GLContext gOpenGLCtx;
SDL_Event gEvent;

typedef struct {
    double x, y, z;
} vec3;


GLuint vbo;
GLuint vao;

// Shader sources
const GLchar *vsprog =
"#version 330 core\n"
"in vec2 position;"
"void main() {"
"   gl_Position = vec4(position, 0.0, 1.0);"
"}";

const GLchar *fsprog =
"#version 330 core\n"
"out vec4 outColor;"
"void main() {"
"   outColor = vec4(1.0, 1.0, 1.0, 1.0);"
"}";


// Apple recommended way to declare OGL extension prototypes (if not weak-linking entire framework)
// extern void glGenBuffers(GLsizei, GLuint*);// __attribute__((weak_import));
// can specify this in linker settings instead...

/* void InitializeGL()
{
//    //Create OpenGL context
//    gOpenGLCtx = SDL_GL_CreateContext(gWindow);
//    
//    if (gOpenGLCtx == NULL) {
//        printf("Error creating OpenGL context: %s\n", SDL_GetError());
//        return;
//    }
//    else {
//        printf("OpenGL context OK!\n");
//        GLuint vbo; //vertex buffer object
//        glGenBuffers(1, &vbo); //generate vertex buffers
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        
//        //GL_STATIC_DRAW    = vertex data uploaded once, drawn many times
//        //GL_DYNAMIC_DRAW   = vertex data changes sometimes, drawn many times
//        //GL_STREAM_DRAW    = vertex data changes almost every frame, drawn many times
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//        printf("glGenBuffers...%u\n", vbo); //verify vbo was generated successfully (prints '1')
//        
//        //Set up vertex and fragment shaders and compile
//        GLint status;
//        char vslog[512];
//        char fslog[512];
//        
//        GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(vertexshader, 1, &vsprog, NULL);
//        glCompileShader(vertexshader);
//
//        glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &status);
//        glGetShaderInfoLog(vertexshader, 512, NULL, vslog);
//        printf("%s\n", vslog);
//        
//        GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragmentshader, 1, &fsprog, NULL);
//        glCompileShader(fragmentshader);
//        
//        glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &status);
//        glGetShaderInfoLog(fragmentshader, 512, NULL, fslog);
//        printf("%s\n", fslog);
//        
//        GLuint shaderprogram = glCreateProgram();
//        glAttachShader(shaderprogram, vertexshader);
//        glAttachShader(shaderprogram, fragmentshader);
//        glBindFragDataLocation(shaderprogram, 0, "outColor");
//        glLinkProgram(shaderprogram);
//        glUseProgram(shaderprogram);
//        
//        GLint posattrib = glGetAttribLocation(shaderprogram, "position");
//        
//        glVertexAttribPointer(posattrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
//        glEnableVertexAttribArray(posattrib);
//    }
} */

bool InitGL()
{
    //Create OpenGL context
    gOpenGLCtx = SDL_GL_CreateContext(gWindow);
    
    if (gOpenGLCtx == NULL) {
        printf("Error creating OpenGL context: %s\n", SDL_GetError());
        
        return false;
    }
    else {
        printf("Open GL context created successfully!\n");
        printf("OpenGL version: %s\n", glGetString(GL_VERSION));
        printf("OpenGL vendor: %s\n", glGetString(GL_VENDOR));
        printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        glClearColor(0.f, 0.f, 0.f, 0.f);
        
        return true;
    }
}

// Initialize SDL2 framework
bool InitializeSDL()
{
    //SDL_INIT_VIDEO
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL2: %s\n", SDL_GetError());
        return false;
    }
    else {
        //Configure OpenGL context attributes for version 3.2
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
        
        gWindow = SDL_CreateWindow( "OpenGL",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGHT,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    }
    
    InitGL();
    
    return true;
}


void CreateVertexBuffer()
{
    vec3 vertices[1];
    vertices[0].x = 0.0f;
    vertices[0].y = 0.0f;
    vertices[0].z = 0.0f;
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void RefreshGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawArrays(GL_POINTS, 0, 1);
    
    glDisableVertexAttribArray(0);
    
    SDL_GL_SwapWindow(gWindow);
}

// Program Main
int main(int argc, const char *argv[]) {
    bool done = false;
    
    //Init
    if (!InitializeSDL()) {
        printf("Engine initialization failed!");
    }
    //Main program loop
    else {
        CreateVertexBuffer();
        
        while(!done) {
            while(SDL_PollEvent(&gEvent)) {
                switch(gEvent.type) {
                    case SDL_QUIT:
                        done = true;
                        break;
                    case SDL_KEYUP:
                        if(gEvent.key.keysym.sym == SDLK_ESCAPE) {
                            done = true;
                        }
                        break;
                }
                
                RefreshGL();
            }
            //glDrawArrays(GL_POINTS, 0, 1);
            //glDisable(GL_DEPTH_TEST);
        }
    }
    
    SDL_GL_DeleteContext(gOpenGLCtx);
    SDL_Quit();
    return 0;
}
