floor = nil

local p
local thing = 50
function on_init()
    p = get_primitive(scene, this)
end

function on_update(dt) 

    p.color.r = 255 - thing
    p.color.g = thing
    p.color.b = (thing + 10) % 255
    thing = (thing + 1) % 255
end

function on_end()
end
