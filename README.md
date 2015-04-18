# SFML Assimp OpenGL Mesh Viewer
###### UWA 2013 CITS3003 Project (Graphics and Animation)

A simple mesh model viewer written as part of a Graphics and Animations unit offered in UWA.
The project was developed in 2 parts over the course of roughly 6 months (1 semester). Rendering is done through a 
simple forward render pipeline. OpenGL 3.1 > was used.

This project also served as a testbed some of the engine components that I've developed for my 2D game engine prototype.


## Features
  - Assimp for model import. (opens many model file formats)
  - first and third person camera movement
  - Blinn-Phong lighting. (adjustable material ambient/diffuse/specular)
  - Skeletal animation (up to 4 bones per vertex).
  - Quaternion transforms (for rotation). This implementation should be gimbal-lock free.
  - Texture swapping.
  - Simple object transform controls (translation, rotation & scale)
  - Scene graph.
  - Event system using std::function as delegates.
  - Component based Entity model. (though wasn't fully utilised in the project).


## Libraries Used
  - SFML
  - Assimp
  - GLEW


## Known Isues
  - Meshes do not transform correctly if there's more than 1 mesh.
    At this stage, this app is only useful as a single mesh model viewer.
  - Surafce normal and winding order might be incorrect for .x files.
  - Camera::SetRoll() method does not work correct and therefore it's behaviour is undefined.
    If anyone figured out how to implement it correctly, please do let me know!
  - Crashes caused by the GUI and SFGUI library.


## How-To
  Under root directrory there are 3 following folders:
    - "CITS3003_Project_PORTABLE"
    - "doc"
    - "src"
  The portable folder contains a VS2012 solution and the necessary binary and folder structure for both a DEBUG and RELEASE build. Build the application using the solution file and then copy your models and textures into the "model-textures" folder inside your binary folder. There is a ModelList.txt file and a TextureList.txt file. Put the list of model and texture file names into the respective text file. Execute the program and it should run.
  
  NOTE: The portable folder actually has it's own version of the source codes for the project. At this point of time I simply cannot remember which one's the most up to date. You can play around with it at your leisure. Just beware there might be extra or missing source files.


## Contributors
  - Benjamin YanXiang Huang: archictect/developer for engine, framework & viewer.
  - John Fielding : GUI implementation, maths and engineering expertise.


## Credits
  - The underlying framework was loosely based on the GCC game from the book "Game Coding Complete 4th ed"
    by Mike McShaffry & David "Rez" Graham.

## Development
  Development on the viewer has been paused (as of 18/11/2013) for now as I'm moving back onto my 2D game engine project.
  Feel free to ask if there're any questions though. I do plan to come back to this some time in the future.
  
  
## License
  Feel free to use the code however you like, but you have to give credit where credit is due.
  I do not offer warranty on the code and it is not my responsibility for any possible injure
  and/or harm that might occur from the use of anything from this repo.
