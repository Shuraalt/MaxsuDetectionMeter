#include "MeterSettings.h"
#include "DataHandler.h"
#include "Renderer.h"

namespace MaxsuDetectionMeter
{
	static constexpr auto ConfigPath = "Data\\SKSE\\Plugins\\MaxsuDetectionMeter.ini";

	MeterSettings::MeterSettings()
	{
		LoadValues();

		PrintSettingValue(fadeSpeed);
		PrintSettingValue(flashSpeed);
		PrintSettingValue(flashScale);
		PrintSettingValue(flashShift);
		PrintSettingValue(minFillingSpeed);
		PrintSettingValue(maxFillingSpeed);
		PrintSettingValue(minTriggerLevel);
		PrintSettingValue(radiusX);
		PrintSettingValue(radiusY);
		PrintSettingValue(centerOffsetX);
		PrintSettingValue(centerOffsetY);

		ScalingForResolution();
	}

	void MeterSettings::LoadValues()
	{
		constexpr double inf = 1.0e9;
		enableDebugLog.Load(ConfigPath, false, false, true);
		fadeSpeed.Load(ConfigPath, 255.0, -inf, inf);
		flashSpeed.Load(ConfigPath, 550.0, -inf, inf);
		flashScale.Load(ConfigPath, 0.25, -inf, inf);
		flashShift.Load(ConfigPath, 36, -1000000, 1000000);
		minFillingSpeed.Load(ConfigPath, 0.35, 0.0, 1.0);
		maxFillingSpeed.Load(ConfigPath, 0.95, 0.0, 1.0);
		minTriggerLevel.Load(ConfigPath, 25, 0, 100);
		radiusX.Load(ConfigPath, 310.0, -inf, inf);
		radiusY.Load(ConfigPath, 310.0, -inf, inf);
		centerOffsetX.Load(ConfigPath, 0, -1000000, 1000000);
		centerOffsetY.Load(ConfigPath, -40, -1000000, 1000000);
	}

	void MeterSettings::ScalingForResolution()
	{
		float widthScale = Renderer::GetResolutionScaleWidth();
		*radiusX *= widthScale;
		*centerOffsetX *= widthScale;

		float heightScale = Renderer::GetResolutionScaleHeight();
		*radiusY *= heightScale;
		*centerOffsetY *= heightScale;
		*flashShift *= heightScale;
	}

	void MeterSettings::Reload()
	{
		LoadValues();
		ScalingForResolution();
		DEBUG("Reload Configuration Settings!");
	}

	bool SettingsHandler::Register()
	{
		static SettingsHandler singleton;
		auto eventSource = SKSE::GetModCallbackEventSource();
		if (!eventSource) {
			ERROR("EventSource not found!");
			return false;
		}

		eventSource->AddEventSink(&singleton);

		INFO("Register {}", typeid(singleton).name());

		return true;
	}

	EventResult SettingsHandler::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource)
	{
		if (a_event && _strcmpi(a_event->eventName.c_str(), "dmenu_updateSettings") == 0 && _strcmpi(a_event->strArg.c_str(), modName) == 0) {
			MeterHandler::GetSingleton()->meterSettings->Reload();
		}

		return EventResult::kContinue;
	}

}
