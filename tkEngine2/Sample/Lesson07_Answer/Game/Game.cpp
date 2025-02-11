#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Star.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{
}
Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_directionLight);
	DeleteGO(m_gameCamera);
	for (auto star : m_starList) {
		DeleteGO(star);
	}
	DeleteGO(m_bgmSource);
}
bool Game::Start()
{
	//HandsOn 1 BGMを再生しよう。
	m_bgmSource = NewGO<prefab::CSoundSource>(0);	//CSoundSourceのインスタンスを作成。
	m_bgmSource->Init("sound/normalBGM.wav");		//音声ファイルをロード。
	m_bgmSource->Play(true);						//再生。

	//カメラを設定。
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(3000.0f);
	g_camera3D->SetPosition({ 0.0f, 500.0f, -600.0f });
	g_camera3D->Update();

	m_player = NewGO<Player>(0, "UnityChan");
	m_backGround = NewGO<BackGround>(0);
	m_gameCamera = NewGO<GameCamera>(0);

	m_directionLight = NewGO<prefab::CDirectionLight>(0);
	CVector3 lightDir = { 0.0f, -0.707, 0.707 };
	lightDir.Normalize();
	m_directionLight->SetDirection(lightDir);
	m_directionLight->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f } );

	GraphicsEngine().GetShadowMap().SetLightDirection(lightDir);

	//星を配置。
	CLocData locData;
	locData.Load(L"loc/star.tks"); //配置情報をロード。
	for (int i = 0; i < locData.GetNumObject(); i++) {
		//星を一つづつ作成していく。
		CQuaternion qRot;
		qRot.SetRotationDeg(g_vec3AxisY, 180.0f);
		auto star = NewGO<Star>(0);
		star->position = locData.GetObjectPosition(i);
		qRot.Multiply(star->position);
		m_starList.push_back(star);
	}
	return true;
}
void Game::Update()
{
	//ゲームをリセットしてみよう。
	if (g_pad[0]->IsTrigger(enButtonSelect)) {//もしもゲームパッドのSelectボタンが押されていたら。
		DeleteGO(this);                               //キーボードのスペースキー
		NewGO<Game>(0, nullptr);
	}

}
void Game::Render(CRenderContext& rc)
{
}