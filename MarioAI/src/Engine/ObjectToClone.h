#pragma once
#include "../GameObject/GameObject.h"


enum class ObjectID
{
	Flor = 0,			//Pod�oga domy�lna
	Lava,				//Lawa pod map� do niszczenia gracza
	Brick,				//Standardowy block do niszczenia
	Steal,				//Block stali nie do zniszczenai najczesciej pokazuje sie po uzyciu power bloku
	Pipe,				//Rura przeszkoda w grze
	PowerBlockCoin,		//Power blok w ktorym jest coin
	PowerBlockMushroom,	//Power blok w ktorym jest grzyb
	PowerBlockFlower,	//Power blok w ktorym jest kwiatek
	Coin,				//Coin na mapie ktory mozemy podnosc
	Flag,				//End Map flag

};

void createObjectToCopy();
void deleteObjectToCopy();
GameObject* cloneObject(ObjectID Id);