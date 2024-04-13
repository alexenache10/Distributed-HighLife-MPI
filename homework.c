#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int** local_matrix = NULL;
int* upper_line = NULL; // doar in fct. de astea vom comunica intre pasi
int* lower_line = NULL;
int* results = NULL;
int** full_matrix = NULL;

void assignNewValues( int amount, int columns)
{   
    int i,j;
    for(i = 0; i < amount; i++)
        for(j = 0; j < columns; j++)
        {
            if(results[i*columns+j] == 0 || results[i*columns+j] == 1 )
                local_matrix[i][j] = results[i*columns+j];
      

        }
}
void assignNewValuesRoot(int amount, int columns)
{   
    for(int i = 0; i < amount; i++)
        for(int j = 0; j < columns; j++)
        {
                if(results[i*columns+j] == 0)
                    full_matrix[i][j] = results[i*columns+j];
                if(results[i*columns+j] == 1 )
                     full_matrix[i][j] = results[i*columns+j];
        }
}

void calculateStep(int columns, int rank, int nProcesses, int amount)
{
    int i, j;
    int count;
        for(i = 0; i < amount; i++) // atatea linii are
            for(j = 0; j < columns; j++)
            {
                count = 0;
                if(rank != 0)
                {
                    if(j != 0 && j != columns -1) // element prin mijloc
                    {
                        count = count + local_matrix[i][j-1] + local_matrix[i][j+1];
                        if(i != 0 && i != amount - 1) // nici prima nici ultima linie
                        {
                            count = count + local_matrix[i+1][j-1] + local_matrix[i+1][j] + local_matrix[i+1][j+1] +
                                            local_matrix[i-1][j] + local_matrix[i-1][j+1] + local_matrix[i-1][j-1];
                        }    
                        else if(i==0)
                        {
                            count = count+ upper_line[j] + upper_line[j-1] + upper_line[j+1] +
                                    local_matrix[i+1][j] + local_matrix[i+1][j-1] + local_matrix[i+1][j+1];
                        }
                        else if(i== amount-1)
                        {
                            count = count + local_matrix[i-1][j] + local_matrix[i-1][j+1] + local_matrix[i-1][j-1];
                            if(rank != nProcesses -1) // daca nu e ultimul proces
                            {
                                count = count + lower_line[j] + lower_line[j+1] + lower_line[j-1];
                            }
                        }
                    }
                    else if(j == 0) // prima coloana
                    {
                        count = count  + local_matrix[i][j+1];
                        if(i != 0 && i != amount - 1) // nici prima nici ultima linie
                        {
                            count = count  + local_matrix[i+1][j] + local_matrix[i+1][j+1] +
                                            local_matrix[i-1][j] + local_matrix[i-1][j+1] ;
                        }    
                        else if(i==0)
                        {
                            count = count+ upper_line[j]  + upper_line[j+1] +
                                    local_matrix[i+1][j]  + local_matrix[i+1][j+1];
                        }
                        else if(i== amount-1)
                        {
                            count = count + local_matrix[i-1][j] + local_matrix[i-1][j+1] ;
                            if(rank != nProcesses -1) // daca nu e ultimul proces
                            {
                                count = count + lower_line[j] + lower_line[j+1];
                            }
                        }
                    }
                    else if(j == columns-1) // ultima coloana
                    {
                        count = count + local_matrix[i][j-1];
                        if(i != 0 && i != amount - 1) // nici prima nici ultima linie
                        {
                            count = count + local_matrix[i+1][j-1] + local_matrix[i+1][j]  +
                                            local_matrix[i-1][j]  + local_matrix[i-1][j-1];
                        }    
                        else if(i==0)
                        {
                            count = count+ upper_line[j] + upper_line[j-1]  +
                                    local_matrix[i+1][j] + local_matrix[i+1][j-1] ;
                        }
                        else if(i== amount-1)
                        {
                            count = count + local_matrix[i-1][j] + local_matrix[i-1][j-1];
                            if(rank != nProcesses -1) // daca nu e ultimul proces
                            {
                                count = count + lower_line[j] + lower_line[j-1];
                            }
                        }
                    }
                }
                else
                {
                    if(j != 0 && j != columns -1) // element prin mijloc
                    {
                        count = count + full_matrix[i][j-1] + full_matrix[i][j+1];
                        if(i != 0 && i != amount - 1) // nici prima nici ultima linie
                        {
                            count = count + full_matrix[i+1][j-1] + full_matrix[i+1][j] + full_matrix[i+1][j+1] +
                                            full_matrix[i-1][j] + full_matrix[i-1][j+1] + full_matrix[i-1][j-1];
                        }    
                         if(i==0)
                        {
                            count = count+ full_matrix[i+1][j] + full_matrix[i+1][j-1] + full_matrix[i+1][j+1];
                        }
                         if(i== amount-1)
                        {
                            count = count + full_matrix[i-1][j] + full_matrix[i-1][j+1] + full_matrix[i-1][j-1];
                            if(nProcesses != 1)
                            {
                                count = count + full_matrix[i+1][j] + full_matrix[i+1][j+1] + full_matrix[i+1][j-1];
                            }
                        }
                    }
                    if(j == 0) // prima coloana
                    {
                        count = count +  full_matrix[i][j+1];
                        if(i != 0 && i != amount - 1) 
                        {
                            count = count  + full_matrix[i+1][j] + full_matrix[i+1][j+1] +
                                            full_matrix[i-1][j] + full_matrix[i-1][j+1];
                        }    
                         if(i==0)
                        {
                            count = count+ full_matrix[i+1][j]  + full_matrix[i+1][j+1];
                        }
                         if(i== amount-1)
                        {
                            count = count + full_matrix[i-1][j] + full_matrix[i-1][j+1];
                            if(nProcesses != 1)
                            {
                                count = count + full_matrix[i+1][j] + full_matrix[i+1][j+1];
                            }
                        }
                    }
                    if(j == columns-1) // ultima coloana
                    {
                        count = count + full_matrix[i][j-1] ;
                        if(i != 0 && i != amount - 1) // nici prima nici ultima linie
                        {
                            count = count + full_matrix[i+1][j-1] + full_matrix[i+1][j] +
                                            full_matrix[i-1][j]  + full_matrix[i-1][j-1];
                        }    
                         if(i==0)
                        {
                            count = count+ full_matrix[i+1][j] + full_matrix[i+1][j-1];
                        }
                         if(i== amount-1)
                        {
                            count = count + full_matrix[i-1][j]  + full_matrix[i-1][j-1];
                            if(nProcesses != 1)
                            {
                                count = count + full_matrix[i+1][j]  + full_matrix[i+1][j-1];
                            }
                        }
                    }
                }
             if(rank != 0)         
            { 
                 if(local_matrix[i][j] == 1)
                 {
                    if(count != 2 && count != 3)
                        results[i*columns+j] = 0;
                 }
                 else
                {
                    if(count == 3 || count == 6)
                    {
                        results[i*columns+j] = 1;
                    }
                }
            }
            else
            {
                if(full_matrix[i][j] == 1)
                 {
                    if(count != 2 && count != 3)
                        results[i*columns+j] = 0;
                 }
                 if(full_matrix[i][j] == 0)
                {
                    if(count == 3)
                    {
                         results[i*columns+j] = 1;
                    }
                    if( count == 6)
                     results[i*columns+j] = 1;
                }
            }
              
            }
             
           

    

}



int main(int argc, char** argv)
{
    int rank, nProcesses;
    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);


    int nReps = atoi(argv[3]);
    
    int i, j;
    int lines, columns;

    MPI_Request request;
    MPI_Status status;
    if(rank == 0)
    {
        FILE* fileptr = fopen(argv[1], "r");
        fscanf(fileptr,"%d %d\n", &lines, &columns);
        MPI_Bcast(&lines, 1, MPI_INT, 0, MPI_COMM_WORLD);
       MPI_Bcast(&columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
   

        full_matrix = (int**)malloc(sizeof(int*)*lines);
        for(i = 0; i < lines; i++)
            full_matrix[i]=(int*)malloc(sizeof(int)*columns);

        for(i = 0; i < lines; i++)
            for(j = 0; j < columns; j++)
                fscanf(fileptr, "%d", &full_matrix[i][j]);
        
        int proc_index = 1;
        int count_index = 0;

        if(nProcesses != 1)
        {
        for(i =  lines / nProcesses + lines % nProcesses; i < lines; i++)
        {
            if(count_index == 0) // suntem prima data in proces, ii dam si linia de dinainte
            {
                MPI_Send(full_matrix[i-1], columns, MPI_INT, proc_index, 0, MPI_COMM_WORLD);
            }
            MPI_Send(full_matrix[i], columns, MPI_INT, proc_index, 0, MPI_COMM_WORLD);
            count_index++;

            // conditie sa ii putem da ultima linie: sa fi terminat de trimis toate liniile sale si sa nu fie ultimul proces 
            if(count_index == lines / nProcesses && proc_index != nProcesses -1) 
            {
                MPI_Send(full_matrix[i+1], columns, MPI_INT, proc_index, 0, MPI_COMM_WORLD);
                count_index=0;
                proc_index++;
              
            }
        }
        
       
         }
        fclose(fileptr);
    }
    else
    {
        MPI_Bcast(&lines, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
       /*      FILE* fileptr = fopen(argv[1], "r");
        fscanf(fileptr,"%d %d\n", &lines, &columns);
  */


        upper_line = (int*)malloc(sizeof(int)*columns); // linia de dinainte trebuie mereu
        MPI_Irecv(upper_line, columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        local_matrix = (int**)malloc(sizeof(int*)*lines/nProcesses);
        for(i = 0; i < lines/nProcesses; i++)
            local_matrix[i] = (int*)malloc(sizeof(int)*columns);
  
        for(i = 0; i < lines/nProcesses; i++)
            MPI_Irecv(local_matrix[i], columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        if(rank != nProcesses -1)
        {
            lower_line = (int*)malloc(sizeof(int)*columns);
            MPI_Irecv(lower_line, columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        }
       // fclose(fileptr);
      
        
    }
    if(rank != 0)
    {
        results = (int*)malloc(sizeof(int)*(lines/nProcesses*columns));
        for(i = 0; i < lines/nProcesses*columns; i++)
            results[i] = 99;
          MPI_Wait(&request, &status);
        for(i = 0; i < nReps; i++)
        {
       
            calculateStep(columns, rank, nProcesses, lines/nProcesses);
            assignNewValues(lines/nProcesses, columns); 
            MPI_Sendrecv(local_matrix[0],columns,MPI_INT, rank-1, 0, upper_line, columns, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
            if(rank != nProcesses - 1)
            {
                MPI_Sendrecv(local_matrix[lines/nProcesses-1], columns, MPI_INT, rank+1, 0, lower_line, columns, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
                
            }
        }
        
        for(i = 0; i < lines/nProcesses; i++)
        {
            MPI_Send(local_matrix[i], columns, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        results = (int*)malloc(sizeof(int)*((lines/nProcesses)+lines%nProcesses)*columns);
        for(i = 0; i < (lines/nProcesses + lines%nProcesses)*columns; i++)
            results[i] = 99;

        for(i = 0; i < nReps; i++)
        {
            
            calculateStep(columns,rank,nProcesses,lines/nProcesses+lines%nProcesses);
            assignNewValuesRoot( lines/nProcesses+lines%nProcesses, columns);
            if(nProcesses != 1)
                 MPI_Sendrecv(full_matrix[lines/nProcesses+lines%nProcesses-1], columns, MPI_INT, rank+1, 0, full_matrix[lines/nProcesses+lines%nProcesses], columns, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
        }



        int line_count = lines/nProcesses+lines%nProcesses;
        if(nProcesses != 1)
        for(i =1 ; i < nProcesses; i++)
        {
            for(j = 0; j < lines/nProcesses; j++)
            {
                MPI_Recv(full_matrix[line_count++], columns, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                
            }

        }
     
    }

    if(rank == 0)
    {
        FILE* out_file =fopen(argv[2], "w");
        fprintf(out_file, "%d %d\n", lines,columns);
        for(i =0; i < lines; i++){
            for(j = 0 ;  j < columns; j++)
                fprintf(out_file,"%d ", full_matrix[i][j]);
            fprintf(out_file, "\n");
        }
        fclose(out_file);
    }

    free(results);
    if(rank != 0)
    {
    for(i = 0; i < lines/nProcesses; i++)
        free(local_matrix[i]);
    free(local_matrix);
    free(upper_line);
    free(lower_line);
    }
    if(rank == 0)
    {
        for(i = 0; i < lines; i++)
            free(full_matrix[i]);
        free(full_matrix);
   }

    MPI_Finalize();
    return 0;
}