#include "matrix.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <time.h>

using namespace std;

//Inicializar una matriz (Rangos que debe tener)
#define RMIN 0
#define RMAX 10

//Funciones genéricas


//Matriz con números aleatorios
void inicializar_aleatorios(Matrix *ptr, int n_row, int n_col){
	ptr->nrow = n_row;
	ptr->ncol = n_col;
	if(ptr->data){free(ptr->data);}
	ptr->data = (double *) malloc(ptr->nrow * ptr->ncol * sizeof(double));
	for (int i = 0; i < ptr->nrow; ++i) {
        for (int j = 0; j < ptr->ncol; ++j) {
			ptr->data[i*ptr->ncol+j] = (int) rand() % (RMAX - RMIN + 1) + RMIN;
		}
	}
}

//Imprimir matriz
void imprimir_matriz(Matrix *ptr){
	for (int i = 0; i < ptr->nrow; ++i) {
        for (int j = 0; j < ptr->ncol; ++j) {
			cout << std::setw(5) << ptr->data[i*ptr->ncol+j];
		}
		cout << '\n';
	}
}


//Operaciones secuenciales


//Suma
void suma_matrices(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_suma){
	ptr_suma->nrow = ptr1->nrow;
	ptr_suma->ncol = ptr1->ncol;
	if(ptr_suma->data){free(ptr_suma->data);}
	ptr_suma->data = (double *) malloc(ptr1->nrow * ptr1->ncol * sizeof(double));
	for (int i = 0; i < ptr_suma->nrow; ++i) {
        for (int j = 0; j < ptr_suma->ncol; ++j) {
			ptr_suma->data[i*ptr_suma->ncol+j] = ptr1->data[i*ptr_suma->ncol+j] + ptr2->data[i*ptr_suma->ncol+j];
		}
	}
}
//Multiplicación
void multiplicacion_matrices(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_prod){
	ptr_prod->nrow = ptr1->nrow;
	ptr_prod->ncol = ptr2->ncol;
	if(ptr_prod->data){free(ptr_prod->data);}
	ptr_prod->data = (double *) malloc(ptr1->nrow * ptr2->ncol * sizeof(double));
	//Inicializarla en 0
	for (int i = 0; i < ptr_prod->nrow; i++) {
        for (int j = 0; j < ptr_prod->ncol; j++) {
			ptr_prod->data[i*ptr_prod->ncol+j] = 0;
		}
	}
	
	for (int i = 0; i < ptr_prod->nrow; i++) {
        for (int j = 0; j < ptr_prod->ncol; j++) {
			for(int k = 0; k < ptr1->ncol; k++){
				ptr_prod->data[i*ptr_prod->ncol+j] += ptr1->data[i*ptr1->ncol+k] * ptr2->data[k*ptr2->ncol+j];
            }
		}
	}
	
}

//Transpuesta
void matriz_transpuesta(Matrix *ptr, Matrix *ptr_t){
	ptr_t->nrow = ptr->ncol;
	ptr_t->ncol = ptr->nrow;
	if(ptr_t->data){free(ptr_t->data);}
	ptr_t->data = (double *) malloc(ptr->nrow * ptr->ncol * sizeof(double));
	for (int i = 0; i < ptr->nrow; ++i) {
        for (int j = 0; j < ptr->ncol; ++j) {
			int slice_size = ptr->t_id !=-1 ? ptr->nrow*ptr->t_id : 0;
			ptr_t->data[j* ptr_t->ncol + slice_size +i] = ptr->data[i*ptr->ncol+j];
		}
	}
}


//Paralelización 

//Suma de matrices
void *suma_matrices_pthread(void *args){
	Matrix *array = (Matrix*) args;
	suma_matrices(&array[0], &array[1], &array[2]);
	pthread_exit(NULL);
}

//Tenemos que separa la suma por renglones
void suma_matrices_paralelo(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_suma, int num_threads){
	ptr_suma->nrow = ptr1->nrow;
	ptr_suma->ncol = ptr1->ncol;
	if(ptr_suma->data){free(ptr_suma->data);}
	ptr_suma->data = (double *) malloc(ptr1->nrow * ptr1->ncol * sizeof(double));
	
	pthread_t thread[num_threads];
	pthread_attr_t attr;
		/* Inicializar*/
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

	int thread_id, pos_id, chunk_size, ncell_id, nrow_id, suma_size;
	chunk_size = (int) ceil((double) ptr1->nrow/num_threads) * ptr1->ncol;
	suma_size = ptr1->nrow * ptr1->ncol;
	Matrix *args_id = (Matrix *) malloc(num_threads * 3 * sizeof(Matrix));
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pos_id = thread_id * chunk_size;
		ncell_id = ((suma_size - pos_id) / chunk_size >= 1) ? chunk_size : (suma_size-pos_id) % chunk_size;
		nrow_id = (int) ncell_id/ptr1->ncol;
		//Sumbatriz de m1
		args_id[thread_id*3+0].nrow = nrow_id;
		args_id[thread_id*3+0].ncol = ptr1->ncol;
		args_id[thread_id*3+0].data = (ptr1->data + pos_id);
		//Submatriz de m2
		args_id[thread_id*3+1].nrow = nrow_id;
		args_id[thread_id*3+1].ncol = ptr1->ncol;
		args_id[thread_id*3+1].data = (ptr2->data + pos_id);

		pthread_create(&thread[thread_id], &attr, suma_matrices_pthread, (void *)(args_id + 3*thread_id));
	}
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pthread_join(thread[thread_id], NULL); 
	}
	
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pos_id = thread_id * chunk_size;
		ncell_id = ((suma_size - pos_id) / chunk_size >= 1) ? chunk_size : (suma_size-pos_id) % chunk_size;
		nrow_id = (int) ncell_id/ptr1->ncol;
		std::memcpy(ptr_suma->data + pos_id, args_id[thread_id*3+2].data, nrow_id * ptr1->ncol * sizeof(double));
	}
	free(args_id);
}



//Multiplicación de matrices
void *multiplicacion_matrices_pthread(void *args){
	Matrix *array = (Matrix*) args;
	multiplicacion_matrices(&array[0], &array[1], &array[2]);
	pthread_exit(NULL);
}

//Separa los productos por renglones
void multiplicacion_matrices_paralelo(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_prod, int num_threads){
	ptr_prod->nrow = ptr1->nrow;
	ptr_prod->ncol = ptr2->ncol;
	if(ptr_prod->data){free(ptr_prod->data);}
	ptr_prod->data = (double *) malloc(ptr1->nrow * ptr2->ncol * sizeof(double));

	pthread_t thread[num_threads];
	pthread_attr_t attr;
		/* Inicializar*/
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

	int thread_id, pos_id, chunk_size, ncell_id, nrow_id, ptr1_size, prod_size;
	chunk_size = (int) ceil((double) ptr1->nrow/num_threads) * ptr1->ncol;
	ptr1_size = ptr1->nrow * ptr1->ncol;
	prod_size = ptr1->nrow * ptr2->ncol;
	Matrix *args_id = (Matrix *) malloc(num_threads * 3 * sizeof(Matrix));
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pos_id = thread_id * chunk_size;
		ncell_id = ((ptr1_size - pos_id) / chunk_size >= 1) ? chunk_size : (ptr1_size-pos_id) % chunk_size;
		nrow_id = (int) ncell_id/ptr1->ncol;
		//Submatriz de m1
		args_id[thread_id*3+0].nrow = nrow_id;
		args_id[thread_id*3+0].ncol = ptr1->ncol;
		args_id[thread_id*3+0].data = (ptr1->data + pos_id);
		//Submatriz de m2
		args_id[thread_id*3+1] = *ptr2;
		pthread_create(&thread[thread_id], &attr, multiplicacion_matrices_pthread, (void *)(args_id + 3*thread_id));
	}
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pthread_join(thread[thread_id], NULL); 
	}
	
	chunk_size = (int) ceil((double) ptr_prod->nrow/num_threads) * ptr_prod->ncol;
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pos_id = thread_id * chunk_size;
		ncell_id = ((prod_size - pos_id) / chunk_size >= 1) ? chunk_size : (prod_size-pos_id) % chunk_size;
		nrow_id = (int) ncell_id/ptr_prod->ncol;
		std::memcpy(ptr_prod->data + pos_id, args_id[thread_id*3+2].data, nrow_id * ptr2->ncol * sizeof(double));
	}
	free(args_id);
}

//No es posible utilizar la función secuencial en transpuesta con pthread
//por tanto, no se lee.
void *matriz_transpuesta_pthread(void *args){
	Matrix *array = (Matrix*) args;
	Matrix *ptr = &array[0], *ptr_t = &array[1];
	if(!ptr_t->data){
		ptr_t->data = (double *) malloc(ptr->nrow * ptr->ncol * sizeof(double));
	}

	for (int i = 0; i < ptr->nrow; ++i) {
        for (int j = 0; j < ptr->ncol; ++j) {
			int slice_size = ptr->t_id !=-1 ? ptr->nrow*ptr->t_id : 0;
			ptr_t->data[j* ptr_t->ncol + slice_size +i] = ptr->data[i*ptr->ncol+j];
		}
	}
	pthread_exit(NULL);
}

void matriz_transpuesta_paralelo(Matrix *ptr, Matrix *ptr_t, int num_threads){
	ptr_t->nrow = ptr->ncol;
	ptr_t->ncol = ptr->nrow;
	if(!ptr_t->data){
		ptr_t->data = (double *) malloc(ptr->nrow * ptr->ncol * sizeof(double));
	}
	
	pthread_t thread[num_threads];
	pthread_attr_t attr;
		/* Inicializar*/
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

	int thread_id, pos_id, chunk_size, ncell_id, nrow_id, t_size;
	chunk_size = (int) ceil((double) ptr->nrow/num_threads) * ptr->ncol;
	t_size = ptr->nrow * ptr->ncol;
	Matrix *args_id = (Matrix *) malloc(num_threads * 2 * sizeof(Matrix));
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pos_id = thread_id * chunk_size;
		ncell_id = ((t_size - pos_id) / chunk_size >= 1) ? chunk_size : (t_size-pos_id) % chunk_size;
		nrow_id = (int) ncell_id/ptr->ncol;
		//Submatriz de m1
		args_id[thread_id*2+0].nrow = nrow_id;
		args_id[thread_id*2+0].ncol = ptr->ncol;
		args_id[thread_id*2+0].t_id = thread_id; //se pasan los hilos
		args_id[thread_id*2+0].data = (ptr->data + pos_id);
		args_id[thread_id*2+1] = *ptr_t;

		pthread_create(&thread[thread_id], &attr, matriz_transpuesta_pthread, (void *)(args_id + 2*thread_id));
	}
	for(thread_id = 0; thread_id < num_threads; thread_id++){
		pthread_join(thread[thread_id], NULL); 
	}
}


int main(){
	
	clock_t inicio, fin;
	double tiempo;
	Matrix m1, *ptr1 = &m1, m2, *ptr2 = &m2, m3, *ptr3 = &m3, resultado, *ptr_resultado = &resultado;
	//crear la primera matriz aleatoria
	inicializar_aleatorios(ptr1,5000,5000);
	//traspuesta
	inicio=clock();
	matriz_transpuesta(ptr1,ptr_resultado);
	fin=clock();
	tiempo=((double) (fin-inicio));
	cout<<"El tiempo para trasponer de manera secuencial es: "<<endl<<tiempo<<endl;
	for (int i = 0; i < 4; ++i) {
	inicio=clock();
	matriz_transpuesta_paralelo(ptr1,ptr_resultado,pow(2,(2*i+1)));
	fin=clock();
	tiempo=((double) (fin-inicio));
	cout<<"El tiempo para trasponer de manera paralela con "<<pow(2,(2*i+1))<<" threads es: "<<endl<<tiempo<<endl;	
	}
	
	//liberacion de memoria
	free(ptr1->data);
	
	//crear las segunda y tercera matriz para sumar y multiplicar
	inicializar_aleatorios(ptr2,5000,5000);
	inicializar_aleatorios(ptr3,5000,5000);
	//suma
	inicio=clock();
	suma_matrices(ptr2,ptr3,ptr_resultado);
	fin=clock();
	tiempo=((double) (fin-inicio));
	cout<<"El tiempo para sumar de manera secuencial es: "<<endl<<tiempo<<endl;
	for (int i = 0; i < 4; ++i) {
	inicio=clock();
	suma_matrices_paralelo(ptr2,ptr3,ptr_resultado,pow(2,(2*i+1)));
	fin=clock();
	tiempo=((double) (fin-inicio));
	cout<<"El tiempo para sumar de manera paralela con "<<pow(2,(2*i+1))<<" threads es: "<<endl<<tiempo<<endl;	
	}
	//producto
	inicio=clock();
	multiplicacion_matrices(ptr2,ptr3,ptr_resultado);
	fin=clock();
	tiempo=((double) (fin-inicio));
	cout<<"El tiempo para multiplicar de manera secuencial es: "<<endl<<tiempo<<endl;
	for (int i = 0; i < 4; ++i) {
	inicio=clock();
	multiplicacion_matrices_paralelo(ptr2,ptr3,ptr_resultado,pow(2,(2*i+1)));
	fin=clock();
	tiempo=((double) (fin-inicio));
	cout<<"El tiempo para multiplicar de manera paralela con "<<pow(2,(2*i+1))<<" threads es: "<<endl<<tiempo<<endl;	
	}







	return 0;	
}
