#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct{
	int size;
	int difficult;
	int **tablero;
}board;
typedef struct{
	char player[10];
	board *sudoku;
	int boardId;
	int puntaje;
}game;

typedef enum code {OK, ERR_NOT_MEM, ERR_FILE_NOT_PERMISSION, ERR_FILE_NOT_FOUND, ERR_BAD_FILE} code;


/*entrega un valor equivalente al tamaño de los subtableros del sudoku*/
int subsize(board board){
	int c;
	c= (int)sqrt(board.size);
	return c;
}
/*entrega un espacio de memoria correspondiente a lo necesario por el tablero dinamico*/
int** createArray( int X, int Y){
	int **miarray;
	miarray = (int**) malloc( Y * sizeof(int*) );
	int i;
	for(i=0; i< Y; i++){
		miarray[i] = (int*) malloc( X * sizeof(int) );
	}
	return miarray;	
}

/*crea un board lleno de ceros*/
board crearTablero(int m, int n, int diff){
	int cry,crx;
	board sudoku;
	int **arrayTablero;
		arrayTablero=createArray(m,n);
		for (cry=0;cry<m;cry++){
				for(crx=0;crx<n;crx++){
					arrayTablero[crx][cry]=0;
			    }
		    }
		sudoku.size=m;
		sudoku.difficult= diff;    
		sudoku.tablero=arrayTablero;
		    return sudoku;
}

/* obtiene la coordenada del cuadrante al que la coordenada entregada aputa*/
int getCoordCuad(board board, int coord){/*tablero, numero equivalente a coordenada*/
	int x,c,cuadCoord,ss;
	ss=subsize(board);
	x=coord;
	while((x+1)%ss != 0){
			x++;
			}
	cuadCoord=	x-ss+1;
	
	return cuadCoord;
}


/*imprime un tablero, ingresa a la funcion como puntero a*/
void printboard(board *tablero){
	int y,x,ss,c,size;
	ss=subsize(*tablero);
	printf("|");
	for (x=0;x<(*tablero).size;x++){
		printf(" %d ",x);
		if ((x+1)%ss == 0){
				printf("|");
			}
	}
	printf("\n");
	for (c=0;c<(*tablero).size-1;c++){
				printf(" -- ");
			}
			printf("\n");		
	for (y=0;y<(*tablero).size;y++){
		printf("|");
		for (x=0;x<(*tablero).size;x++){
			if ((*tablero).tablero[x][y]==0){
			printf(" # ");	
			}else{
			printf(" %d ",(*tablero).tablero[x][y]);		
			}
			if ((x+1)%ss == 0){
				printf("|");
			}
		}
		printf("  %d  ",y);
		if ((y+1)%ss == 0){
			printf("\n");
			for (c=0;c<(*tablero).size-1;c++){
				printf("--- ");
			}
		}
		printf("\n");
	}
}

/*chequea si en la fila cy existe el numero number, entregando un 1 en caso que si, 0 en caso que no*/
int checkFila (board tab_game, int number,int cy){ /*tablero, numero a comparar, fila a chequear */
	int i, c;
	c=0;
	for (i=0;i<tab_game.size;i++){
		if (tab_game.tablero[i][cy] == number){
			c++;
		}
	}
	return c;
}
/*chequea si en la columna cx existe el numero number, entregando un 1 en caso que si, 0 en caso que no*/
int checkColumna (board tab_game, int number,int cx){/*tablero, numero a comparar, columna a chequear */
	int i, c;
	c=0;	
	for (i=0;i<tab_game.size;i++){
		if (tab_game.tablero[cx][i] == number){
			c++;
		}
	}
	return c;
}

/*chequea si en el bloque de coordenadas (bc,bf) existe el numero number, 
entregando la cantidad de veces que aparece el numero*/
				/*tablero,  numero a chequear, coordenada x del cuadrante, coordenada y del cuadrante */
int checkBloque (board tab_game, int number, int bc, int bf){ 
	int x, y, c,ss;
	ss=subsize(tab_game);
	c=0;
	for (y=bf;y<bf+ss;y++){/*for para filas de celdas dentro del cuadrante*/
		for (x=bc;x<bc+ss;x++){/*for para columnas de celdas dentro del cuadrante*/
			if(tab_game.tablero[x][y]== number){
				c++;
			}	
		}
	}	
	return c;
}

/*Para obtener la puntuacion, se utiliza un algoritmo similar  al de checkboard, con la diferencia de que aqui 
los casos erroneos se acumulan  y se les resta la cantidad de celdas generadas en la creacion del tablero, para luego calcular el 
porcentaje de este resultado en funcion del total menos las celdas creadas, quedando la siguiente regla de 3:

(total- generadas al comienzo) =    (total de celdas revisadas positivamente - generadas al comienzo)
          100												score

*/
int getScore(game *g){
	int y,x,c,num,total, celdasIniciadas;
	c=0;
	for (y=0;y<g->sudoku[1].size;y++){
		for (x=0;x<g->sudoku[1].size;x++){
				num=g->sudoku[1].tablero[x][y];
				if (num==0){
					c=c+0;
				}else{
						/*la incongruencia se genera cuando alguna de las funciones check entrega un valor superior a 1 en una
						celda que no sea igual a 0, esto porque el valor dentro de la celda debiera estar solo en esa celda, 
						considerando su fila, columna y bloque */
					if ((checkFila(g->sudoku[1], num,y)==1) & (checkColumna(g->sudoku[1],num,x)==1) & 
					(checkBloque(g->sudoku[1],num,getCoordCuad(g->sudoku[1],x),getCoordCuad(g->sudoku[1],y))==1) ){
						c=c+1;
						
						}
					}
		}
		
	}
	celdasIniciadas = cantidadDeCeldas(g->sudoku[1].size, g->sudoku[1].difficult);
	total = g->sudoku[1].size * g->sudoku[1].size;
	c = lround(  ( (c - celdasIniciadas)*100  )/(total-celdasIniciadas)   );
	
	//si las jugadas generan rompimiento de reglas en las celdas generadas en principio, el puntaje sera negativo
	//por lo que se determina igualar todo puntaje negativo a 0
	if (c<0){
		c=0;
	}
	
	return c;
	
}

/*chequea q un tablero no tenga incongruencias con sus reglas*/
int checkBoard(board *tablero){
	int y,x,c,num;
	c=0;
	for (y=0;y<tablero[1].size;y++){
		for (x=0;x<tablero[1].size;x++){
				num=tablero[1].tablero[x][y];
				if (num==0){
					c=c+0;
				}else{
						/*la incongruencia se genera cuando alguna de las 
						funciones check entrega un valor superior a 1 en una
						celda que no sea igual a 0, esto porque el valor dentro 
						de la celda debiera estar solo en esa celda, 
						considerando su fila, columna y bloque */
					if ((checkFila(tablero[1], num,y)>1) || (checkColumna(tablero[1],num,x)>1)
					 || (checkBloque(tablero[1],num,getCoordCuad(tablero[1],x),getCoordCuad(tablero[1],y))>1) ){
						c=c+1;
						}
					}
		}	
	}
	/*entrega 0 si el tablero es valido, y 1 si no lo es*/
	if (c>1) c=1; 
	return c;
}

/*chequea si el numero a añadir en la creacion d un tablero es factible*/
int checkJugada(board tablero,int x, int y, int num){
	int c;
	c=0;
	if ((checkFila(tablero, num,y)>=1) || (checkColumna(tablero,num,x)>=1) || (checkBloque(tablero,num,getCoordCuad(tablero,x),getCoordCuad(tablero,y))>=1) ){
		c=1;
		
	}
	
	return c;
}

/*determina la cantidad de celdas a mostrar*/
int cantidadDeCeldas(int size, int diff){
	int celdas;
	celdas = lround( 2+(size*size*diff*0.19));
	return celdas;
}

/*añade numeros al azar en el tablero, respetando 
las leyes del sudoku. La cantidad esta determinada 
por una operacion aritmetica relacionada con la dificultad*/
void sudokualazar(board sudoku){
	int x,sudoku_number,randx,randy;
	int dificultad;
	srand(time(NULL));
	//dificultad=lround( 2+(sudoku.size*sudoku.size*sudoku.difficult*0.19));
	dificultad=cantidadDeCeldas(sudoku.size,sudoku.difficult);
	for (x=1;x<=dificultad;x++){
			sudoku_number= 1+ rand() %sudoku.size; 
			randx= rand() %sudoku.size;
			randy= rand() %sudoku.size;
			if ( (checkJugada(sudoku,randx,randy,sudoku_number) 
			== 0) &(sudoku.tablero[randx][randy]==0)){
				sudoku.tablero[randx][randy]=sudoku_number;
			}else{
				x--;
			}		
	}
}


/*funcion q recibe las dimensiones y la dificultad del tablero, y entrega un puntero a un array bidimensional de board
ambos boards se generan identicos el uno con el otro, pero la dimension [1] sera la utilizada a posterior, para recibir las jugadas

*/
board* createBoard (int N, int M, int difficulty,code *statusCode ){
	if ((N %(int)sqrt(M) == 0) &(N>=1) & (N==M) & (difficulty<=3) & (difficulty>=1) ){
		board *sudoku;
		sudoku= (board *) malloc(2*sizeof(board));
		if(sudoku == NULL){
			*statusCode = ERR_NOT_MEM;
			 
		}
		sudoku[1]=crearTablero(N,M,difficulty);
		sudoku[0]=crearTablero(N,M,difficulty);
		
	 	if ((difficulty==3)&(N>4)){
		  }
		  sudokualazar(sudoku[1]);
		  sudoku[0]=sudoku[1];
		return sudoku;
		}
		else{
			printf("Error en los datos ingresados");
		}
}


/* realiza una jugada en el array bidimensional de board, en la dimension destinada para recibir 
la jugada ( la dimension [1]) SOLO si es posible
si los datos ingresados son validos, y si la celda no esta ocupada 
por un numero generado en create board*/
void play(board *b, int x, int y, int value, code *statusCode ){
	if ( (b[0].tablero[x][y]==0) && (x< b[0].size) && (x>= 0) && (y< b[0].size) && (y>= 0) && (value<= b[0].size) && (value>= 0) ){
	
		b[1].tablero[x][y]=value;
		}
		else{printf("la jugada no es posible\n");
		}
}



/*incrementa en una unidad el dato dentro del archivo
countboard,esto para ir generando id unicos*/
int incCountBoard(){
	FILE *f1;
	int ss=0;
	int a;
	char scan[30];
	char s[] = "countboard.dat";
	char head[]= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
	char cantidad[]= "<cantidad>%d</cantidad>";
	f1=fopen(s,"r");
	if (f1==NULL){
		a=1;
		f1=fopen(s,"a+");
		fprintf(f1,head);
		ss = 1;
		fprintf(f1,cantidad,ss);
		fclose(f1);
	}else{
		rewind(f1);
		fscanf(f1,head);
		fscanf(f1,cantidad,&ss);
		ss++;
		fclose(f1);
		f1=fopen(s,"w");
		fprintf(f1, head);
		fprintf(f1,cantidad,ss);
		fclose(f1);	
			 
	}
	
	return ss;
}
	
/*incrementa en una unidad el dato dentro del archivo countgame,esto para ir generando id unicos*/
int incCountgame(){
	FILE *f1;
	int ss=0;
	int a;

	char s[] = "countgame.dat";
	char head[]= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
	char cantidad[]= "<cantidad>%d</cantidad>";
	f1=fopen(s,"r");
	if (f1==NULL){
		a=1;
		f1=fopen(s,"a+");
		fprintf(f1,head);
		ss = 1;
		fprintf(f1,cantidad,ss);
		fclose(f1);
	}else{
		rewind(f1);
		fscanf(f1,head);
		fscanf(f1,cantidad,&ss);
		ss++;
		fclose(f1);
		f1=fopen(s,"w");
		fprintf(f1, head);
		fprintf(f1,cantidad,ss);
		fclose(f1);
			 
	}
	
	return ss;
}
	


 

/*guarda en un archivo board.dat el tablero, con formato xml*/
void saveBoard(board *b, int *id, code *statusCode){
FILE *f1; int x,y,newId;
	newId=incCountBoard();
	*id = newId;
	//printf("id generado para board : %d \n",newId);
	char address[]= "boards/board";
	char num[3];
	char ext[]=".dat";
	char head[]= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
	sprintf(num,"%d",newId);
	strcat(address,num);
	strcat(address,ext);
	f1=fopen(address,"w");
	fprintf(f1,head);
	fprintf(f1,"<sudoku size=%d diff=%d>\n",b[1].size,b[1].difficult);
	for (y=0;y<b[1].size;y++){
		for(x=0;x<b[1].size;x++){
			fprintf(f1,"<celda cx=%d cy=%d c0=%d>%d</celda>\n",x,y,b[0].tablero[x][y],b[1].tablero[x][y]);
		}
	}
	  time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
        
	fprintf(f1,output);
	fclose(f1);
}

/*carga un tablero desde un archivo .dat cuyo nombre es una composicion de "board" mas la id que lo determina*/
board* loadBoard(int id, code *statusCode){
	FILE *f1; int size,diff,x,y,q,w;
	board *sudoku;
	char tandh[128];
	char address[]= "boards/board";
	char num[3];
	char ext[]=".dat";
	char head[]= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";	
	sprintf(num,"%d",id);
	strcat(address,num);
	strcat(address,ext);
	f1=fopen(address,"r");
	if (f1 == NULL){
			*statusCode = ERR_FILE_NOT_FOUND;
		
	}
	if (feof(f1)){
			*statusCode = ERR_BAD_FILE;
	}
	fscanf(f1,head);
	fscanf(f1,"<sudoku size=%d diff=%d>\n",&size,&diff);
	sudoku= (board *) malloc(2*sizeof(board));
	sudoku[1]=crearTablero(size,size,diff);
	sudoku[0]=crearTablero(size,size,diff);
	sudoku[0].difficult=diff;
	sudoku[0].size=size;
	sudoku[1].difficult=diff;
	sudoku[1].size=size;
	for (y=0;y<size;y++){
		for (x=0;x<size;x++){
			fscanf(f1,"<celda cx=%d cy=%d c0=%d>%d</celda>\n",
			&q,&w,&sudoku[0].tablero[x][y],&sudoku[1].tablero[x][y]);		
		}
	}
	
	fclose(f1);
	return sudoku;
	
}
	
/*guarda en un archivo game.dat el tablero, con formato xml*/	
void saveGame(game *g, int *id, code *statusCode){
	FILE *f1; int x,y,newId, boardId;
	newId=incCountgame();
	
	*id= newId;
	char address[]= "games/game";
	char num[3];
	char ext[]=".dat";
	char head[]= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
	sprintf(num,"%d",newId);
	strcat(address,num);
	strcat(address,ext);
  	f1=fopen(address,"w");
	if (f1 == NULL){
		*statusCode = ERR_FILE_NOT_PERMISSION;
	
	}
	fprintf(f1,head);
	fprintf(f1,"<size>%d</size>\n",g->sudoku[1].size);	
	fprintf(f1,"<diff>%d</diff>\n",g->sudoku[1].difficult);
	saveBoard(g->sudoku,&boardId,&statusCode );
	fprintf(f1,"<game player=%s boardid=%d puntaje=%d/>\n",g->player,boardId,g->puntaje);
	g->boardId=boardId;
	time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);        
	fprintf(f1,"<timeAndDate>%s</timeAndDate>",output);
	fclose(f1);
}

/*carga un juego desde un archivo .dat cuyo nombre es una composicion de "board" mas la id que lo determina*/
game* loadGame(int id, code *statusCode){
	FILE *f1; int size,diff,x,y,q,w,boardId;
	game *g;
	char tandh[128];
	char address[]= "games/game";
	char num[3];
	char ext[]=".dat";
	char head[]= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";	
	sprintf(num,"%d",id);
	strcat(address,num);
	strcat(address,ext);
	f1=fopen(address,"r");
	if (f1 == NULL){
			*statusCode = ERR_FILE_NOT_FOUND;
	}
	if (feof(f1)){
			*statusCode = ERR_BAD_FILE;
	}
	fscanf(f1,head);
	fscanf(f1,"<size>%d</size>\n",&size);
	fscanf(f1,"<diff>%d</diff>\n",&diff);
	g= (game *) malloc(sizeof(game));
	g->sudoku = (board *) malloc(2*sizeof(board));
	g->sudoku[0] = crearTablero(size,size,diff);
	g->sudoku[1] = crearTablero(size,size,diff);
	fscanf(f1,"<game player=%s boardid=%d puntaje=%d/>\n",&g->player,&g->boardId,&g->puntaje);
	g->sudoku=loadBoard(g->boardId,statusCode); 
	fclose(f1);
	return g;
}


