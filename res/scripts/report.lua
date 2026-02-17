
local ph
local t

function on_init()
    t  = get_transform(scene, this)
    ph = get_physicsbody(scene, this)
end

function on_update(dt)
    if false then 
        log_info("velocity ({} {} {})",
        ph.velocity.x, ph.velocity.y, ph.velocity.z)

        log_info("position ({} {} {})",
        t:position().x, t:position().y, t:position().z)
        log_info("gravity: {}", ph.gravity)
        --log_info("slipperiness {}", ph.slipperiness)
    end
end


