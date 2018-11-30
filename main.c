#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

/*use -lm parameter to compile this program*/



#define final_vetor_size 5

struct Neuron
{

	long double propagated_value; /*the value that enter in the neuron*/
	long double output; /*the value that out of the neuron*/
	double baias;
	long double * weight;
	struct Neuron * next;
};

typedef struct Neuron N;
/*Just N to refer to a Neuron struct*/

/*functions*/
double activate_function(long double n);
N * create_empty_layers();
N * initialize_layer(N * node , int qtde_of_neurons, int prev_layer_size);
N * data_layer(long double normalized_vector[]);
N * create_new_neuron(long double input);
N * create_hidden_layer(int layer_size);
N * propagation_of_values(N * layer , N * next_layer, int layer_size, int next_layer_size);
double error(long double current_error , int expected_output);
N * output_layer_backpropagation(N * hidden_layer, N * output_layer, double global_error, int hidden_layer_size, double go);
double gradient_output(double global_error , double phi);
double phi_line(double n);
double gradient_hidden_layer(long double weight , double phi, double output_gradient);
N * backpropagation(N * layer, N * next_layer, double global_error, int layer_size , int next_layer_size, double go);


int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	double global_error;
	long double test_vector[] = {2,1, 10, 0 ,-1};

	N * input_data = create_empty_layers(); /*creating the first element to pass to reference to other`s functinos*/
	N * hidden_layer = create_empty_layers();
	N * output_layer = create_new_neuron(0);
	input_data = data_layer(test_vector);

	input_data = initialize_layer(input_data, 5, 5);

	hidden_layer = create_hidden_layer(5);
	hidden_layer = initialize_layer(hidden_layer , 5 , 1);
  	output_layer = initialize_layer(output_layer, 1, 0);

  	
	

	input_data = propagation_of_values(input_data, hidden_layer,  5,  5);
	hidden_layer = propagation_of_values(hidden_layer, output_layer,  5,  1);

	global_error = error(output_layer->output , 1);
	printf("ERROR = %lf\n", global_error);
	double go = gradient_output(global_error , phi_line(output_layer->output));
//-----------------------------------until here is fine, the propagation works appropriately--------------------------
	
	while(global_error < 0)
	{
		
		output_layer = output_layer_backpropagation(hidden_layer, output_layer, global_error, 5, go);
		input_data = backpropagation(input_data, hidden_layer, global_error, 5 , 5, go);
		input_data = propagation_of_values(input_data, hidden_layer,  5,  5);
		hidden_layer = propagation_of_values(hidden_layer, output_layer,  5,  1);
		global_error = error(output_layer->output , 1);
		printf("ERROR = %lf\n", global_error);

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

N * initialize_layer(N * node , int layer_size, int next_layer_size)
{
	/*The next layer size variable is use to take the quantity
	of neuron in the next layer
	*/
	N * temp = node;
	/*now i gonna distribute the vector of weight and baias for the neurons */
	for(size_t i = 0 ; i < layer_size ; i++)
	{ 
		long double  * weight = (long double *) malloc(next_layer_size * sizeof(long double));
		for(size_t j = 0 ; j < next_layer_size ; j++)
		{
			
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
		temp->baias = (rand() % (16000 - (-16000) + 1)) + -(16000); //the baias in the data_layer is not used, is only used when is the 
																	//hidden layer or the output layer
		temp = temp->next;

	}
	temp = node;
	return temp;
}

N * data_layer(long double input[])
{
	/*creating the data_layer*/
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
			temp = node;
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
	new_neuron->propagated_value = input;
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

N * propagation_of_values(N * layer , N * next_layer, int layer_size, int next_layer_size)
{
	N * data_temp = layer;
	if(data_temp == NULL) return NULL;

	N * next_layer_temp = next_layer;
	if(next_layer_temp == NULL) return NULL ;

	long double sum = 0;

	for(size_t i = 0 ; i < next_layer_size ; i++)
	{
		sum = 0;
		for(size_t j = 0 ; j < layer_size ; j++)
		{
			sum += (data_temp->output * *(data_temp->weight + i)); //sum the data with the weight in the position of the neuron(represent by i)
			
			data_temp = data_temp->next; //pass to next data
			
		}
		
		sum += next_layer_temp->baias;    //add the baias of the neuron
		next_layer_temp->propagated_value = sum;  //storage the value on the neuron for future calculus
		next_layer_temp->output = activate_function(sum); //the output is the value after the activate function
		next_layer_temp = next_layer_temp->next; //pass to next neuron 
		data_temp = layer; //return the data_temp to first data node to do the calculus again in the next neuron

		
		

	}

	data_temp = layer; //make the present_layer reference to the first node again to return the reference
	return data_temp;
}

double error(long double current_error , int expected_output)
{
	return current_error - expected_output;
}

double phi_line(double n)
{
/*phi line is the derivade of the activate function, used in process to correct the weights and baias */
	double up = exp((-1) * n);
	double down = (1 + exp((-1) * n));
	double phi = up/pow(down, 2);
	return phi;
}

double gradient_output(double global_error , double phi)
{	
	/*the gradient function of the output neuron is a litte different of the gradient function of hidden_layer
	(see the book "Redes Neurais" by Oswaldo Ludwig Jr. and Eduard Montgomery, available in BCE Gama)*/

	return global_error * phi;
}

double gradient_hidden_layer(long double weight , double phi, double output_gradient)
{	
	return  phi * output_gradient * weight;
}

N * output_layer_backpropagation(N * hidden_layer, N * output_layer, double global_error, int hidden_layer_size, double go)
{
	N * temp = hidden_layer;
	double learn_rate = 0.2;
	double delta = 0;
	for(size_t i = 0 ; i < hidden_layer_size ; i++)
	{
		printf("1.1\n");
		delta = 0;
		delta = 2 * learn_rate * temp->output * go;
		printf("1.2\n");
		*(temp->weight + 0) += delta; //the neurons just have one weight because the output_layer just have one neuron
		printf("1.3\n");
		temp = temp->next;
	}
	delta = 0;
	printf("1.4\n");
	delta = 2 * learn_rate * go;
	printf("1.5\n");
	output_layer->baias += delta;

	temp = hidden_layer;
	return temp;
}


N * backpropagation(N * layer, N * next_layer, double global_error, int layer_size , int next_layer_size, double go)
{
	N * layer_temp = layer;
	N * next_layer_temp = next_layer;
	double learn_rate = 0.2;
	double delta = 0;

	for(size_t i = 0 ; i < layer_size ; i++)
	{
		for(size_t j = 0 ; j < next_layer_size ; j++)
		{
			delta = 0;
			delta = 2 * learn_rate * layer_temp->propagated_value * gradient_hidden_layer( *(next_layer_temp->weight), phi_line(next_layer_temp->output), go);
			*(layer_temp->weight + j) += delta;
			delta = 0;
			delta = 2 * learn_rate * gradient_hidden_layer( *(next_layer_temp->weight + 0), phi_line(next_layer_temp->output), go);
			next_layer_temp->baias += delta;
			next_layer_temp = next_layer_temp->next;
		}
		next_layer_temp = next_layer;
		layer_temp = layer_temp->next;

	}

	layer_temp = layer;
	return layer_temp;

}
