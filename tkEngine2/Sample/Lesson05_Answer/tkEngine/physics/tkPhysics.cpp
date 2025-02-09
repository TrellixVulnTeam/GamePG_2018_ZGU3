#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkPhysics.h"
#include "tkEngine/Physics/tkRigidBody.h"

namespace tkEngine{
	CPhysicsWorld::CPhysicsWorld()
	{
		collisionConfig = NULL;
		collisionDispatcher = NULL;
		overlappingPairCache = NULL;
		constraintSolver = NULL;
		dynamicWorld = NULL;
	}


	CPhysicsWorld::~CPhysicsWorld()
	{
		Release();
	}
	void CPhysicsWorld::Release()
	{
		delete dynamicWorld;
		delete constraintSolver;
		delete overlappingPairCache;
		delete collisionDispatcher;
		delete collisionConfig;

		dynamicWorld = nullptr;
		constraintSolver = nullptr;
		overlappingPairCache = nullptr;
		collisionDispatcher = nullptr;
		collisionConfig = nullptr;
	}
	void CPhysicsWorld::Init()
	{
		Release();
		//物理エンジンを初期化。
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfig = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		collisionDispatcher = new	btCollisionDispatcher(collisionConfig);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		constraintSolver = new btSequentialImpulseConstraintSolver;

		dynamicWorld = new btDiscreteDynamicsWorld(
			collisionDispatcher,
			overlappingPairCache,
			constraintSolver,
			collisionConfig
			);

		dynamicWorld->setGravity(btVector3(0, -10, 0));
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
		m_debugDraw.Init();
		dynamicWorld->setDebugDrawer(&m_debugDraw);
		m_debugDraw.setDebugMode(1);
#endif 
	}
	void CPhysicsWorld::Update()
	{
		dynamicWorld->stepSimulation(GameTime().GetFrameDeltaTime());
	}
	void CPhysicsWorld::AddRigidBody(CRigidBody& rb)
	{
		dynamicWorld->addRigidBody(rb.GetBody());
	}
	void CPhysicsWorld::RemoveRigidBody(CRigidBody& rb)
	{
		dynamicWorld->removeRigidBody(rb.GetBody());
	}
	void CPhysicsWorld::DebubDrawWorld(CRenderContext& rc)
	{
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
		
		m_debugDraw.BeginDraw(rc);
		dynamicWorld->debugDrawWorld();
		m_debugDraw.EndDraw();
#endif
	}
}