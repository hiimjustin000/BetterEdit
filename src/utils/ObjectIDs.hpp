#pragma once

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class ObjectID {
private:
    int m_id;

public:
    ObjectID(int id) : m_id(id) {}

    ccColor3B getTriggerColor() const {
        switch (m_id) {
            case MOVE_TRIGGER: case AREA_MOVE_TRIGGER: case EDIT_AREA_MOVE_TRIGGER:
                return ccc3(255, 0, 255);

            case ROTATE_TRIGGER: case AREA_ROTATE_TRIGGER: case EDIT_AREA_ROTATE_TRIGGER:
                return ccc3(127, 127, 255);
            
            case SCALE_TRIGGER: case AREA_SCALE_TRIGGER: case EDIT_AREA_SCALE_TRIGGER:
                return ccc3(63, 191, 255);
            
            case PULSE_TRIGGER: case AREA_TINT_TRIGGER: case EDIT_AREA_TINT_TRIGGER:
                return ccc3(255, 255, 0);
            
            case ALPHA_TRIGGER: case AREA_FADE_TRIGGER: case EDIT_AREA_FADE_TRIGGER:
                return ccc3(0, 255, 255);
            
            case TOGGLE_TRIGGER: return ccc3(255, 63, 63);
            case SPAWN_TRIGGER:  return ccc3(35, 204, 127);
            case FOLLOW_TRIGGER: return ccc3(255, 127, 127);
            
            case ADVANCED_FOLLOW_TRIGGER: case EDIT_ADVANCED_FOLLOW_TRIGGER: case RE_TARGET_ADVANCED_FOLLOW_TRIGGER:
                return ccc3(204, 255, 199);
            
            case ANIMATE_TRIGGER:           return ccc3(255, 183, 0);
            case FOLLOW_PLAYER_Y_TRIGGER:   return ccc3(255, 255, 127);
            case CHANGE_BG_TRIGGER:         return ccc3(127, 255, 255);
            case CHANGE_GROUND_TRIGGER:     return ccc3(255, 255, 127);
            case CHANGE_MIDGROUND_TRIGGER:  return ccc3(255, 127, 255);
            case TOUCH_TRIGGER:             return ccc3(0, 137, 178);
            case COUNT_TRIGGER:             return ccc3(255, 183, 252);
            case INSTANT_COUNT_TRIGGER:     return ccc3(255, 135, 255);
            case PICKUP_TRIGGER:            return ccc3(255, 109, 0);
            case RANDOM_TRIGGER:            return ccc3(63, 127, 255);
            case STOP_TRIGGER:              return ccc3(163, 0, 86);

            case ADVANCED_RANDOM_TRIGGER: case SEQUENCE_TRIGGER:
                return ccc3(165, 94, 255);

            case SPAWN_PARTICLE_TRIGGER: return ccc3(150, 150, 150);
            case RESET_TRIGGER:          return ccc3(255, 100, 0);
            case CAMERA_ZOOM_TRIGGER:    return ccc3(127, 127, 255);
            case CAMERA_STATIC_TRIGGER:  return ccc3(255, 127, 127);
            case CAMERA_ROTATE_TRIGGER:  return ccc3(255, 245, 0);
            case CAMERA_EDGE_TRIGGER:    return ccc3(0, 199, 255);
            case CAMERA_MODE_TRIGGER:    return ccc3(191, 255, 0);
            case COLLISION_TRIGGER:      return ccc3(83, 66, 250);
            case TIME_WARP_TRIGGER:      return ccc3(153, 127, 153);
            case BG_SPEED_TRIGGER:       return ccc3(127, 255, 255);
            case ON_DEATH_TRIGGER:       return ccc3(204, 101, 101);
            case PLAYER_CONTROL_TRIGGER: return ccc3(101, 255, 255);

            case MIDGROUND_SETUP_TRIGGER: case MIDGROUND_SPEED_TRIGGER:
                return ccc3(255, 127, 255);

            case CAMERA_OFFSET_TRIGGER: case CAMERA_GP_OFFSET_TRIGGER:
                return ccc3(127, 255, 127);
            
            default:
                return ccWHITE;
        }
    }

    static constexpr int MOVE_TRIGGER = 901;
    static constexpr int STOP_TRIGGER = 1616;
    static constexpr int PULSE_TRIGGER = 1006;
    static constexpr int ALPHA_TRIGGER = 1007;
    static constexpr int TOGGLE_TRIGGER = 1049;
    static constexpr int SPAWN_TRIGGER = 1268;
    static constexpr int ROTATE_TRIGGER = 1346;
    static constexpr int SCALE_TRIGGER = 2067;
    static constexpr int FOLLOW_TRIGGER = 1347;
    static constexpr int ANIMATE_TRIGGER = 1585;
    static constexpr int FOLLOW_PLAYER_Y_TRIGGER = 1814;
    static constexpr int ADVANCED_FOLLOW_TRIGGER = 3016;
    static constexpr int EDIT_ADVANCED_FOLLOW_TRIGGER = 3660;
    static constexpr int RE_TARGET_ADVANCED_FOLLOW_TRIGGER = 3661;
    static constexpr int AREA_MOVE_TRIGGER = 3006;
    static constexpr int AREA_ROTATE_TRIGGER = 3007;
    static constexpr int AREA_SCALE_TRIGGER = 3008;
    static constexpr int AREA_FADE_TRIGGER = 3009;
    static constexpr int AREA_TINT_TRIGGER = 3010;
    static constexpr int EDIT_AREA_MOVE_TRIGGER = 3011;
    static constexpr int EDIT_AREA_ROTATE_TRIGGER = 3012;
    static constexpr int EDIT_AREA_SCALE_TRIGGER = 3013;
    static constexpr int EDIT_AREA_FADE_TRIGGER = 3014;
    static constexpr int EDIT_AREA_TINT_TRIGGER = 3015;
    static constexpr int CHANGE_BG_TRIGGER = 3029;
    static constexpr int CHANGE_GROUND_TRIGGER = 3030;
    static constexpr int CHANGE_MIDGROUND_TRIGGER = 3031;
    static constexpr int TOUCH_TRIGGER = 1595;
    static constexpr int COUNT_TRIGGER = 1611;
    static constexpr int INSTANT_COUNT_TRIGGER = 1811;
    static constexpr int PICKUP_TRIGGER = 1817;
    static constexpr int RANDOM_TRIGGER = 1912;
    static constexpr int ADVANCED_RANDOM_TRIGGER = 2068;
    static constexpr int SEQUENCE_TRIGGER = 3607;
    static constexpr int SPAWN_PARTICLE_TRIGGER = 3608;
    static constexpr int RESET_TRIGGER = 3618;
    static constexpr int CAMERA_ZOOM_TRIGGER = 1913;
    static constexpr int CAMERA_STATIC_TRIGGER = 1914;
    static constexpr int CAMERA_OFFSET_TRIGGER = 1916;
    static constexpr int CAMERA_GP_OFFSET_TRIGGER = 2901;
    static constexpr int CAMERA_ROTATE_TRIGGER = 2015;
    static constexpr int CAMERA_EDGE_TRIGGER = 2062;
    static constexpr int CAMERA_MODE_TRIGGER = 2925;
    static constexpr int COLLISION_TRIGGER = 1815;
    static constexpr int TIME_WARP_TRIGGER = 1935;
    static constexpr int MIDGROUND_SETUP_TRIGGER = 2999;
    static constexpr int BG_SPEED_TRIGGER = 3606;
    static constexpr int MIDGROUND_SPEED_TRIGGER = 3612;
    static constexpr int INSTANT_COLLISION_TRIGGER = 3609;
    static constexpr int ON_DEATH_TRIGGER = 1812;
    static constexpr int PLAYER_CONTROL_TRIGGER = 1932;
};
