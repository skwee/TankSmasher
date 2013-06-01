#ifndef TS_PLAYER_DATA_HPP_
#define TS_PLAYER_DATA_HPP_

class PlayerData{
public:
	enum{
		SCORE_FOR_TANK_HIT = 30, SCORE_FOR_BONUS = 5
	};

public:
	PlayerData():mLives(3),mScore(0){}
	~PlayerData(){}

	inline short GetLives() const { return mLives; }
	inline void DecLive() { mLives--; }
	inline void IncLive() { mLives++; }
	inline bool IsDead() { return (mLives == 0); }

	inline int GetScore() const { return mScore; }
	inline void AddScore(int sd) { mScore += sd; }

private:
	short mLives;
	int mScore;
};

#endif