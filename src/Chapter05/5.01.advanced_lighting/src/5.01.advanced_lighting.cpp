#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#include <textures/texture.h>
#include <camera/camera.h>

#include <iostream>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define JARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#define RAND_0_1 (float)rand() / (float)RAND_MAX
#define RAND_COLOR glm::vec3(RAND_0_1, RAND_0_1, RAND_0_1)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 cubeColors[JARRAY_SIZE(cubePositions)];

float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,   0.0f, 1.0f, 0.0f,
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,   0.0f, 1.0f, 0.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f,   0.0f, 1.0f, 0.0f								
};

float cubeVertices[] = {
    // Positions            // TexCoords  // Normals
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   0.0f,  0.0f, -1.0f,  
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,   0.0f,  0.0f, -1.0f,  
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   0.0f,  0.0f, -1.0f,  
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   0.0f,  0.0f, -1.0f,  
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   0.0f,  0.0f, -1.0f,  
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   0.0f,  0.0f, -1.0f,  

    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   0.0f,  0.0f, 1.0f,   
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   0.0f,  0.0f, 1.0f,   
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   0.0f,  0.0f, 1.0f,   
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   0.0f,  0.0f, 1.0f,   
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,   0.0f,  0.0f, 1.0f,   
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   0.0f,  0.0f, 1.0f,   

    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,  
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,  
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,  
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,  
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,  
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,  
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f,  0.0f,  0.0f,  
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f,  0.0f,  0.0f,  
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f,  0.0f,  0.0f,  
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f,  0.0f,  0.0f,  
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f,  0.0f,  0.0f,  
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f,  0.0f,  0.0f,  
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   0.0f, -1.0f,  0.0f,  
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,   0.0f, -1.0f,  0.0f,  
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   0.0f, -1.0f,  0.0f,  
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   0.0f, -1.0f,  0.0f,  
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   0.0f, -1.0f,  0.0f,  
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   0.0f, -1.0f,  0.0f,  
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   0.0f,  1.0f,  0.0f,  
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   0.0f,  1.0f,  0.0f,  
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   0.0f,  1.0f,  0.0f,  
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   0.0f,  1.0f,  0.0f,  
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,   0.0f,  1.0f,  0.0f,  
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   0.0f,  1.0f,  0.0f
};


// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Light
glm::vec3 yellowColor = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 whiteColor = glm::vec3(1.0f);

glm::vec3 lightColor = glm::vec3(1.0f);
// glm::vec3 lightPosition = glm::vec3(-2.0f, 2.0f, 5.0f);
glm::vec3 lightDirection = glm::vec3(-0.2f, -0.25f, -0.3f);

bool useBlinnPhongLightingModel = true;

typedef struct DirectionalLight {
public:
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void uploadDirection(const Shader& shader, const std::string& directionVarName) {
        shader.use();
        shader.setVec3(directionVarName, glm::value_ptr(direction));
    }

    void uploadToShader(const Shader& shader, 
        const std::string& directionVarName, 
        const std::string& ambientVarName, 
        const std::string& diffuseVarName, 
        const std::string& specularVarName ) {
        
        shader.use();
        shader.setVec3(directionVarName, glm::value_ptr(direction));
        shader.setVec3(ambientVarName, glm::value_ptr(ambient));
        shader.setVec3(diffuseVarName, glm::value_ptr(diffuse));
        shader.setVec3(specularVarName, glm::value_ptr(specular));
    }

    void uploadToShader(const Shader& shader) {
        shader.use();
        shader.setVec3("directional_light.direction", glm::value_ptr(direction));
        shader.setVec3("directional_light.ambient", glm::value_ptr(ambient));
        shader.setVec3("directional_light.diffuse", glm::value_ptr(diffuse));
        shader.setVec3("directional_light.specular", glm::value_ptr(specular));
    }
} directional_light;



void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam) {
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // enable OpenGL debug context if context allows for debug context
    int flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
        // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        // If we wanted to only show messages from the OpenGL API, that are errors, and have a high severity, we'd configure it as follows
        glDebugMessageControl(GL_DEBUG_SOURCE_API, 
                      GL_DEBUG_TYPE_ERROR, 
                      GL_DEBUG_SEVERITY_HIGH,
                      0, nullptr, GL_TRUE); 
    }

    directional_light light {
        .direction = lightDirection,
        .ambient = glm::vec3(0.2f, 0.2f, 0.2f),
        .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        .specular = glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // Our Vertex and Fragment Shaders.
    Shader shader("../shaders/advanced_lighting.vert", "../shaders/advanced_lighting.frag");

    Texture woodenTexture("wood.png");

    shader.use();
    woodenTexture.bindToTextureUnit(0);
    shader.setInt("material.diffuse", 0);
    shader.setFloat("material.shininess", 512);

    light.uploadToShader(shader);

    // Cube VAO
    unsigned int cubeVBO, cubeVAO;
    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // Vertex Position Attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Vertex Texture Coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Vertex Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Plane VAO
    unsigned int planeVBO, planeVAO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);

    glm::mat4 identity = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model =  identity;
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.use();
        shader.setMat4f("view", 1, false, glm::value_ptr(view));
        shader.setMat4f("projection", 1, false, glm::value_ptr(projection));
        model = glm::scale(model, glm::vec3(4.0f));
        shader.setMat4f("model", model);

        shader.setVec3("cameraPos", &camera.Position[0]);
        shader.setBool("useBlinnPhong", useBlinnPhongLightingModel);

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    static bool keyPressed = false;
    auto bKeyStatus = glfwGetKey(window, GLFW_KEY_B);
    if (!keyPressed && bKeyStatus == GLFW_PRESS) {
        keyPressed = true;
        useBlinnPhongLightingModel = !useBlinnPhongLightingModel;
    }
    if (bKeyStatus == GLFW_RELEASE) {
        keyPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}