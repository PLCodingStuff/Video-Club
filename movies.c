#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

int dalloc_movies(movie **m,int *m_counter,unsigned char **flag,int *m_capacity){

	int i;

	/*Allocates dynamically the char * variables and the other struct members that are pointers, and checks if something went wrong with the allocation. If yes, it returns a flag.*/
	*m = (movie *) realloc(*m,sizeof(movie)*(*m_capacity));
	*flag = (unsigned char *) realloc(*flag,(*m_capacity)*sizeof(unsigned char));

	/*Checks if there was any error with memory alloctation. If yes, then returns a flag.*/
	if(*m == NULL)
	{
	fprintf(stderr, "Error.\n");
		return -1;
	}


	for(i = *m_counter; i < *m_capacity; i++)
	{
		(*m + i)->director = (director_info *) malloc(sizeof(director_info));
		if((*m+i)->director == NULL)
		{
			fprintf(stderr, "Error.\n");
			return -1;
		}

		(*m + i)->director->director_name = (char *) malloc(20);
		if((*m+i)->director->director_name == NULL)
		{
			fprintf(stderr, "Error.\n");
			return -1;
		}

		(*m + i)->director->director_surname = (char *) malloc(20);
		if((*m+i)->director->director_surname == NULL)
		{
			fprintf(stderr, "Error.\n");
			return -1;
		}

		(*m + i)->title = (char *) malloc(40);
		if((*m+i)->title == NULL)
		{
			fprintf(stderr, "Error.\n");
			return -1;
		}

		(*m + i)->release_date = (date *) malloc(sizeof(date));
		if((*m+i)->release_date == NULL)
		{
			fprintf(stderr, "Error.\n");
			return -1;
		}
	}	
	return 1;
}

void load_movies_from_file(movie **m,unsigned char **flag,int *m_capacity,int *m_counter){
	FILE *fp;
	char *str;
	char line[100];
	
	if(dalloc_movies(m,m_counter,flag,m_capacity) == -1)
	{
		fprintf(stderr,"Error loading movies from file. Terminating program.\n");
		exit(-1);
	}

	/*It opens the file for reading. If it doesn't exist, it creates it. If something goes wrong, it exits.*/
	fp = fopen(FILENAME,"r");
	if(fp==NULL)
	{	
		fp = fopen(FILENAME,"w");
		if(fp==NULL)
		{
			fprintf(stderr,"Error.\n");
			exit(-1);
		}
		fflush(fp);
		fclose(fp);
		if((fp = fopen(FILENAME,"r"))==NULL)
		{
			fprintf(stderr,"Error.\n");
			exit(-1);
		}
	}

	/*Starts the reading. It uses token, because the movies are saved in a line.*/
	while(fgets(line,100,fp))
	{	
		str = strtok(line,"-\n\0");
		if(str != NULL)
		{
			(*m+(*m_counter))->id = atoi(str);
			str = strtok(NULL,"-\n\0");
			if(str != NULL)
			{
				strcpy((*m+(*m_counter))->title,str);
				str = strtok(NULL,"-\n\0");
				if(str != NULL)
				{
					strcpy((*m+(*m_counter))->director->director_surname,str);
					str = strtok(NULL,"-\n\0");
					if(str != NULL)
					{
						strcpy((*m+(*m_counter))->director->director_name,str);
						str = strtok(NULL,"-\n\0");
						if(str != NULL)
						{
							(*m+(*m_counter))->release_date->day = atoi(str);
							str = strtok(NULL,"-\n\0");
							if(str != NULL)
							{
								(*m+(*m_counter))->release_date->month = atoi(str);
								str = strtok(NULL,"-\n\0");
								if(str != NULL)
								{
									(*m+(*m_counter))->release_date->year = atoi(str);
								}
							}
						}
					}
				}
			}
		}
		(*m_counter)++;
		
		/*Checks if there is need for more memory.*/
		if(*m_capacity == *m_counter)
		{	
			(*m_capacity)+=2;
			if(dalloc_movies(m,m_counter,flag,m_capacity) == -1)
			{
				fprintf(stderr,"Error loading movies from file. Terminating program.\n");
				exit(-1);
			}
		}
	}

	/*Initialises the flags for every movie.*/
	int i;
	for(i = 0; i < *m_counter; i++)
	{
		*((*flag+i)) = 1;
	}

	fflush(fp);
	fclose(fp);

}

int insert_movie(movie **m,int *m_counter,int *m_capacity,unsigned char **flag){

	/*Make a movie.*/
	movie *m_enter;
	m_enter = (movie *) malloc(sizeof(movie));
	m_enter->title = (char *) malloc(50);
	m_enter->director = (director_info *) malloc(sizeof(director_info));
	m_enter->director->director_name = (char *) malloc(20);
	m_enter->director->director_surname = (char *) malloc(30);
	m_enter->release_date = (date *) malloc(sizeof(date));

	printf("\nGive new movie\'s title:\t");
	scanf("%s",m_enter->title);

	printf("\nGive new movie\'s director\'s name, with capital first letter:\t");
	scanf("%s",m_enter->director->director_name);
	while(m_enter->director->director_name[0] < 'A' || m_enter->director->director_name[0] > 'Z'){
		printf("\nError! Name without capital first letter. Please try againg:\t");
		scanf("%s",m_enter->director->director_name);
	}

	/*Surname's first letter must be either small or capital, in order for alphabetical sort to work properly. I chose capital.*/
	printf("\nGive new movie\'s director\'s surname, with capital first letter:\t");
	scanf("%s",m_enter->director->director_surname);
	while(m_enter->director->director_surname[0] < 'A' || m_enter->director->director_surname[0] > 'Z')
	{
		printf("\nError! Surname without capital first letter. Please try againg:\t");
		scanf("%s",m_enter->director->director_surname);
	}

	printf("\nGive new movie\'s relese day:\t");
	scanf("%d",&m_enter->release_date->day);

	printf("\nGive new movie\'s relese month:\t");
	scanf("%d",&m_enter->release_date->month);

	printf("\nGive new movie\'s relese year:\t");
	scanf("%d",&m_enter->release_date->year);
	printf("\n");

	/*Checks if the movie exists and if this is the second or greater movie that is going to be inserted.*/
	if((*m_counter > 0) && (check_movie(m,&m_enter,*m_counter) == 1))
	{
		strcpy((*m+(*m_counter))->title,m_enter->title);
		strcpy((*m+(*m_counter))->director->director_name, m_enter->director->director_name);
		strcpy((*m+(*m_counter))->director->director_surname,m_enter->director->director_surname);
		(*m+(*m_counter))->release_date->day = m_enter->release_date->day;
		(*m+(*m_counter))->release_date->month= m_enter->release_date->month;
		(*m+(*m_counter))->release_date->year = m_enter->release_date->year;
		(*m+(*m_counter))->id = (*m+(*m_counter-1))->id + 1;
		*(*flag+(*m_counter)) = 1;
		(*m_counter)++;

		/*It sorts the new entry alphabetically.*/
		alphabetic_sort(m,*m_counter,flag);

		/*Checks if array is full.*/
		if(*m_capacity == *m_counter)
		{	
			(*m_capacity)+=2;
			if(dalloc_movies(m,m_counter,flag,m_capacity) == -1)
			{
				fprintf(stderr,"Error loading movies from file. Terminating program.\n");
				return -1;
			}
		}

		free(m_enter->title);
		free(m_enter->director->director_name);
		free(m_enter->director->director_surname);
		free(m_enter);
		return 1;
	}
	/*If this movie is the first,it just register it.*/
	else if(*m_counter == 0)
	{
		strcpy((*(m+(*m_counter)))->title,m_enter->title);
		strcpy((*(m+(*m_counter)))->director->director_name, m_enter->director->director_name);
		strcpy((*(m+(*m_counter)))->director->director_surname,m_enter->director->director_surname);
		(*(m+(*m_counter)))->release_date->day = m_enter->release_date->day;
		(*(m+(*m_counter)))->release_date->month= m_enter->release_date->month;
		(*(m+(*m_counter)))->release_date->year = m_enter->release_date->year;
		(*(m+(*m_counter)))->id = 1;
		*(*(flag+(*m_counter))) = 1;
		(*m_counter)++;

		free(m_enter->title);
		free(m_enter->director->director_name);
		free(m_enter->director->director_surname);
		free(m_enter);
		return 1;
	}

	return 1;
}

/*Points the movie array with flag, for the movie that user wants to delete.*/
int point_movie(movie **m,int *m_counter,unsigned char **flag,int *m_capacity){

	int i,k,count;
	count = 0;

	printf("Give the id of the movie you want to delete:\t");	
	fscanf(stdin,"%d",&k);

	/*Checks if there are 10 deletions.*/
	for(i = 0; i < *m_counter; i++)
	{
		if(*(*flag + i)==0) count++;
	}

	/*Checks if there are enough deletions. If yes, then calls delete.*/
	if(count == 10) {
		/*Checks if there is an error with deletion.*/
		if(delete_movies(m,flag,m_counter,m_capacity) == -1) return -1;
	}
	
	/*If the movie that is to be deleted is found, then the corresponding flag is marked with 0.*/
	for(i = 0; i < *m_counter; i++)
	{
		if(k == (*m+i)->id)
		{
			*(*flag+i) = 0;
			printf("\n");
			printf("Your movie was deleted succesfully\n");
			return 1;
		}
	}

	printf("\nThere is no movie with this id!\n");
	return 1;
}

void search_title(movie **m,int m_counter,unsigned char **flag){
	
	int i;
	char *title;
	title = (char *) malloc(50);

	printf("Give movie\'s title:\t");
	scanf("%s",title);
	printf("\n");

	/*Check with linear search if the title exists in the movie array.*/
	for(i = 0;i < m_counter; i++)
	{
		if((strcmp(title,(*m+i)->title) == 0) && (*(*flag + i) != 0))
		{
			print_movie(m,i);
			free(title);
			return ;
		}
	}
	
	free(title);
	printf("There is no movie with this title!\n\n");
	
}

void search_director(movie **m,int m_counter,unsigned char **flag){

	int position,left,right,left_counter,right_counter,i;
	char *surname;
	surname = (char *) malloc(30);

	printf("Give movie\'s director surname, with first letter capital:\t");
	scanf("%s",surname);	

	/*It finds the position of a movie from that director with binary search*/
	position = binary_search(m,m_counter,&surname);
	
	/*If the movie was found, then it checks for other movies of this director.*/
	if(position > -1 && position < m_counter)
	{
		/*It checks left and right movies.*/
		right = position + 1;
		left = position - 1;

		/*Counters for every direction.*/
		left_counter = 0;
		right_counter = 0;

			while(left > -2 && right < m_counter)
			{
				/*If a movie on the left is made by the same director, then it movies 1 slot to the left.*/
				if(left > -1 && strcmp((*m+left)->director->director_surname,surname) == 0) 
				{
					left_counter++;
					left--;
				}
				/*If there is the same movie on the right, then it movies 1 slot to the right.*/
				else if(right < m_counter && strcmp((*m+right)->director->director_surname,surname) == 0) 
				{	
					right_counter++;
					right++;
				}
				/*If there are no movies on the left or the right, then it stops searching.*/
				else
				{
					left = -2;
					right = m_counter;
				}	
			}
		}

	if(position > -1 && position < m_counter)
	{
		for(i = (position - left_counter); i < (position + right_counter + 1); i++)
		{
			/*Checks if the movie is not a deletion.*/
			if((*(*flag + i)) != 0) print_movie(m,i);
		}
	}
	/*If no movie was found, it prints the corresponding message.*/
	else
	{
		printf("\nThere is no movie by this director!\n");
	}
}

void save_movies_to_file(movie **m,unsigned char **flag,int m_counter){

	FILE *fp;
	int i;

	if(!(fp=fopen(FILENAME,"w")))
	{
		fprintf(stderr, "Error with saving the movies.\n");
		exit(-1);
	}

	/*Saves the movies in the file.*/
	for(i=0 ; i < m_counter;i++)
	{
		fprintf(fp,"%d-",(*m+i)->id);
		fprintf(fp,"%s-",(*m+i)->title);
		fprintf(fp,"%s-",(*m+i)->director->director_surname);
		fprintf(fp,"%s-",(*m+i)->director->director_name);
		fprintf(fp,"%d-",(*m+i)->release_date->day);
		fprintf(fp,"%d-",(*m+i)->release_date->month);
		fprintf(fp,"%d\n",(*m+i)->release_date->year);
	}

	free(*m);
	free(*flag);
	fflush(fp);
	fclose(fp);
	exit(1);
}

void print_movie(movie **m,int m_position){

	printf("Id:%d\n",(*m+m_position)->id);
	printf("Title:%s\n",(*m+m_position)->title);
	printf("Director's Name:%s\n",(*m+m_position)->director->director_name);
	printf("Director's Surname:%s\n",(*m+m_position)->director->director_surname);
	printf("Release Day:%d\n",(*m+m_position)->release_date->day);
	printf("Release Month:%d\n",(*m+m_position)->release_date->month);
	printf("Release Year:%d\n\n",(*m+m_position)->release_date->year);
	
}

int delete_movies(movie **m,unsigned char **flag,int *m_counter,int *m_capacity){

	int i,j;

	for(i = 0; i < (*m_counter); i++)
	{
		if((*(*flag+i)) == 0)
		{
			/*It push every element, above the deletion, below, by 1 slot.*/
			for(j = i;j < (*m_counter);j++)
			{
				*(*flag+j) =*(*flag+j+1);
				(*m+j)->id = (*m+j+1)->id - 1;
				strcpy((*m+j)->title,(*m+j+1)->title);
				strcpy((*m+j)->director->director_name,(*m+j+1)->director->director_name);
				strcpy((*m+j)->director->director_surname,(*m+j+1)->director->director_surname);
				(*m+j)->release_date->day = (*m+j+1)->release_date->day;
				(*m+j)->release_date->month = (*m+j+1)->release_date->month;
				(*m+j)->release_date->year = (*m+j+1)->release_date->year;
			}
			(*m_counter)--;
			/*Checks if the next element is also a deletion.*/
			 --i;
		}
	}
	return 1;
}

/*Sorts movies in alphabetical order, based on director's surname.*/
void alphabetic_sort(movie **m,int m_counter,unsigned char **flag){

	int i,j;
	char *temps;
	int tempi;

	for(i=0;i<m_counter;i++){
      for(j=i+1;j<m_counter;j++){
         if(strcmp((*m+i)->director->director_surname,(*m+j)->director->director_surname)>0){
            strcpy(temps,(*m+i)->director->director_surname);
            strcpy((*m+i)->director->director_surname,(*m+j)->director->director_surname);
            strcpy((*m+j)->director->director_surname,temps);
         	strcpy(temps,(*m+i)->director->director_name);
            strcpy((*m+i)->director->director_name,(*m+j)->director->director_name);
            strcpy((*m+j)->director->director_name,temps);
            strcpy(temps,(*m+i)->title);
            strcpy((*m+i)->title,(*m+j)->title);
            strcpy((*m+j)->title,temps);
            tempi = (*m+i)->release_date->day;
            (*m+i)->release_date->day = (*m+j)->release_date->day;
            (*m+j)->release_date->day = tempi;
            tempi = (*m+i)->release_date->month;
            (*m+i)->release_date->month = (*m+j)->release_date->month;
            (*m+j)->release_date->month = tempi;
            tempi = (*m+i)->release_date->year;
            (*m+i)->release_date->year = (*m+j)->release_date->year;
            (*m+j)->release_date->year = tempi;
		 }
      }
	}
}

/*Binary search, based on director's surname first letter. It checks for capital first letter.*/
int binary_search(movie **m,int counter,char **surname){
	
	int left, right, position;
	left = 0;
	right = counter - 1;
	
	while(left <= right)
	{
		position = (right + left)/2;
		if(strcmp(*surname,(*m+position)->director->director_surname) < 0) right = position-1;
		else if(strcmp(*surname,(*m+position)->director->director_surname) > 0) left = position+1;
		else return position;
	}
	
	return -1;
}

/*Checks if a movie already exists.If yes it returns a flag.*/
int check_movie(movie **m,movie **m_enter,int m_counter){

	int i;
	for(i = 0; i < m_counter; i++)
	{
		if(strcmp((*m+i)->title , (*m_enter)->title) == 0 &&
		   strcmp((*m+i)->director->director_name , (*m_enter)->director->director_name) == 0 &&
		   strcmp((*m+i)->director->director_surname , (*m_enter)->director->director_surname) == 0 &&
		   (*m+i)->release_date->day == (*m_enter)->release_date->day &&
		   (*m+i)->release_date->month == (*m_enter)->release_date->month &&
		   (*m+i)->release_date->year == (*m_enter)->release_date->year)
		   {
				printf("This movie already exist!\n");
				return 0;
		   }
	}
	return 1;
}
