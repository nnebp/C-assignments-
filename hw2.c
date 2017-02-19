// THIS IS C99!
#include <stdio.h>
#include <stdlib.h>

struct bitmapInfo
{
    unsigned char headerField[2];
    int fileSize;
    int reserved; //dont need this
    int offset;
    int size;//size of header
    int width;
    int height;
    unsigned char dontCare[4*7];
};


int main(void)
{
    struct bitmapInfo header;
    FILE *infile = fopen("test.bmp", "rb");
    FILE *outfile = fopen("reflect.bmp", "wb");

    fread(header.headerField,sizeof(char) ,2,infile);
    fread(&header.fileSize,sizeof(int) ,1,infile);
    fread(&header.reserved,sizeof(int) ,1,infile);
    fread(&header.offset,sizeof(int) ,1,infile);
    fread(&header.size,sizeof(int) ,1,infile);
    fread(&header.width,sizeof(int) ,1,infile);
    fread(&header.height,sizeof(int) ,1,infile);
    fread(header.dontCare,sizeof(char) ,28,infile);
    unsigned char space[header.height][header.width *3];
    unsigned char outSpace[header.height][header.width*3*2];
    unsigned char *p = (char*) outSpace;
    //our main array
    fread(space,sizeof(char) ,header.fileSize-54,infile);  


    
    for (int r = 0 ; r < header.height; r++) 
    {
    
       for (int c = 0; c < (header.width*3); c++) 
       {
           outSpace[r][c] = space[r][c];
       }
    }

    for (int r = 0 ; r < header.height; r++)
    {
       for (int c = 0; c < (header.width*3)/2; c++) 
       {
           unsigned char temp = space[r][c];
           outSpace[r][c + (header.width*3)]= space[r][(header.width*3)-1-c];
           outSpace[r][(header.width*3)+(header.width*3)-c-1] = temp;
       }
    }
    //fix the RGB values from the flip
    for (int g; g < header.width*header.height*3*2; g += 3)
    {
        unsigned char ch =  *(p + g);
        *(p + g) = *(p + g + 2);
        *(p + g + 2) = ch;
    }

    for (int r = 0 ; r < header.height; r++) {
        for (int c = 0; c < (header.width*3); c++) {
            outSpace[r][c] = space[r][c];
        }
    }
    
    int tempwidth = header.width * 2;
    header.fileSize = (header.fileSize *4) -(54*4);
    fwrite(header.headerField,sizeof(char) ,2,outfile);
    fwrite(&header.fileSize,sizeof(int) ,1,outfile);
    fwrite(&header.reserved,sizeof(int) ,1,outfile);
    fwrite(&header.offset,sizeof(int) ,1,outfile);
    fwrite(&header.size,sizeof(int) ,1,outfile);
    fwrite(&tempwidth,sizeof(int) ,1,outfile);
    fwrite(&header.height,sizeof(int) ,1,outfile);
    fwrite(header.dontCare,sizeof(char) ,28,outfile);

    fwrite(outSpace,sizeof(char) ,header.width*header.height*3*2,outfile);  

    //Write quad.bmp
    outfile = fopen("quad.bmp", "wb");
    unsigned char outSpaceQuad[header.height*2][header.width*3*2];

    for (int r = 0 ; r < header.height; r++) {
       for (int c = 0; c < (header.width*3); c++) {
            unsigned char temp = space[r][c];
            outSpaceQuad[r][c] = temp;
            outSpaceQuad[r + header.height][c] = temp;
            outSpaceQuad[r + header.height][c + (header.width*3)] = temp;
            outSpaceQuad[r][c + (header.width*3)] = temp;
       }
    
    }

    header.width *= 2;
    header.height *= 2;
    header.fileSize = (header.fileSize *4) -(54*4);
    fwrite(header.headerField,sizeof(char) ,2,outfile);
    fwrite(&header.fileSize,sizeof(int) ,1,outfile);
    fwrite(&header.reserved,sizeof(int) ,1,outfile);
    fwrite(&header.offset,sizeof(int) ,1,outfile);
    fwrite(&header.size,sizeof(int) ,1,outfile);
    fwrite(&header.width,sizeof(int) ,1,outfile);
    fwrite(&header.height,sizeof(int) ,1,outfile);
    fwrite(header.dontCare,sizeof(char) ,28,outfile);

    fwrite(outSpaceQuad,sizeof(char) ,header.width*header.height*3*4,outfile);  
    return 0;    

}
