#include "Matriz.h"


int main()
{

int seleccion=1;
int r1,r2,c1,c2;
while (!(seleccion==0)){
	cout <<"Seleccione una de las siguientes opciones" <<endl<<"1. Suma de dos matrices"<<endl<<"2.Resta de dos matrices"<<endl<<"3.Multiplicación de dos matrices"<<endl<<
	"4.Transpuesta"<<endl<<"5.Inversa"<<endl<<"6.Reducción Gaussiana"<<endl<<"0.Salir"<<endl<<endl;
	
	cin>> seleccion;
	if(seleccion==1){ 
		cout << "Dame el numero de renglones de la primera matriz"<<endl;
		cin >> r1; 
		cout <<"Dame el numero de columnas de la primera matriz"<<endl;
		cin>>c1;
		matriz m1(r1,c1);
		m1.imprimir_matriz();
		cout<<endl;
		cout << "Dame el numero de renglones de la segunda matriz"<<endl;
		cin >> r2; 
		cout <<"Dame el numero de columnas de la segunda matriz"<<endl;
		cin>>c2;
		matriz m2(r2,c2); 
		m2.imprimir_matriz();
		cout<<endl;
		m1+m2;
	}
	
	if(seleccion==2){
		cout << "Dame el numero de renglones de la primera matriz"<<endl;
		cin >> r1; 
		cout <<"Dame el numero de columnas de la primera matriz"<<endl;
		cin>>c1;
		matriz m1(r1,c1);
		m1.imprimir_matriz();
		cout<<endl;
		cout << "Dame el numero de renglones de la segunda matriz"<<endl;
		cin >> r2; 
		cout <<"Dame el numero de columnas de la segunda matriz"<<endl;
		cin>>c2;
		matriz m2(r2,c2); 
		m2.imprimir_matriz();
		cout<<endl;
		m1-m2;		
	}
	
	if(seleccion==3){
		cout << "Dame el numero de renglones de la primera matriz"<<endl;
		cin >> r1; 
		cout <<"Dame el numero de columnas de la primera matriz"<<endl;
		cin>>c1;
		matriz m1(r1,c1);
		m1.imprimir_matriz();
		cout<<endl;
		cout << "Dame el numero de renglones de la segunda matriz"<<endl;
		cin >> r2; 
		cout <<"Dame el numero de columnas de la segunda matriz"<<endl;
		cin>>c2;
		matriz m2(r2,c2); 
		m2.imprimir_matriz();
		cout<<endl;
		m1*m2;
	}
	
	if(seleccion==4){
		cout << "Dame el numero de renglones de la matriz"<<endl;
		cin >> r1; 
		cout <<"Dame el numero de columnas de la matriz"<<endl;
		cin>>c1;
		matriz m1(r1,c1);
		m1.imprimir_matriz();
		cout<<endl;
		m1.transpuesta();
	}
	
	if(seleccion==5){
		cout << "Dame el numero de renglones de la matriz"<<endl;
		cin >> r1; 
		cout <<"Dame el numero de columnas de la matriz"<<endl;
		cin>>c1;
		matriz m1(r1,c1);
		m1.imprimir_matriz();
		cout<<endl;
		m1.inversa();		
	}
	
	if(seleccion==6){
		cout << "Dame el numero de renglones de la matriz"<<endl;
		cin >> r1; 
		cout <<"Dame el numero de columnas de la matriz"<<endl;
		cin>>c1;
		matriz m1(r1,c1);
		m1.imprimir_matriz();
		cout<<endl;
		m1.reduccion_gauss();
	}
	
	
}

	
}

