// THIS IS C99!
#include <stdio.h>
#include <string.h>

#define NAMELEN 32
#define NUMNAMES 1024

void initArrays(int rank[][10], char names[][NAMELEN])
{
    for (int i = 0; i < NUMNAMES; i++)
    {
        names[i][0] = '\0';
        for (int j = 0; j < 10; j++)
            rank[i][j] = -1;
    }
}

void addName(char* name, int num, char names[][NAMELEN], int rank[][10],
            int year)
{
    for(int i = 0; i < NUMNAMES; i++)
    {
        //add name if it is not there
        if (names[i][0] == 0)
        {
            strcpy (names[i], name);
            rank[i][year] = num;
            return;
        }
        //if it is there add the new rank for the year
        else if (strcmp(names[i], name) == 0)
        {
            rank[i][year] = num;
            return;
        }
    }

}

void readNameFile(char* fileName, char names[][NAMELEN],
                   int rank[][10], int year)
{

    FILE *infile = fopen(fileName, "r");
    char name[NAMELEN], gender[5];
    int num;
    for (int i = 0; i < 100; i++)
    {
        fscanf(infile, "%[^,]%*c%[^,]%*c%d\n", name, gender, &num);
        addName(name, num, names, rank, year);
    }
}

void sortArrays(char names[][NAMELEN], int rank[][10])
{
    for (int i = 0; i < NUMNAMES; i++)
    {
        for (int j = 0; j+1 < NUMNAMES; j++)
        {
            if (names[j][0] != 0 &&
                names[j+1][0] != 0 &&
                strcmp(names[j], names[j+1]) > 0)
            {    
                char nameTemp[NAMELEN];
                int rankTemp[10];

                strcpy(nameTemp, names[j]);
                for (int k = 0; k < 10; k++)
                    rankTemp[k] = rank[j][k];

                strcpy(names[j], names[j+1]);
                for (int k = 0; k < 10; k++)
                    rank[j][k] = rank[j+1][k];

                strcpy(names[j+1], nameTemp);
                for (int k = 0; k < 10; k++)
                    rank[j+1][k] = rankTemp[k];
            }
        }
    }
}

void writeCsvFile(char* filename, char names[][NAMELEN], int rank[][10])
{
    FILE *outFile = fopen(filename, "w+");
    fprintf(outFile, "Name,1920,1930,1940,1950,1960,1970,1980,1990,2000,2010\n");
    for (int i = 0; i < NUMNAMES; i++)
    {
        fprintf(outFile,"%s",names[i]);
        //dont print -1 ranks
        for (int j = 0; j < 10; j++)
        {
            if (rank[i][j] > 0)
                fprintf(outFile, ",%d", rank[i][j]);
            else
                fprintf(outFile, ",");
        }

        fprintf(outFile,"\n");
    }

}

void readFiles ( char names[][NAMELEN], int rank[][10])
{
    readNameFile("yob1920.txt",names, rank, 0);
    readNameFile("yob1930.txt",names, rank, 1);
    readNameFile("yob1940.txt",names, rank, 2);
    readNameFile("yob1950.txt",names, rank, 3);
    readNameFile("yob1960.txt",names, rank, 4);
    readNameFile("yob1970.txt",names, rank, 5);
    readNameFile("yob1980.txt",names, rank, 6);
    readNameFile("yob1990.txt",names, rank, 7);
    readNameFile("yob2000.txt",names, rank, 8);
    readNameFile("yob2010.txt",names, rank, 9);
}

int main(void)
{
    char names[NUMNAMES][NAMELEN];
    int rank[NUMNAMES][10];

    initArrays(rank, names);
    readFiles(names, rank);
    sortArrays(names, rank);
    writeCsvFile("summary.csv", names, rank);

    return 0;    

}
