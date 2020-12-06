#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>


using namespace std;


bool is_sorted(int* values, int size);
void insertion_sort(int* values, int size);
void merge_sort(int* values, int start_i, int end_i);
void merge(int* values, int start_i, int mid_i, int end_i);
void fill_array(int* values, int size);
void copy_array(int* copy, int* original, int size);
void test_sorts();
void print_menu();
void print_array(int* arr, int size);


/*
    Main
*/
int main()
{
    
    char user_input;

    //if q is entered from the menue, working becomes false and exits
    while (true)
    {
        print_menu();
        cin >> user_input;
        user_input = toupper(user_input);

        //If insertion or merge is selected, take in values, perform sort, print values
        if (user_input == 'I' || user_input == 'M')
        {
            vector<int> values;
            int num;

            cout << "Input: ";

            while (cin >> num)
            {
                if (num == -999)        //-999 is delimiter
                    break;
                values.push_back(num);
            }

            int size = values.size();       //convert num vector to array for sorts
            int* value_arr = new int[size];
            for (int i = 0; i < size; i++)
            {
                value_arr[i] = values[i];
            }

            if (user_input == 'I')          //perform correct sorts
            {
                insertion_sort(value_arr, size);
            }
            else
            {
                merge_sort(value_arr, 0, size - 1);
            }


            cout << "Sorted: ";             //prints sorted array
            print_array(value_arr, size);
            cout << endl;
            delete[] value_arr;
        }
        if (user_input == 'Q')      //quit
        {
            return 0;
        }
        if (user_input == 'T')      //perform test making CSV file
        {
            test_sorts();
        }
    }
}

/*
    checks if an array is sorted
    returns true if yes
*/
bool is_sorted(int* values, int size)
{
    for(int i=1; i<size; i++)
    {
        if(values[i] < values[i-1])
            return false;
    }
    return true;
}


//GRADING: INSERT
/*
    Performs insertion sort on an array given its size
*/
void insertion_sort(int* values, int size)
{
    int key;
    for(int j=0; j<size; j++)
    {
        key = values[j];
        int i = j - 1;
        while( (i>=0) && (values[i]>key) )
        {
            values[i + 1] = values[i];
            i = i-1;
        }
        values[i+1] = key;
    }
}

//GRADING: MERGE
/*
    Performs merge sort on an array given the start and end index
*/
void merge_sort(int* values, int start_i, int end_i)
{

    if(start_i < end_i)
    {
        

        int mid_i = (start_i + end_i) / 2 ;      //calculate midpoint
        merge_sort(values, start_i, mid_i);     //perform left sort
        merge_sort(values, mid_i+1, end_i);     //perform right sort
        merge(values, start_i, mid_i, end_i);   //combine
    }
}

/*
    Mergesort helper function that zips together arrays post recursion
*/
void merge(int* values, int start_i, int mid_i, int end_i)
{

    int n1 = mid_i - start_i + 1;    //represents number of things in bucket 1 n1 is always +1 n2
    int n2 = end_i - mid_i;

    int* L = new int[n1 + 1];           //left and right side arrays
    int* R = new int[n2 + 1];           //+1 to make room for infinity last element


    for(int i=0; i<n1; i++)         //placing numbers on correct side
    {
        L[i] = values[start_i + i];
    }
    for(int i=0; i<n2; i++)
    {
        R[i] = values[mid_i + i + 1]; //in case (0,0,1) midi+j = 0 and thus the second element is not in j
    }

    int i = 0;
    int j = 0;

    for(int k=start_i; k<=end_i; k++)
    {
        //if Left index(i) reaches infinity, assume R value is less
        //if Right index(i) reaches infinity, assume L value is less
        if( (j == n2) || ( (L[i] <= R[j]) && (i != n1)) )
        {
            values[k] = L[i++];
        }
        else
        {
            values[k] = R[j++];
        }
    }


    delete[] R;
    delete[] L;
}

/*
    fills an array with random values between -500 and 499
*/
void fill_array(int* values, int size)
{
    for( int i=0; i< size; i++)
    {
        values[i] = rand()%1000 - 500;
    }
}

/*
    copies an array
*/
void copy_array(int* copy, int* original, int size)
{
    for(int i=0; i<size; i++)
    {
        copy[i] = original[i];
    }
    return;
}

/*
    performs time testing on sorts saving the test results to a csv file
*/
void test_sorts()
{
    ofstream fout;
    fout.open("Sort_test.csv");

    std::chrono::time_point<std::chrono::system_clock> start, end; 
    std::chrono::duration<double> insertion_time, merge_time;

    fout << "size,Insertion,Merge" << endl;
    for(int size=2; size<=10000; size+=100)
    {
        int* values = new int[size];    //array to hold orignal values
        int* copy = new int[size];      //array to hold copy of original values

        fill_array(values, size);       //fills array with random integers
        copy_array(copy, values, size);
        start = std::chrono::system_clock::now(); 
        insertion_sort(values, size);
        end = std::chrono::system_clock::now(); 
        insertion_time = end - start;
        start = std::chrono::system_clock::now();
        merge_sort(copy, 0, size-1);
        end = std::chrono::system_clock::now();
        merge_time = end - start;

        fout << size << "," << insertion_time.count() << "," << merge_time.count() << endl;

        delete[] values;
        delete[] copy;
    }
    fout.close();
}


/*
    Prints menue to screen
*/
void print_menu()
{
    cout << "\nEnter T to run time tests\n"
        << "Enter I to accept user input and perform an insertion sort\n"
        << "Enter M to accept user input and perform a merge sort\n"
        << "Enter Q to quit" << endl;

    cout << "Choice: ";
    return;
}


/*
    Prints array to screen
*/
void print_array(int* arr, int size)
{
    if(size == 0)
    {
        return;
    }
    for(int i=0; i<size-1; i++)
    {
        cout << arr[i] << " ";
    }
    cout << arr[size-1];
    return;
}

