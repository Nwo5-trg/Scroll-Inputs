#pragma once
#include "Enums.hpp"

namespace Utils {
    std::pair<InputType, int> isProllyNumber(const std::string& string);

    CCTextInputNode* getTextInputForPos(cocos2d::CCPoint pos, cocos2d::CCNode* scene);
    void getTextInputsRecursive(cocos2d::CCNode* node);

    void tryStepInput(CCTextInputNode* input, bool down);

    float getStep(InputType type);

    bool isModifierDown(ModifierType modifier);
    
    std::string formatFloat(float num, int rounding, bool volume);

    cocos2d::CCNode* getFocusedLayer(cocos2d::CCNode* scene);
}