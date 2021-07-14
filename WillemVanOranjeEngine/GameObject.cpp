
#include "TransformComponent.h"

#include "RenderComponent.h"
#include "TextComponent.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Subject.h"


Willem::GameObject::GameObject(const std::string& name)
	:SceneObject(name), m_pActorChanged{ new Subject() }, m_Enabled{ true }
{}

Willem::GameObject::~GameObject()
{
	for (size_t i = 0; i < m_Components.size(); i++)
		delete m_Components[i];

	if (m_pActorChanged)
		delete m_pActorChanged;
}


void Willem::GameObject::Update(float deltaT)
{
	if (!m_Enabled) return;
	//m_pActorChanged->Update(this,deltaT);

	for (size_t i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Update(deltaT);
	}

}

void Willem::GameObject::Render() const
{
	if (!m_Enabled)
		return;
	TransformComponent* transform = GetComponent<TransformComponent>();

	Vector3 pos;
	Vector3 scale;

	if (transform)
	{
		pos = transform->GetPosition();
		scale = transform->GetScale();
	}
	else
	{
		pos = Vector3(1, 1,1);
		scale = Vector3(1, 1,1);
	}
	for (size_t i = 0; i < m_Components.size(); i++)
	{

		m_Components[i]->Render(Willem::Vector2(pos.x,pos.y), Willem::Vector2(scale.x, scale.y));
	}
}


void Willem::GameObject::SetTexture(const std::string& filename)
{
	auto texture = ResourceManager::GetInstance().LoadTexture(filename);

	if (!texture) { std::cout << "WARNING: Loading texture failed" << std::endl; return; }

	RenderComponent* render = GetComponent<RenderComponent>();

	if (!render) { std::cout << "WARNING: Set texture called on gameObject without render component" << std::endl; return; }

	render->SetTexture(texture);

}

void Willem::GameObject::SetPosition(float x, float y, float z)
{
	TransformComponent* transform = GetComponent<TransformComponent>();

	if (!transform) { std::cout << "WARNING: Set position called on gameObject without transform component" << std::endl; return; }

	transform->SetPosition(x,y,z);
}


void Willem::GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	m_Components.push_back(component);
}

void Willem::GameObject::AddWatcher(Observer* obs)
{
	m_pActorChanged->AddObserver(obs);
}

void Willem::GameObject::Notify(Event event)
{
	m_pActorChanged->Notify(this,event);
}

void Willem::GameObject::SetEnabled(bool b)
{
	m_Enabled = b;
}