# DirectX11 Game Engine is a project that I started to learn DirectX11 and graphics programming
# and to hopefully create a functional enough engine that I can create
# a physics engine in the future.

Explanation of the base layout of the engine:

The main point wherein you will control the components of the engine is the 
Game class. This class is responsible for the creation of the window class and
the actual game logic.
The window class creates the windows and handles the messages that are sent to it.
Inside the game class is the game loop. In this loop you must begin the frame,
and end the frame when appropriate. 

The Renderer:



The main render class is the Graphics class. This class is responsible for creating
the d3d device and swap chain.
The graphics class also contains the Camera object.

A bindable is a class that represents a DirectX11 object that can be bound to the
pipeline. For example, a vertex shader is a bindable. The bindable class is an abstract
class that all bindables inherit from. The bindable class contains a pure virtual function
called Bind(). This function is called when the bindable is bound to the pipeline.

In order to not duplicate bindables when they can be shared, the class BindableCodex is used.
This class is a singleton that contains a map of bindables. When a bindable is requested,
the bindable codex will check if the bindable already exists. If it does, it will return
the bindable. If it does not, it will create the bindable and add it to the map.

