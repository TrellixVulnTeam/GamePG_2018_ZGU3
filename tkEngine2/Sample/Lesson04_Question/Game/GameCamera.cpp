#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	//プレイヤーのインスタンスを名前で検索する。
	Player* pl = FindGO<Player>("UnityChan");
	//HandsOn 1 注視点を動かす。
	

	//HandsOn 2 視点を動かす。
	//視点は注視点からＹ方向に＋４００、Ｚ方向に‐６００の場所にする。


	//HandsOn 3 遠平面を設定する。

	//実習課題１　遠平面を1000.0fに変更しなさい。


	//HandsOn 4 画角を変更してみる。


	//実習課題２　画角を90.0fに変更しなさい。


	//カメラを更新する。
	g_camera3D->Update();
}
