#pragma once
#include <espch.h>


namespace engine {

    struct TransformComp {
        Vector3 position;
        Vector3 size;

        TransformComp(Vector3 position = {0.0f, 0.0f, 0.0f},Vector3 size = {1.0f, 1.0f, 1.0f}) {
            this->position = position;
            this->size = size;
        }
        //Vector3 rotation;
    };

    struct TransformBuilder {
        TransformBuilder& position(Vector3 position);
        TransformBuilder& size(Vector3 size);

        TransformComp build() {
            return this->transform;
        }

        operator TransformComp() {
            return this->transform;
        }
    private:
        TransformComp transform;
    };
}
