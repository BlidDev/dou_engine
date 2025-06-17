local ph
local t
local cm

local speed =3
local jump = 10
local sensi = 0.1

local affected = true


function on_init()
    ph = get_physicbody(scene, this)
    t  = get_transform(scene, this)
    cm = get_camera(scene, this)
    update_camera_target(cm, t.position)
end


function on_update(dt)
    --fps = 1/dt
    --log_info("fps: {}", fps)
    speed = 3
    update_camera_target(cm, t.position);

    if is_key_down(util.KeyboardKey.LEFT_ALT) then
        affected = false
        ph.gravity = 0.0
        log_info("Off")
    end

    if is_key_down(util.KeyboardKey.LEFT_CONTROL) then
        affected = true
        ph.gravity = 0.2
        log_info("On")
    end
  
    if affected then
        if is_key_down(util.KeyboardKey.SPACE) and ph.move_delta.y == 0.0 then ph.velocity.y =  ph.velocity.y + 10.0 end
    else
        ph.velocity.y = 0.0
        if is_key_down(util.KeyboardKey.Q) then ph.velocity.y =   -10.0 end
        if is_key_down(util.KeyboardKey.E) then ph.velocity.y =   10.0 end
    end


    f = is_key(util.KeyboardKey.W) - is_key(util.KeyboardKey.S)
    r = is_key(util.KeyboardKey.D) - is_key(util.KeyboardKey.A)


    -- sprint
    speed = is_key_down(util.KeyboardKey.LEFT_SHIFT) and speed * 3 or speed


    mouse_delta = get_mouse_delta() * -0.1

    handle_mouse_delta(cm, t.position, mouse_delta, true)

    forward = get_flat_forward(cm.target, t.position)
    right   = get_right(cm.target, t.position, cm.up)

    move = (forward * f) + (right * r)
    move.y = 0.0
    ph.velocity = ph.velocity +  move * speed

    if is_key_down(util.KeyboardKey.T) then 
        ot = get_transform(scene, other)
        ot.position.y = ot.position.y + 0.5
    end

end



function is_key(key)
    return is_key_down(key) and 1 or 0
end
