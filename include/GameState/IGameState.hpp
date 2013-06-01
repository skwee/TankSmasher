#ifndef TS_IGAME_STATE_HPP_
#define TS_IGAME_STATE_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

class IGameState{
public:
	typedef boost::shared_ptr<IGameState> ptr;

	IGameState():mFrameRateDeltaTime(0.f){}
	virtual ~IGameState(){}

	virtual void Suspend() = 0;
	virtual void Resume() = 0;

	virtual void HandleInput(const sf::Event& evt) = 0;
	virtual void HandleAsyncInput(const sf::Input& input) = 0;

	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow& wnd) = 0;

	void SetFrameRateDeltaTime(float dt) { mFrameRateDeltaTime = dt; }

protected:
	float mFrameRateDeltaTime;
};

#endif