local ph
local id

function on_init()
    ph = get_physicbody(scene, this)
    id = scene:uuid_to_entt(this)
end

function on_update(dt)

    ph.velocity.z = math.sin(get_time() +  id) * 0.5;
    ph.velocity.x = math.sin(get_time() +  id);
end


function inter(p, e,_)
    scene.manager:set_current("game")
    return 1
end

