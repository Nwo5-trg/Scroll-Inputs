#pragma once
#include "../utils/Enums.hpp"

namespace Settings {
    inline bool reverseScroll;
    inline bool preciseLowVolumes;
    inline bool scrollInputEnabled;
    inline bool volumeScrollEnabled;
    inline bool trimPointZero;

    inline int rounding;
    inline int scrollSensitivity;
    inline int volumePopupHideDelay;

    inline InputType emptyInput;
    inline ModifierType smallModifier;
    inline ModifierType bigModifier;
    inline ModifierType volumeModifier;
    inline ModifierType volumeSFXModifier;

    inline float step;
    inline float intStep;
    inline float smallStep;
    inline float smallIntStep;
    inline float bigStep;
    inline float bigIntStep;
    inline float scrollDistance;
    inline float volumeStep;

    inline std::unordered_map<std::string, InputType> stringToInputType {
        {"None", InputType::String}, {"Int", InputType::Int}, {"Float", InputType::Float}
    };

    inline std::unordered_map<std::string, ModifierType> stringToModifierType {
        {"Alt", ModifierType::Alt}, {"Control", ModifierType::Control}, 
        {"Shift", ModifierType::Shift}, {"Command", ModifierType::Command},
        {"None", ModifierType::None}
    };

    void updateSettings();
}