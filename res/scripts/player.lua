local ph
local t
local cm

local speed = 0.5

local affected = true

function on_init()
    ph = get_physicbody(scene, this)
    t  = get_transfrom(scene, this)
    cm = get_camera(scene, this)
    cm.target   = cm.target + t.position - cm.position
    cm.position = t.position
    print("what")
end

function on_update(dt)
    speed =  no 0.5
    if get_key_pressed() == util.KeyboardKey.Q then
        affected = not affected
        log_info("Switched")
        if gravity == 0.0 then gravity = 0.2 end
    end
    if is_key_down(util.KeyboardKey.LEFT_SHIFT) then speed = speed * 6.0 end

    if affected then
        if is_key_down(util.KeyboardKey.LEFT_CONTROL) then ph.gravity = 0.0 end
        if is_key_down(util.KeyboardKey.LEFT_ALT) then  ph.gravity = 0.2 end
        if is_key_down(util.KeyboardKey.SPACE) and ph.move_delta.y == 0.0 then ph.velocity.y =  ph.velocity.y + 10.0 end
    else
        ph.gravity = 0.0
        ph.velocity.y = 0.0
        if is_key_down(util.KeyboardKey.LEFT_CONTROL) then ph.velocity.y =   -10.0 end
        if is_key_down(util.KeyboardKey.SPACE) then ph.velocity.y =   10.0 end
    end


    cm.target   = cm.target + t.position - cm.position
    cm.position = t.position
    handle_mouse_delta(cm, Vector2.new(
                get_mouse_delta().x * 0.05,
                get_mouse_delta().y * 0.05)
            , true)

    local forward = get_forward(cm.target, t.position) * 3.0
    local right   = get_right(cm.target, t.position, cm.up) * 3.0


    local f = nil
    if is_key_down(util.KeyboardKey.W) then f = 1 else  f = 0 end
    if is_key_down(util.KeyboardKey.S) then f = f - 1 end
    local r = nil
    if is_key_down(util.KeyboardKey.D) then r = 1 else  r = 0 end
    if is_key_down(util.KeyboardKey.A) then r = r - 1 end

    local move = (forward * f) + (right * r)
    move.y = 0.0
    ph.velocity = ph.velocity + (move * speed)
end
