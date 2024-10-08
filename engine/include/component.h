#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include <entt.hpp>

namespace engine {

    struct Tag {
        std::string tag = "";
    };

    struct Transform {
        Vector3 position;
        Vector3 size;
        Transform(Vector3 position = {0.0f, 0.0f, 0.0f},Vector3 size = {1.0f, 1.0f, 1.0f}) {
            this->position = position;
            this->size = size;
        }
        //Vector3 rotation;
    };

#define PRIMITVE_FILLED    0b0001
#define PRIMITVE_WIREFRAME 0b0010
#define PRIMITVE_IMMUNE    0b0100

    struct Primitive {
        Color color = PURPLE;
        enum Shape {
            PLANE,
            CUBE,
            SPHERE
        } shape = CUBE;

        int attributes;

        Primitive(
                Shape shape = Shape::CUBE,
                Color color = PURPLE, 
                int attributes = PRIMITVE_FILLED) {
            this->shape = shape;
            this->color = color;
            this->attributes = attributes;
        }
    };

    struct UpdateComponent {
        virtual void on_update(entt::registry& registry, entt::entity self, float dt) = 0;
        virtual ~UpdateComponent() {}
    };

    struct Actions {
        Actions (std::vector<UpdateComponent*> actions) {this->actions = actions;}
        std::vector<UpdateComponent*> actions;
    };

    struct Text {
        std::string body;
        int font_size;
        Color color;
    };

    struct PhysicsBody {
        float gravity;
        Vector3 velocity;
        Vector3 acceleration;
        bool is_solid;
        bool is_static;
        Vector3 move_delta = {0.0f,0.0f,0.0f};

        int (*intersects_callback)(entt::registry&, entt::entity, entt::entity) = nullptr;
    };
}
