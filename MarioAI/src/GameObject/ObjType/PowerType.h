#pragma once
enum class PowerType
{
	Coin, Mushroom, Plant
};

class PowerTypeObject
{
public:
	virtual PowerType getPower() = 0;
};

