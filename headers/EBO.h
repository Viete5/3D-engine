#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class EBO {
    public:
        unsigned int ID;
        EBO(unsigned int *indeces, size_t size);
        EBO();
        void Bind();
        void Unbind();
        void Delete();
};

#endif //EBO_H