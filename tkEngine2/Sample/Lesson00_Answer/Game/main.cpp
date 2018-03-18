/*!
 *@brief	main.cpp
 */
#include "stdafx.h"
//�v���C���[�N���X�̋@�\���g�������̂Ńw�b�_�[�t�@�C�����C���N���[�h����B
#include "Player.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((unsigned)time(NULL));
	//tkEngine2�̏������p�����[�^��ݒ肷��B
	SInitParam initParam;
	initParam.nCmdShow = nCmdShow;
	initParam.hInstance = hInstance;
	initParam.screenWidth = 1280;
	initParam.screenHeight = 720;
	initParam.frameBufferWidth = 1280;
	initParam.frameBufferHeight = 720;
	//�e�̐ݒ�B
	initParam.graphicsConfing.shadowRenderConfig.isEnable = true;
	initParam.graphicsConfing.shadowRenderConfig.shadowMapWidth = 2048;
	initParam.graphicsConfing.shadowRenderConfig.shadowMapHeight = 2048;
	initParam.graphicsConfing.shadowRenderConfig.softShadowLevel = EnSoftShadowQualityLevel::eSSSS_PCF;
	initParam.graphicsConfing.shadowRenderConfig.lightHeight = UnitM(5.0f);

	//�A���`
	initParam.graphicsConfing.aaConfig.isEnable = true;
	//Bloom
	initParam.graphicsConfing.bloomConfig.isEnable = true;
	//tonemap
	initParam.graphicsConfing.tonemapConfig.isEnable = true;
	initParam.graphicsConfing.tonemapConfig.luminance = 0.5f;

	GraphicsEngine().GetShadowMap().SetFar(1000.0f);
	GraphicsEngine().GetShadowMap().SetNear(50.0f);

	//�G���W�����������B
	if (Engine().Init(initParam) == true) {
		//�J������ݒ�B
		MainCamera().SetTarget({ 0.0f, 100.0f, 0.0f });
		MainCamera().SetNear(0.1f);
		MainCamera().SetFar(3000.0f);
		MainCamera().SetPosition({ 0.0f, 100.0f, 300.0f });
		MainCamera().Update();

		//�v���C���[�̃C���X�^���X���쐬�B
		NewGO<Player>(0);

		//�Q�[�����[�v�����s�B
		Engine().RunGameLoop();
	}
	//�G���W���̏I�������B
	Engine().Final();

	return 0;
}
