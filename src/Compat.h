#pragma once

// Minimal replacements for the parts of DKUtil this plugin used.
#ifdef ERROR
#	undef ERROR
#endif
#ifdef DEBUG
#	undef DEBUG
#endif

#define INFO(...) SKSE::log::info(__VA_ARGS__)
#define DEBUG(...) SKSE::log::debug(__VA_ARGS__)
#define ERROR(...) SKSE::log::error(__VA_ARGS__)

namespace MaxsuDetectionMeter
{
	template <class T>
	class Singleton
	{
	public:
		static T* GetSingleton()
		{
			static T singleton;
			return std::addressof(singleton);
		}

	protected:
		Singleton() = default;
		~Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	};
}
