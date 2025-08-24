#pragma once

namespace GameEngine
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t shaderDataTypeSize(ShaderDataType t)
	{
		switch (t)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 3 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 8;
		case ShaderDataType::Int3: return 12;
		case ShaderDataType::Int4: return 16;
		case ShaderDataType::Bool: return 1;
		}
		CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
	public:
		std::string name;
		ShaderDataType type;
		uint32_t offset;
		uint32_t size;
		bool normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType _type, const std::string& _name, bool _normalized = false)
			: name(_name), type(_type), size(shaderDataTypeSize(_type)), offset(0), normalized(_normalized)
		{}

	public:
		uint32_t getComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3: return 3; //3xfloat3
			case ShaderDataType::Mat4: return 4; //4xfloat4
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;
			case ShaderDataType::Bool: return 1;
			}
			CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};
}