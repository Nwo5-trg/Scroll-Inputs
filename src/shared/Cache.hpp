#pragma once

namespace Cache {
    #ifdef GEODE_IS_ARM_MAC
        inline int dispatchScrollMSGAddress = 0x4b0cb4;
    #endif
    #ifdef GEODE_IS_INTEL_MAC
        inline int dispatchScrollMSGAddress = 0x55a490;
    #endif

    inline bool scrollInputsLayerCreated = false;

    inline std::vector<CCTextInputNode*> inputNodes;
}