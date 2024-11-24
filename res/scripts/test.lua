
local thing 


function on_init()
    print( this .."Just inited!")
end

function on_update(dt) 
    t = get_uuid_transfrom(thing)
    print (string.format("(%f, %f, %f)"), t.position.x, t.position.y, t.position.z)
end

function on_end()
    print("This is the end of: " .. this)
end
