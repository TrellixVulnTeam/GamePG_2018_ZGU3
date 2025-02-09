#pragma once
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Move();
	void Rotation();
	void AnimationControl();
	//ここからメンバ変数。
	enum {
		enAnimationClip_idle,
		enAnimationClip_run,
		enAnimationClip_jump,
		enAnimationClip_num,
	};
	prefab::CSkinModelRender* skinModelRender = nullptr;	//スキンモデルレンダラー。
	CAnimationClip animationClips[enAnimationClip_num];		//アニメーションクリップ。

	CVector3 position = g_vec3Zero;
	CQuaternion rotation = CQuaternion::Identity;
	bool isJump = false;
	float ySpeed = 0.0f;
};

