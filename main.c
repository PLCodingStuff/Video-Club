#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

/*Tells the user to choose which action will be performed.*/
int menu();
/*Inserts a new movie.*/
void insert(movie **m,int *m_counter,int *m_capacity,unsigned char **flag);
/*Points a movie for deletion.*/
void point(movie **m,int *m_counter,unsigned char **flag,int *m_capacity);
/*Search a movie based on movie's title. If the movie is pointed as deleted, it won't appear.*/
void search_t(movie **m,int m_counter,unsigned char **flag);
/*Search movies based on movies director.If the movies are pointed as deleted, they won't appear.*/
void search_d(movie **m,int m_counter,unsigned char **flag);
/*Saves the movies on a text file and terminates the program.*/
void close(movie **m,unsigned char **flag,int *m_counter,int *m_capacity);

int main(int argc, char* argv[]){

	movie *movie_array;
	int choice,movie_array_capacity,movie_array_counter;
	unsigned char *movies_flag;
	
	movie_array_counter = 0;
	movie_array_capacity = 2;
	movie_array = NULL;

	load_movies_from_file(&movie_array,&movies_flag,&movie_array_capacity,&movie_array_counter);

	choice = menu();

	while(choice)
	{
		switch(choice)
		{
			case 1:
				insert(&movie_array,&movie_array_counter,&movie_array_capacity,&movies_flag);
				break;
			case 2:
				point(&movie_array,&movie_array_counter,&movies_flag,&movie_array_capacity);
				break;
			case 3:
				search_t(&movie_array,movie_array_counter,&movies_flag);
				break;
			case 4:
				search_d(&movie_array,movie_array_counter,&movies_flag);
				break;
			case 5:
				close(&movie_array,&movies_flag,&movie_array_counter,&movie_array_capacity);
				break;
			default:
				printf("There was an error with your choice. The program will be terminated.\n");
				close(&movie_array,&movies_flag,&movie_array_counter,&movie_array_capacity);
				break;
		}
		choice  = menu();
	}

	return -1;
}

int menu(){
	int choice;
	printf("Choose which action do you want to perform:\n");
	printf("1. Insert a new movie\n");
	printf("2. Delete a movie based on movie\'s id\n");
	printf("3. Search a movie based on movie\'s title\n");
	printf("4. Search a movie based on movie\'s director\n");
	printf("5. Exit\n");
	scanf("%d",&choice);
	
	/*Checks if the choice is valid.*/
	while(choice)
	{
		if(choice > 0 || choice < 6)
		{
			return choice;
		}
		printf("Invalid option! Please choose again.");
		printf("1. Insert a new movie\n");
		printf("2. Delete a movie based on movie\'s id\n");
		printf("3. Search a movie based on movie\'s title\n");
		printf("4. Search a movie based on movie\'s director\n");
		printf("5. Exit\n");
		scanf("%d",&choice);
	}

	/*If, for any reason, the choice is not within 1 to 5, then returns a negative number as a flag.*/
	return -1;
}

/*Wrapper for inserting movies.*/
void insert(movie **m,int *m_counter,int *m_capacity,unsigned char **flag){
	/*Checks if there was an error with the insartion. If yes, then it saves the current movies and termintates the programm.*/
	if(insert_movie(m,m_counter,m_capacity,flag) == -1) save_movies_to_file(m,flag,*m_counter);
}

/*Wrapper for deleting movies.*/
void point(movie **m,int *m_counter,unsigned char **flag,int *m_capacity){
	/*Checks if there was any error with the deletion of the movies. If yes  then it saves the current movies and termintates the programm.*/
	if(point_movie(m,m_counter,flag,m_capacity) == -1) save_movies_to_file(m,flag,*m_counter);
}

/*Wrapper for searching movies based on their title.*/
void search_t(movie **m,int m_counter,unsigned char **flag){
	search_title(m,m_counter,flag);
}

/*Wrapper for searching movies based on their director.*/
void search_d(movie **m,int m_counter,unsigned char **flag){
	search_director(m,m_counter,flag);
}

/*Wrapper for saving movies and exit.*/
void close(movie **m,unsigned char **flag,int *m_counter,int *m_capacity){
	delete_movies(m,flag,m_counter,m_capacity);
	save_movies_to_file(m,flag,*m_counter);
}
