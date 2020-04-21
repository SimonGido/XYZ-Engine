#pragma once
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <memory>

namespace XYZ {
	class ECSManager
	{
	public:
		static void Init()
		{
			if (!s_Instance.get())
			{
				s_Instance = std::make_unique<ECSManager>();
				s_Instance->m_ComponentManager = std::make_unique<ComponentManager>();
				s_Instance->m_EntityManager = std::make_unique<EntityManager>();
				s_Instance->m_SystemManager = std::make_unique<SystemManager>();
			}
		}
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{	
			return m_SystemManager->RegisterSystem<T>();
		}
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}
		template<typename T>
		void AddComponent(Entity entity,const T& component)
		{
			// checks validity of T data type, if T* is not derived type of IComponent shows error
			m_ComponentManager->AddComponent<T>(entity, component);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), 1);

			m_EntityManager->SetSignature(entity, signature);
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			m_ComponentManager->RemoveComponent<T>(entity);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), 0);

			m_EntityManager->SetSignature(entity, signature);
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			m_SystemManager->SetSignature<T>(signature);
		}
		template<typename T>
		ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentType<T>();
		}

		template<typename T> 
		std::shared_ptr<ComponentStorage<T>> GetComponentStorage()
		{
			return m_ComponentManager->GetComponentStorage<T>();
		}

		template<typename T>
		T* GetComponent(Entity entity)
		{
			return m_ComponentManager->GetComponent<T>(entity);
		}

		template <typename T>
		int GetComponentIndex(Entity entity)
		{
			return m_ComponentManager->GetComponentIndex<T>(entity);
		}

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			return std::static_pointer_cast<T>(m_SystemManager->GetSystem<T>());
		}


		template <typename T>
		bool Contains(Entity entity)
		{
			return m_ComponentManager->Contains<T>(entity);
		}

		void DestroyEntity(Entity entity)
		{
			auto signature = GetEntitySignature(entity);
			m_SystemManager->EntityDestroyed(entity, signature);
			m_ComponentManager->EntityDestroyed(entity);
			m_EntityManager->DestroyEntity(entity);
		}
		Signature GetEntitySignature(Entity entity)
		{
			return m_EntityManager->GetSignature(entity);
		}
		Entity CreateEntity()
		{
			return m_EntityManager->CreateEntity();
		}

		static std::unique_ptr<ECSManager>& Get() { return s_Instance; }
	private:
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager>	  m_EntityManager;
		std::unique_ptr<SystemManager>	  m_SystemManager;
		//bool							  m_EntityEditing;
		static std::unique_ptr<ECSManager> s_Instance;
	};

}