#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

//use -lm parameter to compile this program



#define final_vetor_size 2

struct Neuron
{

	//exist two kind of neurons, the in
	long double output;
	double *weight;
	int baias;
	struct Neuron * next;
};

typedef struct Neuron N;
//Just N to refer to a Neuron struct

/*functions*/
long double n_number(long double input[], int weight[], int baias);
double activate_function(long double n);
N * create_empty_levels();
N * initialize_hide_level(N * node , int qtde_of_neurons, long double input[]);
N * data_level(long double input[]);
N * create_new_neuron(long double input);
N * create_hide_level(int level_size);
//


int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	printf("1\n");
	N * head = create_empty_levels(head); //creating the first element to pass to reference to other`s functinos
	printf("2\n");
	N * second_level = create_empty_levels(second_level);
	printf("3\n");
	long double test_vector[] = {2,1};
	printf("4\n");
	head = data_level(test_vector);
	printf("5\n");
	second_level = create_hide_level(10);

	printf("(1)--> %Lf\n", head->output);
	printf("(1)--> %Lf\n", head->next->output);



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

N * initialize_hide_level(N * node , int level_size, long double input[])
{
	N * temp = node;
	//now i gonna distribute the vector of weight and baias for the neurons 
	for(size_t i = 0 ; i < level_size ; i++)
	{ 
		long double * weight = (long double *) malloc(final_vetor_size * sizeof(long double));
		for(size_t j = 0 ; i < level_size ; i++)
		{
			if(temp == NULL)
			{
				return NULL;
			}
			else
			{
				*(weight + j) = (rand() % (16000 - (-16000) + 1)) + -(16000); //A random value between -16000 and 16000

			}
		
		}
		temp->weight = weight;
		temp = temp->next;
	}
//this funtion is not finished yet
	return temp;
}

N * data_level(long double input[])
{
	N * node = NULL;
	N * temp = node;
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
			
			temp = node;
			while(temp->next != NULL) temp = temp->next;
			temp->next = create_new_neuron(input[i]);

		}
	}
	return node;
}

N * create_new_neuron(long double input)
{//input is a single value
	N * new_neuron = (N *) malloc(sizeof(N));
	if (new_neuron == NULL)
	{
		printf("Failed to alloc node.\n");
		return NULL;
	}
	new_neuron->output = input;
	new_neuron->weight = NULL;
	new_neuron->baias = 0;
	new_neuron->next = NULL;

	

	return new_neuron;
}

N * create_hide_level(int level_size)
{//this function create the level with the input in the beginning of the program
	N * level = NULL;
	for(size_t i = 0 ; i < level_size; i ++)
	{
		if(level == NULL)
		{
			level = create_new_neuron(0); //it`s just a initialization, what i really need is the list.
		}
		else if(level->next == NULL)
		{
			level->next = create_new_neuron(0);  
		}
		else{
			N * temp = level;
			while(temp->next != NULL) temp=temp->next;
			temp->next = create_new_neuron(0);
		}
	}

}