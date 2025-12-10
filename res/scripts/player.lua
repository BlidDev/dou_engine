local ph
local t
local cm

init_speed = 3
local speed =3
jump = 10
sensi = 0.1

local affectedcounter = 0
local affected = true

other = nil
othero = nil

function on_init()
    ph = get_physicbody(scene, this)
    t  = get_transform(scene, this)
    cm = get_camera(scene, this)
    update_camera_target(cm, t:position())
    set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_DISABLED)
end


local counter = 0;
local flashlight = false


local mousecounter = 0;
local captured = true

local wirecounter = 100
local wire = false

local made_child = false

function on_update(dt)
    local fps = 1/dt
    --log_info("fps: {}", fps)
    speed = init_speed
    update_camera_target(cm, t:position());

    if is_key_down(util.KeyboardKey.LEFT_ALT) and affectedcounter > 10 then
        if affected then
            affected = false
            ph.gravity = 0.0
            log_info("Off")
        else
            affected = true
            ph.gravity = 0.2
            log_info("On")
        end
        affectedcounter = 0;
    end
  
    if affected then
        if is_key_down(util.KeyboardKey.SPACE) and ph.move_delta.y == 0.0 then ph.velocity.y =  ph.velocity.y + 10.0 end
    else
        ph.velocity.y = 0.0
        if is_key_down(util.KeyboardKey.Q) then ph.velocity.y =   -10.0 end
        if is_key_down(util.KeyboardKey.E) then ph.velocity.y =   10.0 end
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
    ph.velocity = ph.velocity +  move * speed

    if is_key_down(util.KeyboardKey.T) then 
        ot = get_transform(scene, other)
        ot:translate(vec3.new(0,0.5,0))
    end

    if is_key_down(util.KeyboardKey.F) and counter >= 10 then
        flashlight = not flashlight
        counter = 0
    end

    local spot = get_spotlight(scene, this)
    tmp = get_camera_dir(cm.target, t:position())
    spot.direction = last_dir
    if flashlight then spot.color = vec3.new(0.97, 0.96, 0.51) else spot.color = vec3.new(0.0) end

    if is_key_down(util.KeyboardKey.O) and wirecounter >= 10 then
        set_layer_wireframe(scene, 0, wire)
        wire = not wire
        wirecounter =0
    end

    if is_key_down(util.KeyboardKey.G) and mousecounter >= 10 then
        if captured == true then
            set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_NORMAL)
            captured = false
        else

            set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_DISABLED)
            captured = true
        end
    end

    if is_key_down(util.KeyboardKey.H) and not made_child then
        make_child_of(scene, other, this)
        make_child_of(scene, othero, this)
        log_info("yooo")
         made_child = true
    end

    if is_key_down(util.KeyboardKey.C) and made_child then
        remove_children(scene, this)
        log_info("oooy")
        made_child = false
    end


    affectedcounter = affectedcounter + 1
    counter = counter + 1
    mousecounter = mousecounter + 1
    wirecounter = wirecounter + 1

end



function is_key(key)
    return is_key_down(key) and 1 or 0
end
