#pragma once
#include "../GameObject/GameObject.h"


enum class StaticObjectID
{
	NON = -1,			//Nie ma bloku
	Flor = 0,			//Pod³oga domyœlna
	Lava,				//Lawa pod map¹ do niszczenia gracza
	Brick,				//Standardowy block do niszczenia
	Steal,				//Block stali nie do zniszczenai najczesciej pokazuje sie po uzyciu power bloku
	PowerBlockCoin,		//Power blok w ktorym jest coin
	PowerBlockMushroom,	//Power blok w ktorym jest grzyb
	PowerBlockFlower,	//Power blok w ktorym jest kwiatek
	Coin,				//Coin na mapie ktory mozemy podnosc

	size
};

enum class DynamicObjectID
{
	NON = -1,			//Nie ma bloku
	Pipe = 0,			//Rura przeszkoda w grze
	Flag,				//End Map flag
	Goomba,				//Ten Grzyb
	CoopaRed,			//Ten zó³wik czerwony
	CoopaGreen,			//Ten zó³wik zielony
	Castle,
	size
};

void createObjectToCopy();
void deleteObjectToCopy();
GameObject* cloneStaticObject(StaticObjectID Id);
GameObject* cloneDynamicObject(DynamicObjectID Id);