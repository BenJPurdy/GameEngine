#pragma once

#include <memory>


#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define CORE_ASSERT(x, ...) {if (!(x)) { LOG_ERROR("CoreAssert failed {0}", __VA_ARGS__); __debugbreak();}}


namespace GameEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>...);
	}
}
