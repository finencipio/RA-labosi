#include "../include/Loader.h"

Model Loader::loadToVao(const std::vector<float> &position, const std::vector<unsigned> &indices)
{
    GLuint vao = createVao();

    bindIndexBuffer(indices);
    storeDataInAttributeList(0, position, 2);

    return Model(vao, indices.size());
}

GLuint Loader::createVao()
{
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);

    vaos.emplace_back(vaoID);

    return vaoID;
}

void Loader::bindIndexBuffer(const std::vector<unsigned> &indices)
{
    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    vbos.emplace_back(vboId);
}

void Loader::storeDataInAttributeList(GLuint attributeNumber, const std::vector<float> &data, size_t coordinateSize)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    vbos.emplace_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    attributes.emplace_back(attributeNumber);
}

Loader::~Loader()
{
    for (auto &vao : vaos)
    {
        glDeleteVertexArrays(1, &vao);
    }

    for (auto &vbo : vbos)
    {
        glDeleteBuffers(1, &vbo);
    }
}

void Loader::unbindVao()
{
    for (auto const& attributeNumber : attributes)
    {
        glDisableVertexAttribArray(attributeNumber);
    }
    glBindVertexArray(0);
}

void Loader::bindVao()
{
    for (auto const& attributeNumber : attributes)
    {
        glEnableVertexAttribArray(attributeNumber);
    }
}

Model::Model(GLuint vaoId, unsigned vertexCount)
        : vaoId(vaoId), vertexCount(vertexCount)
{

}
