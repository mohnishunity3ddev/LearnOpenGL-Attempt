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

void setupCubeMaterials();

typedef struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
} material;

glm::vec3 cubePositions[24];
material cubeMaterials[24];

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// Light
glm::vec3 yellowColor = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 whiteColor = glm::vec3(1.0f);
glm::vec3 lightColor = glm::vec3(0.5f);
glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 10.0f);


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

    size_t cubeCount = JARRAY_SIZE(cubePositions);
    setupCubeMaterials();

    // Our Vertex and Fragment Shaders.
    Shader objectShader("../shaders/02.3.materials_exercise01.vert", "../shaders/02.3.materials_exercise01.frag");
    Shader lightShader("../shaders/light_shader.vert", "../shaders/light_shader.frag");
    // Texture containerTexture("container.jpg", GL_TEXTURE_2D, true, GL_RGB, GL_RGB, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
    // Texture awesomeFaceTexture("awesomeface.png", GL_TEXTURE_2D, true, GL_RGB, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
    
    // Vertex Data
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Vertex Position Attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightingVAO;
    glGenVertexArrays(1, &lightingVAO);

    glBindVertexArray(lightingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0 );
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glm::mat4 identity = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);

    const float lightMoveSpeed = 50.0f;
    float angle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // Oscillate light in a circle of radius 15 centered at 0.0
        // angle += deltaTime * lightMoveSpeed;
        // float circleRadius = 6.0f;
        // glm::vec3 circleCenter = glm::vec3(0.0f, 0.0f, -7.5f);
        // float lightXPos = cos(glm::radians(angle)) * circleRadius;
        // float lightZPos = sin(glm::radians(angle)) * circleRadius;
        // lightPosition.x = lightXPos; lightPosition.z = lightZPos;
        // lightPosition += circleCenter;

        // Changing Light color
        glm::vec3 _lightColor = glm::vec3(1);

        lightShader.use();
        lightShader.setVec3("lightColor", &_lightColor[0]);
        glBindVertexArray(lightingVAO);

        glm::mat4 lightModel = glm::translate(identity, lightPosition);
        lightModel = glm::scale(lightModel, glm::vec3(0.25f));
        lightShader.setMat4f("View", 1, false, glm::value_ptr(view));
        lightShader.setMat4f("Projection", 1, false, glm::value_ptr(projection));
        lightShader.setMat4f("Model", 1, false, glm::value_ptr(lightModel));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        objectShader.use();
        objectShader.setMat4f("view", 1, GL_FALSE, glm::value_ptr(view));
        objectShader.setMat4f("projection", 1, GL_FALSE, glm::value_ptr(projection));
        objectShader.setVec3("viewPos", &camera.Position[0]);
        objectShader.setVec3("lightPos", glm::value_ptr(lightPosition));

        
        glBindVertexArray(VAO); 
        for(int i = 0; i < cubeCount; ++i) {
            glm::mat4 model = glm::translate(identity, cubePositions[i]);
            objectShader.setMat4f("model", 1, GL_FALSE, glm::value_ptr(model));

            objectShader.setVec3("material.ambient",glm::value_ptr(cubeMaterials[i].ambient));
            objectShader.setVec3("material.diffuse",glm::value_ptr(cubeMaterials[i].diffuse));
            objectShader.setVec3("material.specular",glm::value_ptr(cubeMaterials[i].specular));
            objectShader.setFloat("material.shininess", cubeMaterials[i].shininess * 128);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

void setupCubeMaterials() {
    uint32_t cubeCount = JARRAY_SIZE(cubePositions);
    for(uint32_t i = 0; i < 5; ++i) {
        for(uint32_t j = 0; j < 5; ++j) {
            uint32_t index = (i * 5) + j;
            float xPos = ((float)j - 2.0f) * 2.5f;
            float yPos = ((float)(5 - i) - 2.0f) * 2.5f;
            float zPos = -7.5f;
            if(index < cubeCount) {
                cubePositions[index] = glm::vec3( xPos, yPos, zPos );
            }
        }
    }

    uint32_t i = 0;
    cubeMaterials[i++] = material {
        .name = "emerald",
        .ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f),
        .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
        .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
        .shininess = 0.6f
    };

    cubeMaterials[i++] = material {
        .name = "jade",
        .ambient = glm::vec3(0.135f, 0.2225f, 0.1575f),
        .diffuse = glm::vec3(0.54f,	0.89f,	0.63f), 
        .specular = glm::vec3(0.316228f, 0.316228f,	0.316228f), 
        .shininess = 0.1f
    };

    cubeMaterials[i++] = material {
        .name = "obsidian",
        .ambient = glm::vec3(0.05375f,	0.05f,	0.06625f),
        .diffuse = glm::vec3(0.18275f,	0.17f,	0.22525f), 
        .specular = glm::vec3(0.332741f, 0.328634f,	0.346435f), 
        .shininess = 0.3f
    };

    cubeMaterials[i++] = material {
        .name = "pearl",
        .ambient = glm::vec3(0.25f, 0.20725f, 0.20725f),
        .diffuse = glm::vec3(1.0f, 0.829f, 0.829f), 
        .specular = glm::vec3(0.296648f, 0.296648f, 0.296648f), 
        .shininess = 0.088f
    };

    cubeMaterials[i++] = material {
        .name = "ruby",
        .ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f),
        .diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f), 
        .specular = glm::vec3(0.727811f, 0.626959f, 0.626959f), 
        .shininess = 0.6f
    };

    cubeMaterials[i++] = material {
        .name = "turquoise",
        .ambient = glm::vec3(0.1f, 0.18725f, 0.1745f),
        .diffuse = glm::vec3(0.396f, 0.74151f, 0.69102f), 
        .specular = glm::vec3(0.297254f, 0.30829f, 0.306678f), 
        .shininess = 0.1f
    };

    cubeMaterials[i++] = material {
        .name = "brass",
        .ambient = glm::vec3(0.329412f, 0.223529f, 0.027451f),
        .diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f), 
        .specular = glm::vec3(0.992157f, 0.941176f, 0.807843f), 
        .shininess = 0.21794872f
    };

    cubeMaterials[i++] = material {
        .name = "bronze",
        .ambient = glm::vec3(0.2125f, 0.1275f, 0.054f),
        .diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f), 
        .specular = glm::vec3(0.393548f, 0.271906f, 0.166721f), 
        .shininess = 0.2f
    };

    cubeMaterials[i++] = material {
        .name = "chrome",
        .ambient = glm::vec3(0.25f, 0.25f, 0.25f),
        .diffuse = glm::vec3(0.4f, 0.4f, 0.4f), 
        .specular = glm::vec3(0.774597f, 0.774597f, 0.774597f), 
        .shininess = 0.6f
    };

    cubeMaterials[i++] = material {
        .name = "copper",
        .ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f),
        .diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f), 
        .specular = glm::vec3(0.256777f, 0.137622f, 0.086014f), 
        .shininess = 0.1f
    };

    cubeMaterials[i++] = material {
        .name = "gold",
        .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
        .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f), 
        .specular = glm::vec3(0.628281f, 0.555802f, 0.366065f), 
        .shininess = 0.4f
    };

    cubeMaterials[i++] = material {
        .name = "silver",
        .ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f),
        .diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f), 
        .specular = glm::vec3(0.508273f, 0.508273f, 0.508273f), 
        .shininess = 0.4f
    };

    cubeMaterials[i++] = material {
        .name = "black_plastic",
        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.01f, 0.01f, 0.01f), 
        .specular = glm::vec3(0.50f, 0.50f, 0.50f), 
        .shininess = .25f
    };

    cubeMaterials[i++] = material {
        .name = "cyan_plastic",
        .ambient = glm::vec3(0.0f, 0.1f, 0.06f),
        .diffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f), 
        .specular = glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 
        .shininess = .25f
    };

    cubeMaterials[i++] = material {
        .name = "green_plastic",
        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.1f, 0.35f, 0.1f), 
        .specular = glm::vec3(0.45f, 0.55f, 0.45f), 
        .shininess = .25f
    };

    cubeMaterials[i++] = material {
        .name = "red_plastic",
        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.5f, 0.0f, 0.0f), 
        .specular = glm::vec3(0.7f, 0.6f, 0.6f), 
        .shininess = .25f
    };

    cubeMaterials[i++] = material {
        .name = "white_plastic",
        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.55f, 0.55f, 0.55f), 
        .specular = glm::vec3(0.70f, 0.70f, 0.70f), 
        .shininess = .25f
    };

    cubeMaterials[i++] = material {
        .name = "yellow_plastic",
        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.5f, 0.5f, 0.0f), 
        .specular = glm::vec3(0.60f, 0.60f, 0.50f), 
        .shininess = .25f
    };

    cubeMaterials[i++] = material {
        .name = "black_rubber",
        .ambient = glm::vec3(0.02f, 0.02f, 0.02f),
        .diffuse = glm::vec3(0.01f, 0.01f, 0.01f), 
        .specular = glm::vec3(0.4f, 0.4f, 0.4f), 
        .shininess = .078125f
    };

    cubeMaterials[i++] = material {
        .name = "cyan_rubber",
        .ambient = glm::vec3(0.0f, 0.05f, 0.05f),
        .diffuse = glm::vec3(0.4f, 0.5f, 0.5f), 
        .specular = glm::vec3(0.04f, 0.7f, 0.7f), 
        .shininess = .078125f
    };

    cubeMaterials[i++] = material {
        .name = "green_rubber",
        .ambient = glm::vec3(0.0f, 0.05f, 0.0f),
        .diffuse = glm::vec3(0.4f, 0.5f, 0.4f), 
        .specular = glm::vec3(0.04f, 0.7f, 0.04f), 
        .shininess = .078125f
    };

    cubeMaterials[i++] = material {
        .name = "red_rubber",
        .ambient = glm::vec3(0.05f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.5f, 0.4f, 0.4f), 
        .specular = glm::vec3(0.7f, 0.04f, 0.04f), 
        .shininess = .078125f
    };

    cubeMaterials[i++] = material {
        .name = "white_rubber",
        .ambient = glm::vec3(0.05f, 0.05f, 0.05f),
        .diffuse = glm::vec3(0.5f, 0.5f, 0.5f), 
        .specular = glm::vec3(0.7f, 0.7f, 0.7f), 
        .shininess = .078125f
    };

    cubeMaterials[i++] = material {
        .name = "yellow_rubber",
        .ambient = glm::vec3(0.05f, 0.05f, 0.0f),
        .diffuse = glm::vec3(0.5f, 0.5f, 0.4f), 
        .specular = glm::vec3(0.7f, 0.7f, 0.04f), 
        .shininess = .078125f
    };  
    std::cout << "i ==> " << i << "\n";  	   
}