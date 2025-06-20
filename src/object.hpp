#pragma once

#include <glad/glad.h>

#include <iostream>
#include <numeric>
#include <vector>

class Object {
public:
    GLuint VAO, VBO;
    const unsigned int byteSize;
    const float *vertices;
    const int stride;
    const int vertexCount; 


    Object(const float *_vertices, const unsigned int _size, const std::vector<int> attributes) :
        vertices(_vertices), byteSize(_size), 
        stride(std::reduce(attributes.begin(), attributes.end())),
        vertexCount(byteSize / (sizeof(float) * stride))
        
        {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW);

        int attribIndex = 0;
        int offset = 0;

        for (const int size : attributes) {
            glVertexAttribPointer(attribIndex, size, GL_FLOAT, GL_FALSE,
                stride * sizeof(float),
                reinterpret_cast<void *>(offset * sizeof(float)));
            glEnableVertexAttribArray(attribIndex);
            attribIndex++;
            offset += size;
        }
    }

    ~Object() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void draw() { glDrawArrays(GL_TRIANGLES, 0, vertexCount); }

    void drawLines() { glDrawArrays(GL_LINES, 0 ,vertexCount); }

    void bindVertexArray() { glBindVertexArray(VAO); }
};
