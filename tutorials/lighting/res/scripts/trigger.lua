
-- Keep in mind that e and o are UUIDs rather than Entity
function inter(e,o)
    if not has_tag(scene, o) then return 0 end

    log_critical("HOW DARE YOU {}", 
        get_tag(scene,o).tag) -- scene is already set by Dou
    return 0
end
