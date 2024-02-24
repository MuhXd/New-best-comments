#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
int per =0;
int Best = 0;
auto levelname = "";
void uploadNewBest() {
   auto manager = GameLevelManager::sharedState();
   manager->uploadAccountComment("New Best on " << (levelname) << " and got " << (Best) << " %");
   levelname = "";
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
                levelname = m_level->m_levelName;
            
                if (percent > (m_level->m_normalPercent)) {
                    per = percent;
                } 
            }
	}

};
