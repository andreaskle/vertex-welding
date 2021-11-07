#include "scene.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION

#include "glm/gtc/type_ptr.hpp"

#include "opengl/utils.h"
#include "stb_image.h"
#include "vertex.h"
#include "vertex_welder.h"
Scene::~Scene() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

std::pair<std::vector<Vertex>, std::vector<std::uint32_t>> CreateGeometry() {
    const auto num_segments = 100;
    const float radius = 2.0f;

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    // Create a triangle fan with n segments using single triangles which results in 3*(n+1) vertices.
    // A triangle fan can be welded to n+1 vertices.
    for (int i = 0; i <= num_segments; ++i) {
        float t = 2 * M_PI * i / static_cast<float>(num_segments);
        float t2 = 2 * M_PI * ((i + 1) % num_segments) / static_cast<float>(num_segments);
        vertices.emplace_back(Vertex{glm::vec3(0, 0, 0)});
        indices.push_back(vertices.size() - 1);
        vertices.emplace_back(Vertex{glm::vec3(std::sin(t) * radius, std::cos(t) * radius, 0)});
        indices.push_back(vertices.size() - 1);
        vertices.emplace_back(Vertex{glm::vec3(std::sin(t2) * radius, std::cos(t2) * radius, 0)});
        indices.push_back(vertices.size() - 1);
    }
    return {vertices, indices};
}

void Scene::Init(int width, int height) {
    vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, "vertex-welding/shaders/vertex.glsl");
    fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, "vertex-welding/shaders/fragment.glsl");
    shaderProgram = CreateProgram({vertexShader, fragmentShader});

    auto [vertices, indices] = CreateGeometry();

    VertexWelder welder;
    auto [weldedVertices, weldedIndices] = welder.Weld(vertices, indices);

    std::cout << "Reduced vertices from " << vertices.size() << " to: " << weldedVertices.size() << std::endl;
    numIndices = weldedIndices.size();

    // Create Index Buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Create Vertex Buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Create Vertex Array Object
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    // enable first vertex attrib (position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glBindVertexArray(0);

    glClearColor(0.4f, 0.4f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    camera.perspective(width, height);
    camera.SetPosition(glm::vec3(0, 0, 10));
    camera.SetTarget(glm::vec3(0, 0, 0));
}

void Scene::Render(float dt) {
    Update(dt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glUseProgram(shaderProgram);

    glm::mat4 mvpMatrix = camera.GetViewProjMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewProj"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glUseProgram(0);

    glBindVertexArray(0);
}

void Scene::ToggleWireframe() {
    wireFrame = !wireFrame;
    glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
}

void Scene::Update(float dt) {
    if (!pause) {
        time += dt;
    }

    if (time > maxTime) {
        time = 0;
    }
}
