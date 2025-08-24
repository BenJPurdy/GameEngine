#pragma once

#include "Core/Layer.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace GameEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetatch() override;
		virtual void onEvent(Event&) override;

		void begin();
		void end();

		void blockEvents(bool b) { block = b; }
	private:
		bool block = true;
		float time = 0.0f;
	};
}