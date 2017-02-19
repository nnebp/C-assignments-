#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "aidb.h"

char *getMajor(int major)
{
    char result[64];

    switch (major)
    {
        case 0:
            strcpy(result, "CS");
            break;
        case 1:
            strcpy(result, "CE");
            break;
        case 2:
            strcpy(result, "MATH");
            break;
        case 3:
            strcpy(result, "SOC WORK");
            break;
        case 4:
            strcpy(result, "NURSING");
            break;
    }

    return strdup(result);
}

char *getString(int tableOffset, int stringOffset, char *filebytes)
{
    char result[64];
    
    int i;
     for ( i = 0 ; '\0' != filebytes[tableOffset + stringOffset + i]; i++)
     {
         result[i] = filebytes[tableOffset + stringOffset + i];
     }
     //add null 
     result[i] = '\0';
    
    return strdup(result);

}

void printStudents(int offset, int string_table_offset, char *filebytes, int entries)
{
    Student *studentptr =  (Student *) (filebytes + offset);
    for (int i = 0; i < entries; i++)
    {
        Student *student = studentptr + i;
        printf("%04d\t%s\t\t%s\n", student->id_num, getString(string_table_offset, student->name ,filebytes), 
                getMajor(student->major));
    }
    
}

void printCourses(int course_table_offset, int string_table_offset, char *filebytes, int entries)
{
    Course *courseptr = (Course *) (filebytes + course_table_offset);
    for (int i = 0; i < entries; i++)
    {
        Course *course = courseptr + i;
        printf("%-25s %d\n", getString(string_table_offset, course->name, filebytes), course->max_enroll);
    }
}

void printEnroll(int course_table_offset, int string_table_offset, int student_table_offset,
                 int enroll_table_offset, char *filebytes, int entries)
{
    Enroll *enrollptr = (Enroll *) (filebytes + enroll_table_offset);
    for (int i = 0; i < entries; i++)
    {
        Enroll *enroll = enrollptr + i;
        
        Student* student = (Student *) (filebytes + student_table_offset);
        student += enroll->student;

        Course *course = (Course *) (filebytes + course_table_offset);
        course += enroll->course;
        
        printf("%s\t\t%s\n", getString(string_table_offset, student->name, filebytes),
                          getString(string_table_offset, course->name, filebytes));

    }
}

int main ()
{
    int student_table_offset;
    int course_table_offset;
    int enroll_table_offset;
    int string_table_offset;

    int student_entries;
    int course_entries;
    int enroll_entries;

    struct stat st;
    stat("aidb.bin", &st);
    char filebytes[st.st_size];
    
    FILE *input = fopen("aidb.bin", "rb");
    fread(filebytes, 1, st.st_size, input);


    AIDB_FileHeader *header = (AIDB_FileHeader *) filebytes;

    //get offsets
    SectionHeader *section = (SectionHeader *) (filebytes + header->section_table_offset);
    for (int i = 0; i < header->section_count; i++)
    {
        SectionHeader *sectionptr = section + i;
        switch (sectionptr->type)
        {
            case 0:
                student_table_offset = sectionptr->offset;
                student_entries = sectionptr->num_entries;
                break;
            case 1:
                course_table_offset = sectionptr->offset;
                course_entries = sectionptr->num_entries;
                break;
            case 2:
                enroll_table_offset = sectionptr->offset;
                enroll_entries = sectionptr->num_entries;
                break;
            case 3:
               string_table_offset = sectionptr->offset;
                break;
        }
        
    
    }
    printf("AIDB Tables \n\nStudents\n");
    printStudents(student_table_offset, string_table_offset, filebytes, student_entries);
    printf("\nCourses\n");
    printCourses(course_table_offset, string_table_offset, filebytes, course_entries);
    printf("\nEnrollments\n");
    printEnroll(course_table_offset, string_table_offset, student_table_offset,
                 enroll_table_offset, filebytes, enroll_entries);
    

}
