using namespace geode::prelude;

void updateDifficultySprite(GJDifficultySprite* sprite, GJGameLevel* level) {
    if (level->m_stars.value() == 0 && level->m_demon.value() == 0 && level->m_featured == 0 && level->m_isEpic == 0 && level->m_starsRequested > 0) {
        auto difficulty = 0;
        auto update = false;
        switch (level->m_starsRequested) {
            case 1: // Auto
                difficulty = -1;
                update = Mod::get()->getSettingValue<bool>("enable-auto");
                break;
            case 2: // Easy
                difficulty = 1;
                update = Mod::get()->getSettingValue<bool>("enable-easy");
                break;
            case 3: // Normal
                difficulty = 2;
                update = Mod::get()->getSettingValue<bool>("enable-normal");
                break;
            case 4: case 5: // Hard
                difficulty = 3;
                update = Mod::get()->getSettingValue<bool>("enable-hard");
                break;
            case 6: case 7: // Harder
                difficulty = 4;
                update = Mod::get()->getSettingValue<bool>("enable-harder");
                break;
            case 8: case 9: // Insane
                difficulty = 5;
                update = Mod::get()->getSettingValue<bool>("enable-insane");
                break;
            case 10: // Demon
                difficulty = 6;
                update = Mod::get()->getSettingValue<bool>("enable-demon");
                break;
            default: // NA
                difficulty = 0;
                update = Mod::get()->getSettingValue<bool>("na-override");
                break;
        }

        if (update) sprite->updateDifficultyFrame(difficulty, GJDifficultyName::Short);
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
