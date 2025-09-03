#include "Settings.hpp"

using namespace geode::prelude;

namespace Settings {
    void updateSettings() {
        auto mod = Mod::get();

        reverseScroll = mod->getSettingValue<bool>("reverse-scroll");
        preciseLowVolumes = mod->getSettingValue<bool>("precise-low-volumes");
        scrollInputEnabled = mod->getSettingValue<bool>("scroll-input-enabled");
        volumeScrollEnabled = mod->getSettingValue<bool>("volume-scroll-enabled");
        trimPointZero = mod->getSettingValue<bool>("trim-point-zero");

        rounding = mod->getSettingValue<int64_t>("rounding");
        scrollSensitivity = mod->getSettingValue<int64_t>("scroll-sensitivity");
        volumePopupHideDelay = mod->getSettingValue<int64_t>("volume-popup-hide-delay");

        emptyInput = stringToInputType[mod->getSettingValue<std::string>("empty-input")];
        smallModifier = stringToModifierType[mod->getSettingValue<std::string>("small-modifier")];
        bigModifier = stringToModifierType[mod->getSettingValue<std::string>("big-modifier")];
        volumeModifier = stringToModifierType[mod->getSettingValue<std::string>("volume-modifier")];
        volumeSFXModifier = stringToModifierType[mod->getSettingValue<std::string>("volume-sfx-modifier")];

        step = mod->getSettingValue<double>("step");
        intStep = mod->getSettingValue<double>("int-step");
        smallStep = mod->getSettingValue<double>("small-step");
        smallIntStep = mod->getSettingValue<double>("small-int-step");
        bigStep = mod->getSettingValue<double>("big-step");
        bigIntStep = mod->getSettingValue<double>("big-int-step");
        scrollDistance = mod->getSettingValue<double>("scroll-distance");
        volumeStep = mod->getSettingValue<double>("volume-step");
    }
}

$on_mod(Loaded) {
    Settings::updateSettings();

    listenForAllSettingChangesV3([] (std::shared_ptr<geode::SettingV3>) {
        Settings::updateSettings();
    });
}