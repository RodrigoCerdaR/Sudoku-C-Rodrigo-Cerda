#include "sudoku.h"


int main(int argc, char *argv[]) {

/* declaracion de variables y punteros para el test*/
	board *sudoku, *sudoku2;
	game *g, g2;
	int boardId, gameId,N,M,Diff,LoadBoardId; 
	code statusCode;
	statusCode= OK;
	
	
	/*el siguiente comentario son las funciones correspondientes a generar 
	un sudoku con las variables de abajo, y lo imprime*/
	
	/*datos del sudoku a generar*/
	///*
	N=9;  
	M=9;
	Diff=3;
	
	sudoku=createBoard(N,M,Diff,&statusCode);
	printf("funcion createboard de %dx%d y dificultad %d\n",N,M,Diff);
	printboard(&sudoku[1]);
	printf("satus: create board %d \n", (int)statusCode);
	printf("\n");
	//*/
	
	
	/*saveboard, comentar los caracteres de comentario para probar*/
	///*
	printf("funcion saveboard\n");
	saveBoard(sudoku,&boardId,&statusCode);	
	printf("Id generada para el tablero: %d \n",boardId);
	printf("El tablero se guardo en el archivo boards/board%d.dat\n",boardId);
	printf("satus: save board %d \n", (int)statusCode);
	printf("\n");
	//*/
	
	/*loadboard, comentar los caracteres de comentario para probar*/
	///*
	printf("funcion loadboard\n");
	LoadBoardId= 8 ; /*ingresar el id del tablero a cargar*/
	printf("tablero id: %d  cargado desde un fichero \n",LoadBoardId);
	sudoku=loadBoard(LoadBoardId,&statusCode); 
	printboard(&sudoku[1]);
	printf("satus load board: %d \n", (int)statusCode);
	printf("\n");
	//*/
	
	

	
		
	/*play, comentar los caracteres de comentario para probar*/
	///*
	int X=0;
	int Y=2;
	int valor=1;
	printf("funcion play\n");
	g2.sudoku=sudoku;
	play(g2.sudoku,X,Y,valor,&statusCode );
	printf("se intento la jugada en el tablero cargado desde el juego, en la posicion (%d,%d), con el valor %d\n",X,Y,valor);
	printboard(&g2.sudoku[1]);
	printf("puntaje : %d\n", getScore(&g2));
	printf("satus: play %d \n", (int)statusCode);
	printf("\n");
	//*/
	
	/*savegame, comentar los caracteres de comentario para probar*/
	///*
	printf("funcion savegame\n");
	char player[10]="Rodrigo";
	sprintf(g2.player,"%s",player);
	g2.sudoku=sudoku;
	g2.puntaje=getScore(&g2);
	saveGame(&g2,&gameId,&statusCode);
	printf("id generada para game: %d \n",gameId);
	printf("id generada para board de game: %d \n",g2.boardId);
	printf("Jugador: %s . Puntaje: %d . Id de board: %d \n",g2.player, g2.puntaje, g2.boardId);
	printf("se ha guardado un juego en el fichero games/game%d.dat \n",gameId);
	printf("satus: saveGame %d \n", (int)statusCode);
	printf("\n");
	//*/
	
	
	/*loadgame, comentar los caracteres de comentario para probar*/
	///*
	int idjuego=8;
	printf("funcion loadgame\n");
	g=loadGame(idjuego,&statusCode);
	printf("juego cargado con id: %d\n",idjuego);
	printf("Jugador: %s . Puntaje: %d . Id de board: %d \n",g->player, g->puntaje, g->boardId);
	printboard(&g->sudoku[1]);
	printf("satus: loadGame %d \n", (int)statusCode);
	printf("\n");
	//*/
	
	
	/*checkgame, comentar los caracteres de comentario para probar*/
	///*
	int respuesta;
	printf("funcion checkboard\n");
	respuesta=checkBoard(g->sudoku) ;
	printf("si es 0 cumple con las reglas del juego, si es 1, no lo hace:  %d",respuesta);
	printf("\n");
	//*/
	getchar();
}
