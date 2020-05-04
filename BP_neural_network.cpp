//BP神经网络
#include <iostream>
#include <string.h>
#include <time.h>
#include<math.h>
using namespace std;

class sigmoidNeuron {     //sigmoid神经元
public:
	double input;
	double output;
	double weight[2];
};

class inputNeuron {     //输入神经元与偏差单元
public:
	double value;
	double weight[2];
	/*inputNeuron(double val);*/	
};
//inputNeuron::inputNeuron(double val) {
//	value = val;
//}

double sigmoid(double input) {
	double output = 0;
	output = 1 / (1 + exp(-input));
	return output;
}

template <class T>
int getArrayLen(T& array)
{
	return sizeof(array) / sizeof(array[0]);
}

//void forward(inputNeuron input[], inputNeuron bias[], sigmoidNeuron hidden[], sigmoidNeuron output[]) {
//	for (int i = 0; i < getArrayLen(hidden); i++) {
//		for (int j = 0; j < getArrayLen(input); j++) {
//			hidden[i].input += input[j].value * input[j].weight[i];
//		}
//		hidden[i].input += bias[0].value * bias[0].weight[i];
//	}
//	for (int i = 0; i < getArrayLen(hidden); i++) {
//		hidden[i].output = sigmoid(hidden[i].input);
//	}
//
//	for (int i = 0; i < getArrayLen(output); i++) {
//		for (int j = 0; j < getArrayLen(hidden); j++) {
//			output[i].input += hidden[j].output * hidden[j].weight[i];
//		}
//		output[i].input += bias[1].value * bias[1].weight[i];
//	}
//	for (int i = 0; i < getArrayLen(output); i++) {
//		output[i].output = sigmoid(output[i].input);
//	}
//}


void forward(inputNeuron(&input)[2], inputNeuron(&bias)[2], sigmoidNeuron(&hidden)[2], sigmoidNeuron(&output)[2]) {    //前向计算时要先对神经元的输入进行初始化，不能直接累加
	for (int i = 0; i < getArrayLen(hidden); i++) {
		for (int j = 0; j < getArrayLen(input); j++) {
			hidden[i].input = 0;
		}
	}
	for (int i = 0; i < getArrayLen(output); i++) {
		for (int j = 0; j < getArrayLen(hidden); j++) {
			output[i].input = 0;
		}
	}
	for (int i = 0; i < getArrayLen(hidden); i++) {
		for (int j = 0; j < getArrayLen(input); j++) {
			hidden[i].input += input[j].value * input[j].weight[i];
		}
		hidden[i].input += bias[0].value * bias[0].weight[i];
	}
	for (int i = 0; i < getArrayLen(hidden); i++) {
		hidden[i].output = sigmoid(hidden[i].input);
	}

	for (int i = 0; i < getArrayLen(output); i++) {
		for (int j = 0; j < getArrayLen(hidden); j++) {
			output[i].input += hidden[j].output * hidden[j].weight[i];
		}
		output[i].input += bias[1].value * bias[1].weight[i];
	}
	for (int i = 0; i < getArrayLen(output); i++) {
		output[i].output = sigmoid(output[i].input);
	}
}

void backPropagation(double (&object)[2], inputNeuron(&input)[2], sigmoidNeuron(&hidden)[2], sigmoidNeuron(&output)[2], double (&learningRate)) {    //这里要先更新输入层的权重，再更新隐藏层的权重！
	double partialDer = 0.0;
	for (int i = 0; i < getArrayLen(input); i++) {
		for (int j = 0; j < getArrayLen(input[i].weight); j++) {
			for (int k = 0; k < getArrayLen(output); k++) {
				partialDer += (object[k] - output[k].output) * output[k].output * (1 - output[k].output) * hidden[j].weight[k] * hidden[j].output * (1 - hidden[j].output) * input[i].value;
			}
			input[i].weight[j] += learningRate * partialDer;
			partialDer = 0.0;
		}
	}
	for (int i = 0; i < getArrayLen(hidden); i++) {
		for (int j = 0; j < getArrayLen(hidden[i].weight); j++) {
			hidden[i].weight[j] += learningRate * (object[j] - output[j].output) * output[j].output * (1 - output[j].output) * hidden[i].output;
		}
	}
}

//void backPropagation(double object[], inputNeuron input[], sigmoidNeuron hidden[], sigmoidNeuron output[],double learningRate) {
//	double partialDer = 0.0;
//	for (int i = 0; i < getArrayLen(hidden); i++) {
//		for (int j = 0; j < getArrayLen(hidden[i].weight); j++) {
//			hidden[i].weight[j] += learningRate * (object[i] - output[i].output) * output[i].output * (1 - output[i].output) * hidden[i].output;
//		}
//	}
//	for (int i = 0; i < getArrayLen(input); i++) {
//		for (int j = 0; j < getArrayLen(input[i].weight); j++) {
//			for (int k = 0; k < getArrayLen(output); k++) {
//				partialDer += (object[k] - output[k].output) * output[k].output * (1 - output[k].output) * hidden[j].weight[k] * hidden[j].output * (1 - hidden[j].output) * input[i].value;
//			}
//			input[i].weight[j] += learningRate * partialDer;
//		}
//	}
//}

bool calError(double(&objVale)[2], sigmoidNeuron(&output)[2]) {
	bool judge = true;
	for (int i = 0; i < getArrayLen(objVale); i++) {
		if (abs(objVale[i] - output[i].output) > 0.2) {
			judge = false;
		}
	}
	return judge;
}

int main()
{
	inputNeuron input[2];
	input[0].value = 0.05;
	input[1].value = 0.1;
	input[0].weight[0] = 0.15;
	input[0].weight[1] = 0.25;
	input[1].weight[0] = 0.20;
	input[1].weight[1] = 0.30;
	inputNeuron bias[2];
	bias[0].value = bias[1].value = 1;
	bias[0].weight[0] = 0.35;
	bias[0].weight[1] = 0.35;
	bias[1].weight[0] = 0.60;
	bias[1].weight[1] = 0.60;

	sigmoidNeuron hidden[2];
	hidden[0].weight[0] = 0.40;
	hidden[0].weight[1] = 0.50;
	hidden[1].weight[0] = 0.45;
	hidden[1].weight[1] = 0.55;
	sigmoidNeuron output[2];
	
	/*hidden[0].input = 0; 
	hidden[1].input = 0;*/
	//hidden[0].output = hidden[1].output = 0;
	/*output[0].input = 0;
	output[1].input = 0;*/
	//output[0].output = output[1].output = 0;
	double object[2] = {0.01,0.99};

	forward(input, bias, hidden, output);

	/*forward(input, bias, hidden, output);*/
	int iters = 0;
	double learningRate = 0.5;
	while (!calError(object,output)) {
		backPropagation(object, input, hidden, output, learningRate);
		forward(input, bias, hidden, output);
		cout << "此次迭代后，解为：" << output[0].output << " " << output[1].output;
		cout << endl;
		iters += 1;
	}
	cout << "算法迭代了" << iters << "次才找到了满意解。";
	cout << "最终满意解为：" << output[0].output << " " << output[1].output;
	return 0;
}