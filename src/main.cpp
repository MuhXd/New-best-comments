#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
int per =0;
int Best = 0;
auto levelname = std::string("PlaceHolder");
void uploadNewBest() {
   auto manager = GameLevelManager::sharedState();
   if (Best > 0) {
    if (Best >= 100) {
              manager->uploadAccountComment(fmt::format("I Beat {}",levelname));
    }
    else {
              manager->uploadAccountComment(fmt::format("Got new best on {} ({}%)",levelname,Best));
    }
   };
   levelname = "PlaceHolder";
   Best = 0;
   per= 0; 
}

int convertRobTopLevelToAssetKey(int lvlID) {
	switch (lvlID) {
		case 1:
			return 2; // Stereo Madness
		case 2:
			return 2; // Back On Track
		case 3:
			return 3; // Polargeist
		case 4:
			return 3; // Dry Out
		case 5:
			return 3; // Base After Base
		case 6:
			return 4; // Can't Let Go
		case 7:
			return 4; // Jumper
		case 8:
			return 5; // Time Machine
		case 9:
			return 5; // Cycles
		case 10:
			return 5; // xStep
		case 11:
			return 5; // Clutterfunk
		case 12:
			return 5; // Theory of Everything
		case 13:
			return 5; // Electroman Adventures
		case 14:
			return 13; // Clubstep
		case 15:
			return 9; // Electrodynamix
		case 16:
			return 9; // Hexagon Force
		case 17:
			return 4; // Blast Processing
		case 18:
			return 13; // TOE 2
		case 19:
			return 5; // Geometrical Dominator
		case 20:
			return 13; // Deadlocked
		case 21:
			return 9; // Fingerdash
		case 22:
			return 9; // Dash
		case 23:
			return 13; // Explorers
		case 3001:
			return 5; // The Challenge
		// Spinoffs
		// Meltdown
		case 1001:
			return 2; // The Seven Seas
		case 1002:
			return 3; // Viking Arena
		case 1003:
			return 4; // Airborne Robots
		// GD World
		case 2001:
			return 2; // Payload
		case 2002:
			return 2; // Beast Mode
		case 2003:
			return 3; // Machina
		case 2004:
			return 3; // Years
		case 2005:
			return 3; // Frontlines
		case 2006:
			return 3; // Space Pirates
		case 2007:
			return 3; // Striker
		case 2008:
			return 3; // Embers
		case 2009:
			return 3; // Round 1
		case 2010:
			return 3; // Monster Dance Off
		case 4001:
			return 3; // Press Start
		case 4002:
			return 4; // Nock Em
		case 4003:
			return 6; // Power Trip
	}
	return 0;
};
int convertGJDifficultyToAssetKey(int difficulty) {
	switch (difficulty) {
		case -1:
			return 1;
		case 0:
			return 0;
		case static_cast<int>(GJDifficulty::Easy):
			return 2;
		case static_cast<int>(GJDifficulty::Normal):
			return 3;
		case static_cast<int>(GJDifficulty::Hard):
			return 4;
		case static_cast<int>(GJDifficulty::Harder):
			return 5;
		case static_cast<int>(GJDifficulty::Insane):
			return 7;
	}
	return 0;
}
auto getAverageDifficulty(GJGameLevel* level) {
	return level->m_ratings / level->m_ratingsSum;
}

int convertGJDifficultyDemonToAssetKey(int difficulty) {
	switch (difficulty) {
		case 3:
			return 13;
		case 4:
			return 14;
		case 0:
			return 15;
		case 5:
			return 16;
		case 6:
			return 17;
	}
	return 0;
}
int getratingsnumbs(GJGameLevel* level) {
    int stars = level->m_stars.value();
	auto difficulty = level->getAverageDifficulty();
    if (level->m_levelID.value() < 128 || level->m_levelID.value() == 3001) {
		return convertRobTopLevelToAssetKey(level->m_levelID.value());
	}
    if (stars == 0) {
		return convertGJDifficultyToAssetKey(difficulty);
	}
	if (stars == 10) {
		return convertGJDifficultyDemonToAssetKey(level->m_demonDifficulty);
	}
    return 0;
};

bool dif(GJGameLevel* level) {
    if ( getratingsnumbs(level) >= Mod::get()->getSettingValue<int64_t>("difficulty")) {
        return true;
    };
        return false;
};

void setvalue(GJGameLevel* level, int overrightper) {
    int persentlook = overrightper;
    if (level==nullptr) {
        log::debug("Caught Crash: Level: {} \n Per : {}",level,per);
        return; 
    };
	if (Best < persentlook) {
        if (dif(level)) {
            Best = persentlook;
        }
	};
}
class $modify(PlayerObject){
	void playerDestroyed(bool p0){

		PlayerObject::playerDestroyed(p0);
		setvalue(PlayLayer::get()->m_level,per);
		if (Mod::get()->getSettingValue<bool>("Death")) {
            uploadNewBest();
        };
	}
};
class $modify(PlayLayer) {
	void levelComplete() {
		PlayLayer::levelComplete();
		setvalue(PlayLayer::get()->m_level,100);
	}
	void onExit() {
		uploadNewBest();
        PlayLayer::onExit();
	}
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
            if (!m_level->isPlatformer()) {
                if ( m_isPracticeMode) {
                    return;
                }
               
                float percent = static_cast<float>(PlayLayer::getCurrentPercentInt());
                levelname = std::string(m_level->m_levelName);
            
                if (percent > (m_level->m_normalPercent)) {
                    per = percent;
                } 
            }
	}

};
