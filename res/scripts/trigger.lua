function inter(e,o)
   if get_tag(scene, o).tag == "Player" then
       scene.manager:set_current("light")
       return 1
   end

    return 0
end

