local ph
local t
local cm

init_speed = nil
local speed = 0.00
jump = nil
sensi = nul


other = nil
othero = nil

function on_init()
    ph = get_physicsbody(scene, this)
    t  = get_transform(scene, this)
    cm = get_camera(scene, this)
    set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_DISABLED)
end


local flashlight = false
local affected = true
local captured = true
local wire = false



function on_update(dt)
    speed = init_speed

    if is_key_clicked(util.KeyboardKey.LEFT_ALT)then
        if affected then
            affected = false
            ph.gravity = 0.0
            log_info("Off")
        else
            affected = true
            ph.gravity = 9.8
            log_info("On")
        end
    end

    if affected then

        if is_key_down(util.KeyboardKey.SPACE) and ph.move_delta.y == 0.0 then ph.velocity.y =  jump end
    else
        ph.velocity.y = 0.0
        if is_key_down(util.KeyboardKey.E) then ph.velocity.y =   (speed * 30) * dt end
        if is_key_down(util.KeyboardKey.Q) then ph.velocity.y =  -(speed * 30) * dt end
    end


    local f = is_key(util.KeyboardKey.W) - is_key(util.KeyboardKey.S)
    local r = is_key(util.KeyboardKey.D) - is_key(util.KeyboardKey.A)


    -- sprint
    speed = is_key_down(util.KeyboardKey.LEFT_SHIFT) and speed * 3 or speed


    local mouse_delta = get_mouse_delta() * -dt * sensi

    local last_dir = get_camera_dir(cm)

    if captured then handle_mouse_delta(cm, mouse_delta, true) end

    local forward = get_camera_flat_forward(cm)
    local right   = get_camera_right(cm)

    local move = (forward * f) + (right * r)
    move.y = 0.0
    ph.velocity = ph.velocity +  move * speed * dt

    if is_key_clicked(util.KeyboardKey.F) then
        flashlight = not flashlight
    end

    local spot = get_spotlight(scene, this)
    spot.direction = last_dir
    if flashlight then spot.color = vec3.new(0.97, 0.96, 0.51) else spot.color = vec3.new(0.0) end

    if is_key_down(util.KeyboardKey.O) then
        set_layer_wireframe(scene, 0, wire)
        wire = not wire
    end

    if is_key_clicked(util.KeyboardKey.G) then
        local mode = captured and util.InputMode.CURSOR_NORMAL or util.InputMode.CURSOR_DISABLED;

        set_input_mode(scene, util.InputSbj.CURSOR, mode)
        captured = not captured
    end


end



function is_key(key)
    return is_key_down(key) and 1 or 0
end
