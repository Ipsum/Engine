#if !defined(VAO_H)

#include "../../core_common.h"
#include "vbo.h"
#include "opengl.h"

namespace core {

    typedef enum {
        STATIC_DRAW = GL_STATIC_DRAW,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    } usage_type_t;

    class CORE_API vao {
    public:
        unsigned int id;
        vbo* indices_vbo;

        vao();
        void bind();
        void unbind();
        void bind_attributes();
        void unbind_attributes();
        void create_index_buffer( int* indices, int num_indices );
        void create_attribute( int attribute, float* data, int data_size, int attribute_size );
        void create_attribute( int attribute, int* data, int data_size, int attribute_size );
    };

} //end namespace core

#define VAO_H
#endif
