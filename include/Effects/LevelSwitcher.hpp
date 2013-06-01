#ifndef TS_LEVEL_SWITCHER_HPP_
#define TS_LEVEL_SWITCHER_HPP_

#include <SFML/Graphics.hpp>

class LevelSwitcher{
public:
	static const float FADEOUT_TIME;
	static const float DISPLAY_TIME;

public:
	LevelSwitcher();
	~LevelSwitcher();

	std::string GetNextLevel() { std::string b = mLevels.back(); mLevels.pop_back(); return b; }
	bool HaveMoreLevels() { return !mLevels.empty(); }

	bool SwitcherActive() { return mStarted; }
	bool SwitcherDone() { return mStarted && mDone; }
	void SwitcherStart();
	
	void Update(float dt);
	void Draw(sf::RenderWindow& wnd);

private:
	std::vector<std::string> mLevels;

	bool mStarted;
	bool mDone;
	bool mDisplaying;

	int mCurrentStage;

	float mFadeOutTime;
	float mDisplayTime;
};

#endif