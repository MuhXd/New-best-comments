#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
int per =0;
int Best = 0;
auto levelname = std::string("a");
void uploadNewBest() {
   auto manager = GameLevelManager::sharedState();
   manager->uploadAccountComment(std::string("New Best on ") + std::string(levelname) + std::string(" and got ") + (Best) + std::string(" %"));
   levelname = std::string("a");
   Best = 0;
}
void setvalue() {
	if (Best < per) {
	Best = per;
	};
};

class $modify(PlayerObject){
	void playerDestroyed(bool p0){

		PlayerObject::playerDestroyed(p0);

		setvalue();
		
	}
};


class $modify(PlayLayer) {
	void levelComplete() {
		PlayLayer::levelComplete();
		setvalue();
	}
	void onExit() {
		uploadNewBest();
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
