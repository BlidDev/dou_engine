#ifndef MACROS
#define MACROS

#define HANDLE_AABB(pos, t, ot, ph, oph, axis) \
     if (allowed_##axis) { \
        if(aabb_3d_intersects(pos,t.size,ot.position,ot.size)) { \
           if(!oph.is_static){\
               oph.velocity.axis += ph.velocity.axis;\
           }\
           else { \
               ph.move_delta.axis = 0.0f; \
               ph.velocity.axis = 0.0f; \
               allowed_##axis =false; \
           }\
        }\
     }
    


// Define the EG_ASSERT macro
#define EG_ASSERT(condition, ...) \
    do { \
        if ((condition)) { \
            EG_CORE_CRITICAL(__VA_ARGS__); \
            std::abort(); \
        } \
    } while (0)
#endif
