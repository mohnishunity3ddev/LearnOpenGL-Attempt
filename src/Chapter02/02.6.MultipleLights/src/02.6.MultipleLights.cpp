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

float cubeVertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
glm::vec3 lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);



typedef struct PointLight {
public:
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // for attenuation purposes.
    float constant;
    float linear;
    float quadratic;

    void uploadPosition(const Shader& shader, const std::string& positionVarName) {
        shader.use();
        shader.setVec3(positionVarName, glm::value_ptr(position));
    }

    void uploadToShader(const Shader& shader, 
        const std::string& positionVarName, 
        const std::string& ambientVarName, const std::string& diffuseVarName, const std::string& specularVarName,
        const std::string& constantVarName, const std::string& linearVarName, const std::string& quadraticVarName ) {

        shader.use();
        shader.setVec3(positionVarName, glm::value_ptr(position));

        shader.setVec3(ambientVarName, glm::value_ptr(ambient));
        shader.setVec3(diffuseVarName, glm::value_ptr(diffuse));
        shader.setVec3(specularVarName, glm::value_ptr(specular));

        shader.setFloat(constantVarName, constant);
        shader.setFloat(linearVarName, linear);
        shader.setFloat(quadraticVarName, quadratic);
    }

    void uploadToShader(const Shader& shader) {
        shader.use();
        shader.setVec3("point_light.position", glm::value_ptr(position));
        shader.setVec3("point_light.ambient", glm::value_ptr(ambient));
        shader.setVec3("point_light.diffuse", glm::value_ptr(diffuse));
        shader.setVec3("point_light.specular", glm::value_ptr(specular));
        shader.setFloat("point_light.constant", constant);
        shader.setFloat("point_light.linear", linear);
        shader.setFloat("point_light.quadratic", quadratic);
    }

    void uploadToShader(const Shader& shader, int index) {
        shader.use();
        std::string indexString = std::to_string(index);
        std::string name = "point_light[" + indexString + "].position";
        std::cout << "name = " << name << "\n";
        shader.setVec3("point_lights[" + indexString + "].position", glm::value_ptr(position));
        shader.setVec3("point_lights[" + indexString + "].ambient", glm::value_ptr(ambient));
        shader.setVec3("point_lights[" + indexString + "].diffuse", glm::value_ptr(diffuse));
        shader.setVec3("point_lights[" + indexString + "].specular", glm::value_ptr(specular));
        shader.setFloat("point_lights[" + indexString + "].constant", constant);
        shader.setFloat("point_lights[" + indexString + "].linear", linear);
        shader.setFloat("point_lights[" + indexString + "].quadratic", quadratic);
    }
} point_light;

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

    void uploadToShader(const Shader& shader, int index) {
        shader.use();
        shader.setVec3("directional_light.direction", glm::value_ptr(direction));
        shader.setVec3("directional_light.ambient", glm::value_ptr(ambient));
        shader.setVec3("directional_light.diffuse", glm::value_ptr(diffuse));
        shader.setVec3("directional_light.specular", glm::value_ptr(specular));
    }
} directional_light;

typedef struct AreaLight {
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutoff;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // for attenuation purposes.
    float constant;
    float linear;
    float quadratic;

    void uploadToShader(const Shader& shader) {
        shader.use();
        shader.setVec3("area_light.position", glm::value_ptr(position));
        shader.setVec3("area_light.direction", glm::value_ptr(direction));
        shader.setFloat("area_light.cutoff", cutoff);
        
        shader.setVec3("area_light.ambient", glm::value_ptr(ambient));
        shader.setVec3("area_light.diffuse", glm::value_ptr(diffuse));
        shader.setVec3("area_light.specular", glm::value_ptr(specular));

        shader.setFloat("area_light.constant", constant);
        shader.setFloat("area_light.linear", linear);
        shader.setFloat("area_light.quadratic", quadratic);
    }

    void updatePositionAndDirection(const Shader& shader, const glm::vec3& position, const glm::vec3& direction) {
        shader.use();
        this->position = position;
        this->direction = direction;
        shader.setVec3("area_light.position", glm::value_ptr(position));
        shader.setVec3("area_light.direction", glm::value_ptr(direction));
    }
} area_light;

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

    // Initialize Cube Colors
    const uint32_t cubeColorSize = JARRAY_SIZE(cubeColors);
    for(uint32_t i = 0; i < cubeColorSize; ++i) {
        cubeColors[i] = RAND_COLOR;
    }

    // Our Vertex and Fragment Shaders.
    Shader objectShader("../shaders/02.6.multiple_lights.vert", "../shaders/02.6.multiple_lights.frag");
    Shader lightShader("../shaders/light_shader.vert", "../shaders/light_shader.frag");
    Texture diffuseMap("container2_diffuse.png", GL_TEXTURE_2D, true, GL_RGB, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
    Texture specularMap("container2_specular.png", GL_TEXTURE_2D, true, GL_RGB, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
    Texture cookieMap("Cookie_tutorial_texture_flashlight.png", GL_TEXTURE_2D, true, GL_RGB, GL_RGB, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
    
    // Vertex Data
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Vertex Position Attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Normal Attribute.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex Texture Coordinates.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightingVAO;
    glGenVertexArrays(1, &lightingVAO);

    glBindVertexArray(lightingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glm::mat4 identity = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    size_t cubeCount = JARRAY_SIZE(cubePositions);

    
    diffuseMap.bindToTextureUnit(0);
    specularMap.bindToTextureUnit(1);
    cookieMap.bindToTextureUnit(2);

    objectShader.use();
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    objectShader.setInt("area_light.cookie", 2);

    directional_light directional_light {
        .direction = lightDirection,
        .ambient = glm::vec3(0.2f, 0.2f, 0.2f),
        .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        .specular = glm::vec3(1.0f, 1.0f, 1.0f)
    };
    directional_light.uploadToShader(objectShader);

    point_light point_lights[4];
    glm::vec3 point_light_positions[4] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    glm::vec3 point_light_colors[4] = {
        glm::vec3(1.0f, 0.6f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0, 0.0),
        glm::vec3(0.2f, 0.2f, 1.0f)
    };
    
    for(int i = 0; i < 4; ++i) {
        point_lights[i] = {
            .position = point_light_positions[i],
            .ambient = point_light_colors[i] * 0.1f,
            .diffuse = point_light_colors[i],
            .specular = point_light_colors[i],
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032f
        };
        point_lights[i].uploadToShader(objectShader, i);
    }

    area_light flashlight { 
        .position   = camera.Position,
        .direction  = camera.Front,
        .cutoff     = glm::radians(12.5f),
        .ambient    = glm::vec3(0.2f, 0.2f, 0.2f),
        .diffuse    = glm::vec3(1.0f),
        .specular   = glm::vec3(1.0f, 1.0f, 1.0f),
        .constant   = 1.0f,
        .linear     = 0.09f,
        .quadratic  = 0.032f
     };
     flashlight.uploadToShader(objectShader);
    
    const float lightMoveSpeed = 50.0f;
    float angle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 lightProjection = glm::perspective(glm::radians(16.5f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // Oscillate light in a circle of radius 15 centered at 0.0
        // angle += deltaTime * lightMoveSpeed;
        // float circleRadius = 8.0f;
        // glm::vec3 circleCenter = glm::vec3(0.0f, 0.0f, -6.0f);
        // float lightXPos = cos(glm::radians(angle)) * circleRadius;
        // float lightZPos = sin(glm::radians(angle)) * circleRadius;
        // lightPosition.x = lightXPos; lightPosition.z = lightZPos;
        // lightPosition += circleCenter;

        lightShader.use();
        for(int i = 0; i < 4; i++) {
            lightShader.setVec3("lightColor", glm::value_ptr( point_lights[i].diffuse ));
            glBindVertexArray(lightingVAO);

            glm::mat4 lightModel = glm::translate(identity, point_lights[i].position);
            lightModel = glm::scale(lightModel, glm::vec3(0.25f));
            lightShader.setMat4f("View", 1, false, glm::value_ptr(view));
            lightShader.setMat4f("Projection", 1, false, glm::value_ptr(projection));
            lightShader.setMat4f("Model", 1, false, glm::value_ptr(lightModel));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        objectShader.use();
        objectShader.setMat4f("view", 1, GL_FALSE, glm::value_ptr(view));
        objectShader.setMat4f("projection", 1, GL_FALSE, glm::value_ptr(projection));
        objectShader.setVec3("viewPos", &camera.Position[0]);

        objectShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        objectShader.setFloat("material.shininess", 32);

        flashlight.updatePositionAndDirection(objectShader, camera.Position, camera.Front);
        objectShader.setMat4f("flashlightProj", 1, GL_FALSE, glm::value_ptr(lightProjection));
        
        glBindVertexArray(VAO); 

        for(size_t i = 0; i < 10; i++) {
            glm::mat4 model = glm::translate(identity, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objectShader.setMat4f("model", 1, GL_FALSE, glm::value_ptr(model));
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