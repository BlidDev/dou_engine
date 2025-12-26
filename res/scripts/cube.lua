local ph
local id
local t

function on_init()
    t  = get_transform(scene, this)
    ph = get_physicbody(scene, this)
    id = scene:uuid_to_entt(this)
end

function on_update(dt)
    --t.rotation.y = math.sin(get_time() +  id) * 50
    --t.rotation.z = math.sin(get_time() +  id) * 50
    ph.velocity.y =  math.sin(get_time()) * 0.5
    ph.velocity.x =  math.sin(get_time()) * 0.5
end


