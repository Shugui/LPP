#include <stdio.h>
#include <omp.h>
#include <math.h>
#define maxSize 4

/*Ejercicio 1 */
void ejercicio1() {
	omp_set_num_threads(4);
	printf("El número total de threads antes de la seccion paralela es: %d\n", omp_get_num_threads());

	#pragma omp parallel 
	{
	printf("El número threads dentro de la sección es %d, firmado: Hilo Nº%d \n", omp_get_num_threads(),omp_get_thread_num());
	}
	printf("La versión de OMP es: %d\n",_OPENMP );
}

/*Ejercicio 2
Escribe un programa en OpenMP que calcule la suma de las p componentes de un vector de n´umeros
reales expresados en doble precisi´on. El n´umero m´aximo de threads a emplear debe ser p/2. Sugerencia:
tomar p como una potencia de 2.
*/

double ejercicio2(double vector[], int p){
	int i ;
	double sum =0;
	int hilos = p/2;
	omp_set_num_threads(p);
	#pragma omp parallel for reduction(+:sum)
	for(i =0 ; i<p; i++){
		sum = sum + vector[i];
	}
	printf("Suma es %f\n", sum);
	return sum;
}

/*
Ejercicio 3
Escribe un programa en OpenMP que calcule la suma de las n componentes de un vector de n´umeros reales
expresados en doble precisi´on utilizando un n´umero m´aximo de p threads sin utilizar instrucciones de reducci´on
y utilizando instrucciones de reducci´on. Sugerencia: Tomar p como una potencia de 2 y n como un m´ultiplo
de p.
*/

double ejercicio3(double vector[], int n, int p){
	double sum , sumnr=0;
	int i =0;
	omp_set_num_threads(p);
	//con reduccion
	#pragma omp parallel for reduction(+:sum)
	for(i =0 ; i<n; i++){
		sum = sum + vector[i];
	}
	printf("Suma es %f\n", sum);
	//sin reduccion
	/*
	#pragma omp parallel for 
	for(i=0; i < n; i++) {
		#pragma omp atomic
		sum += vector[i];
	}
	*/
return sum;
	
}
/*
Ejercicio 4
Escribe un programa en OpenMP que calcule la componente de mayor valor absoluto de un vector de n
componentes reales expresadas en doble precisi´on utilizando un n´umero m´aximo de p threads.
*/
double ejercicio4 (double vector[], int len, int p){
	int i;
	omp_set_num_threads(p);
	double maxim=0;	
	#pragma omp parallel for reduction(max: maxim)
	for(i =0 ; i<len; i++){
		maxim = fabs(vector[i]);
	}
	printf("Maximo: %f\n", maxim);
}



/*
Ejercicio 5
Escribe un programa en OpenMP que calcule el producto escalar de dos vectores de n componentes reales
expresadas en doble precisi´on, utilizando un n´umero m´aximo de p threads
.g*/

double* ejercicio5(double vector1[], double vector2[], int n, int p){

	double vectorP [n];
	omp_set_num_threads(p);
	int i = 0;
	#pragma  omp parallel for
	for(i =0; i< n ;i++){
		vectorP[i] = vector1[i] * vector2[i];
	}
	for(i=0;i< n ; i++)
		printf("%f ", vectorP[i]);
	printf("\n");
}

/*
Ejercicio 6
Escribe un programa en OpenMP que calcule la 2-norma de un vector de n componentes reales expresadas
en doble preci´on, utilizando un n´umero m´aximo de p threads.
*/
void ejercicio6(double vector[], int n, int p){
	int i;
	double raiz = 0;
	omp_set_num_threads(p);
	#pragma parallel for reduction (+:raiz)
	for(i = 0; i < n ; i++){
		raiz = raiz + pow(vector[i],2);

	}
	printf("Módulo: %f\n",sqrt(raiz));
	
}


/*
Ejercicio 7
Escribe un programa en OpenMP que calcule la suma de dos vectores de n componentes reales expresadas
en doble precisi´on, uno de ellos multiplicado por una constante α (operaci´on saxpy del BLAS1
), utilizando
un n´umero m´aximo de p threads.

*/
void ejercicio7(double vector1[], double vector2[], int n,int p, double alpha){
	double res[n];
	int i ;
	omp_set_num_threads(p);
	#pragma omp parallel for
	for(i = 0; i < n; i++) {
		res[i] = alpha*vector1[i] + vector2[i];
	}
	printf("El vector resultantes es: \n (");
	for(i = 0; i < n; i++) {
		printf("%lf,",res[i]);
	}
	printf(")\n");

}
/*
Ejercicio 8
Escribe y eval´ua un programa en OpenMP que calcule la integral entre a y b, de una funci´on polin´omica
de grado m, dividiendo el intervalo [a, b] en n subintervalos y utilizando un n´umero m´aximo de p threads.
*/
void ejercicio8(double a, double b, int n, int m, int p, double F[]){
	omp_set_num_threads(p);
	double base, alt, res, aux;
	int i, j;
	base = (b-a) / n;
	res = 0.0;

	
	#pragma omp parallel for private(aux,j,alt) reduction(+:res)
	for(i = 0; i < n; i++) {
		aux = a + i*base;
		alt = 0.0;
		for(j = 0; j < m; j++) {
			alt += F[j]*pow(aux,j);
		}
		res += base * alt;
	}
	printf("El resultado de la integral es : %lf", res);

}

/*
Ejercicio 9
Escribe y eval´ua un programa en OpenMP que calcule el producto de dos enteros largos almacenados
como dos vectores, utilizando un n´umero m´aximo de p threads.
*/

long ejercicio9(int a[], int b[], int len, int p){
	long res= 0;
	omp_set_num_threads(p);
	int i,j = 0;
	#pragma parallel for reduction (+:res)
	for(i =0;i < len; i++){
		for(j=0; j< len ; j++){
			res =res + b[i]*a[j]*pow(10,i)*pow(10,j);
		}
		
	}
	printf("La multiplicación es: %d\n",res );
	return res;

}

/*
Ejercicio 10
Escribe un programa en OpenMP lo m´as eficiente que se te ocurra para realizar el c´alculo x^T Ax, donde
x es una vector de n componentes reales y A una matriz cuadrada de orden n.
*/

void ejercicio10(double A[], double M[maxSize][maxSize], int n) {
	int i,j;
	double res[n];

	#pragma omp parallel for private(j)
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++) {
			res[i] += A[j] * M[i][j];
		}
	printf("El vector resultantes es: \n (");
	for(i = 0; i < n; i++) {
		printf("%lf,",res[i]);
	}
	printf(")\n");
}


void quick_sort (double *a, int n) {
    int i, j;
    double p, t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
#pragma omp task
    quick_sort(a, i);
#pragma omp task
    quick_sort(a + i, n - i);
}


/*
Ejercicio 11
Implementa una versi´on eficiente en OpenMP para resolver el problema de la ordenaci´on de un vector
mediante el m´etodo del quicksort
*/

void ejercicio11(double A[], int n) {
	    quick_sort(A,n);

	    int i;

	    printf("\n\nLa lista ordenada es: \n");
    	for (i=0; i<n; i++) {
        	printf("%lf",A[i]);
        	if(i<n-1)
            	printf(",");
    	}

}



void merge (double *a, int n, int m) {
    int i, j, k;
    double *x = malloc(n * sizeof (double));
    for (i = 0, j = m, k = 0; k < n; k++) {
        x[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
    }
    for (i = 0; i < n; i++) {
        a[i] = x[i];
    }
    free(x);
}
 
void merge_sort (double *a, int n) {
    if (n < 2)
        return;
    int m = n / 2;
#pragma omp task
    merge_sort(a, m);
#pragma omp task
    merge_sort(a + m, n - m);
#pragma omp taskwait
    merge(a, n, m);
}


/*
Ejercicio 12
Implementa una versi´on eficiente en OpenMP para resolver el problema de la ordenaci´on de un vector
mediante el m´etodo del mergesort.
*/

void ejercicio12(double A[], int n) {
	merge_sort(A, n);
	int i;

	    printf("\n\nLa lista ordenada es: \n");
    	for (i=0; i<n; i++) {
        	printf("%lf",A[i]);
        	if(i<n-1)
            	printf(",");
    	}

}

int main (int argc, const char * argv[])
{
/*Ejercicio 1
Escribe un programa en OpenMP que se ejecute con 4 threads, identifique los threads que se est´an
ejecutando y el n´umero total de threads, y saque los resultados por pantalla. Mostrar tambi´en la versi´on de
OpenMP utilizada.
*/
/*
int i;

omp_set_num_threads(4);
#pragma omp parallel for
for (i=0; i<4; i++) {
	printf("Hilos = %d\n",omp_get_num_threads());
	printf("Yo soy %d\n",omp_get_thread_num());
}

*/
	
	double vector[] = {2,1,3,4};
	double vector2[] = {3,3,3,3};
	int vectorInt[] = {0,1,9,8};
	int vector2Int[] = {3,2,9,2};
	int grado = 4;

	double M[4][4];
	int j,i;
	for (i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			M[i][j] = vector[i];
		}

	double lista[] ={0,-46,5,9,8,6,23,5,11,2,4,12,4};
    int size = sizeof(lista)/sizeof(lista[0]);
    double lista2[] ={0,-46,5,9,8,6,23,5,11,2,4,12,4};
    int size2 = sizeof(lista)/sizeof(lista[0]);



	printf("Ejercicio 1\n");
	ejercicio1();
	printf("Ejercicio 2\n");
	ejercicio2(vector, 4);
	printf("Ejercicio 3\n");
	ejercicio3(vector, 4, 2);
	printf("Ejercicio 4\n");
	ejercicio4(vector, 4, 2);
	printf("Ejercicio 5\n");
	ejercicio5(vector,vector2, 4, 4);
	printf("Ejercicio 6\n");
	ejercicio6(vector,4, 2);
	printf("Ejercicio 7\n");
	ejercicio7(vector, vector2, 4, 3, 3);
	printf("Ejercicio 8\n");
	ejercicio8(1.0,10.0,5000,grado,4, vector);
	printf("Ejercicio 9\n");
	ejercicio9(vectorInt, vector2Int,4, 3);
	printf("Ejercicio 10\n");
	ejercicio10(vector, M,4);
	printf("Ejercicio 11\n");
	printf("La lista desordenada es: \n");
	for (i=0; i<size; i++) {
	    printf("%lf",lista[i]);
	    if(i<size-1)
		    printf(",");
	}
	#pragma omp parallel
	#pragma omp single
	ejercicio11(lista,size); 

	printf("Ejercicio 12\n");
	printf("La lista desordenada es: \n");
	for (i=0; i<size2; i++) {
	    printf("%lf",lista2[i]);
	    if(i<size2-1)
		    printf(",");
	}
	#pragma omp parallel
	#pragma omp single
	ejercicio11(lista2,size2); 
		
}

