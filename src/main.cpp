#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include "shared/Cache.hpp"
#include "ScrollInputsLayer.hpp"

using namespace geode::prelude;

class $modify(MenuLayerHook, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (!Cache::scrollInputsLayerCreated) {
            auto ScrollInputsLayer = ScrollInputsLayer::get();
            CCDirector::sharedDirector()->getRunningScene()->addChild(ScrollInputsLayer);
            SceneManager::get()->keepAcrossScenes(ScrollInputsLayer);
            Cache::scrollInputsLayerCreated = true;
        }

        return true;
    }
};

class $modify(CCMouseDispatcher) {
    bool dispatchScrollMSG(float x, float y) {
        ScrollInputsLayer::get()->scroll(x, y);
        return CCMouseDispatcher::dispatchScrollMSG(x, y);
    }
};