#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void toggleWireframeMode();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool wireframeMode = false;

float tri01_vertices[] = {
    -0.40f,  0.50f, 0.0f,
     0.00f, -0.50f, 0.0f,
    -0.80f, -0.50f, 0.0f
};

float tri02_vertices[] = {
     0.40f,  0.50f, 0.0f, 
     0.80f, -0.50f, 0.0f,
     0.00f, -0.50f, 0.0f,
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *triangle01_fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *triangle02_fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Checking for Compile Errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex Shader Compile Error: " << infoLog << "\n"; 
    }

    // Triangle 01 Shader
    unsigned int triangle01_fragmentShader;
    triangle01_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(triangle01_fragmentShader, 1, &triangle01_fragmentShaderSource, NULL);
    glCompileShader(triangle01_fragmentShader);
    glGetShaderiv(triangle01_fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        memset(infoLog, 0, 512);
        glGetShaderInfoLog(triangle01_fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment Shader Compile Error: " << infoLog << "\n";
    }
    unsigned int triangle01_shaderProgram;
    triangle01_shaderProgram = glCreateProgram();
    glAttachShader(triangle01_shaderProgram, vertexShader);
    glAttachShader(triangle01_shaderProgram, triangle01_fragmentShader);
    glLinkProgram(triangle01_shaderProgram);
    glGetProgramiv(triangle01_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        memset(infoLog, 0, 512);
        glGetProgramInfoLog(triangle01_shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Program Link Error: " << infoLog << "\n";
    }

    // Triangle 02 Shader
    unsigned int triangle02_fragmentShader;
    triangle02_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(triangle02_fragmentShader, 1, &triangle02_fragmentShaderSource, NULL);
    glCompileShader(triangle02_fragmentShader);
    glGetShaderiv(triangle02_fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        memset(infoLog, 0, 512);
        glGetShaderInfoLog(triangle02_fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment Shader Compile Error: " << infoLog << "\n";
    }
    unsigned int triangle02_shaderProgram;
    triangle02_shaderProgram = glCreateProgram();
    glAttachShader(triangle02_shaderProgram, vertexShader);
    glAttachShader(triangle02_shaderProgram, triangle02_fragmentShader);
    glLinkProgram(triangle02_shaderProgram);
    glGetProgramiv(triangle02_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        memset(infoLog, 0, 512);
        glGetProgramInfoLog(triangle02_shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Program Link Error: " << infoLog << "\n";
    }


    glDeleteShader(vertexShader);
    glDeleteShader(triangle01_fragmentShader);
    glDeleteShader(triangle02_fragmentShader);

    // Vertex Data
    unsigned int VBOs[2], VAOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // Triangle 01 Vertex Data Input
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri01_vertices), tri01_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    // Triangle 02 Vertex Data Input
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri02_vertices), tri02_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(triangle01_shaderProgram);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Render Triangle 01
        glUseProgram(triangle01_shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Render Triangle 02
        glUseProgram(triangle02_shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(triangle01_shaderProgram);
    glDeleteProgram(triangle02_shaderProgram);

    glfwTerminate();
    return 0;
}

void toggleWireframeMode() {
    wireframeMode = !wireframeMode;
    GLenum mode = wireframeMode ? GL_LINE : GL_FILL;
    const char* str = wireframeMode ? "Wireframe Mode ON" : "Wireframe Mode OFF";
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    std::cout << str << "\n";
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    int wKeyStatus = glfwGetKey(window, GLFW_KEY_W);
    static bool wKeyPressed = false;
    if (!wKeyPressed && wKeyStatus == GLFW_PRESS) {
        wKeyPressed = true;
        toggleWireframeMode();
    }
    if (wKeyStatus == GLFW_RELEASE) {
        wKeyPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}