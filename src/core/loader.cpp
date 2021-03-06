#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include "loader.h"
#include "math/vector.h"
#include "platform/platform.h"

namespace core {

Mesh* load_model( std::string file_path, file_type_t model_type )
{
    switch ( model_type ) {
    case OBJ_FILE: {
        Mesh* temp_mesh = new Mesh();
        load_obj_file(file_path, temp_mesh);
        return temp_mesh;
    }
    default: {
        return NULL;
    }
    }

}

Texture* load_texture( std::string file_path, file_type_t texture_type )
{
    switch ( texture_type ) {
    case TGA_FILE: {
    #if 0
        Texture* temp_text = new Texture();
        Tga info = Tga(file_path.c_str(), NULL);
        glGenTextures(1, &temp_text->texID);
        glBindTexture(GL_TEXTURE_2D, temp_text->texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_text->width, temp_text->height, 0, GL_RGB, GL_UNSIGNED_BYTE, temp_text->imageData);
        glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetWidth(), 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());
        //uh_glGenerateMipmap(GL_TEXTURE_2D);
        return temp_text;
        #endif
        return NULL;
    }
    default: {
        return NULL;
    }
    }
}

#define bufSize 1024

void _replace_multi_space_with_single_space( char *str )
{
    char *dest = str;

    while (*str != '\0')
    {
        while ( *str == ' ' && *(str + 1) == ' ' )
            str++;
            *dest++ = *str++;
    }

    *dest = '\0';
}

typedef struct {
    int vert;
    int norm;
    int text;
} indice_data;

int _get_indice( int vert , int text, int norm, std::vector<indice_data*>* temp_data )
{
    for ( int i = 0; i < temp_data->size(); i++ ) {
        indice_data* data = (*temp_data)[i];
        if ( vert == data->vert && text == data->text && norm == data->norm ) {
            return i;
        }
    }

    indice_data* temp = new indice_data;
    temp->vert = vert;
    temp->norm = norm;
    temp->text = text;
    temp_data->push_back(temp);

    return (int)(temp_data->size() - 1);
}

void load_obj_file( std::string file_path, Mesh* mesh )
{
    int indice_index = 0;
    FILE *file;
    errno_t err;
    std::vector<Vector3f> vert_vector(1000);
    std::vector<Vector3f> norm_vector(1000);
    std::vector<Vector3f> text_vector(1000);
    std::vector<int> indice_vector(1000);
    std::vector<indice_data*> temp_data;
    vert_vector.clear();
    norm_vector.clear();
    text_vector.clear();
    temp_data.clear();
    indice_vector.clear();
    int first = 0;

    char buf[bufSize];
    int current_indice = 0;

    err = fopen_s(&file, file_path.c_str(), "r");

    if ( err != 0 ) {
        LOG_ERROR("Error opening File at path " << file_path);
        return;
    }

    while ( fgets(buf, sizeof(buf), file) != NULL ) {
        buf[strlen(buf) - 1] = '\0';
        _replace_multi_space_with_single_space(buf);
        std::string str(buf);
        std::istringstream buff(str);
        std::istream_iterator<std::string> beg(buff), end;
        std::vector<std::string> tokens(beg, end);

        if ( tokens.size() == 0 ) {
            continue;
        }

        float x, y, z;
        char type1;
        char type2;
        int vert1, vert2, vert3;
        int text1, text2, text3;
        int norm1, norm2, norm3;


        sscanf_s(buf, "%c%c", &type1, (unsigned)sizeof(char), &type2, (unsigned)sizeof(char));

        if ( tokens.at(0) == "v" ) {
            vert_vector.push_back(Vector3f((float)atof(tokens.at(1).c_str()), (float)atof(tokens.at(2).c_str()), (float)atof(tokens.at(3).c_str())));
        } else if ( tokens.at(0) == "vt" ) {
            text_vector.push_back(Vector3f((float)atof(tokens.at(1).c_str()), (float)atof(tokens.at(2).c_str()), 0));

        } else if ( tokens.at(0) == "vn" ) {
            sscanf_s(buf, "%c%c %f %f %f", &type1, (unsigned)sizeof(char), &type2, (unsigned)sizeof(char), &x, &y, &z);
            norm_vector.push_back(Vector3f((float)atof(tokens.at(1).c_str()), (float)atof(tokens.at(2).c_str()), (float)atof(tokens.at(3).c_str())));
        } else if ( tokens.at(0) == "f" ) {
            if ( first == 0 ) {
                indice_vector.clear();
                first = 1;
            }
            int total_tri = (int)tokens.size() - 3;
            for ( int i = 0; i < 1; i++ ) {
                sscanf_s(tokens.at(i+1).c_str(), "%d/%d/%d", &vert1, &text1, &norm1);
                sscanf_s(tokens.at(i+2).c_str(), "%d/%d/%d", &vert2, &text2, &norm2);
                sscanf_s(tokens.at(i+3).c_str(), "%d/%d/%d", &vert3, &text3, &norm3);
                indice_vector.push_back(_get_indice( vert1 , text1, norm1, &temp_data ));
                indice_vector.push_back(_get_indice( vert2 , text2, norm2, &temp_data ));
                indice_vector.push_back(_get_indice( vert3 , text3, norm3, &temp_data ));

                if ( tokens.size() > 4 ) {
                    sscanf_s(tokens.at(i+3).c_str(), "%d/%d/%d", &vert1, &text1, &norm1);
                    sscanf_s(tokens.at(i+4).c_str(), "%d/%d/%d", &vert2, &text2, &norm2);
                    sscanf_s(tokens.at(i+1).c_str(), "%d/%d/%d", &vert3, &text3, &norm3);
                    indice_vector.push_back(_get_indice( vert1 , text1, norm1, &temp_data ));
                    indice_vector.push_back(_get_indice( vert2 , text2, norm2, &temp_data ));
                    indice_vector.push_back(_get_indice( vert3 , text3, norm3, &temp_data ));
                }
            }
        }
    }

    mesh->vertices = (float*)malloc(sizeof(float) * 3 * temp_data.size());
    mesh->normals = (float*)malloc(sizeof(float) * 3 * temp_data.size());
    mesh->textures = (float*)malloc(sizeof(float) * 2 * temp_data.size());

    int vec3_offset = 0;
    int vec2_offset = 0;

    for ( int i = 0; i < temp_data.size(); i++ ) {

        indice_data* data = temp_data[i];

        //vert
        mesh->vertices[vec3_offset] = vert_vector[data->vert - 1].vec.x;
        mesh->vertices[vec3_offset + 1] = vert_vector[data->vert - 1].vec.y;
        mesh->vertices[vec3_offset + 2] = vert_vector[data->vert - 1 ].vec.z;

        //norm
        mesh->normals[vec3_offset] = norm_vector[data->norm - 1].vec.x;
        mesh->normals[vec3_offset + 1] = norm_vector[data->norm - 1].vec.y;
        mesh->normals[vec3_offset + 2] = norm_vector[data->norm - 1 ].vec.z;

        //text
        mesh->textures[vec2_offset] = text_vector[data->text -1].vec.x;
        mesh->textures[vec2_offset + 1] = text_vector[data->text -1].vec.y;

        vec3_offset += 3;
        vec2_offset += 2;
    }
    mesh->indices_count = (int)indice_vector.size();
    mesh->indices = (uint32_t*)malloc(sizeof(uint32_t) * mesh->indices_count);

    for ( uint32_t j = 0; j < mesh->indices_count; j++ ) {
        mesh->indices[j] = indice_vector[j];
    }

    mesh->vertices_count = 3 * (int)temp_data.size();
    mesh->normals_count = 3 * (int)temp_data.size();
    mesh->textures_count = (int)temp_data.size() * 2;

    mesh->vao = new vao();
    mesh->vao->bind();
    mesh->vao->create_index_buffer((int*)mesh->indices, (int)mesh->indices_count); //TODO: need to get ride of this cast
    mesh->vao->create_attribute(0, mesh->vertices, mesh->vertices_count * sizeof(float), 3);
    mesh->vao->create_attribute(1, mesh->normals, mesh->normals_count * sizeof(float), 3);
    mesh->vao->create_attribute(2, mesh->textures, mesh->textures_count * sizeof(float), 2);
    mesh->vao->unbind();
}

} //end namespace core
