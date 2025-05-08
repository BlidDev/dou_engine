#include "model.h"

namespace engine {

    unsigned int apply_format(int format) {
        unsigned int counter = 0, size = 0;
        bool basic = ((format & VAO::BASIC) == VAO::BASIC);
        bool textured = ((format & VAO::TEXTURE) == VAO::TEXTURE);
        unsigned int width = (basic ? 3 : 0) + (textured ? 2 : 0);

        if (basic) {
            glVertexAttribPointer(counter, 3, GL_FLOAT, GL_FALSE, width * sizeof(float), (void*)(size * sizeof(float)));
            glEnableVertexAttribArray(counter);
            counter++;
            size+=3;
        }

        if (textured) {
            glVertexAttribPointer(counter, 2, GL_FLOAT, GL_FALSE, width * sizeof(float), (void*)(size * sizeof(float)));
            glEnableVertexAttribArray(counter);
            counter++;
            size+=2;
        }

        return 0;

    }
    
    ModelBuilder::ModelBuilder (std::string name) {
        model.name = name;
        vertices_p = nullptr;
        indices_p  = nullptr;
        model.nvertices = 0;
        model.nindices = 0;
        vao_format = VAO::NONE;

    }

    ModelBuilder& ModelBuilder::vertices(float vertices[], unsigned int size) {
        vertices_p = vertices;
        model.nvertices = size;
        vao_format |= VAO::BASIC;
        return *this;
    }

    ModelBuilder& ModelBuilder::textured() {
        vao_format |= VAO::TEXTURE;
        return *this;
    }

    ModelBuilder& ModelBuilder::indices(unsigned int indices[], unsigned int size) {
        indices_p = indices;
        model.nindices = size;
        vao_format |= VAO::INDICES;
        return *this;
    }

    Model ModelBuilder::build() {
        glGenVertexArrays(1, &model.VAO);
        glGenBuffers(1, &model.VBO);
        if (indices_p)
            glGenBuffers(1, &model.EBO);
        EG_ASSERT(!vertices_p,"Null vertices gave to model [{}]", model.name);
        
        glBindVertexArray(model.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.nvertices, vertices_p, GL_STATIC_DRAW);

        if (indices_p) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model.nindices, indices_p, GL_STATIC_DRAW);
        }


        apply_format(vao_format);


        glBindVertexArray(0);
        //EG_INFO("vao: {} vbo: {} ebo: {} nv: {} ni: {}", model.VAO, model.VBO, model.EBO, model.nvertices, model.nindices);
        return model;
    }

    enum ReadIndex {
        None = -1,
        Format, Vertices, Indices
    };

    Model model_from_file(const char* path) {
        ModelBuilder model_builder;
        ReadIndex index = None;

        std::ifstream file(path);
        EG_ASSERT(!file.is_open(), "Could not open file [{}]", path);

        std::vector<float>vertices = {};
        std::vector<unsigned int>indices = {};

        std::string line = "";
        while (std::getline(file, line)) {
            if (line.empty()) { continue; }
            else if (line == "@VERTICES") {
                index = ReadIndex::Vertices; continue;
            } else if (line == "@FORMAT") {
                index = ReadIndex::Format; continue;
            } else if (line == "@INDICES") {
                index = ReadIndex::Indices; continue;
            }

            if (index == ReadIndex::Format) {
                std::istringstream iss (line);
                std::string format_word = "";
                while (iss>>format_word) {
                    if (format_word == "POS") {continue;}
                    else if (format_word == "TEX") {model_builder.textured(); continue;}
                    EG_CORE_ERROR("Unkown format word give [{}]", format_word);
                }
            }
            else if (index == ReadIndex::Vertices) {
                std::istringstream iss (line);
                float p;
                while (iss>>p) {
                    vertices.push_back(p);
                }
            }
            else if (index == ReadIndex::Indices) {
                std::istringstream iss (line);
                unsigned int i;
                while (iss>>i) {
                    indices.push_back(i);
                }
            }
            else {
                EG_CORE_ERROR("First line must be one of @FORMAT/VERTICES/INDICES");
            }
        }
        file.close();

        EG_ASSERT(vertices.empty(), "No positions given to model [{}]", path);
        model_builder.vertices(&vertices[0], vertices.size());

        if (!indices.empty()) {
            model_builder.indices(&indices[0], indices.size());
        }

        return model_builder.build();
    }


}
