
function inter(p, e,o)
    tag = get_tag(scene, e)
    if tag.tag == "Player" then 

        log_warn("{}", tag.tag)
        scene.manager:set_current("light")
        return 1
    else
        log_warn("{}", tag.tag)
    end
    return 0
end

