//Header (Creación del header con las especificaciones para la clase matriz)
#include <iostream>
#include <fstream>
#include <string>
#include<cstdio>
#include<cstdlib>
#include<time.h>

using namespace std;

// Crear la clase matriz
// Constructor
class matriz
{
private:
	int renglones1;
	int columnas1;
	double **values1;

public:
	matriz(){
		renglones1=0;
    	columnas1=0;
        values1= NULL;	
	}
    matriz(int r, int c){
        renglones1=r;
        columnas1=c;
        values1= new double *[r];
        srand (time(0));
        for(int i=0;i<r;i++)
        {
            values1[i]= new double [c];
            for(int j=0;j<c;j++)
            {
                values1[i][j] = (double) (rand() % 100);
            }
        }
    }; /*  Fin del constructor */
    
    
    //Imprimir matriz
    
    void imprimir_matriz()
    {
        for(int i=0;i<renglones1;i++)
        {
            for(int j=0;j<columnas1;j++)
            {
                	cout << values1[i][j] << " ";
            }
            cout <<endl;
        }
    };
    
    
//Método de suma (Sobrecarga del operador +) La suma se realiza del lado izquierdo del operador suma

matriz operator+(matriz der){
	int a,b,c,d;
		a=this-> renglones1;
		b=this-> columnas1;
		c=der.renglones1;
		d=der.columnas1;
		matriz resultado;
			if(!((a==c)&&(b==d))){
		
					cout << "No se puede realizar la suma de estas dos matrices" << endl<< endl;
						return resultado;  
	} else{
		matriz resultado(a,b);
		for(int i=0;i<a;i++){
			for(int j=0;j<b;j++){
				resultado.values1[i][j]=this->values1[i][j]+der.values1[i][j];
			}
		}
		cout <<"El resultado de la suma de matrices es:"<< endl<< endl;
		resultado.imprimir_matriz();
		return resultado;
	}
	
}

//Método de resta (Sobrecarga del operador -) La resta se realiza del lado izquierdo del operador resta

matriz operator-(matriz der){
	int a,b,c,d;
		a=this-> renglones1;
		b=this-> columnas1;
		c=der.renglones1;
		d=der.columnas1;
		matriz resultado;
			if(!((a==c)&&(b==d))){
		
					cout << "No se puede realizar la resta de estas matrices" << endl<< endl;
						return resultado;  
	} else{
		matriz resultado(a,b);
		for(int i=0;i<a;i++){
			for(int j=0;j<b;j++){
				resultado.values1[i][j]=this->values1[i][j]-der.values1[i][j];
			}
		}
		cout <<"El resultado de la resta de estas matrices es:"<< endl<< endl;
		
		resultado.imprimir_matriz();
		return resultado;
	}
	
}


//Método de multiplicación (Sobrecarga del operador *) La multiplicación se realiza del lado izquierdo del operador resta

    matriz operator*(matriz der){
	int a,b,c,d;
		a=this-> renglones1;
		b=this-> columnas1;
		c=der.renglones1;
		d=der.columnas1;
		matriz resultado;
			if(!((b==c))){
		
					cout << "No se puede realizar la multiplicación de estas matrices" << endl<< endl;
						return resultado;  
	} else{
		matriz resultado(a,d);
		for(int i=0;i<a;i++){
			for(int j=0;j<d;j++){
				resultado.values1[i][j]=0; //Se inicializará en 0 para que se realice correctamente la multiplicación, ya que se tienen valores ale
				for(int k=0; k<b;k++){
					resultado.values1[i][j]+=this->values1[i][k]*der.values1[k][j];
				}
				
			}
		}

		cout << "El resultado de la multiplicación de estas matrices es:"<< endl<< endl;
		resultado.imprimir_matriz();
		return resultado;
	}
	
}

//Método de la transpuesta

 matriz transpuesta()
    {
    	matriz resultado(columnas1,renglones1);
        for(int i=0;i<columnas1;i++)
        {
            for(int j=0;j<renglones1;j++)
            {
                	resultado.values1[i][j]=values1[j][i];
            }
        }
        
		cout <<"La matriz transpuesta es:"<< endl<< endl;
		resultado.imprimir_matriz();
        return resultado;
    };
    
//Reducción Gaussiana

matriz reduccion_gauss(){
	matriz resultado(renglones1,columnas1);
	//Crea la copia de la matriz
	for(int i=0;i<renglones1;i++)
        {
            for(int j=0;j<columnas1;j++)
            {
                resultado.values1[i][j] = values1[i][j] ;
            }
        }
    //Una vez creada la copia, se puede hacer reducción gaussiana sobre la matriz resultado.    
	for( int j=0;j<columnas1;j++)
        {
            for(int i=0;i<renglones1;i++)
            {
                if(i>j){
                	if(!(resultado.values1[i][j]==0)){
                		double factor=(-resultado.values1[i][j])/resultado.values1[j][j];
                		for(int k=0;k<columnas1;k++){
                			resultado.values1[i][k]=resultado.values1[i][k]+(factor*resultado.values1[j][k]);
						}
					}
				}
        }
	}
	
	cout << "La reducción gaussiana de esta matriz es:"<< endl<< endl;
	resultado.imprimir_matriz();
	return resultado;
}

//Método de la inversa 

matriz inversa(){
	
	matriz resultado;
	
	if (!(renglones1==columnas1)){
		cout << "No se puede obtener la inversa, porque la matriz no es cuadrada" << endl<< endl;
		
		return resultado;  
	}
	 else{	
			
	matriz resultado(renglones1,columnas1);
	
	//Crea la copia de la matriz
	
	matriz copia(renglones1,columnas1);
	double pivote;
	for(int i=0;i<renglones1;i++)
        {
            for(int j=0;j<columnas1;j++)
            {
                copia.values1[i][j] = values1[i][j] ;
                
                if (i==j){
                	resultado.values1[i][j]=1;
				}else{
					resultado.values1[i][j]=0;
				}
            }
        }

        //Se hace la reducción Gauss Jordan
        	for( int j=0;j<columnas1;j++)
        {
        	pivote= copia.values1[j][j];
        	for (int k=0; k<columnas1; k++){
        		copia.values1[j][k]= copia.values1[j][k]/ pivote;
        		resultado.values1[j][k]= resultado.values1[j][k]/ pivote;
			}
            for(int i=0;i<renglones1;i++)
            {
                if(!(i==j)){
                	if(!(copia.values1[i][j]==0)){
                		double factor=(-copia.values1[i][j]);
                		//printf("  %d  ",factor);
                		for(int k=0;k<columnas1;k++){
                			copia.values1[i][k]=copia.values1[i][k]+(factor*copia.values1[j][k]);
                			resultado.values1[i][k]=resultado.values1[i][k]+(factor*resultado.values1[j][k]);
						}
					}
				}
        }
	}

	// Checar si rango es igual al número de columnas
	
	int rango=0;
	for( int j=0; j<columnas1; j++){
		if(copia.values1[j][j]==1){
			rango+=1;
		}
	}
	 
	 if (rango==columnas1 ){
	 	cout<< "La inversa de la matriz es: " <<endl <<endl;
	 	resultado.imprimir_matriz();
	 	return resultado;
	 }else{

	 	cout << "No se puede obtener la inversa, porque la matriz no es invertible" << endl<< endl;
	 	matriz resultado2;
	 	return resultado2;
	 	
	 }


	}
}
};







