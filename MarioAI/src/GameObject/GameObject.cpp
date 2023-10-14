#include "GameObject.h"

std::list<textureSource> GameObject::texturesLoaded;

GameObject::GameObject(Rectangle pos, std::string path)
{
	this->pos = pos;
	for(auto t:texturesLoaded)
		if (path.compare(t.path))
		{
			this->texture = texture;
			return;
		}
	texture.texture = LoadTexture(path.c_str());
	texture.path = path;
	if (texture.texture.id > 0)
		texturesLoaded.push_back(texture);
}

void GameObject::draw()
{
	Rectangle source = { 0,0,(float)texture.texture.width,(float)texture.texture.height };
	DrawTexturePro(texture.texture, source, pos, { 0,0 }, 0, WHITE);
}