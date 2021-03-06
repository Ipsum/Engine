#include "dynamic_text_component.h"

Dynamic_Text_Component::Dynamic_Text_Component()
{
    size = 0;
    vao = NULL;
    text_font = NULL;
}

void Dynamic_Text_Component::generate_dynamic_text( std::string user_text, uint16_t font_size, core::Font* font )
{
#if 0
    text = user_text;
    size = font_size;

    vao = NULL;
    vertices.clear();
    indices.clear();
    textures.clear();

    int idc_offset = 0;

    float text_x = 0;
    float text_y = 0;

    int text_idx = 0;

    float x2 = 0;
    float y2 = 0;
    float w = 0;
    float h = 0;

    for (int i = 0; i < text.size(); i++) {

         float x_offset = font->generate_char_data(text[i], size)->offset_x;
         float y_offset = font->generate_char_data(text[i], size)->offset_y;

         x2 = text_x + font->generate_char_data(text[i], size)->map_left;
         y2 = -text_y - font->generate_char_data(text[i], size)->map_top;
         w = font->generate_char_data(text[i], size)->map_width;
         h = font->generate_char_data(text[i], size)->map_height;

         vertices.push_back(x2);
         vertices.push_back(-y2);
         vertices.push_back(0);
         textures.push_back(x_offset);
         textures.push_back(y_offset);

         vertices.push_back(x2 + w);
         vertices.push_back(-y2);
         vertices.push_back(0);
         textures.push_back(x_offset + w / font->text_width);
         textures.push_back(y_offset);

         vertices.push_back(x2);
         vertices.push_back(-y2 - h);
         vertices.push_back(0);
         textures.push_back(x_offset);
         textures.push_back(y_offset + h / font->text_height);

         vertices.push_back(x2 + w);
         vertices.push_back(-y2);
         vertices.push_back(0);
         textures.push_back(x_offset + w / font->text_width);
         textures.push_back(y_offset);

         vertices.push_back(x2);
         vertices.push_back(-y2 - h);
         vertices.push_back(0);
         textures.push_back(x_offset);
         textures.push_back(y_offset + h / font->text_height);

         vertices.push_back(x2 + w);
         vertices.push_back(-y2 - h);
         vertices.push_back(0);
         textures.push_back(x_offset + w / font->text_width);
         textures.push_back(y_offset + h / font->text_height);

         text_x += font->generate_char_data(text[i], size)->advance_x;
         text_y += font->generate_char_data(text[i], size)->advance_y;

         for (int j = 0; j < 6; j++) {
            indices.push_back(idc_offset);
            idc_offset++;
         }
    }

    vao = new core::vao;
    vao->bind();
    vao->create_index_buffer((int*)indices.data(), (int)indices.size());
    vao->create_attribute(0, vertices.data(), (int)vertices.size() * sizeof(float), 3);
    vao->create_attribute(2, textures.data(), (int)textures.size() * sizeof(float), 2);
    vao->unbind();
    #endif
}

void Dynamic_Text_Component::regenerate_text()
{

}
