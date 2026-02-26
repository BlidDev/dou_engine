-- res/scripts/fps_controller.lua
  
local phys
local cam

local captured = true

function on_init()
    phys = get_physicsbody(scene, this)
    cam = get_camera(scene, this)

    -- capture mouse
    if captured then set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_DISABLED) end
end



function on_update(dt)
    -- get input
    local f = is_key(util.KeyboardKey.W) - is_key(util.KeyboardKey.S)
    local r = is_key(util.KeyboardKey.D) - is_key(util.KeyboardKey.A)

    -- jump when grounded
    if is_key_down(util.KeyboardKey.SPACE) and phys.move_delta.y == 0.0 then
        local jump_force = 5.0
        phys.velocity.y = jump_force
    end

    local sensi = 4.0
    local mouse_delta = get_mouse_delta() * -dt * sensi


    -- free and capture the mouse cursor
    if is_key_clicked(util.KeyboardKey.G) then
        mode = captured and util.InputMode.CURSOR_NORMAL or util.InputMode.CURSOR_DISABLED;

        set_input_mode(scene, util.InputSbj.CURSOR, mode)
        captured = not captured
    end


    -- handle input
    if captured then handle_mouse_delta(cam, mouse_delta, true) end

    local forward = get_camera_flat_forward(cam)
    local right   = get_camera_right(cam)

    local move = (forward * f) + (right * r) 
    local speed = 10.0

    phys.velocity = phys.velocity +  move * speed * dt
end


-- convert from bool to int
function is_key(key)
    return is_key_down(key) and 1 or 0
end

