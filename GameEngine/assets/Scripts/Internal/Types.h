#include "glm/glm.hpp"

class Entity;


using SetTransformFunc = void(*)(Entity, Transform);
using GetTransformFunc = Transform(*)(Entity);

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::Vec3 scale;
}