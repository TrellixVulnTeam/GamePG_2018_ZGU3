#pragma once
class Player;

class Star : public IGameObject
{
public:
	~Star();
	bool Start();
	void Update();
	//prefab::CSkinModelRender*型のメンバ変数を追加。
	prefab::CSkinModelRender* skinModelRender = nullptr;
	CVector3 position ;
	float angle = 0.0f;	//回転角度。
	Player* player = nullptr;
	int moveCount = 0;
	
};

