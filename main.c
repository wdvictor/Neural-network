#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

/*use -lm parameter to compile this program*/



#define final_vetor_size 2

struct Neuron
{

	long double output;
	int baias;
	long double * weight;
	struct Neuron * next;
};

typedef struct Neuron N;
/*Just N to refer to a Neuron struct*/

/*functions*/
double activate_function(long double n);
N * create_empty_levels();
N * initialize_hide_level(N * node , int qtde_of_neurons, long double input[]);
N * data_level(long double input[]);
N * create_new_neuron(long double input);
N * create_hide_level(int level_size);
void local_inducted_field(N * data_level , N * hide_level, int hide_level_size);



int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	//printf("1\n");
	N * input_data = create_empty_levels(input_data); /*creating the first element to pass to reference to other`s functinos*/
	//printf("2\n");
	N * hide_process = create_empty_levels(hide_process);
	//printf("3\n");
	long double test_vector[] = {2,1};
	//printf("4\n");
	input_data = data_level(test_vector);
	//printf("5\n");
	hide_process = create_hide_level(2);
	//printf("6\n");
	hide_process = initialize_hide_level(hide_process , 2 , test_vector);

	local_inducted_field(input_data, hide_process, 2);

	N * temp = hide_process;
	while(temp != NULL) 
	{
		printf("Nv = %Lf\n", temp->output);
		temp =  temp->next;
	}



	return 0;
}


double activate_function(long double n)
{
	return  (1 / (1 + exp((-1)*n)));

}

N * create_empty_levels()
{
	return NULL;

}

N * initialize_hide_level(N * node , int level_size, long double input[])
{
	N * temp = node;
	/*now i gonna distribute the vector of weight and baias for the neurons */
	for(size_t i = 0 ; i < level_size ; i++)
	{ 
		//printf("lv(1)%zu\n", i);
		long double  * weight = (long double *) malloc(final_vetor_size * sizeof(long double));
		for(size_t j = 0 ; j < final_vetor_size ; j++)
		{
			/*Inside this for I gonna distribute the weight`s for all neuron. But to see
			more clearly what i do, let`s talk about more the theory.
			All neuron in the region on the input(the data level) pass his data to the hide
			level multiplying by a weight. This weights is in the graphical representetion of
			neural networks is the edges(rows). To implement this is C, i make the neuron of the 
			hide level have an array with values that represent the "edges" of the previously
			level. When the data pass to data level to the hide level, it`s multiplying by this
			weights in the vector in current position of the neuron. For example: if the neuron 2
			of the data level is passing his value to hide_level, is going be multiplicated by the weight
			in position 2 in the vector( 1 if the vector is zero indexed)*/
			//printf("lv(2)%zu\n", j);
			if(temp == NULL)
			{
				printf("NULL\n");
				return NULL;
			}
			else
			{
				//printf("lv(3)\n");
				*(weight + j) = (rand() % (16000 - (-16000) + 1)) + -(16000); 
				/*A random value between -16000 and 16000*/
				//printf("lv(4)%Lf\n", *(weight + j));
			}
		}
		//printf("lv(5)\n");
		temp->weight = weight;

		for(size_t j = 0 ; j < final_vetor_size ; j++)
		{
			//printf("Lv(5.1)W-->%Lf\n",*(temp->weight + j));

		}
		

		temp->baias = (rand() % (16000 - (-16000) + 1)) + -(16000); 
		//printf("lv(6)\n");
		temp = temp->next;

		//printf("lv(Final)\n");
	}
	temp = node;
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
{
	/*input is a single value*/
	N * new_neuron = (N *) malloc(sizeof(N));
	if (new_neuron == NULL)
	{
		printf("Failed to alloc node.\n");
		return NULL;
	}
	new_neuron->output = input;
	new_neuron->baias = 0;
	new_neuron->next = NULL;

	

	return new_neuron;
}

N * create_hide_level(int level_size)
{
	/*this function create the level with the input in the beginning of the program*/
	N * level = NULL;
	for(size_t i = 0 ; i < level_size; i ++)
	{
		if(level == NULL)
		{
			level = create_new_neuron(0); 
			/*it`s just a initialization, what i really need is the list.*/
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

	return level;
}

void local_inducted_field(N * data_level , N * hide_level, int hide_level_size)
{
	printf("LIF(1)\n");
	N * data_temp = data_level;
	if(data_level == NULL) return;
	printf("LIF(2)\n");
	N * hide_level_temp = hide_level;
	if(hide_level == NULL) return ;
	long double sum = 0;
	printf("LIF(3)\n");
	for(size_t i = 0 ; i < hide_level_size ; i++)
	{
		printf("----------------------------%zu----------------------------\n",i);
		for(size_t j = 0 ; j < final_vetor_size ; j++)
		{
			printf("--------------------%zu--------------------\n",j);
			sum += (data_temp->output * *(hide_level_temp->weight + j));
			printf("LIF(6)\n");
			data_temp = data_temp->next;
		}
		printf("LIF(7)\n");
		sum += hide_level_temp->baias;
		printf("LIF(8)\n");
		hide_level_temp->output = activate_function(sum);
		printf("LIF(9)\n");
		hide_level_temp = hide_level_temp->next;
		printf("LIF(10)\n");
		data_temp = data_level;
		printf("LIF(Final)\n");
	}

	
}