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
    ph = get_physicbody(scene, this)
    t  = get_transform(scene, this)
    cm = get_camera(scene, this)
    update_camera_target(cm, t:position())
    set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_DISABLED)
end


local flashlight = false
local affected = true
local captured = true
local wire = false


function print_fps(dt) 
    local fps = 1/dt
    if fps >= 120 then
        log_info("fps: {}", fps)
    elseif fps >= 60 then
        log_warn("fps: {}", fps)
    else 
        log_error("fps: {}", fps)
    end
end


local made_child = false

function on_update(dt)
    --print_fps(dt)
    speed = init_speed
    update_camera_target(cm, t:position());

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


    local mouse_delta = get_mouse_delta() * -0.1

    local last_dir = get_camera_dir(cm.target, t:position())

    if captured then handle_mouse_delta(cm, t:position(), mouse_delta, true) end

    local forward = get_flat_forward(cm.target, t:position())
    local right   = get_right(cm.target, t:position(), cm.up)

    local move = (forward * f) + (right * r)
    move.y = 0.0
    ph.velocity = ph.velocity +  move * speed * dt
    --log_info("{} {} {}", cm.target.x, cm.target.y, cm.target.z)

    if is_key_down(util.KeyboardKey.T) then 
        ot = get_transform(scene, other)
        ot:translate(vec3.new(0,0.5,0))
    end

    if is_key_clicked(util.KeyboardKey.F) then
        flashlight = not flashlight
    end

    local spot = get_spotlight(scene, this)
    tmp = get_camera_dir(cm.target, t:position())
    spot.direction = last_dir
    if flashlight then spot.color = vec3.new(0.97, 0.96, 0.51) else spot.color = vec3.new(0.0) end

    if is_key_down(util.KeyboardKey.O) then
        set_layer_wireframe(scene, 0, wire)
        wire = not wire
    end

    if is_key_clicked(util.KeyboardKey.G) then
        mode = captured and util.InputMode.CURSOR_NORMAL or util.InputMode.CURSOR_DISABLED;

        set_input_mode(scene, util.InputSbj.CURSOR, mode)
        captured = not captured
    end


end



function is_key(key)
    return is_key_down(key) and 1 or 0
end
