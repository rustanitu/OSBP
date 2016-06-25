#ifndef SHADER_GROUP_H
#define SHADER_GROUP_H

#include "ShaderObject.h"

#include <vector>

class Group : public ShaderObject
{
public:
  Group();
  ~Group();

  void InitDraw();

  virtual void Draw(glm::mat4& inh_model = glm::mat4());

  void EndDraw();

  void AddChild(ShaderObject* child);

private:
  std::vector<ShaderObject*> m_children;
};

#endif