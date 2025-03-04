#include "GameEngine_PCH.h"
#include "Maths.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace GameEngine::Maths
{
	bool decomposeTransform(const glm::mat4& transfrom, glm::vec3& pos, glm::vec3& rot, glm::vec3& scl)
	{
		using namespace glm;
		using T = float;

		mat4 local(transfrom);

		if (epsilonEqual(local[3][3], static_cast<float>(0), epsilon<T>())) return false;

		if (epsilonNotEqual(local[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(local[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(local[2][3], static_cast<T>(0), epsilon<T>()))
		{
			local[0][3] = local[1][3] = local[2][3] = static_cast<T>(0);
			local[3][3] = static_cast<T>(1);
		}

		pos = vec3(local[3]);
		local[3] = vec4(0, 0, 0, local[3].w);

		vec3 row[3], pdum3;

		for (length_t i = 0; i < 3; ++i)
		{
			for (length_t j = 0; j < 3; ++j)
			{
				row[i][j] = local[i][j];
			}
		}

		scl.x = length(row[0]);
		row[0] = detail::scale(row[0], static_cast<T>(1));
		scl.y = length(row[1]);
		row[1] = detail::scale(row[1], static_cast<T>(1));
		scl.z = length(row[2]);
		row[2] = detail::scale(row[2], static_cast<T>(1));

		rot.y = asin(-row[0][2]);
		if (cos(rot.y) != 0)
		{
			rot.x = atan2(row[1][2], row[2][2]);
			rot.z = atan2(row[0][1], row[0][0]);
		}
		else
		{
			rot.x = atan2(-row[2][0], row[1][1]);
			rot.z = 0;
		}

		return true;

	}
}