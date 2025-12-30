#include "mesh.h"
#include "util.h"

namespace engine {


    bool Mesh::textured() {
        return ((vao_format & VAO::TEXTURE) == VAO::TEXTURE);
    }

    bool Mesh::normals() {
        return ((vao_format & VAO::NORMAL) == VAO::NORMAL);
    }


    void Mesh::free() {
        glDeleteVertexArrays(1, &VAO);
        if (nindices > 0) {
            glDeleteBuffers(1, &EBO);
        }
        glDeleteBuffers(1, &VBO);


        VAO = 0; VBO = 0; EBO = 0;
        nindices = 0; nvertices = 0; 
        DU_CORE_DEBUG_TRACE("Freed {}", name);
        name = "UNKNOWN";
    }

    unsigned int apply_format(int format) {
        unsigned int counter = 0, size = 0;
        bool basic = ((format & VAO::BASIC) == VAO::BASIC);
        bool textured = ((format & VAO::TEXTURE) == VAO::TEXTURE);
        bool normals = ((format & VAO::NORMAL) == VAO::NORMAL);
        unsigned int width = (basic ? 3 : 0) + (textured ? 2 : 0) + (normals ? 3 : 0);

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

        if (normals) {
            glVertexAttribPointer(counter, 3, GL_FLOAT, GL_FALSE, width * sizeof(float), (void*)(size * sizeof(float)));
            glEnableVertexAttribArray(counter);
            counter++;
            size+=3;
        }

        return 0;

    }
    
    MeshBuilder::MeshBuilder (std::string name) {
        mesh.name = name;
        vertices_p = nullptr;
        indices_p  = nullptr;
        mesh.nvertices = 0;
        mesh.nindices = 0;
        vao_format = VAO::NONE;

    }

    MeshBuilder& MeshBuilder::vertices(float vertices[], unsigned int size) {
        vertices_p = vertices;
        mesh.nvertices = size;
        vao_format |= VAO::BASIC;
        return *this;
    }

    MeshBuilder& MeshBuilder::textured() {
        vao_format |= VAO::TEXTURE;

        return *this;
    }

    MeshBuilder& MeshBuilder::normals() {
        vao_format |= VAO::NORMAL;
        return *this;
    }

    MeshBuilder& MeshBuilder::indices(unsigned int indices[], unsigned int size) {
        indices_p = indices;
        mesh.nindices = size;
        vao_format |= VAO::INDICES;
        return *this;
    }

    Mesh MeshBuilder::build() {
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        if (indices_p)
            glGenBuffers(1, &mesh.EBO);
        DU_ASSERT(!vertices_p,"Null vertices gave to mesh [{}]", mesh.name);
        
        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.nvertices, vertices_p, GL_STATIC_DRAW);

        if (indices_p) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.nindices, indices_p, GL_STATIC_DRAW);
        }


        apply_format(vao_format);
        mesh.vao_format = vao_format;

        glBindVertexArray(0);
        //DU_INFO("vao: {} vbo: {} ebo: {} nv: {} ni: {}", mesh.VAO, mesh.VBO, mesh.EBO, mesh.nvertices, mesh.nindices);
        return mesh;
    }

    enum ReadIndex {
        None = -1,
        Name, Format, Vertices, Indices
    };

    Mesh mesh_from_file(const char* path, std::string* name) {
        MeshBuilder mesh_builder;
        ReadIndex index = None;

        std::string tmp_name = "unnamed";

        std::ifstream file(path);
        DU_ASSERT(!file.is_open(), "Could not open file [{}]", path);

        std::vector<float>vertices = {};
        std::vector<unsigned int>indices = {};

        std::string line = "";
        while (std::getline(file, line)) {
            if (line.empty()) { continue; }
            else if (line == "@NAME") {
                index = ReadIndex::Name; continue;
            } else if (line == "@VERTICES") {
                index = ReadIndex::Vertices; continue;
            } else if (line == "@FORMAT") {
                index = ReadIndex::Format; continue;
            } else if (line == "@INDICES") {
                index = ReadIndex::Indices; continue;
            }

            if (index == ReadIndex::Name) {
               tmp_name = trim(line);
            }
            else if (index == ReadIndex::Format) {
                std::istringstream iss (line);
                std::string format_word = "";
                while (iss>>format_word) {
                    if (format_word == "POS") {continue;}
                    else if (format_word == "TEX") {mesh_builder.textured(); continue;}
                    else if (format_word == "NOR") {mesh_builder.normals(); continue;}
                    DU_CORE_ERROR("Unkown format word give [{}]", format_word);
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
                DU_CORE_ERROR("First line must be one of @FORMAT/VERTICES/INDICES");
            }
        }
        file.close();

        DU_ASSERT(vertices.empty(), "No positions given to mesh [{}]", path);
        mesh_builder.vertices(&vertices[0], vertices.size());

        if (!indices.empty()) {
            mesh_builder.indices(&indices[0], indices.size());
        }

        if (name) *name = tmp_name;

        return mesh_builder.build();
    }


}
