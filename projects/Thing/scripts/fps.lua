
player = nil

local p
local tx

function on_init()
    tx = get_text(scene, this)
    p = get_transfrom(scene, player)
end

function on_update(dt)
    tx.body= string.format("FPS: %d | (%f. %f, %f)", get_fps(),p.position.x, p.position.y, p.position.z)
end

