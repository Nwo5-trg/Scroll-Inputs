#pragma once

class ScrollInputsLayer : public cocos2d::CCLayer {
private:
    unsigned int m_ticksSinceScroll;
    unsigned int m_ticksSinceVolumeModifier;
    float m_scrollingDistance;
    cocos2d::CCLabelBMFont* m_volumeLabel;
    cocos2d::extension::CCScale9Sprite* m_volumeBackground;
    CCTextInputNode* m_input;
    EffectGameObject* m_trigger;

    void update(float dt);
    
    void showVolume();
    void hideVolume();
    void onHideVolume();
    
public:
    static ScrollInputsLayer* s_instance;

    static ScrollInputsLayer* get();
    static ScrollInputsLayer* create();
    bool init();
    
    void scroll(float x, float y);
};