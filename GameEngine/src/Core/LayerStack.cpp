#include "GameEngine_PCH.h"
#include "LayerStack.h"

namespace GameEngine
{
	LayerStack::~LayerStack()
	{
		for (auto* l : layers)
		{
			delete l;
		}
	}

	void LayerStack::push(Layer* l)
	{
		layers.emplace(layers.begin() + insertIndex, l);
		insertIndex++;
	}

	void LayerStack::pop(Layer* l)
	{
		auto it = std::find(layers.begin(), layers.end(), l);
		if (it != layers.end())
		{
			l->onDetatch();
			layers.erase(it);
			insertIndex--;
		}
	}

	void LayerStack::pushOverlay(Layer* l)
	{
		layers.emplace_back(l);
		insertIndex++;
	}

	void LayerStack::popOverlay(Layer* l)
	{
		auto it = std::find(layers.begin(), layers.end(), l);
		if (it != layers.end())
		{
			l->onDetatch();
			layers.erase(it);
		}
	}
}