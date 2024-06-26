#include <Geode/Geode.hpp>

using namespace geode::prelude;

void updateDifficultySprite(GJDifficultySprite* sprite, GJGameLevel* level) {
    if (level->m_stars.value() == 0 && level->m_starsRequested > 0) switch (level->m_starsRequested) {
        case 1: sprite->updateDifficultyFrame(-1, GJDifficultyName::Short); break;
        case 2: sprite->updateDifficultyFrame(1, GJDifficultyName::Short); break;
        case 3: sprite->updateDifficultyFrame(2, GJDifficultyName::Short); break;
        case 4: case 5: sprite->updateDifficultyFrame(3, GJDifficultyName::Short); break;
        case 6: case 7: sprite->updateDifficultyFrame(4, GJDifficultyName::Short); break;
        case 8: case 9: sprite->updateDifficultyFrame(5, GJDifficultyName::Short); break;
        case 10: sprite->updateDifficultyFrame(6, GJDifficultyName::Short); break;
        default:
            if (Mod::get()->getSettingValue<bool>("na-override")) sprite->updateDifficultyFrame(0, GJDifficultyName::Short);
            break;
    }
}

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(RRLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        updateDifficultySprite(m_difficultySprite, level);

        return true;
    }

    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);

        updateDifficultySprite(m_difficultySprite, level);
    }

    void levelUpdateFinished(GJGameLevel* level, UpdateResponse response) {
        LevelInfoLayer::levelUpdateFinished(level, response);

        updateDifficultySprite(m_difficultySprite, level);
    }

    void likedItem(LikeItemType type, int id, bool liked) {
        LevelInfoLayer::likedItem(type, id, liked);

        updateDifficultySprite(m_difficultySprite, m_level);
    }
};

#include <Geode/modify/LevelCell.hpp>
class $modify(RRLevelCell, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        if (auto difficultyContainer = m_mainLayer->getChildByID("difficulty-container"))
            updateDifficultySprite(static_cast<GJDifficultySprite*>(difficultyContainer->getChildByID("difficulty-sprite")), level);
    }
};
