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
    


#endif
