#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

/*use -lm parameter to compile this program*/



#define final_vetor_size 5

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
N * create_empty_layers();
N * initialize_layer(N * node , int qtde_of_neurons, int prev_layer_size);
N * data_layer(long double input[]);
N * create_new_neuron(long double input);
N * create_hidden_layer(int layer_size);
N * local_inducted_field(N * prev_layer , N * present_layer, int prev_layer_size, int present_layer_size);



int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	N * input_data = create_empty_layers(); /*creating the first element to pass to reference to other`s functinos*/
	N * hidden_process = create_empty_layers();
	long double test_vector[] = {2,1, 10, 0 ,-1};
	input_data = data_layer(test_vector);
	hidden_process = create_hidden_layer(5);
	hidden_process = initialize_layer(hidden_process , 5 , 5);

  	N * output_layer = create_new_neuron(0);
  	output_layer = initialize_layer(output_layer, 1 , 5);



	hidden_process =  local_inducted_field(input_data, hidden_process, 5,  5);
	output_layer = local_inducted_field(hidden_process , output_layer , 5 , 1);

	printf("S->%Lf\n",output_layer->output);
	printf("S->%d\n",output_layer->baias);

	N * temp = hidden_process;
	while(temp != NULL) 
	{
		printf("****************************\n");
		printf("Nv = %Lf\n", temp->output);
		printf("B = %d\n", temp->baias);
		printf("1v = %Lf\n", *(temp->weight + 0));
		printf("****************************\n");
		temp =  temp->next;
	}



	return 0;
}


double activate_function(long double n)
{
	return  (1 / (1 + exp((-1)*n)));

}

N * create_empty_layers()
{
	return NULL;

}

N * initialize_layer(N * node , int layer_size, int prev_layer_size)
{
	/*The prev_layer_size variable is use to take the quantity
	of neuron in the previously layer
	*/
	N * temp = node;
	/*now i gonna distribute the vector of weight and baias for the neurons */
	for(size_t i = 0 ; i < layer_size ; i++)
	{ 
		long double  * weight = (long double *) malloc(prev_layer_size * sizeof(long double));
		for(size_t j = 0 ; j < prev_layer_size ; j++)
		{
			/*Inside this for I gonna distribute the weight`s for all neuron. But to see
			more clearly what i do, let`s talk about more the theory.
			All neuron in the region on the input(the data layer) pass his data to the hidden
			layer multiplying by a weight. This weights is in the graphical representetion of
			neural networks is the edges(rows). To implement this is C, i make the neuron of the 
			hidden layer have an array with values that represent the "edges" of the previously
			layer. When the data pass to data layer to the hidden layer, it`s multiplying by this
			weights in the vector in current position of the neuron. For example: if the neuron 2
			of the data layer is passing his value to hidden_layer, is going be multiplicated by the weight
			in position 2 in the vector( 1 if the vector is zero indexed)*/
			//printf("lv(2)%zu\n", j);
			if(temp == NULL)
			{
				printf("NULL\n");
				return NULL;
			}
			else
			{
				
				*(weight + j) = (rand() % (16000 - (-16000) + 1)) + -(16000); 
				
			}
		}
		
		temp->weight = weight;
		temp->baias = (rand() % (16000 - (-16000) + 1)) + -(16000); 
		temp = temp->next;
	}
	temp = node;
	return temp;
}

N * data_layer(long double input[])
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

N * create_hidden_layer(int layer_size)
{
	N * layer = NULL;
	for(size_t i = 0 ; i < layer_size; i ++)
	{
		if(layer == NULL)
		{
			layer = create_new_neuron(0); 
		}
		else if(layer->next == NULL)
		{
			layer->next = create_new_neuron(0);  
		}
		else{
			N * temp = layer;
			while(temp->next != NULL) temp=temp->next;
			temp->next = create_new_neuron(0);
		}
	}

	return layer;
}

N * local_inducted_field(N * prev_layer , N * present_layer, int prev_layer_size, int present_layer_size)
{
	N * data_temp = prev_layer;
	if(prev_layer == NULL) return NULL;
	N * present_temp = present_layer;
	if(present_temp == NULL) return NULL ;
	long double sum = 0;
	for(size_t i = 0 ; i < present_layer_size ; i++)
	{
		sum = 0;
		for(size_t j = 0 ; j < prev_layer_size ; j++)
		{
			sum += (data_temp->output * *(present_temp->weight + j));
			data_temp = data_temp->next;
		}
		sum += present_temp->baias;
		present_temp->output = activate_function(sum);
		present_temp = present_temp->next;
		data_temp = prev_layer;
	}

	present_temp = present_layer;
	return present_layer;
}

