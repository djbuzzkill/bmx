#include "Bullet_platform.h"

#include <Dx/Collision.h>

#include <Dx/GlobalTypes.h>

#include <memory>

#include <LinearMath/btVector3.h>
#include <btBulletCollisionCommon.h>

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

using namespace Ma;

//
//
class Co::Collision_space { public: 

   Collision_space() {}
   ~Collision_space() {}

	std::shared_ptr<btDefaultCollisionConfiguration>	config;	
	std::shared_ptr<btCollisionDispatcher>				   dispatcher;
	std::shared_ptr<btAxisSweep3>						      broadphase;
	std::shared_ptr<btCollisionWorld>					   world;


   typedef std::map<Co::Collider*, btCollisionObject*> ColliderMap;

   ColliderMap collider_table;
}; 


//
//
void Co::Destroy_collision_space  (Collision_space* cs)
{
   delete cs; 
}


//
//
Co::Collision_space* Co::Create_collision_space (const Ma::Vec3f& vmin, const Vec3f& vmax)
{
   btVector3 
      worldAabbMin (X(vmin), Y(vmin), Z(vmin)),
      worldAabbMax (X(vmax), Y(vmax), Z(vmax));

   Collision_space* cs = new Co::Collision_space; 

   //xp::Init_Bullet (*this);
   cs->config.reset (new btDefaultCollisionConfiguration ());
   cs->dispatcher.reset    (new btCollisionDispatcher(cs->config.get()));
   cs->broadphase.reset    (new btAxisSweep3 (worldAabbMin,worldAabbMax));
   cs->world.reset         (new btCollisionWorld (cs->dispatcher.get(), cs->broadphase.get(), cs->config.get()));
   return cs;
}


//
//
int Co::Remove_collider (Co::Collider* me, Co::Collision_space* cs)
{
   if(cs->collider_table.count (me))
   {
      cs->world->removeCollisionObject (cs->collider_table[me]); 

      delete cs->collider_table[me]->getCollisionShape (); 
      cs->collider_table.erase (me); 
      return 0;
   }
   return -1;
}


//
//
int Co::Add_collider_capsule (Co::Collision_space* cs,  Collider* me, int major_axis, float radius, float length)
{
   Vec3f vpos; 
   btTransform tr; 

   me->Get_position (vpos); 

   tr.setIdentity();
   tr.setOrigin (btVector3 (X(vpos), Y(vpos), Z(vpos)));

   btCollisionObject* coll_obj = new btCollisionObject;
   coll_obj->setUserPointer (me);
   coll_obj->setWorldTransform (tr);

	btCapsuleShape* btshape = 0;
	switch (major_axis)
	{
	case 0:
		btshape = new btCapsuleShapeX (radius, length);
	   break;

	case 1:
		btshape = new btCapsuleShape (radius, length);
	   break;

	case 2:
		btshape = new btCapsuleShapeZ (radius, length);
	   break;
   };

   coll_obj->setCollisionShape (btshape); 


   coll_obj->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
   //coll_obj->setCollisionFlags (btCollisionObject::CF_KINEMATIC_OBJECT);


   cs->collider_table[me] = coll_obj;

   cs->world->addCollisionObject (coll_obj); 

//{
//	if (internal_collision == 0)
//	{
//		btCollisionObject* bt_collision;
//
//		btTransform tr;
//		tr.setIdentity();
//		tr.setOrigin(btVector3(0,0,0));
//
//		// create terrain collision
//		bt_collision = new btCollisionObject ();
//		bt_collision->setUserPointer (this);
//		bt_collision ->setWorldTransform (tr);
//		if (GetObj ()->obj_flags & (0x1 << GobyStatic))
//		{
//			//bt_collision->setCollisionFlags (btCollisionObject::CF_STATIC_OBJECT);
//		}
//		else
//		{
//			// bt_collision->setCollisionFlags (btCollisionObject::CF_KINEMATIC_OBJECT);
//		}
//
//		internal_collision = bt_collision;
   return 0;
   }

int Co::Add_collider_height_field (Co::Collision_space* cs,  Co::Collider* me, const Heightfield_params& params)
{
   Vec3f vpos; 
   btTransform tr; 

   me->Get_position (vpos); 

   tr.setIdentity();
   tr.setOrigin ( btVector3 (X(vpos), Y(vpos), Z(vpos)) );

   btCollisionObject* coll_obj = new btCollisionObject;
   coll_obj->setUserPointer (me);
   coll_obj->setWorldTransform (tr);

//	_internal_init_ ();

//	SetVec3Zero (&offsetPos);
//	SetVec3Zero (&offsetRot);

//	TerrCLoDPatchRes* res = patch->resRef;

//	// res->shared.ptr->uSize == res->shared.ptr->vSize 
//	Assert (res->shared.ptr->uStride == res->shared.ptr->uStride, "patch dimensions should be symmetric");

//	float* txrHgt = reinterpret_cast<float*>(res->hgtMap.ptr->data);
//	unsigned numHeights = res->hgtMap.ptr->width * res->hgtMap.ptr->height;

//	float* intHeight = new float[numHeights]; 

//	for (unsigned iV = 0; iV < res->hgtMap.ptr->height; iV++) {
//		for (unsigned iU = 0; iU < res->hgtMap.ptr->width; iU++) { 
//			int txrInd = ((res->hgtMap.ptr->height - iV - 1) * res->hgtMap.ptr->width) + iU;
//			int hgtInd = (iV * res->hgtMap.ptr->width) + iU;
//			intHeight[hgtInd] = txrHgt[txrInd];
//		}
//	}


   float min_height = -1000.0;
   float max_height = 1000.0;
   float height_scale = 1.0f;
	//btCollisionShape* internal_shape;
	bool flipQuadEdges = false;
   btHeightfieldTerrainShape* shape = new btHeightfieldTerrainShape  (
      params.U_dim, //res->hgtMap.ptr->width,
      params.V_dim, //res->hgtMap.ptr->height,
		params.data, 
		height_scale,
      min_height, 
      max_height,
		1,
		PHY_FLOAT,
		flipQuadEdges
	);

	// track its own data
	shape->setUserPointer (me); 

   // scale the shape
   float vertStepU = params.U_size / (params.U_dim - 1); 
   float vertStepV = params.V_size / (params.V_dim - 1); 

   btVector3 localScaling (vertStepU, 1.0f, vertStepV);
   shape->setUseDiamondSubdivision(true);
   shape->setLocalScaling(localScaling);
   //
   coll_obj->setCollisionShape (shape);
   coll_obj->setCollisionFlags (btCollisionObject::CF_STATIC_OBJECT);
   //bt_collision->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

   cs->collider_table[me] = coll_obj;
   cs->world->addCollisionObject(coll_obj); 
//
//	internal_flags = btBroadphaseProxy::StaticFilter; // | 0x10;
//	internal_group = btBroadphaseProxy::StaticFilter; // | 0x10;
//
//	internal_shape = shape;
//	Update (0.0f);
   return 0; 
   }


int Co::Add_collider_tri_mesh (Co::Collision_space* cs,  Collider* me, const TriMesh_params& params)
{
   Assert ((params.num_inds % 3) == 0, "not a multiple of 3"); 

   Ma::Vec3f vpos; 
   btTransform tr; 

   me->Get_position (vpos); 

   tr.setIdentity();
   tr.setOrigin ( btVector3(Ma::X(vpos),Ma::Y(vpos),Ma::Z(vpos)) );

   btCollisionObject* coll_obj = new btCollisionObject;
   coll_obj->setUserPointer (me);
   coll_obj->setWorldTransform (tr);

   btTriangleMesh* tri_mesh = new btTriangleMesh; 
   int num_tris = params.num_inds / 3;
   for (int i = 0; i < num_tris; i++)
   {
      const Ma::Vec3f& v0 = params.verts[ params.inds[i * 3 + 0] ];
      const Ma::Vec3f& v1 = params.verts[ params.inds[i * 3 + 1] ];
      const Ma::Vec3f& v2 = params.verts[ params.inds[i * 3 + 2] ];
   
      tri_mesh->addTriangle (
         btVector3 (Ma::X(v0), Ma::Y(v0), Ma::Z(v0)), 
         btVector3 (Ma::X(v1), Ma::Y(v1), Ma::Z(v1)), 
         btVector3 (Ma::X(v2), Ma::Y(v2), Ma::Z(v2))
         ); 
   }

   //

   btCollisionShape* shape = new btBvhTriangleMeshShape (tri_mesh , false);

   btVector3 vmin, vmax;
   shape->getAabb (tr,vmin, vmax);

   shape->setLocalScaling (btVector3(1.0f, 1.0f, 1.0f));
	// track its own data
	shape->setUserPointer (me); 
   

   // btTransform wtrans = coll_obj->getWorldTransform (); 
   // const btVector3 wpos = wtrans .getOrigin (); 

   coll_obj->setCollisionShape (shape);
   coll_obj->setCollisionFlags (btCollisionObject::CF_STATIC_OBJECT);
   //bt_collision->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

   cs->collider_table[me] = coll_obj;
   cs->world->addCollisionObject(coll_obj); 

   return 0; 

} 

//
//
int Co::DispatchCollisionEvents (Co::Collision_space* cs, double Dt)
{
   cs->world->updateAabbs();

	cs->world->performDiscreteCollisionDetection();

	int numManifolds = cs->world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = cs->world->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		Co::Collider* collA = static_cast<Co::Collider*>(obA->getUserPointer());
		Co::Collider* collB = static_cast<Co::Collider*>(obB->getUserPointer());

		int flagsA = obA->getCollisionFlags ();
		int flagsB = obB->getCollisionFlags ();

		int numContacts = contactManifold->getNumContacts();
		for (int j=0; j < numContacts; j++)
		{
			// Debug ( "CollidableA : %s\n", collA->GetObj()->labelRef);
			// Debug ( "CollidableB : %s\n", collB->GetObj()->labelRef);

			float fracA = obA->getHitFraction ();
			float fracB = obB->getHitFraction ();

			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (flagsA & btCollisionObject::CF_CHARACTER_OBJECT)
			{

				Ma::Vec3f p;
				Ma::Vec3f n;
				btVector3& worldNorm = pt.m_normalWorldOnB;

				if (numContacts == 2)
				{
					int idbg =0;
				}

				// :::::::::::::::::::::::::::::::::
				// 
				// NOTE: => (pt.m_distance1 < 0.0f)
				//
				// sometimes they return negative 
				// penetrations or i just dont understand the 
				// m_distance1 var, just ignore them for now


				if (pt.m_distance1 < 0.0f) 
				{

					Ma::Set (
						p, 
						pt.m_localPointA.getX (),
						pt.m_localPointA.getY (), 
						pt.m_localPointA.getZ ()
					);

					Ma::Set  (
						n, 
						worldNorm.getX (), 
						worldNorm.getY (), 
						worldNorm.getZ ()
					);

					collA->On_contact (collB, p, n, -pt.m_distance1); 
               //AddContact (collB, &p, &n, -pt.m_distance1);
				}
			}

			if (flagsB & btCollisionObject::CF_CHARACTER_OBJECT)
			{
				Ma::Vec3f p;
				Ma::Vec3f n;

				Ma::Set  (
					p, 
					pt.m_localPointB.getX (), 
					pt.m_localPointB.getY (), 
					pt.m_localPointB.getZ ()
				);
				Ma::Set(
					n, 
					pt.m_normalWorldOnB.getX (), 
					pt.m_normalWorldOnB.getY (), 
					pt.m_normalWorldOnB.getZ ()
				);

				if (pt.m_distance1 < 0.0f)
				{
					int idbg = 0;
				}

            collB->On_contact (collA, p, n, pt.m_distance1); 
            //collB->AddContact (collA, &p, &n, pt.m_distance1);
			}
		}
	}

   return 0; 
}



//
//


//
//
struct CollidableRayResult : public btCollisionWorld::RayResultCallback
{
	CollidableRayResult (const btVector3& rayFromWorld, const btVector3& rayToWorld, Co::Raycast_result& res)
	   : m_rayFromWorld (rayFromWorld)
      , m_rayToWorld (rayToWorld)
      , ray_res (res)
	{
		//m_rayFromWorld.setValue (
	}

   Co::Raycast_result& ray_res;

	btVector3	m_rayFromWorld; //used to calculate hitPointWorld from hitFraction
	btVector3	m_rayToWorld;

	btVector3	m_hitNormalWorld;
	btVector3	m_hitPointWorld;
	// ::::::::::::::::::::::::::::::::::::::::::::::::::::::
	virtual bool needsCollision(btBroadphaseProxy* proxy0) const
	{
		bool collides = (proxy0->m_collisionFilterGroup & m_collisionFilterMask) != 0;
		collides = collides && (m_collisionFilterGroup & proxy0->m_collisionFilterMask);
		return collides;
	}
	// ::::::::::::::::::::::::::::::::::::::::::::::::::::::
	virtual btScalar addSingleResult (btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
	{
		//caller already does the filter on the m_closestHitFraction
		btAssert(rayResult.m_hitFraction <= m_closestHitFraction);
		
		m_closestHitFraction = rayResult.m_hitFraction;
		m_collisionObject = rayResult.m_collisionObject;


      Co::Collider* coll = static_cast<Co::Collider*>(m_collisionObject->getUserPointer ()); 
      
      const char* coll_label = coll->Get_label (); 

		if (normalInWorldSpace)
		{
			m_hitNormalWorld = rayResult.m_hitNormalLocal;
		} else
		{
			// need to transform normal into worldspace
			m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
		}
		m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);
		return rayResult.m_hitFraction;
	}
};

//
//
bool Co::Raycast (Raycast_result& query, Collision_space* cs, const Ma::Vec3f& ray_from, const Ma::Vec3f& ray_to)
{

		btVector3 a (Ma::X(ray_from), Ma::Y(ray_from), Ma::Z(ray_from)); // query->rayFrom->x, query->rayFrom->y, query->rayFrom->z);
		btVector3 b (Ma::X(ray_to), Ma::Y(ray_to), Ma::Z(ray_to)); //query->rayTo->x, query->rayTo->y, query->rayTo->z);

		CollidableRayResult resCB (a, b, query);

		//btBroadphaseProxy::CollisionFilterGroups { 
		//  DefaultFilter = 1, StaticFilter = 2, 
		//  KinematicFilter = 4, DebrisFilter = 8, 
		//  SensorTrigger = 16, AllFilter = -1 
		//};
      
		resCB.m_collisionFilterMask = btBroadphaseProxy::DefaultFilter ;
		resCB.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter ;
		//resCB.m_collisionFilterMask = btBroadphaseProxy::StaticFilter;
		//resCB.m_collisionFilterGroup = btBroadphaseProxy::StaticFilter;

		cs->world->rayTest (a, b, resCB);
		query.hasHit = resCB.hasHit ();
		query.hitTime = resCB.m_closestHitFraction;

		if (query.hasHit) 
			query.who = static_cast<Collider*> (resCB.m_collisionObject->getUserPointer ());

		return query.hasHit;
	}
