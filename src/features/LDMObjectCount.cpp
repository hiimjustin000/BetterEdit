#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/GameObject.hpp>

using namespace geode::prelude;

class $modify(EditorPauseLayer) {
    bool init(LevelEditorLayer* lel) {
        if (!EditorPauseLayer::init(lel))
            return false;

        if (auto label = static_cast<CCLabelBMFont*>(this->querySelector("object-count-label"))) {
            size_t ldmCount = 0;
            for (auto obj : CCArrayExt<GameObject*>(lel->m_objects)) {
                ldmCount += obj->m_isHighDetail;
            }
            label->setString(fmt::format(
                "{} | LDM: {} ({}%)",
                label->getString(), ldmCount,
                lel->m_objects->count() ? static_cast<int>(100.f * ldmCount / lel->m_objects->count()) : 0
            ).c_str());
        }
        
        return true;
    }
};
