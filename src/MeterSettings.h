#pragma once

namespace MaxsuDetectionMeter
{
	// Small ini-backed setting, replaces DKUtil's config types.
	template <class T>
	class Setting
	{
	public:
		Setting(const char* a_key, const char* a_section) :
			_key(a_key), _section(a_section) {}

		const char* get_key() const { return _key; }
		T get_data() const { return _data; }
		T& operator*() { return _data; }

		void Load(const char* a_path, T a_default, T a_min, T a_max)
		{
			char buf[64]{};
			GetPrivateProfileStringA(_section, _key, "", buf, sizeof(buf), a_path);
			T value = a_default;
			if (buf[0] != '\0') {
				if constexpr (std::is_same_v<T, bool>) {
					std::string str(buf);
					std::transform(str.begin(), str.end(), str.begin(), ::tolower);
					if (str == "true" || str == "1")
						value = true;
					else if (str == "false" || str == "0")
						value = false;
				} else if constexpr (std::is_floating_point_v<T>) {
					char* end = nullptr;
					const double parsed = std::strtod(buf, &end);
					if (end != buf)
						value = static_cast<T>(parsed);
				} else {
					char* end = nullptr;
					const long long parsed = std::strtoll(buf, &end, 10);
					if (end != buf)
						value = static_cast<T>(parsed);
				}
			}
			if constexpr (!std::is_same_v<T, bool>) {
				value = std::clamp(value, a_min, a_max);
			}
			_data = value;
		}

	private:
		const char* _key;
		const char* _section;
		T _data{};
	};

	using Boolean = Setting<bool>;
	using Double = Setting<double>;
	using Integer = Setting<std::int32_t>;

	struct MeterSettings
	{
	private:
		template <class T>
		static void PrintSettingValue(const T& a_setting)
		{
			INFO("Setting:\"{}\" is {}"sv, a_setting.get_key(), a_setting.get_data());
		}

		void ScalingForResolution();
		void LoadValues();

	public:
		MeterSettings();

		void Reload();

		Boolean enableDebugLog{ "EnableDebugLog", "Main" };

		Double fadeSpeed{ "FadeSpeed", "AnimatedEffect" };

		Double flashSpeed = { "FlashSpeed", "AnimatedEffect" };
		Double flashScale = { "FlashScale", "AnimatedEffect" };
		Integer flashShift = { "FlashShift", "AnimatedEffect" };

		Double minFillingSpeed = { "MinFillingSpeed", "AnimatedEffect" };
		Double maxFillingSpeed = { "MaxFillingSpeed", "AnimatedEffect" };

		Integer minTriggerLevel = { "MinTriggerLevel", "Display" };

		Double radiusX = { "RadiusX", "Display" };
		Double radiusY = { "RadiusY", "Display" };

		Integer centerOffsetX = { "CenterOffsetX", "Display" };
		Integer centerOffsetY = { "CenterOffsetY", "Display" };
	};

	using EventResult = RE::BSEventNotifyControl;

	class SettingsHandler : public RE::BSTEventSink<SKSE::ModCallbackEvent>
	{
		static constexpr char modName[] = "MaxsuDetectionMeter";

	public:
		static bool Register();

		virtual EventResult ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource);

	private:
		SettingsHandler() = default;
	};

}
