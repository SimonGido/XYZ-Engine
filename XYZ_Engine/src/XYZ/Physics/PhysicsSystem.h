#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Components.h"



namespace XYZ {
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity) override;

	private:
		struct Component : public System::Component
		{
			Transform2D* transform;
			RigidBody2D* rigidbody;
		};

		float m_Gravity = 10.0f;
		std::vector<Component> m_Components;
	};

}