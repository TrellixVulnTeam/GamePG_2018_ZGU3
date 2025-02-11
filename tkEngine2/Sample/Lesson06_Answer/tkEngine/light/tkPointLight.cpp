/*!
 *@brief	ポイントライト
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/light/tkPointLight.h"

namespace tkEngine{
	namespace prefab {
		CPointLight::CPointLight()
		{
			light.color = {1.0f,1.0f,1.0f,1.0f};
			light.position = g_vec3Zero;
			light.positionInView = g_vec3Zero;
			light.attn.Set(10.0f, 1.0f, 0.0f, 0.0f);
		}
		CPointLight::~CPointLight()
		{

		}
		void CPointLight::SetPosition(const CVector3& pos)
		{
			light.position = pos;
		}
		void CPointLight::SetColor(const CVector4& color)
		{
			light.color = color;
		}
		void CPointLight::SetAttn(const CVector3& attn)
		{
			light.attn = attn;
			if (light.attn.y <= 0.0f) {
				TK_WARNING("attn.yの値が不正です。この値は必ず0より大きな値を設定するようにしてください。");
				light.attn.y = max(0.001f, light.attn.y);
			}
		}
		bool CPointLight::StartSub()
		{
			return true;
		}
		void CPointLight::Update()
		{
			//カメラ空間での位置を計算する。
			const CMatrix& mView = MainCamera().GetViewMatrix();
			light.positionInView = light.position;
			mView.Mul(light.positionInView);
			light.lightingMaterialIDGroup = GetLightingMaterialIDGroup();
		}
	}
}