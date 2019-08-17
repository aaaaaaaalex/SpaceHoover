# SpaceHoover
SDL / C++ half-finished game - collect all the stars to win.


## General Concept

This is an "isometric" 2-D game where your player attracts stars, and takes damage from asteroids.
The concept was inspired by the print of a commonly-sold T-shirt.

## Design

All physical objects seen on-screen are essentially entity classes, which are derived from the WorldObject base class.
The World class holds many WorldObject pointers (WorldObjects are stored in the heap), and also holds a single Player class.
The Player class is also derived from WorldObject, and a unique Player pointer is held in World only for convenience.

The physics_Visitor class holds the physics-related behaviour for each derivation of WorldObject, and is held by World.
Every "tick" of time, World will make the physics_Visitor visit() each WorldObject, and through method overloading, the correct
behaviour will be executed for each WorldObject derivation.

The render_Visitor class functions in the same manner as physics_Visitor - but it contains all the rendering-related behaviours
for each WorldObject derivation. The render-visitor also decides which textures to draw for each WorldObject, and how to animate them.

The input_Visitor class probably shouldn't be a visitor. It is passed Player, and will manipulate Player according to keyboard input.

Level is a base class, which is derived by LevelOne - the idea being that a collection of Level objects could be held, all with different
setups and routines - much like WorldObjects.
World accepts setup from a Level, by passing itself to the Level - the level will then add WorldObjects to World.
The Level, once set up, is told to start, and will initiate a game-loop - advancing World's physics, rendering, etc, and also
checking the status of WorldObjects in order to determine if the level has been completed or failed. 
The game-loop will then return an enum called LevelBreakType, which can be used to describe
the context in-which the level was exited. LevelBreakType values are: COMPLETE, FAIL, PAUSE, SAVE_AND_QUIT, NONE.

SFX_Module provides a way for classes to make noises, without including the entire SDL_Mixer library.
Using an enum called CustomChunkType, classes such as physics_Visitor will queue noises in the SFX_Module, awaiting playback.
The playback queue has a small size, since a large queue could result in hundreds of sounds being played simultaneously.
At the end of a "tick" of time, the SFX_Module is asked to process its queue of sounds, and plays them on different channels.
