
#include <stdio.h>//including the header files
#include <stdlib.h>//including standard library
#include <unistd.h>//for miscellaneous
#include <string.h>//for strtok
#include <pthread.h>//to create the pthreads


int arrayIni[500]; //this is the initial array, to store the initial array


//there are 4 threads. One is the parent thread, two for sorting the two arrays using alg and one to merge them into the final array

//these are the two structs to indicate the indices
typedef struct {
int starting_index;
int ending_index;
} parameters;


typedef struct {
int starting_index;
int mid_index;
int ending_index;
} parametersN;


//prototyping the methods
void *sorter(void *params);
void *merger(void *params);
void merge_sort(int start, int end, int array[]);
void merge(int start, int mid, int end, int array[]);



int main(int argc, const char* argv[])//storing the arguments and the number of arguments
{

    char numbers[1999];//500 integers, each could be max 3 digits, 499 commas, altogether max possibility 1999

   
    FILE *fp, *fp1;//first one to read from, second one to write to
    fp = fopen("IntegerList.txt" , "r");//opening the given file and reading it

    fgets(numbers,1999,fp); //store the whole list of input numbers in the numbers array

    char delimeter[] = ",";//, is the delimiter in our case

	char *p = strtok(numbers, delimeter);
	int m=0;//index of the array
	int count=1;//counter to count no of items in the array
	
	while(p != NULL)//tokenize whole input by delimiter and store them in the array
	{
		arrayIni[m]= atoi(p);//turning the string into integer
		p = strtok(NULL, delimeter);//collects the end of input tokens
		m++;//incrementing index
		if (p!=NULL)//to check if the token is null
		count++;//incrementing the counter
	}

	fclose(fp);//closing the file


	pthread_t thread1, thread2, thread3;//declaring the threads

	//create 1st thread
	parameters *data1= (parameters *) malloc(sizeof(parameters));
    data1->starting_index= 0;
    if((count%2)==0)
    data1->ending_index = (count/2)-1;
    else
    data1->ending_index = (count/2);
    pthread_create(&thread1, NULL, sorter, data1);

	//create second thread
	parameters *data2= (parameters *) malloc(sizeof(parameters));
    if((count%2)==0)
    data2->starting_index = (count/2);
    else
    data2->starting_index = (count/2)+1;
    data2->ending_index = count-1;
    pthread_create(&thread2, NULL, sorter, data2);

	//wait them to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	//create third thread
	parametersN *data3= (parametersN *) malloc(sizeof(parametersN));
    data3->starting_index= 0;
    if((count%2)==0)
    data3->mid_index = (count/2)-1;
    else
    data3->mid_index = (count/2);
    data3->ending_index = count-1;
    pthread_create(&thread3, NULL, merger, data3);

	//wait for it to finish
    pthread_join(thread3, NULL);


	//from here it is the writing file code
    fp1= fopen("SortedIntegerList.txt", "w");  //creating the file to write on
    int k=0;

    for(int k=0;k<m;k++){
        if((m-k)==1)
        fprintf(fp1,"%d\n",arrayIni[k]);//we have to change it to arrayFin when the final array is decided
        else
        fprintf(fp1,"%d,",arrayIni[k]);
	}

	fclose(fp1);//closing the writing file
	
    pthread_exit(0);//terminating the parent thread

    return 0;//return 0 to main function
}


void *sorter(void *params){//to evaluate the input struct and call function merge_sort that uses MERGESORT ALGORITHM

    int n1= ((parameters *)params)->starting_index; //accessing the variables of the parameters struct
    int n2= ((parameters *)params)->ending_index;

    merge_sort(n1, n2, arrayIni);//sorting the half array in thread

}


void *merger(void *params){//to evaluate input struct and call merge function to merge the two arrays from two threads

    int n1=((parametersN *)params)->starting_index;//accessing the variables of the parametersN struct
    int n2=((parametersN *)params)->mid_index;
    int n3=((parametersN *)params)->ending_index;

    merge(n1, n2, n3, arrayIni);//merging the two halves from two threads in third thread

}


void merge_sort(int start, int end, int array[]) { //Implementing sorting algorithm- MERGE SORT
    if (start >= end) {//if the start is more or equal to end we stop
            return;
    }
    
    //continue to this code if the start is smaller than end
        int mid = (start + end)/ 2; //defining the mid point of the array
        
        merge_sort(start, mid, array); //sorting the first half
        merge_sort(mid+1, end, array); //sorting the second half
        
        merge(start, mid, end, array); //merging the two halves
    }


void merge(int start, int mid, int end, int array[]) { //implementing merging algorithm- TWO-FINGER WALKING ALGORITHM    
    
    int item1 = mid - start + 1; //declaring the number of items in the two sorted arrays
    int item2 = end - mid;

    int array1[item1], array2[item2]; //these are two arrays to temporarily hold the sorted arrays

    for (int i = 0; i < item1; i++) {//storing the first sorted half of the main array
        array1[i] = array[start + i];
    }

    for (int j = 0; j < item2; j++) {//storing the second sorted half of the main array
        array2[j] = array[mid + 1 + j];
    }

    int m = 0;//to keep track of the pointer in the two sorted arrays
    int n = 0;

    int counter = start;//to keep track of the number of item in the main array


    while (m < item1 && n < item2) {//while there is same number of item in array1 and array2, two sorted arrays

        if (array1[m] <= array2[n]) {//if array1 has smaller number, it is being replaced first in the main array
            array[counter] = array1[m];
            m++;
        } else {
            array[counter] = array2[n];//if array2 has smaller instead
            n++;
        }
        counter++;//moving on the counter in main array
    }
    

    while (m < item1) { //to add the left overs of the array1
        array[counter] = array1[m];
        m++;
        counter++;
    }
    

    while (n < item2) { //to add the left overs of the array2
        array[counter] = array2[n];
        n++;
        counter++;
    }
    
    return;//returning nothing
}








