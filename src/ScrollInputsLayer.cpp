#include "ScrollInputsLayer.hpp"
#include "utils/Utils.hpp"
#include "shared/Settings.hpp"

using namespace geode::prelude;

ScrollInputsLayer* ScrollInputsLayer::s_instance = nullptr;

ScrollInputsLayer* ScrollInputsLayer::get() {
    if (!s_instance) {
        s_instance = ScrollInputsLayer::create();
    }
    return s_instance;
}

ScrollInputsLayer* ScrollInputsLayer::create() {
    auto ret = new ScrollInputsLayer;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

bool ScrollInputsLayer::init() {
    if (!CCLayer::init()) return false;
    m_scrollingDown = false;
    m_ticksSinceScroll = 0;
    m_ticksSinceVolumeModifier = 0;
    m_scrollingDistance = 0.0f;
    m_input = nullptr;
    m_trigger = nullptr;

    setContentSize({0.0f, 0.0f});
    setPosition(0.0f, 0.0f);
    setID("scroll-detector-9000"_spr);

    scheduleUpdate();

    auto pos = ccp(CCDirector::sharedDirector()->getWinSize().width - 15, 15.0f);

    m_volumeBackground = CCScale9Sprite::create("square02_001.png");
    m_volumeBackground->setContentSize({70.0f, 30.0f});
    m_volumeBackground->setAnchorPoint({1.0f, 0.0f});
    m_volumeBackground->setPosition(pos);
    m_volumeBackground->setOpacity(100);
    m_volumeBackground->setVisible(false);
    m_volumeBackground->setID("volume-background"_spr);
    this->addChild(m_volumeBackground);

    m_volumeLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_volumeLabel->setPosition(pos + ccp(-35.0f, 15.0f));
    m_volumeLabel->setAlignment(kCCTextAlignmentRight);
    m_volumeLabel->setVisible(false);
    m_volumeLabel->setID("volume-label"_spr);
    this->addChild(m_volumeLabel);

    return true;
}

void ScrollInputsLayer::update(float dt) {
    bool shouldDisableMouseFallthrough = false;

    auto mousePos = cocos::getMousePos();

    if (m_ticksSinceScroll++ == Settings::scrollSensitivity) {
        m_scrollingDistance = 0;
    } 

    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (Settings::scrollInputEnabled && scene->getChildrenCount() != 0) {
        m_input = Utils::getTextInputForPos(mousePos, Utils::getFocusedLayer(scene));
        shouldDisableMouseFallthrough = m_input;
    } else {
        m_input = nullptr;
    }

    // ============================================================================================
    bool volumeDown = Utils::isModifierDown(Settings::volumeModifier);

    if (Settings::volumeScrollEnabled && volumeDown) {
        m_ticksSinceVolumeModifier = 0;
        shouldDisableMouseFallthrough = true;
    } else m_ticksSinceVolumeModifier++;

    if (m_volumeBackground->isVisible()) {
        if (m_ticksSinceVolumeModifier == 50) hideVolume();

        if (Utils::isModifierDown(Settings::volumeSFXModifier)) {
            m_volumeLabel->setString(
                (Utils::formatFloat(FMODAudioEngine::get()->getEffectsVolume() * 100, 2, true)
                + " SFX").c_str()
            );
        }
        else {
            m_volumeLabel->setString(
                (Utils::formatFloat(FMODAudioEngine::get()->getBackgroundMusicVolume() * 100, 2, true)
                + " Music").c_str()
            );
        }
    }

    // ============================================================================================

    auto* editor = LevelEditorLayer::get();
    if (Settings::groupScrollEnabled && editor && !volumeDown && Utils::isModifierDown(Settings::groupModifier)) {
        auto obj = editor->objectAtPosition(editor->m_objectLayer->convertToNodeSpace(mousePos));
        m_trigger = obj && obj->isTrigger() ? static_cast<EffectGameObject*>(obj) : nullptr;
        if (m_trigger) shouldDisableMouseFallthrough = true;
    }
    else {
        m_trigger = nullptr;
    }

    setMouseEnabled(shouldDisableMouseFallthrough);
}

void ScrollInputsLayer::scroll(float x, float y) {
    m_ticksSinceScroll = 0;

    float dt = fabsf(x) > fabsf(y) ? x : y;
    bool down = 0 > dt;
    if ((m_scrollingDistance += fabsf(dt)) < Settings::scrollDistance && m_scrollingDown == down) return;

    m_scrollingDown = down;
    m_scrollingDistance = 0.0f;

    bool textInputChanged = false;

    if (Settings::scrollInputEnabled && m_input) {
        auto string = m_input->getString();
        auto type = Utils::isProllyNumber(string);

        if (type.first != InputType::String) {       
            float num = std::strtof(string.c_str(), nullptr) 
                + (Utils::getStep(type.first) * (down ? -1 : 1));

            m_input->setString(
                Settings::trimPointZero ? Utils::formatFloat(num, type.second, false) : numToString(num, type.second)
            );

            if (m_input->m_delegate) m_input->m_delegate->textChanged(m_input);
            textInputChanged = true;
        }
    }

    // =========================================================================================================

    bool volumeDown = Utils::isModifierDown(Settings::volumeModifier);

    if (Settings::volumeScrollEnabled && !textInputChanged && volumeDown) {
        if (!m_volumeBackground->isVisible()) showVolume();

        auto engine = FMODAudioEngine::get();
        bool sfx = Utils::isModifierDown(Settings::volumeSFXModifier);
        float volume = (sfx ? engine->getEffectsVolume()
            : engine->getBackgroundMusicVolume()) * 100;

        if (Settings::preciseLowVolumes) {
            volume += volume < 10.00005f 
                ? (down ? -0.1f : 0.1f) : Settings::volumeStep * (down ? -1 : 1);
        }
        else {
            volume += Settings::volumeStep * (down ? -1 : 1);
        }

        if (sfx) engine->setEffectsVolume(std::clamp(volume, 0.0f, 100.0f) / 100);
        else engine->setBackgroundMusicVolume(std::clamp(volume, 0.0f, 100.0f) / 100);
    }

    if (!textInputChanged && !volumeDown && m_trigger && m_trigger->m_objectLabel) {
        int num = std::strtol(m_trigger->m_objectLabel->getString(), nullptr, 10) 
                + (Utils::getStep(InputType::Int) * (down ? -1 : 1));

        if (m_trigger->m_objectID == 1817) m_trigger->m_itemID = std::clamp(num, 0, 9999);
        else m_trigger->setTargetID(std::clamp(num, 0, 9999));
        
        m_trigger->m_objectLabel->setString(numToString(num).c_str());
    }
}

void ScrollInputsLayer::showVolume() {
    m_volumeBackground->stopAllActions();
    m_volumeBackground->setScale(0.0f);
    m_volumeBackground->setVisible(true);
    m_volumeBackground->runAction(
        CCEaseExponentialOut::create(CCScaleTo::create(0.2f, 1.0f))
    );
    m_volumeLabel->stopAllActions();
    m_volumeLabel->setOpacity(0);
    m_volumeLabel->setVisible(true);
    m_volumeLabel->runAction(
        CCSequence::create(CCDelayTime::create(0.1f), CCFadeTo::create(0.2f, 255), nullptr) 
    );
}
void ScrollInputsLayer::hideVolume() {
    m_volumeBackground->runAction(
        CCSequence::create(
            CCDelayTime::create(0.2f), CCEaseExponentialOut::create(CCScaleTo::create(0.2f, 0.0f)), 
            CCCallFunc::create(this, callfunc_selector(ScrollInputsLayer::onHideVolume)), nullptr) 
    );
    m_volumeLabel->runAction(
       CCFadeTo::create(0.2f, 0)
    );
}
void ScrollInputsLayer::onHideVolume() {
    m_volumeBackground->setVisible(false);
    m_volumeLabel->setVisible(false);
}