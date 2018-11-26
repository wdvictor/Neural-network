#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

//use -lm parameter to compile this program

struct Neuron
{
	//this value just hold the number after the input value 
	//pass through activate function
	long double value;
	struct Neuron * next;
};

#define final_vetor_size 536

typedef struct Neuron N;
//Just N to refer to a Neuron

/*functions*/
long double n_number(long double input[], int weight[], int baias);
double activate_function(long double n);
N * create_empty_levels();
N * initialize_hide_level(N * node , int qtde_of_neurons, long double input[]);
N * data_level(long double input[]);
N * create_new_neuron(long double input);
//


int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	N * head = create_empty_levels(head);


	return 0;
}

long double n_number(long double input[], int weight[], int baias)
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

N * create_empty_levels()
{
	return NULL;

}

N * initialize_hide_level(N * node , int qtde_of_neurons, long double input[])
{
	/*
		We decided to create the hide levels with single-linked-list`s, but why?
		to pass the input for all Neurons in the level, we need a tecnique or a method to
		access all neurons in the level. So, we use the foward reference  to pass the input value to others
		neurons in the level, and the behind reference to use in backpropagation
	*/
	for(size_t i = 0 ; i < qtde_of_neurons ; i++)
	{
		
		
	}

}

N * data_level(long double input[])
{
	N * node = NULL;

	for(size_t i = 0 ; i < final_vetor_size ; i++)
	{
		if(node == NULL)
		{
			node = create_new_neuron(input[i]);
	
		}
		else if(node->next == NULL)
		{
			node->next = create_new_neuron(input[i]);
		
		}
		else
		{
			N * temp = node;
			while(temp->next != NULL) temp = temp->next;
			temp->next = create_new_neuron(input[i]);
		}
	}
	return node;
}

N * create_new_neuron(long double input)
{//input is a single value
	N * new_neuron = (N *) malloc(sizeof(N));
	new_neuron->value = input;
	new_neuron->next = NULL;

	return new_neuron;
}