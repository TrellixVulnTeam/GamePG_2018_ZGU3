/*!
 * @brief	アニメータークラス。
 */


#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/graphics/animation/tkAnimation.h"

namespace tkEngine{

	CAnimation::CAnimation()
	{
	}
	CAnimation::~CAnimation()
	{
		if (m_skeleton != nullptr) {
		}
	}
	
	void CAnimation::Init(CSkinModel& skinModel, CAnimationClip animClipList[], int numAnimClip)
	{
		TK_ASSERT(animClipList != nullptr, "animClipListがNULLです。");
		m_skeleton = &skinModel.GetSkinModelData()->GetSkeleton();
		m_skeleton->SetAnimation(this);
		skinModel.SetAnimation(this);
		for (int i = 0; i < numAnimClip; i++) {
			m_animationClips.push_back(&animClipList[i]);
		}
		for (auto& ctr : m_animationPlayController) {
			ctr.Init(m_skeleton);
		}
		
		Play(0);
		m_footIK.Init(m_skeleton);
	}
	/*!
	 * @brief	ローカルポーズの更新。
	 */
	void CAnimation::UpdateLocalPose(float deltaTime)
	{
		m_interpolateTime += deltaTime;
		if (m_interpolateTime >= 1.0f) {
			//補間完了。
			//現在の最終アニメーションコントローラへのインデックスが開始インデックスになる。
			m_startAnimationPlayController = GetLastAnimationControllerIndex();
			m_numAnimationPlayController = 1;
			m_interpolateTime = 1.0f;
		}
		for (int i = 0; i < m_numAnimationPlayController-1; i++) {
			int index = GetAnimationControllerIndex(m_startAnimationPlayController, i );
			m_animationPlayController[index].Update(deltaTime, this);
		}
		//最後のポーズだけ進めていく。
		int lastIndex = GetLastAnimationControllerIndex();
		m_animationPlayController[lastIndex].Update(deltaTime, this);
		
	}
	/*!
	 * @brief	グローバルポーズの更新。
	 */
	void CAnimation::UpdateGlobalPose()
	{
		//グローバルポーズ計算用のメモリをスタックから確保。
		int numBone = m_skeleton->GetNumBones();
		CQuaternion* qGlobalPose = (CQuaternion*)alloca(sizeof(CQuaternion) * numBone);
		CVector3* vGlobalPose = (CVector3*)alloca(sizeof(CVector3) * numBone);
		for (int i = 0; i < numBone; i++) {
			qGlobalPose[i] = CQuaternion::Identity;
			vGlobalPose[i] = g_vec3Zero;
		}
		//グローバルポーズを計算していく。
		int startIndex = m_startAnimationPlayController;
		for (int i = 0; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(startIndex, i);
			float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
			const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
			for (int boneNo = 0; boneNo < numBone; boneNo++) {
				//平行移動の補完
				CMatrix m = localBoneMatrix[boneNo];
				vGlobalPose[boneNo].Lerp(
					intepolateRate, 
					vGlobalPose[boneNo], 
					*(CVector3*)m.m[3]
				);
				//平行移動成分を削除。
				m.m[3][0] = 0.0f;
				m.m[3][1] = 0.0f;
				m.m[3][2] = 0.0f;
				
				//回転の補完
				CQuaternion qBone;
				qBone.SetRotation(m);
				qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
			}
		}
		//グローバルポーズをスケルトンに反映させていく。
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			CMatrix boneMatrix;
			boneMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
			CMatrix transMat;
			transMat.MakeTranslation(vGlobalPose[boneNo]);
			boneMatrix.Mul(boneMatrix, transMat);

			m_skeleton->SetBoneLocalMatrix(
				boneNo,
				boneMatrix
			);
		}
		
		//最終アニメーション以外は補間完了していたら除去していく。
		int numAnimationPlayController = m_numAnimationPlayController;
		for (int i = 1; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(startIndex, i);
			if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
				//補間が終わっているのでアニメーションの開始位置を前にする。
				m_startAnimationPlayController = index;
				numAnimationPlayController = m_numAnimationPlayController - i;
			}
		}
		m_numAnimationPlayController = numAnimationPlayController;
	}
	
	void CAnimation::PostProcess()
	{
		//FootIKを実行。
		m_footIK.Update();
	}
	
	void CAnimation::Update(float deltaTime)
	{
		if (m_numAnimationPlayController == 0) {
			return;
		}
		//ローカルポーズの更新をやっていく。
		UpdateLocalPose(deltaTime);
		
		//グローバルポーズを計算していく。
		UpdateGlobalPose();
	}
}