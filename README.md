SFML-OpenGL_mesh_viewer
=======================

A simple mesh model viewer written as part of a Graphics and Animations unit offered in UWA.
The project was developed in 2 parts over the course of roughly 6 months (1 semester). Rendering is done through a 
simple forward render pipeline. OpenGL 3.1 > was used.

This project also served as a testbed some of the engine components that I've developed for my 2D game engine prototype.


Features:
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
  
  
Libraries Used:
  - SFML
  - Assimp
  - GLEW


Known Isues:
  - Meshes do not transform correctly if there's more than 1 mesh.
    At this stage, this app is only useful as a single mesh model viewer.
  - Surafce normal and winding order might be incorrect for .x files.
  - Camera::SetRoll() method does not work correct and therefore it's behaviour is undefined.
    If anyone figured out how to implement it correctly, please do let me know!


Contributors:
  - Ben Huang (me): entire framework and 99% of the codebase.
  - John Fielding : GUI and his math and engineering knowledge.
  
  
Credits:
  - The underlying framework was loosely modeled the GCC game from the book "Game Coding Complete 4th ed"
    by Mike McShaffry & David "Rez" Graham.

Development:
  Development on the viewer has been paused (as of 18/11/2013) for now as I'm moving back onto my 2D game engine project.
  Feel free to ask if there're any questions though. I do plan to come back to this some time in the future.
  
  
License:
  Feel free to use the code however you like, but you have to give credit where credit is due.
  I do not offer warranty on the code and it is not my responsibility for any possible injure
  and/or harm that might occur from the use of anything from this repo.
