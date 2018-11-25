#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//use -lm parameter to compile this program

struct Neuron
{
	//this value just hold the number after the input value 
	//pass through activate function
	double value;
	struct Neuron * next;
	struct Neuron * prev;
};

#define final_vetor_size 536

typedef struct Neuron N;
//Just N to refer to a Neuron

/*functions*/
long double n_number(long double input[], double weight[], int baias);
double activate_function(double n);
N * create_empty_levels(N * head);
N * initialize_level(N * head , int qtde_of_neurons);
//


int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	N * head = create_empty_levels(head);


	return 0;
}

long double n_number(long double input[], double weight[], int baias)
{
	/*
		This function return the 'n' variable described in Prof. Mendelson article how
	the variable that will be pass how parameter in the activate function
	 */

	long double n = 0;//variable that will be returned
	for(size_t i = 0 ; final_vetor_size ; i++)
	{
		n += *(input + i) + *(weight + i);
	}
	n+= baias;
	return n;

}

double activate_function(long double n)
{//log-sigmoid
	return  (1 / (1 + exp((-1)*n)));

}

N * create_empty_levels(N * head)
{
	
	head = NULL;

	return head;

}

N * initialize_level(N * head , int qtde_of_neurons, long double input[])
{
	/*
		We decided to create the hide levels with double-linked-list`s, but why?
		to pass the input for all Neurons in the level, we need a tecnique or a method to
		access all neurons in the level. So, we use the foward reference  to pass the input value to others
		neurons in the level, and the behind reference to use in backpropagation
	*/
	for(size_t i = 0 ; i < qtde_of_neurons ; i++)
	{

		head->value = activate_function(n_number());
	}

}