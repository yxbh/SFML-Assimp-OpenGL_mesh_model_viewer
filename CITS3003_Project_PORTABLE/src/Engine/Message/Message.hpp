#pragma once
#include "../common.hpp"
#include "../Entity/EntityID.hpp"
#include "../Entity/EntityComponentID.hpp"

namespace KE
{
	/*! \class Message
		Similar to SFML's Event class. Message class is used for system and component communication.
	*/
	class Message
	{
	public :

		struct TestMessage
		{
			int val;
		};

		struct SizeMessage
		{
			unsigned int origin_x;
			unsigned int origin_y;
			unsigned int width;  
			unsigned int height; 
		};

		struct KeyMessage
		{
			sf::Keyboard::Key	code;
			bool				alt;
			bool				control;
			bool				shift;
			bool				system;
		};

		struct MouseMessage
		{
			int delta_x, delta_y;
			int x, y;
		};

		struct EntityMessage
		{
			KE::EntityID			entity_id;
			KE::EntityComponentID	entity_component_id;
		};

		enum Type
		{    
			Null,Test,
			Resized,

			KeyPressed,
			KeyReleased,
			MouseMoved,

			EntityDestoryed,

			Count
		};

		// Member data
		Type type; 

		union
		{
			TestMessage			test;
			SizeMessage         size;
			KeyMessage			key;
			MouseMessage		mouse;
			EntityMessage		entity;
		};

	public:
		Message(void);
		/*! Translate a sf::Event into KE::Message. */
		Message(const KE::SystemEvent & p_rEvent);
	};

} // KE ns
