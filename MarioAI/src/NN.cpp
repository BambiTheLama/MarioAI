#include "NN.h"

NN::NN()
{
	for (int i = 0; i < inputsSize; i++)
		input[i] = 0;
	for (int i = 0; i < outputsSize; i++)
		output[i] = 0;
}

void NN::add()
{
	for (int i = 0; i < inputsSize; i++)
		input[i]++;
	for (int i = 0; i < outputsSize; i++)
		output[i]++;
}