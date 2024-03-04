#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include <Geode/ui/Notification.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/PauseLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
int per =0;
int Best = 0;
GJGameLevel* m_levelb = nullptr;
bool pract = false;
int id = 1;
int respawn = 0;
float CurrentAttempRespawn = 0;
auto levelname = std::string("PlaceHolder");

std::string ID() {
		if (Mod::get()->getSettingValue<bool>("ID")) {
			return fmt::format("(ID: {})",id);
		};
		return "";
};

std::string Msg() {
    if (Best >= 100) {
		if (respawn > 0) {
			return fmt::format("I Beat {} from {}% {}",levelname,respawn,ID());
		}
			return fmt::format("I Beat {} {}",levelname,ID());
	}
	else {
		if (respawn > 0) {
			return fmt::format("I Got {}-{}% on {} {}",respawn,Best,levelname,ID());
		}
			return fmt::format("I Got a best on {} ({}%) {}",levelname,Best,ID());
		}
	};

void uploadNewBest() {
   auto manager = GameLevelManager::sharedState();
   if (Best > 0 && Mod::get()->getSettingValue<bool>("Enabled")) {
				auto Send = Msg();
				manager->uploadAccountComment(fmt::format("{}",Send));
			   if (Mod::get()->getSettingValue<bool>("Notify")) {
			  		Notification::create(fmt::format("Commented: {}",Send),NotificationIcon::Success,2.5)->show();
			   }
   };
   levelname = "PlaceHolder";
   Best = 0;
   respawn=0;
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
			return 12; // Clubstep
		case 15:
			return 9; // Electrodynamix
		case 16:
			return 9; // Hexagon Force
		case 17:
			return 4; // Blast Processing
		case 18:
			return 12; // TOE 2
		case 19:
			return 5; // Geometrical Dominator
		case 20:
			return 12; // Deadlocked
		case 21:
			return 9; // Fingerdash
		case 22:
			return 9; // Dash
		case 23:
			return 12; // Explorers
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
    if (level->m_ratings && level->m_ratingsSum) {
	    return level->m_ratings / level->m_ratingsSum;
    };
    return 0;
}

int convertGJDifficultyDemonToAssetKey(int difficulty) {
	switch (difficulty) {
		case 3:
			return 10;
		case 4:
			return 11;
		case 0:
			return 12;
		case 5:
			return 13;
		case 6:
			return 14;
	}
	return 0;
}
int getratingsnumbs(GJGameLevel* level) {
    int stars = level->m_stars.value();
	float difficulty = getAverageDifficulty(level);
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

bool Rated(GJGameLevel* level) {
	
	if (level->m_isEpic || level->m_featured || !Mod::get()->getSettingValue<int64_t>("ratedlevels")) {
		return true;
	};
	return false;
};

bool unlisted(GJGameLevel* level) {
	if (level->m_unlisted && !Mod::get()->getSettingValue<int64_t>("unlisted")) {
		return true;
	};
	return false;
};

bool ifs(GJGameLevel* level) {
	  if (dif(level)) {
			if (Rated(level)) {
				if (!unlisted(level) ) {
					return true;
				};
			};
	  };
	return false;
}
float calPracSaved(GJGameLevel* level) {
	return Mod::get()->getSavedValue<float>(fmt::format("StartPosB_E{}",level->m_levelID.value())) - Mod::get()->getSavedValue<float>(fmt::format("StartPosB_S{}",level->m_levelID.value()));
}
float calPrac(float current, float persentlook) {
	return (persentlook - current);
}

void setvalue(GJGameLevel* level, int overrightper, float startpos) {
    int persentlook = overrightper;
    if (level==nullptr) {
        log::debug("Caught Crash: Level: {} \n Per : {}",level,per);
        return; 
    };
	 if ( startpos > 0) {
		auto calprac = calPrac(startpos,persentlook);
			if (calPracSaved(level) < calprac ) {
				if (Mod::get()->getSettingValue<int64_t>("PracticeOffset") < calprac) {
					Mod::get()->setSavedValue(fmt::format("StartPosB_S{}",level->m_levelID.value()), startpos);
					Mod::get()->setSavedValue(fmt::format("StartPosB_E{}",level->m_levelID.value()), persentlook);
					Best = persentlook;
					respawn=startpos;
					id = level->m_levelID.value();
				}	
			}
		return;
	 };
	if (Best < persentlook) {
        if (ifs(level)) {
			Best = persentlook;
			respawn=0;
			id = level->m_levelID.value();	
        }
	};
}

class $modify(PlayerObject){
	void playerDestroyed(bool p0){
		auto keep = CurrentAttempRespawn;
		PlayerObject::playerDestroyed(p0);
		if (GameManager::sharedState()->getPlayLayer() ) {
			setvalue(PlayLayer::get()->m_level,per,keep);
				if (Mod::get()->getSettingValue<bool>("Death")) {
            		uploadNewBest();
        		}
	};
		 };
		
};
class $modify(PauseLayer) {
	void onQuit(CCObject* sender) {
		PauseLayer::onQuit(sender);
		uploadNewBest();
	};
	void onEdit(CCObject* sender) {
		PauseLayer::onEdit(sender);
		uploadNewBest();
	};
};
class $modify(EndLevelLayer) {
	void onMenu(CCObject* sender) {
		EndLevelLayer::onMenu(sender);
		uploadNewBest();
	};
	void onEdit(CCObject* sender) {
		EndLevelLayer::onEdit(sender);
		uploadNewBest();
	};
};

class $modify(PlayLayer) {
	void levelComplete() {
		PlayLayer::levelComplete();
		setvalue(PlayLayer::get()->m_level,100,CurrentAttempRespawn);
	};
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
		levelname = std::string(m_level->m_levelName);
            if (!m_level->isPlatformer()) {
			float percent = static_cast<float>(PlayLayer::getCurrentPercentInt());
                if (m_isPracticeMode && !CurrentAttempRespawn == 0) {
					pract=true;
                    per = percent;
                    return;
                };
				pract = false;
                if (percent > (m_level->m_normalPercent)) {
                    per = percent;
                } 
            }
	};
	void resetLevel() {
		PlayLayer::resetLevel();
		CurrentAttempRespawn = PlayLayer::getCurrentPercent();
		//log::debug("Spawned at {}",CurrentAttempRespawn);
	};
};
// GJ_closeBtn_001

class $modify(customLayer,PauseLayer) {
  void Remove(CCObject*) {
     geode::createQuickPopup(
			"New best comment",
			"Are you sure you want to <cr>Remove StartPos Data</c>?",
			"Cancel", "Yes",
			[](auto, bool btn2) {
				if (btn2) {
					if (GameManager::sharedState()->getPlayLayer() ) {
						id =  PlayLayer::get()->m_level->m_levelID.value();
						Mod::get()->setSavedValue(fmt::format("StartPosB_S{}",id), 0);
						Mod::get()->setSavedValue(fmt::format("StartPosB_E{}",id), 0);
					};
				}
			}
		)->show();
  };

  void customSetup() {
    PauseLayer::customSetup();
    auto menu = this->getChildByID("left-button-menu");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto spr = CCSprite::create("GJ_closeBtn_001.png");
    
	auto btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(customLayer::Remove)
      );
	  
      btn->setPosition({menu->getContentSize().width/2, btn->getContentSize().height/2});
      btn->setID("RemoveStartPosData"_spr);
      menu->addChild(btn);
	  menu->updateLayout();
    }
};