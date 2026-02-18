---@meta


---@section BuiltIns
--- These functions are called automatically by Dou

--- Gets called upon creation of the entity
function on_init() end

--- Gets called every frame.
--- @param dt number The delta time since the last frame in seconds
function on_update(dt) end

--- Called once when the entity is destroyed or the scene stops.
function on_end() end
---@sectionback

---@type Scene 
---The active Scene pointer injected by the engine.
scene = nil

---@type UUID
---The UUID of the specific Entity currently executing this script.
this = nil

--#region Engine Types

--#region Entity

---Entity handler class
---@class Entity
Entity = {}

--- Creates a null/empty Entity
--- @return Entity
function Entity:new() end

--- Creates an Entity from a Scene pointer and an EnTT handle
--- @param scene Scene Parent scene
--- @param id integer A registered EnTT entity ID number
--- @return Entity
function Entity:new(scene, id) end

--- Returns the unique persistent identifier (UUID) for this entity
--- @return UUID
function Entity:uuid() end

--- Returns the underlying EnTT entity handle
--- @return integer
function Entity:id() end

--- Returns a pointer to the Scene that owns this entity
--- @return Scene
function Entity:scene_ptr() end

--- Checks if this entity has children assigned to it
--- @return boolean
function Entity:is_parent() end

--- Checks if this entity is a child of another entity
--- @return boolean
function Entity:is_child() end

--- Retrieves a list of child UUIDs.
--- @note IMPORTANT: You must call 'is_parent()' to check if this entity is a parent before calling this.
--- @return UUID[] A vector of child UUIDs
function Entity:get_children() end

--#endregion

--#region SceneManager

---@class SceneManager
SceneManager = {}
--- Sets the current scene
---@param name string Registered scene name we want to switch to 
---@return nil
function SceneManager:set_current(name) end

--- Set the app_should_close flag
---@param flag boolean
---@return nil
function SceneManager:set_should_close(flag) end

--- Returns the app_should_close flag
--- @return boolean
function SceneManager:set_should_close() end
--#endregion

--#region Scene

---@class Scene
---@field manager SceneManager
Scene = {}

--- Returns the registerd EnTT ID counterpart to a given UUID
--- @param uuid UUID
--- @return integer
function Scene:uuid_to_entt(uuid) end

--- Returns the registerd shader object with the given name
--- @param name string Name of the registered shader
--- @return Shader
function Scene:get_shader(name) end

--- Returns the registerd mesh object with the given name
--- @param name string Name of the registered mesh
--- @return Mesh
function Scene:get_mesh(name) end
--#endregion

---@class Shader
---@field program integer OpenGL shader program ID
---@field path string Registered asset path
 Shader = {}

 --- Default constructor for Shader
 --- @return Shader
 function Shader:new() end

 --- @param program integer OpenGL shader program ID
 --- @param path string Registered asset path
 --- @return Shader
 function Shader:new(program, path) end

---@class Mesh
---@field VAO integer
---@field VBO integer
---@field EBO integer 0 by default
---@field nvertices integer Number of vertices
---@field nindices  integer Number of indices, 0 by default
---@field name string Registered asset name
---@field vao_format integer Bitwise ID of VAO format
 Mesh = {}

 --- Returns true if the vertex data contain texture coords
 --- @return boolean
 function Mesh:textured() end
 
 --- Returns true if the vertex data contain nomal coords
 --- @return boolean
 function Mesh:normals() end

---@class Texture
---@field texture integer OpenGL texture ID
---@field path string Registered asset path
---@field w integer
---@field h integer
---@field nrc integer
 Texture = {}

--#region Material
---@class Material
---@field shader Shader
---@field texture Texture
---@field ambient vec3
---@field diffuse vec3 
---@field specular vec3
---@field shininess number
---@field attributes integer
Material = {}

--- Prints out debug info about material
function Material:print() end


---@class MaterialBuilder
MaterialBuilder = {}
---@param shader Shader
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_shader(shader) end

---@param texture Texture
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_texture(texture) end

---@param ambient vec3
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_ambient(ambient) end

---@param diffuse vec3
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_diffuse(diffuse) end

---@param specular vec3
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_specular(specular) end

---@param shininess number
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_shininess(shininess) end

---@param attributes integer
---@return MaterialBuilder returns MaterialBuilder reference
function MaterialBuilder:set_attributes(attributes) end

---Builds and return the Material object
---@return Material
function MaterialBuilder:build() end

--#endregion

--#endregion

--#region 3rd Party and math
---@class vec2
---@field x number
---@field y number
 vec2 = {}

 --- Creates a new vec2 object
 --- @param x number
 --- @param y number
function vec2:new(x,y)end


---@class vec3
---@field x number
---@field y number
---@field z number
 vec3 = {}
 --- Creates a new vec3 object
 --- @param x number
 --- @param y number
 --- @param z number
function vec3:new(x,y,z)end

---@class vec4
---@field x number
---@field y number
---@field z number
---@field w number
 vec4 = {}
 --- Creates a new vec4 object
 --- @param x number
 --- @param y number
 --- @param z number
 --- @param w number
function vec4:new(x,y,z,w)end
--#endregion

--#region Component checkers

--- Returns true if the entity has a UUID component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_uuid(scene, entity) end

--- Returns true if the entity has a Tag component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_tag(scene, entity) end

--- Returns true if the entity has a Transform component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_transform(scene, entity) end

--- Returns true if the entity has a Model component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_modelcomp(scene, entity) end

--- Returns true if the entity has a Action component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_action(scene, entity) end

--- Returns true if the entity has a PhysicsBody component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_physicbody(scene, entity) end

--- Returns true if the entity has a Camera component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_camera(scene, entity) end

--- Returns true if the entity has a LuaScript component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_script(scene, entity) end

--- Returns true if the entity has a DirLight component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_dirlight(scene, entity) end

--- Returns true if the entity has a PointLight component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_pointlight(scene, entity) end

--- Returns true if the entity has a SpotLight component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_spotlight(scene, entity) end

--- Returns true if the entity has a Childer component
--- @param scene Scene The parent scene
--- @param entity UUID Entity to check
--- @return boolean
function has_children(scene, entity) end

---Returns true if the entity has a Parent component
---@param scene Scene The parent scene
---@param entity UUID Entity to check
---@return boolean
function has_parent(scene, entity) end

--#endregion

--#region Component getters

--Returns the UUID component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return UUID
function get_uuid(scene, entity) end

---Returns the Tag component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return Tag
function get_tag(scene, entity) end

---Returns the Transform component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return Transform
function get_transform(scene, entity) end

---Returns the Model component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return Model
function get_modelcomp(scene, entity) end

---Returns the Action/Animation component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return Action
function get_action(scene, entity) end

---Returns the Physics Body component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return PhysicsBody
function get_physicsbody(scene, entity) end

---Returns the Camera component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return Camera
function get_camera(scene, entity) end

---Returns the Script component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return Script
function get_script(scene, entity) end

---Returns the Directional Light component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return DirLight
function get_dirlight(scene, entity) end

---Returns the Point Light component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return PointLight
function get_pointlight(scene, entity) end

---Returns the Spotlight component.
---Note: Ensure the entity has this component before calling.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the component from
---@return SpotLight
function get_spotlight(scene, entity) end

---Returns a list of all child entities attached to this entity.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the children from
---@return UUID[] children An array of UUIDs
function get_children(scene, entity) end

---Returns the parent entity of the given entity.
---@param scene Scene The parent scene
---@param entity UUID The entity to retrieve the parent from
---@return UUID parent The UUID of the parent, or nil if root
function get_parent(scene, entity) end
--#endregion


--#region Component Types
---@class UUID
---@field uuid integer The UUID number
UUID = {}


---@class Tag
---@field tag string

--#region Transform
---@class Transform
Transform = {}
function Transform:position() end
function Transform:size() end
function Transform:rotation() end

---@param new_position vec3
function Transform:set_position(new_position) end
---@param new_size vec3
function Transform:set_size(new_size) end
---@param new_rotation vec3
function Transform:set_rotation(new_rotation) end

---@param delta vec3
function Transform:translate(delta) end
---@param delta number
function Transform:translate_x(delta) end
---@param delta number
function Transform:translate_y(delta) end
---@param delta number
function Transform:translate_z(delta) end

---@param delta vec3
function Transform:scale(delta) end
---@param delta number
function Transform:scale_x(delta) end
---@param delta number
function Transform:scale_y(delta) end
---@param delta number
function Transform:scale_z(delta) end

---@param delta vec3
function Transform:rotate(delta) end
---@param delta number
function Transform:rotate_x(delta) end
---@param delta number
function Transform:rotate_y(delta) end
---@param delta number
function Transform:rotate_z(delta) end

---@class TransformBuilder
TransformBuilder = {}

--- Creates an TransformBuilder object
--- @return TransformBuilder
function TransformBuilder:new() end

---@param value vec3
---@return TransformBuilder returns TransformBuilder reference
function TransformBuilder:position(value) end

---@param value vec3
---@return TransformBuilder returns TransformBuilder reference
function TransformBuilder:size(value) end

---@param value vec3
---@return TransformBuilder returns TransformBuilder reference
function TransformBuilder:rotation(value) end

---Finalizes the Transform object construction and returns it
---@return Transform
function TransformBuilder:build() end
--#endregion



--#region Model
---@class Model
---@field mesh Mesh
---@field material Material
---@field layer integer
Model = {}

 --- Default constructor for Model
 --- @return Model
 function Model:new() end

 --- @param mesh Mesh
 --- @param material Material
 --- @param layer integer
 --- @return Model
 function Model:new(mesh, material, layer) end

--#endregion


--#region PhysicsBody
---@class PhysicsBody
---@field gravity number
---@field velocity vec3
---@field slipperiness number
---@field mass number
---@field is_solid boolean
---@field is_static boolean
---@field move_delta vec3
PhysicsBody = {}

---Default constructor for PhysicsBody
---@return PhysicsBody
function PhysicsBody:new() end


---@class PhysicsBodyBuilder
PhysicsBodyBuilder = {}

--- Creates a PhysicsBodyBuilder object
--- @return PhysicsBodyBuilder
function PhysicsBodyBuilder:new() end

--- @param value number
--- @return PhysicsBodyBuilder returns PhysicsBodyBuilder reference
function PhysicsBodyBuilder:gravity(value) end

--- @param value vec3
--- @return PhysicsBodyBuilder returns PhysicsBodyBuilder reference
function PhysicsBodyBuilder:velocity(value) end

--- @param value number
--- @return PhysicsBodyBuilder returns PhysicsBodyBuilder reference
function PhysicsBodyBuilder:mass(value) end

--- @param value number
--- @return PhysicsBodyBuilder returns PhysicsBodyBuilder reference
function PhysicsBodyBuilder:slipperiness(value) end

--- @param value boolean
--- @return PhysicsBodyBuilder returns PhysicsBodyBuilder reference
function PhysicsBodyBuilder:is_solid(value) end

--- @param value boolean
--- @return PhysicsBodyBuilder returns PhysicsBodyBuilder reference
function PhysicsBodyBuilder:is_static(value) end

--- Finalizes the PhysicsBody object construction and returns it
--- @return PhysicsBody
function PhysicsBodyBuilder:build() end

--#endregion
--#region Camera
---@enum CameraProjection
CameraProjection = {
    Perspective = 0,
    Orthographic = 1,
}
---@class Camera
---@field up vec3 The up vector of the camera (usually 0, 1, 0)
---@field fovy number Field of view in the y-direction (degrees)
---@field max_distance number Render limit distance
---@field projection CameraProjection The current projection mode (e.g., Perspective/Orthographic)
local Camera = {}

--- Returns the current pitch (up/down rotation)
--- @return number
function Camera:pitch() end

--- Returns the current yaw (left/right rotation)
--- @return number
function Camera:yaw() end

--- Sets the camera pitch with an optional constraint
--- @param pitch number The new pitch value
--- @param max? number The maximum rotation limit (default 180.0)
--- @return number The actual pitch set
function Camera:set_pitch(pitch, max) end

--- Sets the camera yaw with an optional constraint
--- @param yaw number The new yaw value
--- @param max? number The maximum rotation limit (default 360.0)
--- @return number The actual yaw set
function Camera:set_yaw(yaw, max) end

--- Prints camera debug information to the console
function Camera:log() end


--#endregion

---@class Action
---@class Script
---@class PointLight
---@class SpotLight

--#region DirLight
---@class DirLight
---@field direction vec3 Default (-0.2, -1.0, -0.3)
---@field color vec3 Default (1,1,1)
---@field strength number Default 0.5
---@field ambient number Default 0.1
DirLight = {}

--- Default constructor
--- @return DirLight
function DirLight:new() end

--#endregion

--#region PntLight
---@class PntLight
---@field color vec3 Default (1,1,1)
---@field constant number Default 1.0
---@field linear number Default 0.05
---@field quadratic number Default 0.021
PntLight = {}

--- Default constructor
--- @return PntLight
function PntLight:new() end

--#endregion

--#region SptLight
---@class SptLight
---@field color vec3 Default (1,1,1)
---@field constant number Default 1.0
---@field linear number Default 0.05
---@field quadratic number Default 0.021
---@field direction vec3 Default (0,-1,0)
---@field cutoff number Default 12.5 degrees
---@field outer_cutoff number Default 17.5 degrees
SptLight = {}

--- Default constructor
--- @return SptLight
function SptLight:new() end

--#endregion



--#region Hierarchy
---@class Parent
---@field parent Entity The reference to the parent entity
Parent = {}


---------------------------------------

---@class Children
---@field children UUID[] A list (table) of child UUIDs
Children = {}
--#endregion


--#endregion

--#region Rendering

--- Returns glfwGetTime value
--- @return number
function get_time() end

--- Returns the mouse movement delta 
--- @return vec2
function get_mouse_delta() end

--- Updates a camera object according to a mouse movement delta
---@param camera Camera Camera to update
---@param mouse_delta vec2 Movement delta
---@param lock boolean Lock camera pitch at +-90 degrees
---@return nil
function handle_mouse_delta(camera, mouse_delta, lock) end

--- Returns a camera's direction vector
--- @param camera Camera
--- @return vec3
function get_camera_forward(camera) end

--- Returns a camera's direction vector on a flat plane disregarding its pitch
--- @param camera Camera
--- @return vec3
function get_camera_flat_forward(camera) end

--- Returns a camera's right vector
--- @param camera Camera
--- @return vec3
function get_camera_right(camera) end

--- Returns a camera's direction vector
--- @param camera Camera
--- @return vec3
function get_camera_dir(camera) end


--- Sets wireframe mode flag for a given layer
--- @param scene Scene Parent scene
--- @param layer integer Layer ID (0-15 by default)
--- @param flag  boolean Wireframe mode flag
function set_layer_wireframe(scene, layer, flag) end

--#endregion

--#region Shaders
--- Returns the normalized value of a given vec3
--- @param value vec3
--- @return vec3
function v3_normalize(value) end

--- Sets a float uniform in a given shader
--- @param shader Shader Shader object
--- @param name string Name of the uniform to be set
--- @param value number Value
--- @return nil
function set_shader_f(shader, name, value) end

--- Sets a vec2 uniform in a given shader
--- @param shader Shader Shader object
--- @param name string Name of the uniform to be set
--- @param value vec2 Value
--- @return nil
function set_shader_v2(shader, name, value) end

--- Sets a vec3 uniform in a given shader
--- @param shader Shader Shader object
--- @param name string Name of the uniform to be set
--- @param value vec3 Value
--- @return nil
function set_shader_v3(shader, name, value) end

--- Sets a vec4 uniform in a given shader
--- @param shader Shader Shader object
--- @param name string Name of the uniform to be set
--- @param value vec4 Value
--- @return nil
function set_shader_v4(shader, name, value) end
--#endregion


--#region Input
--- @param key integer Number from util.Keyboardkey
--- @return boolean
function is_key_down(key) end

--- @param key integer Number from util.Keyboardkey
--- @return boolean
function is_key_released(key) end

--- @param key integer Number from util.Keyboardkey
--- @return boolean
function is_key_clicked(key) end

--- Wrapper for glfwSetInputMode
--- @param scene Scene Parent scene
--- @param subject integer Number from util.InputSbj
--- @param mode integer Number from util.InputMode
--- @return nil
function set_input_mode(scene, subject, mode) end

--#endregion


--#region Logging
--- log_trace("msg {}", ...)
--- @param message string
function log_trace(message, ...) end

--- log_info("msg {}", ...)
--- @param message string
function log_info(message, ...) end

--- log_warn("msg {}", ...)
--- @param message string
function log_warn(message, ...) end

--- log_error("msg {}", ...)
--- @param message string
function log_error(message, ...) end

--- log_critical("msg {}", ...)
--- @param message string
function log_critical(message, ...) end
--#endregion


--#region Hierarchy

---Establishes a parent-child relationship between two entities.
---@param owner Scene The scene context where these entities exist.
---@param child UUID The entity that will be moved under the parent.
---@param parent UUID The entity that will become the new parent.
function make_child_of(owner, child, parent) end

---Removes a specific child from its parent entity.
---@param owner Scene The scene context where these entities exist.
---@param parent UUID The parent entity to remove the child from.
---@param child UUID The specific child entity to detach.
function remove_child(owner, parent, child) end

---Detaches all children currently attached to the specified parent.
---@param owner Scene The scene context.
---@param parent UUID The entity whose transform hierarchy will be cleared.
function remove_children(owner, parent) end

---Checks if the given entities are a part of the same hierarachy tree and returns an integer representing the relation.
---@param owner Scene The scene context.
---@param a UUID The first entity to compare.
---@param b UUID The second entity to compare.
---
--- Return value guide:
--- 1: 'a' is an ancestor of 'b'
----1: 'b' is an ancestor of 'a'
--- 0: both are not related a
---@return integer
function get_entities_relation(owner, a, b) end

---Checks if entity 'a' exists higher up in the hierarchy of entity 'b'.
---@param owner Scene The scene context.
---@param a UUID The potential ancestor (parent, grandparent, etc).
---@param b UUID The potential descendant.
---@return boolean 
function is_ancestor_of(owner, a, b) end
--#endregion


