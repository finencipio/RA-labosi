#ifndef LAB3_LOADER_H
#define LAB3_LOADER_H

#include <vector>
#include <GL/glew.h>

struct Model
{
    GLuint vaoId;
    unsigned int vertexCount;

    Model(GLuint vaoId, unsigned vertexCount);
};

class Loader
{
private:
    std::vector<GLuint> vbos;
    std::vector<GLuint> vaos;
    std::vector<GLuint> attributes;

    GLuint createVao();

    void bindIndexBuffer(const std::vector<unsigned>& indices);

    void storeDataInAttributeList(GLuint attributeNumber,const std::vector<float>& data, size_t coordinateSize);

public:
    Model loadToVao(const std::vector<float>& position, const std::vector<unsigned>& indices);

    ~Loader();

    void unbindVao();

    void bindVao();
};


#endif //LAB3_LOADER_H
