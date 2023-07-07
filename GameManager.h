#pragma once
class GameManager
{
public:
	//Getters
	bool isGoal() const { return _isGoal; }
	bool getCollide() const { return _collide; }

	//Setters
	void reset() { _isGoal = false; }
	void p1Scores() { _p1Score++; _isGoal = true; }
	void p2Scores() { _p2Score++; _isGoal = true; }
	void setCollide(bool newValue) { _collide = newValue; }

private:
	int _p1Score = 0;
	int _p2Score = 0;
	bool _isGoal = false;
	bool _collide = false;
};

