#include <glm/glm.hpp>

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 direction() {
		glm::vec3 forwardDir = glm::vec3(0, 0, -1);
		glm::vec3 direction = rotation * forwardDir;
		return glm::normalize(direction);
	}
};