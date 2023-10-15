#include "GameObject.h"

std::list<textureSource> GameObject::texturesLoaded=std::list<textureSource>();

GameObject::GameObject(Rectangle pos, std::string path)
{
	this->pos = pos;
	bool loaded = false;
	if (texturesLoaded.size() > 0)
	{
		for (auto t : texturesLoaded)
			if (path.compare(t.path) == 0)
			{
				this->texture = texture;
				loaded = true;
			}
	}

	if (!loaded)
	{
		texture.texture = LoadTexture(path.c_str());
		texture.path = path;
		if (texture.texture.id > 0)
			texturesLoaded.push_back(texture);
	}
}

GameObject::GameObject(GameObject& o)
{
	pos = o.pos;
	texture = o.texture;
}

void GameObject::draw()
{
	Rectangle source = { 0,0,(float)texture.texture.width,(float)texture.texture.height };
	DrawTexturePro(texture.texture, source, pos, { 0,0 }, 0, WHITE);
}