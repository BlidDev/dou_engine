#pragma once
#include <espch.h>

namespace engine {

    struct DirLightComp {
      DirLightComp() {
        direction = {-0.2f, -1.0f, -0.3f};
        color = {1.0f, 1.0f, 1.0f};
        strength = 0.5f;
        ambient = 0.1f;
      };

      glm::vec3 direction;
      float ambient;
      glm::vec3 color;
      float strength;
    };

    struct PntLightComp {
      PntLightComp() {
        color = glm::vec3(1.0f);
        constant = 1.0f;
        linear = 0.22f;
        quadratic = 0.20f;
      }

      glm::vec3 color;
      float constant;
      float linear;
      float quadratic;

      float _pad[2];
    };

    struct SptLightComp { // ugly order to match std140
      SptLightComp() {
        color = glm::vec4(1.0f);
        constant = 1.0f;
        linear = 0.22f;
        quadratic = 0.20f;
        direction = {0.0f, -1.0f, 0.0f};
        cutoff = glm::cos(glm::radians(12.5f));
        outer_cutoff = glm::cos(glm::radians(17.5f));
      }

      glm::vec3 color;
      float constant;
      
      glm::vec3 direction;
      float linear;
     

      float quadratic;
      float cutoff;
      float outer_cutoff;
      float _pad;

    };
    static_assert(sizeof(SptLightComp) == 3 * sizeof(glm::vec4));


} // namespace engine
