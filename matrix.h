//Creación de header para matriz (MATRIX)

#ifndef MATRIX
#define MATRIX

struct Matrix{
	long int nrow;
	long int ncol;
	int t_id; //identificador del thread que usa a la matriz
	double *data;
	Matrix(): nrow(0), ncol(0), t_id(-1), data(nullptr){}
}; 
typedef struct Matrix Matrix;

//Funciones genéricas
void inicializar_aleatorios(Matrix *ptr, int n_row, int n_col);
void imprimir_matriz(Matrix *ptr);


//Operaciones secuenciales

void suma_matrices(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_suma);
void multiplicacion_matrices(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_prod);
void matriz_transpuesta(Matrix *ptr, Matrix *ptr_t);

//Paralelización****

//Suma
void *suma_matrices_pthread(void *args);
void suma_matrices_paralelo(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_suma, int num_threads);

//Multiplicación
void *multiplicacion_matrices_pthread(void *args);
void multiplicacion_matrices_paralelo(Matrix *ptr1, Matrix *ptr2, Matrix *ptr_prod, int num_threads);

//Transpuesta
void matriz_transpuesta_paralelo(Matrix *ptr, Matrix *ptr_t, int num_threads);

#endif
