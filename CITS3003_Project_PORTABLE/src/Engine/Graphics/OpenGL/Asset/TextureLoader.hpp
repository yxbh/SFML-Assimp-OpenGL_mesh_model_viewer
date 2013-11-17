#include "../../../common.hpp"
#include <IL/il.h>
#include <IL/ilu.h>		// might not need this at all.
//#include <IL/ilut.h>	// might not need this at all.


namespace KG
{
	class Texture;
	typedef std::shared_ptr<Texture>	Texture_SmartPtr;
	typedef std::weak_ptr<Texture>		Texture_WeakPtr;


	/*! \class TextureLoader

		TODO : Turn this into a singleton class. Problem with doing this now is shutting down.
				The entire engine requires a proper shut down system. Things would just fail randomly
				when the OpenGL context is destoryed too fast during multi-thread mode.
	*/
	class TextureLoader
	{
	private:

	public:
		TextureLoader(void);
		~TextureLoader(void);

		static void Init(void);

		KG::Texture_SmartPtr LoadFromFile(const std::string & p_Path);

	}; // class

	typedef std::shared_ptr<KG::TextureLoader>	TextureLoader_SmartPtr;

	extern const bool check_for_DevIL_error(void);
} // KG ns
