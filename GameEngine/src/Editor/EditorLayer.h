#pragma once
#include "GameEngine.h"

namespace GameEngine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetatch() override;
		void onUpdate(Timestep) override;
	};
}