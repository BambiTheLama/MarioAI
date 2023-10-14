#pragma once
#define inputsSize 10
#define outputsSize 6
class NN
{
	int input[inputsSize];
	bool output[outputsSize];
public:
	NN();

	void add();

	int* getInputs() { return input; }

	bool* getOutputs() { return output; }
};

