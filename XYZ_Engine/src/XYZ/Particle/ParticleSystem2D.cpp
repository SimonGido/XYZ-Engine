#include "stdafx.h"
#include "ParticleSystem2D.h"
#include "XYZ/Renderer/Renderable2D.h"
#include "XYZ/Renderer/RenderCommand.h"
#include "XYZ/Renderer/Renderer2D.h"
#include "XYZ/Renderer/RenderCommandQueue.h"

namespace XYZ {
	ParticleSystem2D::ParticleSystem2D()
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::ParticleEffect2D>());
		m_ParticleStorage = ECSManager::Get()->GetComponentStorage<ParticleEffect2D>();
		m_EmitterStorage = ECSManager::Get()->GetComponentStorage<ParticleEmitter>();
	}

	void ParticleSystem2D::Update(float dt)
	{
		int16_t currentKey = 0;
		for (auto it : m_Components)
		{
			int16_t newKey = (*m_ParticleStorage)[it.particleIndex].GetMaterial()->GetSortKey();
			if (currentKey != newKey)
			{
				currentKey = newKey;
				(*m_ParticleStorage)[it.particleIndex].GetMaterial()->Set("u_Time", dt);
				(*m_ParticleStorage)[it.particleIndex].GetMaterial()->Bind();
			}
			(*m_ParticleStorage)[it.particleIndex].Render();
			
			for (int i = 0; i < it.numberOfChildren; ++i)
			{
				(*m_EmitterStorage)[it.childrenIndex[i]].material->Bind();
				(*m_ParticleStorage)[it.particleIndex].Bind((*m_EmitterStorage)[it.childrenIndex[i]]);
			}
		}
	}

	void ParticleSystem2D::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with id ", entity, " added");

		Component component;
		component.particleIndex = ECSManager::Get()->GetComponentIndex<ParticleEffect2D>(entity);
		if (ECSManager::Get()->Contains<ChildrenComponent>(entity))
		{
			auto children = ECSManager::Get()->GetComponent<ChildrenComponent>(entity);
			for (auto child : children->children)
			{
				component.childrenIndex[component.numberOfChildren] = ECSManager::Get()->GetComponentIndex<ParticleEmitter>(child);
				component.numberOfChildren++;
			}

		}

		component.entity = entity;
		auto it = std::lower_bound(m_Components.begin(), m_Components.end(), component, [](const Component& a, const Component& b) {

			auto storage = ECSManager::Get()->GetComponentStorage<ParticleEffect2D>();
			return (*storage)[a.particleIndex].GetMaterial()->GetSortKey() > (*storage)[b.particleIndex].GetMaterial()->GetSortKey();
		});

		m_Components.insert(it, component);
	}

	void ParticleSystem2D::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			it = m_Components.end() - 1;
			m_Components.erase(m_Components.end() - 1);
		}
	}

	bool ParticleSystem2D::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}


}