#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}
bool Player::Start()
{
	//アニメーションクリップをロード。
	animationClips[enAnimationClip_run].Load(L"animData/run.tka");
	animationClips[enAnimationClip_run].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_idle].Load(L"animData/idle.tka");
	animationClips[enAnimationClip_idle].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_jump].Load(L"animData/jump.tka");

	//スキンモデルレンダラーを作成。
	skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	skinModelRender->Init(L"modelData/unityChan.cmo", animationClips, enAnimationClip_num, CSkinModel::enFbxUpAxisY);

	rotation.SetRotationDeg(g_vec3AxisY, 180.0f);

	skinModelRender->SetShadowReceiverFlag(true);
	skinModelRender->SetShadowCasterFlag(true);

	//HandsOn4 CCharacterControllerクラスのオブジェクトの初期化
	charaCon.Init(
		20.0f, //キャラクターの半径。 
		75.0f, //キャラクターの高さ。 
		position //キャラクターの初期座標。 
	);


	
	return true;
}
//キャラクターの移動処理。位置
void Player::MovePosition()
{
	//キャラクターの移動処理。

	if (g_pad[0]->IsPress(enButtonRight)) { //もしもゲームパッドの右ボタンが押されていたら。
		position.x += 10.0f;                      //キーボードの６キー
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {  //もしもゲームパッドの左ボタンが推されていたら。
		position.x -= 10.0f;                      //キーボードの４キー
	}
	if (g_pad[0]->IsPress(enButtonUp)) {  //もしもゲームパッドの上ボタンが推されていたら。
		position.z += 10.0f;                    //キーボードの８キー
	}
	if (g_pad[0]->IsPress(enButtonDown)) {  //もしもゲームパッドの下ボタンが推されていたら。
		position.z -= 10.0f;                      //キーボードの２キー
	}

	//キャラクターを複製してみよう。
	if (g_pad[0]->IsTrigger(enButtonSelect)) {//もしもゲームパッドのSelectボタンが押されていたら。
		NewGO<Player>(0);                           //キーボードのスペースキー
	}

	//キャラクターをジャンプさせてみよう。
	if (g_pad[0]->IsPress(enButtonA)) {  //もしもゲームパッドのAボタンが推されていたら。
		position.y += 5.0f;                    //キーボードのJ キー
	}
	//重力の影響を与える。
	position.y -= 0.5f;


	//キャラクターのY座標が0より小さくなったら
	//ジャンプ力を0にして、キャラのY座標も0にする。
	if (position.y <= 0.0f) {
		position.y = 0.0f;
		isJump = false;
	}
	//モデルに座標を反映させる。
	skinModelRender->SetPosition(position);
}
//キャラクターの移動処理。速度
void Player::MoveSpeed()
{
	// HandsOn6 キャラクターの移動速度を初期化しよう。
	float fSpeed = 10.0f;
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;
	
	//　実習課題 1 Bボタンを押しながら移動すると、移動速度が２倍になるようにしなさい。
	if (g_pad[0]->IsPress(enButtonB)) {  //キーボードのKキー
		fSpeed = fSpeed * 2.0f;
	}

	//　実習課題 2 Yボタンを押しながら移動すると、移動速度が半分になるようにしなさい。
	if (g_pad[0]->IsPress(enButtonY)) {  //キーボードのIキー
		fSpeed = fSpeed / 2.0f;
	}

	// HandsOn7 キャラクターを左右に動かしてみよう。
	//もしもゲームパッドの右ボタンが押されていたら。
	if (g_pad[0]->IsPress(enButtonRight)) { //キーボードの６キー 
		moveSpeed.x = fSpeed;
	}
	//もしもゲームパッドの左ボタンが押されていたら。 
	if (g_pad[0]->IsPress(enButtonLeft)) { //キーボードの４キー 
		moveSpeed.x = -fSpeed;
	}

	// HandsOn8 キャラクターを前後に動かしてみよう。
	//もしもゲームパッドの上ボタンが押されていたら。 
	if (g_pad[0]->IsPress(enButtonUp)) { //キーボードの８キー 
		moveSpeed.z = fSpeed;
	}
	//もしもゲームパッドの下ボタンが押されていたら。 
	if (g_pad[0]->IsPress(enButtonDown)) { //キーボードの２キー 
		moveSpeed.z = -fSpeed;
	}


	// HandsOn9 キャラクターをジャンプさせて、重力を加えよう。
	//もしもゲームパッドのAボタンが押されていたら。 
	if (g_pad[0]->IsTrigger(enButtonA)) { //キーボードのJキー 
	//ジャンプする。 
		moveSpeed.y = 40.0f;
	}
	//重力の影響を与える。 
	moveSpeed.y -= 2.0f;

	// HandsOn9の後、文頭の【//】を消して、コードを変更しておきましょう。
	if (charaCon.IsJump() == false) {
		isJump = false;
	}



	//移動はキャラクターコントローラーに移動速度を与えて行う。
	//プレイヤーはキャラクタコントローラーによる移動結果を得るのみ。

	//  HandsOn5 CCharacterController クラスを使って、キャラクターを移動させる。
	position = charaCon.Execute(1.0f, moveSpeed);
	
	//モデルに座標を反映させる。
	skinModelRender->SetPosition(position);
}
//キャラクターの回転処理。
void Player::Rotation()
{
	
	if (g_pad[0]->IsPress(enButtonRight)) {
		rotation.SetRotationDeg(g_vec3AxisY, 90.0f);
		//rotation.SetRotation(g_vec3AxisY, CMath::PI * 0.5f);	//ラジアン単位の別解。
	}
	
	if (g_pad[0]->IsPress(enButtonLeft)) {
		rotation.SetRotationDeg(g_vec3AxisY, -90.0f);
		//rotation.SetRotation(g_vec3AxisY, CMath::PI * -0.5f);	//ラジアン単位の別解。
	}
	
	if (g_pad[0]->IsPress(enButtonUp)) {
		rotation.SetRotationDeg(g_vec3AxisY, 0.0f);
		//rotation.SetRotation(g_vec3AxisY, 0.0f);		//ラジアン単位の別解。
	}
	
	if (g_pad[0]->IsPress(enButtonDown)) {
		rotation.SetRotationDeg(g_vec3AxisY, 180.0f);
		//rotation.SetRotation(g_vec3AxisY, CMath::PI);	//ラジアン単位の別解。
	}

	//モデルに回転を反映させる。
	skinModelRender->SetRotation(rotation);

}
void Player::AnimationControl()
{
	if (isJump == false) {
		
		//走りアニメーションを再生してみよう。
		if (g_pad[0]->IsPress(enButtonUp)) {
			//ゲームパッドの上ボタンが押されているなら。
			//走るアニメーションを再生する。
			skinModelRender->PlayAnimation(enAnimationClip_run);
		}
		else if (g_pad[0]->IsPress(enButtonDown)) {
			//ゲームパッドの下ボタンが押されているなら。
			//走るアニメーションを再生する。
			skinModelRender->PlayAnimation(enAnimationClip_run);
		}
		else if (g_pad[0]->IsPress(enButtonRight)) {
			//ゲームパッドの右ボタンが押されているなら。
			//走るアニメーションを再生する。
			skinModelRender->PlayAnimation(enAnimationClip_run);
		}
		else if (g_pad[0]->IsPress(enButtonLeft)) {
			//ゲームパッドの左ボタンが押されているなら。
			//走るアニメーションを再生する。
			skinModelRender->PlayAnimation(enAnimationClip_run);
		}
		else {
			//何も入力されていなければ立ちアニメーションを再生する。
			skinModelRender->PlayAnimation(enAnimationClip_idle);
		}
	}
	//ジャンプ中でなければ。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//ジャンプアニメーションを再生する。
		skinModelRender->PlayAnimation(enAnimationClip_jump);
		isJump = true;
	}
}
void Player::Update()
{
	//MovePosition();
	MoveSpeed();
	Rotation();
	AnimationControl();
}