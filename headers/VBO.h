#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO {
    public:
        unsigned int ID;
        VBO(float *vertices, size_t size);
        VBO();

        void Bind();
        void Unbind();
        void Delete();
};

#endif //VBO_H