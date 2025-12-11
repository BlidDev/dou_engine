# Dou Engine - Core

Dou Engine is a small and relatively complex DoD 3D game engine written in C++.
This engine started as a small 2 day personal project and pretty quickly grown into a relatively complex (and still in fact, personal) one. 

This is the core engine library, there's also [Whisk - a GUI based editor](https://github.com/BlidDev/Whisk) for Dou. Check it out!


#### _Disclaimer_
_DouEngine was created purely for learning/fun purpose and by **no means** was meant not recommend to be used as an actual engine for your games, have fun using it for anything else though!_

[add cool screenshot idk]
<br>

---

## List of main features
- ### Base:
    - Scene based workflow (easily interchangeable)
    - ECS based (using EnTT)
    - Entity hierarchy 
    - Support for both native and Lua scripts
    - Efficient and cached resource management
    - Read and write project and scene custom YAML files
- ### Rendering
    - Camera post processing support
    - Minimal (but functional) lighting calculations
    - Layer based rendering
    - Custom model file
- ### Physics
    - AABB collision system
    - Complex on-intersect behaviour using either Lua or C++
    - Basic physics calculation system



<br>
<hr style="border:2px solid darkgray">
<br>


## Known bugs/ incomplete features
- #### Physics:
    - Rotations are purely for show, they don't take affect in the collision checking, AABB means the collider is fixed to the object's size with no rotation.
    - The IsStatic flag for PhysicsBody doesn't do anything (YET!) so leave it on ```true``` in order to avoid unexpected behaviour.
    - 2 Solid moving objects colliding can cause unexpected results.
    - **As you can probably can tell from the previous bugs the AABB system isn't quite finished; It's on top priority but is really basic in this version.**

- #### Rendering
    - While the lighting system does work **shadows are not implemented yet** unfortunately, meaning enclosed areas are still affected by outside lights for example.

- #### Other
    - The Lua support is pretty rich but sometimes a C++ function will miss a Lua binding, so let me know if you encounter a missing function!

<br>
<hr style="border:2px solid darkgray">
<br>

## Building From Source

In your terminal run the following lines:

```bash
git clone https://github.com/BlidDev/dou_engine
cd dou_engine
git submodule update --init
mkdir build && cd build

# Adjust for your liking
cmake .. -DCMAKE_BUILD_TYPE=Debug/Release -G Ninja/... -DEXAMPLE_GAME=On/Off
ninja # or make
cd ../
```

The engine files can be found in the `bin` directory along with the compiled example game if you enabled the option.


<br>
<hr style="border:2px solid darkgray">
<br>

## Making Your First Project
Check out the [First Project](engine/include/engine.h) wiki page in order to create and run you first project in Dou!


<br>
<hr style="border:2px solid darkgray">
<br>

## Documentation 
You can find everything over on the [Dou wiki](engine/include/epch.h). If you encounter something that's not documented let me know and I'll add a wiki page as soon a possible.



<br>
<br>
<br>

---
Have Fun! :D
