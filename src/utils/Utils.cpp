#include "Utils.hpp"
#include "../shared/Settings.hpp"
#include "../shared/Cache.hpp"

using namespace geode::prelude;

namespace Utils {
    std::pair<InputType, int> isProllyNumber(const std::string& string) {
        if (string.empty()) return {Settings::emptyInput, 0};
        
        bool hasDigit = false;
        bool decimal = false;
        int rounding = 0;

        for (int i = 0; i < string.size(); i++) {
            char c = string[i];

            if (std::isdigit(c)) {
                hasDigit = true;
                if (decimal) rounding++;
            }
            else if (c == '.') {
                if (decimal) return {InputType::String, 0};
                decimal = true;
            }
            else if (c == '-') {
                if (i != 0) return {InputType::String, 0};
            }
            else return {InputType::String, 0};
        }

        if (!hasDigit) return {InputType::String, 0};
        if (decimal) return {InputType::Float, rounding};
        return {InputType::Int, 0};
    }

    CCTextInputNode* getTextInputForPos(CCPoint pos, CCNode* scene) {
        Cache::inputNodes.clear();

        getTextInputsRecursive(scene);

        for (auto input : Cache::inputNodes) {
            auto rect = input->boundingBox();
            rect.origin -= rect.size / 2;
            if (rect.containsPoint(input->getParent()->convertToNodeSpace(pos))) return input;
        }

        return nullptr;
    }
    void getTextInputsRecursive(cocos2d::CCNode* node) {
        for (auto child : CCArrayExt<CCNode>(node->getChildren())) {
            if (!child->isVisible()) continue;

            if (auto input = typeinfo_cast<CCTextInputNode*>(child)) {
                Cache::inputNodes.push_back(input);
            }
            else {
                getTextInputsRecursive(child);
            }
        }
    }

    float getStep(InputType type) {
        if (Utils::isModifierDown(Settings::smallModifier)) {
            return type == InputType::Float ? Settings::smallStep : Settings::smallIntStep;
        }
        else if (Utils::isModifierDown(Settings::bigModifier)) {
            return type == InputType::Float ? Settings::bigStep : Settings::bigIntStep;
        }
        else {
            return type == InputType::Float ? Settings::step : Settings::intStep;
        }
    }

    bool isModifierDown(ModifierType modifier) {
        switch (modifier) {
            case ModifierType::Alt: {
                return CCKeyboardDispatcher::get()->getAltKeyPressed();
            }
            case ModifierType::Control: {
                return CCKeyboardDispatcher::get()->getControlKeyPressed();
            }
            case ModifierType::Shift: {
                return CCKeyboardDispatcher::get()->getShiftKeyPressed();
            }
            case ModifierType::Command: {
                return CCKeyboardDispatcher::get()->getCommandKeyPressed();
            }
            case ModifierType::None: {
                return false;
            }
        }
    }

    std::string formatFloat(float num, int rounding, bool volume) {
        if (volume && (num >= 10.00005f || !Settings::preciseLowVolumes)) num = roundf(num);

        auto str = numToString(num, rounding);
        
        if (str.find('.') != std::string::npos) {
            auto end = str.find_last_not_of('0');
            if (end != std::string::npos) str.erase(end + 1);
            if (!str.empty() && str.ends_with('.')) str.pop_back();
        }

        return str;
    }

    CCNode* getFocusedLayer(CCNode* scene) {
        auto nodes = scene->getChildren();
        auto highestNode = static_cast<CCNode*>(nodes->objectAtIndex(0));

        CCNode* alert = nullptr;
        for (auto child : CCArrayExt<CCNode>(nodes)) {
            if (auto alertChild = typeinfo_cast<FLAlertLayer*>(child)) alert = child;
        }
        
        return alert ? alert : highestNode;
    }
}