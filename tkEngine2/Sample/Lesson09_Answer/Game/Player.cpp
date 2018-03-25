#include "stdafx.h"
#include "Player.h"

//////////////////////////////////////////////////////////
// �R���X�g���N�^�B�C���X�^���X�����������Ƃ��ɌĂ΂�鏈���B
//////////////////////////////////////////////////////////
Player::Player()
{
}
//////////////////////////////////////////////////////////
// �f�X�g���N�^�B�C���X�^���X���j�������Ƃ��ɌĂ΂�鏈���B
//////////////////////////////////////////////////////////
Player::~Player()
{
	//�X�L�����f�������_���[��j��
	DeleteGO(m_skinModelRenderer);
}
//////////////////////////////////////////////////////////
// ���߂�Update�֐����Ă΂�钼�O�ɁA��x�����Ă΂��֐��B
//////////////////////////////////////////////////////////
bool Player::Start()
{
	//�X�L�����f�������_���[���쐬�B
	m_skinModelRenderer = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRenderer->Init(L"modelData/AirPlane.cmo", NULL, 0, CSkinModel::enFbxUpAxisY);

	return true;
}
//////////////////////////////////////////////////////////
// ���Ԋu�ŌĂ΂��X�V�����B
//////////////////////////////////////////////////////////
void Player::Update()
{
	CQuaternion qRot = CQuaternion::Identity;
	if (Pad(0).IsPress(enButtonRight)) {
		m_position.x += 200.0f;
	}
	if (Pad(0).IsPress(enButtonLeft)) {
		m_position.x -= 200.0f;
	}
	if (Pad(0).IsPress(enButtonUp)) {
		m_position.y += 200.0f;
		//Z������̉�]�N�H�[�^�j�I�����쐬�B
		qRot.SetRotationDeg(CVector3::AxisZ, -10.0f);
	}
	if (Pad(0).IsPress(enButtonDown)) {
		m_position.y -= 200.0f;
		//Z������̉�]�N�H�[�^�j�I�����쐬�B
		qRot.SetRotationDeg(CVector3::AxisZ, 10.0f);
	}
	//Y�������-90���񂷉�]�N�H�[�^�j�I�����쐬����B
	CQuaternion qRot2;
	qRot2.SetRotationDeg(CVector3::AxisY, -90.0f);
	m_skinModelRenderer->SetRotation(qRot);

	//qRot��qRot2�̃N�H�[�^�j�I���������B
	qRot.Multiply(qRot2);

	//������m_position.x��10000�����傫���Ȃ�����B
	if (m_position.x > 10000.0f) {
		//10000.0f��������B
		m_position.x = 10000.0f;
	}
	//������m_position.x��150�����������Ȃ�����B
	if (m_position.x < -10000.0f) {
		//-20000.0f��������B
		m_position.x = -10000.0f;
	}

	//������m_position.y��10000�����傫���Ȃ�����B
	if (m_position.y > 5625.0f) {
		//10000.0f��������B
		m_position.y = 5625.0f;
	}
	//������m_position.x��150�����������Ȃ�����B
	if (m_position.y < -5625.0f) {
		//-20000.0f��������B
		m_position.y = -5625.0f;
	}

	//���W���X�L�����f�������_���[�ɔ��f������B
	m_skinModelRenderer->SetPosition(m_position);
	//��]���X�L�����f�������_���[�ɔ��f������B
	m_skinModelRenderer->SetRotation(qRot);
}