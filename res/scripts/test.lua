
function v3_str(v)
    formatted = string.format("(%f, %f, %f)", v.x, v.y, v.z)
    return formatted
end

-- This is a desc
function color_str(c)
    formatted = string.format("(%f, %f, %f)", c.r,c.g,c.g)
    return formatted
end


local t
local tx
local thing = 0
player = nil



function on_init()
    t = get_transfrom(scene, player)
    tx = get_text(scene, this)
end

function on_update(dt) 
    log_trace(string.format("%s %s", color_str(tx.color), v3_str(t.position)))
    tx.color.r = 255 - thing
    tx.color.g = thing
    tx.color.b = (thing + 10) % 255
    thing = (thing + 1) % 255
end

function on_end()
end

