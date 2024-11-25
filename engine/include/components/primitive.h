#pragma once
#include <espch.h>

namespace engine {

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
}
