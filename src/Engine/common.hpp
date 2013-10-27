#pragma once

#include <GL/glew.h>

// SFGUI
#include <SFGUI/SFGUI.hpp> // must be included before SFML/Graphics.hpp

// SFML & OpenGL
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// GLM library
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) // nameless struct/union warning.
#endif
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#ifdef _MSC_VER
#pragma warning(pop) 
#endif

// assimp library
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// ImageMagick


// STD stuff
#include <cstdint>
#include <cassert>
#include <exception>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <string>
#include <map>
#include <array>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>

// engine commonly used
#include "../Utility/System/Duration.hpp"

// debug stuff
#include "../Utility/dbg.hpp"
#include "../Utility/utility.hpp"

namespace KE
{
	typedef sf::Event SystemEvent;
	typedef sf::Window RenderDevice;
	typedef sf::Window InputDevice;
	//typedef sf::RenderWindow RenderDevice;
	
namespace Keyboard
{
	typedef sf::Keyboard::Key		Key;
}
}
