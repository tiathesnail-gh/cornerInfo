#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool enabled = Mod::get()->getSavedValue<bool>("active", true);

#include <Geode/modify/PlayLayer.hpp>
class $modify(CornerInfoPlayLayer, PlayLayer) {
	bool init(GJGameLevel * level, bool a, bool b) {
		if (!PlayLayer::init(level, a, b)) {
			return false;
		}
		// part of the mod that adds the ui
		auto winsize = CCDirector::get()->getWinSize();

		auto corner = CCNodeRGBA::create();
		corner->setCascadeOpacityEnabled(true);
		corner->setScaledContentSize(CCSize(winsize.height * .15, winsize.height * .15));
		corner->setID("corner-tia");

		// info about difficulty and coin count
		int diff = level->m_stars.value();
		int coins = level->m_coins;


		std::string starImage;
		if (level->isPlatformer()) starImage = "GJ_bigMoon_001.png";
		else starImage = "GJ_bigStar_001.png";

		CCSprite* starSprite = CCSprite::createWithSpriteFrameName(starImage.c_str());
		CCSprite* coinSprite = CCSprite::createWithSpriteFrameName("usercoin_small01_001.png");
		if (!level->m_coinsVerified.value()) {
			coinSprite->setColor({ 255,175,75 });
		}
		auto starNum = CCLabelBMFont::create(std::to_string(diff).c_str(), "bigFont.fnt");
		auto coinNum = CCLabelBMFont::create(std::to_string(coins).c_str(), "bigFont.fnt");


		corner->addChildAtPosition(starNum,Anchor::BottomLeft,ccp(winsize.height / 16, winsize.height / 8));
		corner->addChildAtPosition(starSprite, Anchor::BottomLeft, ccp(winsize.height / 8, winsize.height / 8 - 1));
		corner->addChildAtPosition(coinNum, Anchor::BottomLeft, ccp(winsize.height / 16, winsize.height / 16));
		corner->addChildAtPosition(coinSprite, Anchor::BottomLeft, ccp(winsize.height / 8, winsize.height / 16 - 1));

		starSprite->setScale(0.4);
		coinSprite->setScale(1.225);
		starNum->setScale(0.6);
		coinNum->setScale(0.6);

		corner->setAnchorPoint(ccp(0, 0));
		corner->setZOrder(99999);
		this->addChild(corner);






		double scalefactor = Mod::get()->getSettingValue<double>("scale");
		double opacity = Mod::get()->getSettingValue<double>("opacity");

		corner->setOpacity(opacity * 255);
		corner->setZOrder(1000);
		corner->setScale(scalefactor);
		corner->setVisible(enabled);

		return true;
	}
	void resume() {
		PlayLayer::resume();
		this->getChildByID("corner-tia")->setVisible(enabled);
	}
};
#include <Geode/modify/PauseLayer.hpp>
class $modify(CornerInfoPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto menu = CCMenu::create();
		this->addChild(menu);
		menu->setPosition(0, 0);

		auto toggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(CornerInfoPauseLayer::onClick), .7);
		menu->addChild(toggle);
		toggle->setPosition(20, 20);
		auto textLabel = CCLabelBMFont::create("Corner", "bigFont.fnt");
		menu->addChild(textLabel);
		textLabel->setPosition(70, 20);
		textLabel->setScale(.6);

		if (enabled) {
			toggle->activate();
			enabled = true;
			Mod::get()->setSavedValue("active", true);

		}

	}
	void onClick(CCObject * sender) {
		if (enabled) {
			enabled = false;
			Mod::get()->setSavedValue("active",false);

		}
		else {
			enabled = true;
			Mod::get()->setSavedValue("active", true);

		}
	}
};