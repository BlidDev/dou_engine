-- res/scripts/fps_controller.

local phys
local trans
local cam

local captured = true -- Mouse cursor captured flag
local picked = false  -- Is cube picked up flag

Other = nil
Pickup_distance = nil

function on_init()
    phys = get_physicsbody(scene, this)
    cam = get_camera(scene, this)
    trans = get_transform(scene, this)

    -- capture mouse
    if captured then set_input_mode(scene, util.InputSbj.CURSOR, util.InputMode.CURSOR_DISABLED) end

end



function on_update(dt)
    -- get input
    local f = Is_key(util.KeyboardKey.W) - Is_key(util.KeyboardKey.S)
    local r = Is_key(util.KeyboardKey.D) - Is_key(util.KeyboardKey.A)

    if is_key_clicked(util.KeyboardKey.F) then
        Try_pick_up()
    end

    -- jump when grounded
    if is_key_down(util.KeyboardKey.SPACE) and phys.move_delta.y == 0.0 then
        local jump_force = 5.0
        phys.velocity.y = jump_force
    end

    local sensi = 4.0
    local mouse_delta = get_mouse_delta() * -dt * sensi


    -- free and capture the mouse cursor
    if is_key_clicked(util.KeyboardKey.G) then
        local mode = captured and util.InputMode.CURSOR_NORMAL or util.InputMode.CURSOR_DISABLED;

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
function Is_key(key)
    return is_key_down(key) and 1 or 0
end


function Try_pick_up()
    if picked then
       remove_child(scene, this, Other)
       picked = false
    else
       local Other_pos = get_transform(scene, Other):position()
       local self_pos = trans:position()
       if v3_distance(Other_pos, self_pos) < Pickup_distance then
           make_child_of(scene, Other, this)
           picked = true
       end
    end
end
