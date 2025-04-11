#pragma once
#include "Texture.h"
#include "Core/Core.h"
#include "gl3w.h"


namespace GameEngine
{
	class Texture2d : public Texture
	{
	public:
		Texture2d(uint32_t _w, uint32_t _h);
		Texture2d(std::string path);
		Texture2d() = default;
		virtual ~Texture2d();

		virtual uint32_t getWidth() const override {return width;}
		virtual uint32_t getHeight() const override { return height; }
		virtual uint32_t getRenderID() const override { return renderID; }

		virtual void setData(void*, uint32_t) override;

		virtual void bind(uint32_t) const override;
		virtual bool operator==(const Texture& other) const override 
		{
			return renderID == ((Texture2d&)other).renderID;
		}
		virtual const std::string getPath() const override { return path; }
		virtual const std::string getType() const override { return type; }
		virtual void setType(std::string t) override { type = t; }
		

		
	

		std::string path;
		std::string type;
		uint32_t width, height;
		uint32_t renderID;
		GLenum intFmt, dataFmt;
	};
}