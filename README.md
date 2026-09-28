# Nanus, a miniature (attempt at a) game engine
Nanus is an attempt to create a game engine "contract", one which backend developers implement in order to provide support for any platform they desire. Once this is done, the higher "layers" of the engine provide the remainder of the logic and code usually necessary to build a somewhat complex game. This includes rendering logic, entity-component systems, networking, resource management, scripting, etc. The platform itself provides the platform-specific "facilities", and then Nanus is able to build off of that, supplying the rest, and allowing for a platform-independent game engine.

Because of this structure, Nanus intentionally doesn't link to the C or C++ standard libraries, in order to keep in compatibility with as many platforms as it can, though it does rely on standard C library headers for. In fact, Nanus' main library has zero dependencies on its own, only the ones brought in by the platform implementing it. This means everything it may need is either a platform-implemented function, or is brought in by itself, such as a vector library, various useful algorithms, as well as more complex algorithms.

The name "nanus" comes from the Latin word, which means a dwarf or a markedly small person. Cheekily, I borrowed this word to refer to the intended small and miniature nature of the "engine" I sought out to create.

## The code
The following repository holds the [headers](./include) for Nanus, the [implementation](./src) of the game engine logic, as well as a desktop implementation based on OpenGL 3.3 and SDL2, meant to serve as an example of how implementation could be done. I am also attempting to work on a Wii U implementation, though progress is ever-slow.

Additionally, you may look into the [sandbox](./sandbox) directory, which showcases the use of the desktop implementation of the engine in a simple C++ project. 

Lastly, I have created a variety of tools that can be used for project development on your standard desktop operating systems. This includes:

- A tool which can export a large number of 3D file formats into a singular, Nanus native file format (powered by Assimp)
- A tool which can preview assets in Nanus' environment, make changes to them, etc, before exporting them to Nanus' file format.
- A tool that 

