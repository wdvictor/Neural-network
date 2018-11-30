#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <string.h>

/*use -lm parameter to compile this program*/
// ********************************************************************************************
// | > TRABALHO 6																				|
// |------------------------------------------------------------------------------------------- |
// |	# ALUNO(S):																				|
// |		- JULIO CESAR LITWIN LIMA	| MATRÍCULA: 16/0129443									|
// |		- VICTOR HUGO 				| MATRÍCULA: 17/0063844									|
// |                                                                                            |
// |																							|
// ********************************************************************************************/





#define final_vetor_size 536 
#define vetors_of_vectors 25 //is the size of the training/test set
#define era 1000

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

/*functions of the neural network*/
double activate_function(long double n);
N * create_empty_layers();
N * initialize_layer(N * node , int layer_size, int next_layer_size);
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
/*--------------------------------------------------*/

/*function of the image_processor*/

int *alocate_image(FILE *fp,char directory[], int ncols , int nrows);
void imagesize(FILE *fp, char directory[], int *ncols , int *nrows);
long double *ilbp(int *img1, int nrows , int cols);
long double *glcm(int *img ,int ncols , int nrows);
int random_database(char **training, char **tests, char **directory);
long double *normalization(long double *final);

int main(int argc, char const *argv[])
{
	time_t t;
	srand((unsigned) time(&t));
	double global_error;
	long double test_vector[] = {2,1, 10, 0 ,-1};

	FILE *fp;

 	char *directorys_asphalt[50] = {"asphalt/asphalt_01.txt", "asphalt/asphalt_02.txt","asphalt/asphalt_03.txt", "asphalt/asphalt_04.txt",
	    "asphalt/asphalt_05.txt","asphalt/asphalt_06.txt", "asphalt/asphalt_07.txt", "asphalt/asphalt_08.txt", "asphalt/asphalt_09.txt",
	    "asphalt/asphalt_10.txt", "asphalt/asphalt_11.txt", "asphalt/asphalt_12.txt", "asphalt/asphalt_13.txt",
	    "asphalt/asphalt_14.txt","asphalt/asphalt_15.txt", "asphalt/asphalt_16.txt", "asphalt/asphalt_17.txt", "asphalt/asphalt_18.txt",
	    "asphalt/asphalt_19.txt", "asphalt/asphalt_20.txt", "asphalt/asphalt_21.txt", "asphalt/asphalt_22.txt", "asphalt/asphalt_23.txt",
	    "asphalt/asphalt_24.txt", "asphalt/asphalt_25.txt", "asphalt/asphalt_26.txt", "asphalt/asphalt_27.txt", "asphalt/asphalt_28.txt",
	    "asphalt/asphalt_29.txt", "asphalt/asphalt_30.txt", "asphalt/asphalt_31.txt", "asphalt/asphalt_32.txt", "asphalt/asphalt_33.txt",
	    "asphalt/asphalt_34.txt", "asphalt/asphalt_35.txt", "asphalt/asphalt_36.txt", "asphalt/asphalt_37.txt", "asphalt/asphalt_38.txt"
	    ,"asphalt/asphalt_39.txt", "asphalt/asphalt_40.txt", "asphalt/asphalt_41.txt", "asphalt/asphalt_42.txt", "asphalt/asphalt_43.txt",
	    "asphalt/asphalt_44.txt", "asphalt/asphalt_45.txt", "asphalt/asphalt_46.txt", "asphalt/asphalt_47.txt",
	    "asphalt/asphalt_48.txt", "asphalt/asphalt_49.txt", "asphalt/asphalt_50.txt"};
 	char *directorys_grass[50] = {"grass/grass_01.txt", "grass/grass_02.txt","grass/grass_03.txt", "grass/grass_04.txt",
	    "grass/grass_05.txt","grass/grass_06.txt", "grass/grass_07.txt", "grass/grass_08.txt", "grass/grass_09.txt",
	    "grass/grass_10.txt", "grass/grass_11.txt", "grass/grass_12.txt", "grass/grass_13.txt",
	    "grass/grass_14.txt","grass/grass_15.txt", "grass/grass_16.txt", "grass/grass_17.txt", "grass/grass_18.txt",
	    "grass/grass_19.txt", "grass/grass_20.txt", "grass/grass_21.txt", "grass/grass_22.txt", "grass/grass_23.txt",
	    "grass/grass_24.txt", "grass/grass_25.txt", "grass/grass_26.txt", "grass/grass_27.txt", "grass/grass_28.txt",
	    "grass/grass_29.txt", "grass/grass_30.txt", "grass/grass_31.txt", "grass/grass_32.txt", "grass/grass_33.txt",
	    "grass/grass_34.txt", "grass/grass_35.txt", "grass/grass_36.txt", "grass/grass_37.txt", "grass/grass_38.txt"
	    ,"grass/grass_39.txt", "grass/grass_40.txt", "grass/grass_41.txt", "grass/grass_42.txt", "grass/grass_43.txt",
	    "grass/grass_44.txt", "grass/grass_45.txt", "grass/grass_46.txt", "grass/grass_47.txt",
	    "grass/grass_48.txt", "grass/grass_49.txt", "grass/grass_50.txt"};

  int *img;
  int *_ncols, *_nrows;
  char **training_asphalt = (char **) malloc(25 * sizeof(char *));
  char **training_grass = (char **) malloc(25 * sizeof(char *));
  char **tests_asphalt = (char **) malloc(25 * sizeof(char *));
  char **tests_grass = (char **) malloc(25 * sizeof(char *));

  long double *ilbp_vector;
  long double *glcm_vector;
  long double *final = (long double *) calloc(final_vetor_size, sizeof(long double ));

  //-------------------------------------------------------------
  random_database(training_asphalt, tests_asphalt , directorys_asphalt);
  random_database(training_grass, tests_grass, directorys_grass);
  //--------------------------------------------------------------
  _ncols = (int *) malloc(sizeof(int));
  _nrows = (int *) malloc(sizeof(int));
  img = (int *) malloc(*_ncols * *_nrows * sizeof(int));

   int count = 0;
   N * input_data = create_empty_layers();
   
		 /*creating the first element to pass to reference to other`s functinos*/
	N * hidden_layer = create_empty_layers();
	N * output_layer = create_new_neuron(0);
  /*training the neural network for asphalt*/
  for(int i = 0 ; i < vetors_of_vectors  ; i++){
  	printf("step1:part:%d of 25\n", i);
	  	/*image processor functions */
	    imagesize(fp , *(training_asphalt+i), _ncols, _nrows); //set the size of the image and the number of cols and rows of this image
	    img = alocate_image(fp, *(training_asphalt+i),  *_ncols, *_nrows); //alocate the image in the memory
	    final = ilbp(img, *_nrows, *_ncols); //do the ilbp calc
	    memcpy(final + 512 , glcm(img, *_ncols, *_nrows) , 24 * sizeof(long double)); //do the glcm calc anc concatenate both
	    //-----------------------
	    /*neural network calculus */

	    
		input_data = data_layer(normalization(final));
		input_data = initialize_layer(input_data, final_vetor_size, atoi(argv[1]));
		
		hidden_layer = create_hidden_layer(atoi(argv[1]));
		hidden_layer = initialize_layer(hidden_layer , atoi(argv[1]) , 1);
	  	output_layer = initialize_layer(output_layer, 1, 0);
		input_data = propagation_of_values(input_data, hidden_layer,  final_vetor_size,  atoi(argv[1]));
		hidden_layer = propagation_of_values(hidden_layer, output_layer,  atoi(argv[1]),  1);
		global_error = error(output_layer->output , 1);
		double go = gradient_output(global_error , phi_line(output_layer->output));
		
		
		while(global_error < 0.2)
		{
			double go = gradient_output(global_error , phi_line(output_layer->output));
			output_layer = output_layer_backpropagation(hidden_layer, output_layer, global_error, atoi(argv[1]), go);
			input_data = backpropagation(input_data, hidden_layer, global_error, final_vetor_size , atoi(argv[1]), go);
			input_data = propagation_of_values(input_data, hidden_layer,  final_vetor_size,  atoi(argv[1]));
			hidden_layer = propagation_of_values(hidden_layer, output_layer,  atoi(argv[1]),  1);
			global_error = error(output_layer->output , 1);
			count++;
			if(count == era) break;
		}
		count = 0;
	}

	count = 0;
	for(int i = 0 ; i < vetors_of_vectors  ; i++)
	{
		printf("step2:part:%d of 25\n", i);
	    imagesize(fp , *(training_grass+i), _ncols, _nrows);
	    img = alocate_image(fp, *(training_grass+i),  *_ncols, *_nrows);
	    final = ilbp(img, *_nrows, *_ncols);
	    memcpy(final + 512 , glcm(img, *_ncols, *_nrows) , 24 * sizeof(long double));
	    

	    N * input_data = create_empty_layers();
	    input_data = data_layer(normalization(final));
		 /*creating the first element to pass to reference to other`s functinos*/
		N * hidden_layer = create_empty_layers();
		N * output_layer = create_new_neuron(0);
	
		input_data = initialize_layer(input_data, final_vetor_size, atoi(argv[1]));
		
		hidden_layer = create_hidden_layer(atoi(argv[1]));
		hidden_layer = initialize_layer(hidden_layer , atoi(argv[1]) , 1);
	  	output_layer = initialize_layer(output_layer, 1, 0);
		input_data = propagation_of_values(input_data, hidden_layer,  final_vetor_size,  atoi(argv[1]));
		hidden_layer = propagation_of_values(hidden_layer, output_layer,  atoi(argv[1]),  1);
		global_error = error(output_layer->output , 0);
		
		double go = gradient_output(global_error , phi_line(output_layer->output));
		
		int count = 0;
		while(global_error > 0.2)
		{
			double go = gradient_output(global_error , phi_line(output_layer->output));
			output_layer = output_layer_backpropagation(hidden_layer, output_layer, global_error, atoi(argv[1]), go);
			input_data = backpropagation(input_data, hidden_layer, global_error, final_vetor_size , atoi(argv[1]), go);
			input_data = propagation_of_values(input_data, hidden_layer,  final_vetor_size,  atoi(argv[1]));
			hidden_layer = propagation_of_values(hidden_layer, output_layer,  atoi(argv[1]),  1);
			global_error = error(output_layer->output , 1);
			count++;
			if(count == era) break;
		}
		count = 0;
	}


	double t1 = 0 , f1 = 0;
	for(int i = 0 ; i < vetors_of_vectors  ; i++)
	{
	printf("step3:part:%d of 25\n", i);
    imagesize(fp , *(tests_grass+i), _ncols, _nrows);
    img = (int *) malloc(*_ncols * *_nrows * sizeof(int));
    final = ilbp(img, *_nrows, *_ncols);
    memcpy(final + 512 , glcm(img, *_ncols, *_nrows) , 24 * sizeof(long double));
    printf("1\n");
    input_data = data_layer(normalization(final)); //creating the data layer
    input_data = propagation_of_values(input_data, hidden_layer,  final_vetor_size,  atoi(argv[1])); //propaging the data in the data layer to hidden layer
	hidden_layer = propagation_of_values(hidden_layer, output_layer,  atoi(argv[1]),  1); //propaging the data in the hidden layer for output layer

	

	if(output_layer->output <= 0.5) //if the neural network classify the image how grass, true, otherwise, false
	{
		t1++;
	}
	else
	{
		f1++;
	}

    
  }
  double t2 = 0 , f2 = 0;
  for(int i = 0 ; i < vetors_of_vectors  ; i++)
  {
  	printf("step4:part:%d of 25\n", i);
    imagesize(fp , *(tests_asphalt+i), _ncols, _nrows);
    img = alocate_image(fp, *(tests_asphalt+i),  *_ncols, *_nrows);
    final = ilbp(img, *_nrows, *_ncols);
    memcpy(final + 512 , glcm(img, *_ncols, *_nrows) , 24 * sizeof(long double));
 
    input_data = data_layer(normalization(final)); //creating the data layer
    input_data = propagation_of_values(input_data, hidden_layer,  final_vetor_size,  atoi(argv[1])); //propaging the data in the data layer to hidden layer
	hidden_layer = propagation_of_values(hidden_layer, output_layer,  atoi(argv[1]),  1); //propaging the data in the hidden layer for output layer

	if(output_layer->output <= 0.5) //if the neural network classify the image how grass, true, otherwise, false
	{
		t2++;
	}
	else
	{
		f2++;
	}

 	}

 	printf("Accept rate = %lf%%\n", (t1+t2)/100);
 	printf("False Accept rate = %lf%%\n", f2/100);
 	printf("False rejection rate = %lf%%\n", f1/100);



	return 0;
}

/*Function of the neural network*/
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
		delta = 0;
		delta = 2 * learn_rate * temp->output * go;		
		*(temp->weight + 0) += delta; //the neurons just have one weight because the output_layer just have one neuron
		
		temp = temp->next;
	}
	delta = 0;
	delta = 2 * learn_rate * go;
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


/*End of the function of the neural network*/


/*Function of the image processor */

void imagesize(FILE *fp, char directory[], int *ncols , int *nrows){
  int imagesize , n_cols, n_rows;
  char c;
   fp = fopen(directory, "r");
   if(fp == NULL){
     printf("File %s not found!\n", directory);
     exit(1);
   }
   //------------------------------
   imagesize=0;n_cols = 0;n_rows = 0;
   while( c = fgetc(fp)){
     if (c == EOF) break;
     if( c == ';') n_cols +=1;
     if(c == '\n') {
       n_cols +=1;
       n_rows +=1;
     }
     imagesize +=1;
   }
   ////////////////--------------
  n_cols = n_cols/n_rows;
  *ncols = n_cols;
  *nrows = n_rows;
}

long double *normalization(long double *final)
{
  unsigned long long int min, max;
  long double *normalization = (long double *) calloc(final_vetor_size , sizeof(long double));
  for(int i = 0 ; i < final_vetor_size ; i++){
      //printf("x[%d] = %LF\n", i, *(final + i));
    if(i == 0) min = *(final + i);
    if(i == 0) max = *(final + i);
    if(*(final + i) > max){
      max = *(final + i);
    }
    if(*(final + i) < min){
      min = *(final + i);
    }
  }

  for(int i = 0 ; i < final_vetor_size ; i++){
    *(normalization + i) = ((*(final + i) - min)/ (max - min));
  }


  return normalization;
}

long double *ilbp(int *img, int nrows , int ncols)
{
  unsigned long long int *binary_matrix;
  long double *ilbp;
  int short bit[9], bit_temp[9];
  int c_bit = 0;
  int dec_value = 0;
  int min = 0, avg = 0;

  ilbp = (long double *) calloc(512 , sizeof(long double));

  binary_matrix = (unsigned long long int*) malloc((nrows * ncols) * sizeof(unsigned long long int));
  //make the avarage and replacing in the central pixel
  for(unsigned short int i = 1 ; i < (nrows-1) ; i++)
  {
    for(unsigned short int j =  1; j < (ncols-1) ; j++)
    {
      // IBLP = Sum(gc-avg(gp));
      avg = ((*(img + ((i-1) * ncols) + (j-1)) +
      *(img + ((i-1) * ncols) + j) +
      *(img + ((i-1) * ncols) + j+1) +
      *(img + ((i) * ncols) + j-1) +
      *(img + ((i) * ncols) + j) +
      *(img + ((i) * ncols) + j+1) +
      *(img + ((i+1) * ncols) + j-1) +
      *(img + ((i+1) * ncols) + j) +
      *(img + ((i+1) * ncols) + j+1))/ 9);

      //verifying if the pixel are more or equal of the avarage
      //if there is, 1, otherwise 0
      if( *(img + ((i-1) * ncols) + (j-1)) >= avg )
      {
        binary_matrix[((i-1) * ncols) + (j-1)] = 1;
      }
      else 
      {
          binary_matrix[((i-1) * ncols) + (j-1)] = 0;
      }
      if(*(img + ((i-1) * ncols) + (j)) >= avg)
      {
        binary_matrix[((i-1) * ncols) + (j)] = 1;
      }
      else 
      {
          binary_matrix[((i-1) * ncols) + (j)] = 0;
      }
      if( (*(img + ((i-1) * ncols) + (j+1)))  >= avg)
      {
        binary_matrix[((i-1) * ncols) + (j+1)] = 1;
      }
      else 
      {
          binary_matrix[((i-1) * ncols) + (j+1)] = 0;
      }
      if(*(img + (i * ncols) + (j-1)) >= avg)
      {
        binary_matrix[((i * ncols) + (j-1))] = 1;
      }
      else
      {
          binary_matrix[((i * ncols) + (j-1))] = 0;
      }
      if( (*(img + ((i) * ncols) + (j)) >= avg))
      {
        binary_matrix[((i * ncols) + (j))] = 1;
      }
      else 
      {
          binary_matrix[((i * ncols) + (j))] = 0;
      }
      if(  (*(img + ((i) * ncols) + (j+1))) >= avg)
      {
        binary_matrix[((i * ncols) + (j+1))] = 1;
      }
      else
      {
        binary_matrix[((i * ncols) + (j+1))] = 0;
      }

      if( (*(img + ((i+1) * ncols) + (j-1))) >= avg)
      {
        binary_matrix[(((i+1) * ncols) + (j-1))] = 1;
      }
      else
      {
          binary_matrix[(((i+1) * ncols) + (j-1))] = 0;
      }
      if((*(img + ((i+1) * ncols) + (j))) >= avg)
      {
        binary_matrix[(((i+1) * ncols) + (j))] = 1;
      }
      else
      {
          binary_matrix[(((i+1) * ncols) + (j))] = 0;
      }
      if((*(img + ((i+1) * ncols) + (j+1))) >= avg)
      {
        binary_matrix[(((i+1) * ncols) + (j+1))] = 1;
      }
      else 
      {
          binary_matrix[(((i+1) * ncols) + (j+1))] = 0;
      }
    }
  }
  //create the rotations calc'scanf
  for(int i = 1 ; i < (nrows-1) ; i++)
  {
    for(int j =  1; j < (ncols-1) ; j++)
    {
      bit[0] = *(binary_matrix + ((i-1) * (ncols-1)) + (j-1));
      bit[1] = *(binary_matrix + ((i-1) * (ncols-1)) + (j));
      bit[2] = *(binary_matrix + ((i-1) * (ncols-1)) + (j+1));

      bit[3] = *(binary_matrix + ((i) * (ncols-1)) + (j+1));
      bit[4] = *(binary_matrix + ((i+1) * (ncols-1)) + (j+1));

      bit[5] = *(binary_matrix + ((i+1) * (ncols-1)) + (j));
      bit[6] = *(binary_matrix + ((i+1) * (ncols-1)) + (j-1));
      bit[7] = *(binary_matrix + ((i) * (ncols-1)) + (j-1));
      bit[8] = *(binary_matrix + ((i) * (ncols-1)) + (j));
      for(int i = 0; i < 9 ; i++)
      {
        bit_temp[i] = bit[i];
      }
      for(int i = 0 ; i <= 8; i++)
      { //rotation
        for(int i = 8, j = 0; i >= 0; i--,j++)
        {
          if(bit[i] == 1)
          {
            dec_value += (int )pow(2 , j);
          }
        }
        if(i == 0)
        {
          min = dec_value;
        }
        if(dec_value < min)
        {
          min = dec_value;
        }
        *(ilbp + min) +=1;
        dec_value = 0;

        bit[0] = bit_temp[7];
        bit[1] = bit_temp[0];
        bit[2] = bit_temp[1];
        bit[3] = bit_temp[2];
        bit[4] = bit_temp[3];
        bit[5] = bit_temp[4];
        bit[6] = bit_temp[5];
        bit[7] = bit_temp[6];
        bit[8] = bit_temp[8];

        bit_temp[0] = bit[0];
        bit_temp[1] = bit[1];
        bit_temp[2] = bit[2];
        bit_temp[3] = bit[3];
        bit_temp[4] = bit[4];
        bit_temp[5] = bit[5];
        bit_temp[6] = bit[6];
        bit_temp[7] = bit[7];
        bit_temp[8] = bit[8];
      }
    }
  }
  free(binary_matrix);
  return ilbp;
}

int *alocate_image(FILE *fp,char directory[], int ncols , int nrows)
{
  char *trash;
  int *img;
  img = (int *) malloc(ncols * nrows * sizeof(int));
  trash = malloc(ncols * nrows * 2 * sizeof(char));
  fp = fopen(directory, "r");
  if(fp == NULL){
    printf("File %s not found!\n", directory);
    exit(1);
  }
  for(int i = 0; i< nrows; i++){
    for(int j = 0 ; j< ncols ; j++){
      fscanf(fp , "%d%c", (img + (i * ncols)+j) , (trash + (i * ncols)+j));
    }
  }
  free(trash);
  fclose(fp);
  return img;
}

long double *glcm(int *img ,int ncols , int nrows)
{
  int **glcm = ( int **) calloc(8 , sizeof( int *));
  long double *metrics = (long double *) calloc(24 , sizeof(long double)); 
  for(size_t k = 0 ; k < 8 ; k++){
  *(glcm + k) = ( int *) calloc(256*256 , sizeof(int));
    }
 
  for(int i = 0 ; i < nrows;i++){
    for(int j = 0 ; j < (ncols-1); j++){
      *(*glcm + (*(img + (i * ncols-1) + j) * 256) + *(img + (i * ncols-1) + (j+1))) += 1;    
    }
  }
  
  for(int i = 0; i < nrows;i++){
    for(int j = 1 ; j < (ncols); j++){
      *(*(glcm + 1) + (*(img + (i * ncols) + (j-1)) * 256) + *(img + (i * ncols-1) + j)) += 1;
    }
  }
  
  for(int i = 1; i < nrows;i++){
    for(int j = 1 ; j < (ncols); j++){
      *(*(glcm + 2) + (*(img + ((i-1) * ncols) + (j-1)) * 256) + *(img + (i * ncols-1) + j)) +=1;
    }
  }
  
 for(int i = 1; i < nrows;i++){
    for(int j = 1 ; j < ncols; j++){
      *(*(glcm + 3) + (*(img + ((i-1) * ncols) + j) * 256) + *(img + (i * ncols-1) + j) ) +=1;      }
    }
   
    for(int i = 1; i < nrows;i++){
      for(int j = 1 ; j < ncols; j++){
        *(*(glcm + 4) + (*(img + ((i-1) * ncols) + (j+1)) * 256) + *(img + (i * ncols-1) + j)) +=1;
      }
    }

    for(int i = 0; i < (nrows - 1);i++){
      for(int j = 1 ; j < (ncols); j++){
        *(*(glcm + 5) + (*(img + ((i+1) * ncols) + (j-1)) * 256) + *(img + (i * ncols-1) + j)) +=1;
      }
    }
 
    for(int i = 0 ; i < (nrows-1);i++){
      for(int j = 0 ; j < (ncols); j++){
        *(*(glcm + 6) + (*(img + ((i+1) * ncols) + (j)) * 256) + *(img + (i * ncols-1) + j)) +=1;
      }
    }

  
    for(int i = 1; i < (nrows)-1;i++){
      for(int j = 1 ; j < (ncols-1); j++){
        *(*(glcm + 7) + (*(img + ((i+1) * ncols) + (j+1)) * 256) + *(img + (i * ncols-1) + j) ) +=1;
      }
    }

  for(int k = 0 ; k < 8 ; k++){
    for(int i = 0 ; i < 255 ; i++){
      for(int j = 0; j < 256 ; j++){
        *(metrics + k) += (pow((i-j), 2) * *(*(glcm + k) + (i+j)));
        }
      }
 
    
    for(int i = 0 ; i < 256 ; i++){
      for(int j = 0; j < 256 ; j++){
      *(metrics + k + 8) += (pow( *(*(glcm + k) + (i+j)) , 2));
      }
    }
   
    int mod = 0;
    for(int i = 0 ; i < 255 ; i++){
      for(int j = 0; j < 256 ; j++){
        if((i-j) < 0){
          mod = (i-j) * (-1);
        }else{
          mod = i-j;
        }
      *(metrics + k + 16) += (( *(*(glcm + k) + (i+j)) ) / (1 + mod));

      }
    }
  }
  
  return metrics;
}
int random_database(char **training, char **tests, char **directory)
{
  int ar_sel[50] = {0};
  int r = 0;
  for(int i = 0 ; i < 25 ; i++){
    if (i == 0){
      r = rand() % 50;
      ar_sel[r] = 1;
      *(training + i) = (char *) malloc(50 * sizeof(char));
      *(training + i) = *(directory + r);
      continue;
    }
    r = rand() % 50;
    if(ar_sel[r] == 0){
      ar_sel[r] = 1;
      training[i] = (char *) malloc(50 * sizeof(char));
      training[i] = *(directory + r);
    }else{
      i--;
    }
  }
  int c= 0;
  for(int i = 0 ; i < 50 ; i++){
    if(ar_sel[i] == 0){
      tests[c] = (char *) malloc(50 * sizeof(char));
      tests[c] = *(directory + i);
      c++;
    }else{
      continue;
      c++;
    }

  }

 return 0;

}