#ifndef MOVIES_H
#define MOVIES_H

#define FILENAME "Movies.txt" /*Constant for the name of the txt file, were the movies will be stored.*/

/*The struct of a date. That is the day, the month and the year.*/
typedef struct date{
	int day, month, year;
}date;

/*The sruct of the director info. The info includes the name and the surname of the director.*/
typedef struct director_info{
	char *director_surname, *director_name;
}director_info;

/*The struct of a movie that is going to be stored. A movie includes an id, a title, the director's info and the date of its release.*/
typedef struct movie{
	int id;
	char* title;
	director_info* director;
	date* release_date; 
}movie;

/*It allocates memory dynamically, using realloc. For the first time, it uses a NULL pointer and it works like malloc. 
Returns a negative number as flag, if there was any problem.*/
int dalloc_movies(movie **m,int *m_counter,unsigned char **flag,int *m_capacity);
/*Initialize the array from the FILENAME text file.*/
void load_movies_from_file(movie **m,unsigned char **flag,int *m_capacity,int *m_counter);
/*Inserts a movie. Returns a negative number as flag, if there was any problem.*/
int insert_movie(movie **m,int *m_counter,int *m_capacity,unsigned char **flag);
/*Points which movie will be deleted. Returns a negative number as flag, if there was any problem.*/
int point_movie(movie **m,int *m_counter,unsigned char **flag,int *m_capacity);
/*Search a movie based on movie's title. If it is pointed as deleted (flag = 0), it won't appear.*/
void search_title(movie **m,int m_counter,unsigned char **flag);
/*Search a movie based on movie's director. If it is pointed as deleted (flag = 0), it won't appear.*/
void search_director(movie **m,int m_counter,unsigned char **flag);
/*Saves the movies in the FILENAME text file. It needs an array with flags for every movie element that is going to get deleted and the size of the array.*/
void save_movies_to_file(movie **m,unsigned char **flag,int m_counter);
/*Prints a movie from a movie array, given the position of its.*/
void print_movie(movie **m,int m_position);
/*Delete the movies that are flaged. Returns a negative number as flag, if there was any problem.*/
int delete_movies(movie **m,unsigned char **flag,int *m_counter,int *m_capacity);
/*Sorts the movie array in alphabetical order, by directr's name.*/
void alphabetic_sort(movie **m,int m_counter,unsigned char **flag);
/*Binary search for movie's director. Return the position of the first movie with this director's surname.*/
int binary_search(movie **m,int counter,char **surname);
/*Checks if there is already the movie that the user entered. Returns 0 if the movie is already in the array.*/
int check_movie(movie **m,movie **m_enter,int m_counter);

#endif
