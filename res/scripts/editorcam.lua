local t
local cm

local speed = 0.3
local sensi = 0.1

first = true

function on_init()
    t  = get_transform(scene, this)
    cm = get_camera(scene, this)
    update_camera_target(cm, t.position)
end

function on_update(dt)
    if first then 
        mouse_delta = get_mouse_delta() * -sensi; first = false 
    end

    velocity = vec3.new(0.0)
    speed = 3
    update_camera_target(cm, t.position);
  
    if is_key_down(util.KeyboardKey.Q) then velocity.y =  -0.1 end
    if is_key_down(util.KeyboardKey.E) then velocity.y =   0.1 end


    f = is_key(util.KeyboardKey.W) - is_key(util.KeyboardKey.S)
    r = is_key(util.KeyboardKey.D) - is_key(util.KeyboardKey.A)

    -- sprint
    speed = is_key_down(util.KeyboardKey.LEFT_SHIFT) and speed * 3 or speed


    mouse_delta = get_mouse_delta() * -sensi

    handle_mouse_delta(cm, t.position, mouse_delta, true)

    forward = get_flat_forward(cm.target, t.position)
    right   = get_right(cm.target, t.position, cm.up)



    move = (forward * f) + (right * r)
    move.y = 0.0
    velocity = velocity +  move * speed * dt

    t.position = t.position + velocity;
end



function is_key(key)
    return is_key_down(key) and 1 or 0
end
