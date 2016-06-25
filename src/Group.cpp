#include "Group.h"

Group::Group()
{

}

Group::~Group()
{
  m_children.clear();
}

void Group::InitDraw()
{

}

void Group::Draw(glm::mat4& inh_model)
{
  std::vector<ShaderObject*>::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++)
  {
    (*it)->InitDraw();
    (*it)->Draw(inh_model * GetModel());
    (*it)->EndDraw();
  }
}

void Group::EndDraw()
{

}

void Group::AddChild(ShaderObject* child)
{
  m_children.push_back(child);
}