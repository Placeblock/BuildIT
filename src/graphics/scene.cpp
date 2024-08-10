//
// Created by felix on 8/10/24.
//

#include "scene.h"
#include "graphics/shapes/shapes.h"
#include "graphics/elements/nodes/gate.h"
#include "simulation/gate.h"

void Scene::onResize(intVec2 newSize) {
    this->updateFrameBufferSize(newSize);
    this->world->onResize(this->calculateWorldSize());
    this->updateWorldQuadVertices();
}

intVec2 Scene::calculateWorldSize() {
    return this->size - intVec2(160, 0);
}

void Scene::render() {
    this->world->render();

    this->programs->updateProjectionUniforms(this->size, this->camera);

    this->useFrameBuffer();
    this->programs->textureProgram->use();
    glBindTexture(GL_TEXTURE_2D, this->world->texture);
    glBindVertexArray(this->worldVAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    this->mesh.render(this->programs->instancedProgram);
    this->nodes.pinRenderer.render(this->programs->pinProgram);
}

void Scene::onScroll(glm::vec2 offset) {

}

void Scene::onKeyAction(int key, int scanCode, int keyAction, int mods) {

}

void Scene::onMouseAction(int button, int mouseAction, int mods) {

}

void Scene::onMouseMove(glm::vec2 abs, glm::vec2 delta) {

}

static InstancedMeshRenderer generateMesh() {
    std::vector<float> outlineR = Shapes::generateRoundedRectangle(96, 96, 10);
    std::vector<float> inlineR = Shapes::generateRoundedRectangle(80, 80, 2, glm::vec2(8, 8));
    outlineR.insert(outlineR.end(), inlineR.begin(), inlineR.end());
    std::vector<unsigned char> outlineC = Shapes::getRepeatedColor(glm::vec3(0, 126, 126), 29);
    std::vector<unsigned char> inlineC = Shapes::getRepeatedColor(glm::vec3(35, 43, 43), 29);
    outlineC.insert(outlineC.end(), inlineC.begin(), inlineC.end());
    std::vector<unsigned int> indices;
    Shapes::getRoundedRectangleIndices(&indices);
    Shapes::getRoundedRectangleIndices(&indices, 29);
    return {outlineR, outlineC, indices};
}

Scene::Scene(Sim::Simulation* simulation, Programs* programs, intVec2 size)
    : simulation(simulation), programs(programs), mesh(generateMesh()), FrameBufferRenderable(size) {
    this->world = new World{programs, this->calculateWorldSize()};

    glGenVertexArrays(1, &this->worldVAO);
    glBindVertexArray(this->worldVAO);

    const std::vector<float> vertexData = this->generateWorldQuadVertices();
    static const GLfloat textureData[] = {
            0, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 1
    };

    glGenBuffers(2, this->worldVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    std::shared_ptr<Sim::AndGate> simAndGate = std::make_shared<Sim::AndGate>();
    std::shared_ptr<Gate> andGate = std::make_shared<Gate>(intVec2(1, 1), &this->mesh, "test", this->simulation, simAndGate);
    this->nodes.addNode(andGate);
}

void Scene::updateWorldQuadVertices() {
    const std::vector<float> vertexData = this->generateWorldQuadVertices();
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size()*sizeof(float), vertexData.data());
}

std::vector<float> Scene::generateWorldQuadVertices() {
    std::vector<float> vertices(12);
    vertices[0] = 160; vertices[1] = 0; vertices[2] = this->size.x;
    vertices[3] = 0; vertices[4] = this->size.x; vertices[5] = this->size.y;
    vertices[6] = this->size.x; vertices[7] = this->size.y; vertices[8] = 160;
    vertices[9] = this->size.y; vertices[10] = 160; vertices[11] = 0;
    return vertices;
}
