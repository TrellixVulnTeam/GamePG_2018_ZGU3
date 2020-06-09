/*!
 * @brief	�u���[�B
 */

#pragma once

#include "tkEngine/graphics/tkPrimitive.h"

namespace tkEngine{
	/*!
	 * @brief	�u���[�B
	 */
	class CBlur : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CBlur();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CBlur();
		/*!
		 * @brief	�������B
		 *@param[in]	srcTexture		���e�N�X�`���B
		 *@param[in]	blurIntensity	�u���[�̋����B�l���傫���قǃ{�P��B
		 */
		void Init( CShaderResourceView& srcTexture, float blurIntensity = 25.0f );
		/*!
		 * @brief	�u���[�����s�B
		 */
		void Execute(CRenderContext& rc);
		/*!
		* @brief	���s���ʂ��i�[���Ă���SRV���擾�B
		*/
		CShaderResourceView& GetResultSRV()
		{
			return m_yBlurRT.GetRenderTargetSRV();
		}
	private:
		/*!
		* @brief	�K�E�X�t�B���^�̏d�݂��X�V�B
		*/
		void UpdateWeight(float dispersion);
	private:
		static const int NUM_WEIGHTS = 8;
		/*!
		* @brief	�u���[�p�̃p�����[�^�o�b�t�@�B
		*@details
		* �����ύX������AAssets/shader/blur.fx��CBBlur�̒��g���ύX����悤�ɁB
		*/
		struct SBlurParam {
			CVector4 offset;
			float weights[NUM_WEIGHTS];
		};
		SBlurParam m_blurParam;				//!<�u���[�p�̃p�����[�^�B
		CConstantBuffer m_cbBlur;			//!<�u���[�p�̃p�����[�^�̒萔�o�b�t�@�B
		CShaderResourceView* m_srcTexture;	//!<�\�[�X�e�N�X�`���B
		CRenderTarget m_xBlurRT;			//!<X�u���[�̕`�����ݐ�B
		CRenderTarget m_yBlurRT;			//!<Y�u���[�̏������ݐ�B
		CShader m_vsXBlurShader;			//!<X�u���[�p�̒��_�V�F�[�_�[�B
		CShader m_vsYBlurShader;			//!<Y�u���[�p�̒��_�V�F�[�_�[�B
		CShader m_psBlurShader;				//!<�u���[�p�̃s�N�Z���V�F�[�_�[�B
		int m_srcTextureWidth = 0;			//!<�\�[�X�e�N�X�`���̕��B
		int m_srcTextureHeight = 0;			//!<�\�[�X�e�N�X�`���̍����B
		CPrimitive	m_fullscreenQuad;		//!<�t���X�N���[���`��p�̋�`�v���~�e�B�u�B
		CSamplerState m_samplerState;		//!<�T���v���X�e�[�g�B@todo �ЂƂ܂Ƃ߂ɂ�����������?�B
		float m_blurIntensity = 25.0f;
	};
}