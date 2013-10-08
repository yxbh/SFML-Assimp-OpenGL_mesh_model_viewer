#pragma once

namespace KG
{
	enum class RenderPass
	{
		Null,			// uninitialized state.
		Dynamic,		// dynamic render/drawable object. (things that move)
		Static,			// static render/drawable object
		Sky,
		NotRendered,
		Size			// size of enum array, for for-loop.
	};

}
