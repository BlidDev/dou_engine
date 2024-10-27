#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include <entt.hpp>

namespace engine {

    struct TagComp {
        std::string tag = "";
    };

    struct TransformComp {
        Vector3 position;
        Vector3 size;
        TransformComp(Vector3 position = {0.0f, 0.0f, 0.0f},Vector3 size = {1.0f, 1.0f, 1.0f}) {
            this->position = position;
            this->size = size;
        }
        //Vector3 rotation;
    };

#define PRIMITVE_FILLED    0b0001
#define PRIMITVE_WIREFRAME 0b0010
#define PRIMITVE_IMMUNE    0b0100

    struct PrimitiveComp {
        Color color = PURPLE;
        enum Shape {
            PLANE,
            CUBE,
            SPHERE
        } shape = CUBE;

        int attributes;

        PrimitiveComp(
                Shape shape = Shape::CUBE,
                Color color = PURPLE, 
                int attributes = PRIMITVE_FILLED) {
            this->shape = shape;
            this->color = color;
            this->attributes = attributes;
        }
    };

    struct UpdateComp {
        virtual void on_update(entt::registry& registry, entt::entity self, float dt) = 0;
        virtual ~UpdateComp() {}
    };

    struct ActionsComp {
        ActionsComp();
        ActionsComp (std::vector<UpdateComp*> actions) {this->actions = actions;}

        ActionsComp& add(UpdateComp* comp);
        std::vector<UpdateComp*> actions;
    };

    struct TextComp {
        std::string body;
        int font_size;
        Color color;
    };

    struct PhysicsBodyComp {
        float gravity;
        Vector3 velocity;
        Vector3 acceleration;
        bool is_solid;
        bool is_static;
        Vector3 move_delta = {0.0f,0.0f,0.0f};

        int (*intersects_callback)(entt::registry&, entt::entity, entt::entity) = nullptr;
    };


    struct CameraBuilder {
        CameraBuilder();

        CameraBuilder& target(Vector3 target);
        CameraBuilder& up(Vector3 up);
        CameraBuilder& fovy(float fovy);
        CameraBuilder& projection(CameraProjection projection);

        Camera build();
    private:
        Camera camera;
    };
}
