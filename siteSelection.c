/****** compile:  cc siteSelection.c -o siteSelection +DAportable -lm  ************/
/****** compile notes:  The software is compatible with the cc compiler on HP3600 machines running HP-UX 10.20. ***********/

/* Written by Svadrut Kukunooru (to whoever reading this, Wisconsin is the best school) */

/* Revision History */

/* Version 0.9.0 - 06 June 2023 - Finished building the random site selector as well as the fixed site selector. Currently working on outputting
                                the resulting coordinates as a .waf/.tpl file. May have to port into LOR to do this. -SK */

/*/////////////////////////////////////////
//////////////// LIBRARIES ////////////////
/////////////////////////////////////////*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h>
#include <ctype.h>
#include <time.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
/*/////////////////////////////////////////////////////////////////////
//////////////// HELPER FUNCTIONS VARIABLE DEFINITIONS ////////////////
/////////////////////////////////////////////////////////////////////*/

/* dimensions of die array */
int DIMX;
int DIMY;
/* number of valid sites*/
int VALID_SITES;
/* counter variables */
int i;
int j;
int counter; 
int arrCounter;
/* variables for reading in file */ 
char line[200];
int lineCounter;
int xcoords[10000]; 
int ycoords[10000];
char * toRead;
int ** die;
char * file;
time_t start; 
time_t current;
char * dot;
FILE *fp;

/* variables for outputting file */ 
char outFile[100];
char ch;
FILE *source, *output, *tmp;
int x; 
int y;

/*////////////////////////////////////////////////////////////////////////////////////
//////////////////// SITE SELECTION FUNCTION VARIABLE DEFINITIONS ////////////////////
////////////////////////////////////////////////////////////////////////////////////*/

/* variables for center coordinates */
int centerX; 
int centerY;

int radius; 
int randIndex; 
int randX1; 
int randY1; 
int randX2; 
int randY2; 
int randX3; 
int randY3; 
int randX4; 
int randY4;
int randX5;
int randY5;

/* arrays for random sites */ 
int * firstSectionXCoords;
int * firstSectionYCoords; 
int * secondSectionXCoords;
int * secondSectionYCoords;
int * thirdSectionXCoords;
int * thirdSectionYCoords;
int * fourthSectionXCoords;
int * fourthSectionYCoords;
int * fifthSectionXCoords;
int * fifthSectionYCoords;

/* arrays for random sites relative to center */
double * secondSectionXCoordsRelativeToCenter;
double * secondSectionYCoordsRelativeToCenter;
double * thirdSectionXCoordsRelativeToCenter;
double * thirdSectionYCoordsRelativeToCenter;
double * fourthSectionXCoordsRelativeToCenter;
double * fourthSectionYCoordsRelativeToCenter;

/* counters for random sites */ 
int firstCounter;
int secondCounter;
int thirdCounter;
int fourthCounter;
int fifthCounter;
double ** relativeToCenterCoords;

/*////////////////////////////////////////////////////////////////////////////////////
////////////////////// MAIN FUNCTION VARIABLE DEFINITIONS ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////*/
int sites; 
char * last;
/* struct for random sites */
typedef struct SITES {
    int ** coords;
    int numSites;
} SITES;

SITES randomSites; /* struct for random sites */
SITES fixedSites; /* struct for fixed sites */

/* compare r values for qsort */
int compare(a, b) double * a; double * b; {
    return ( *(int*)a - *(int*)b );
}

/*////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// PRINT SITES //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////*/
void * printSites(sites) SITES sites; {
    printf("You have selected %d sites. \n", sites.numSites);
    printf("Sites will be printed via coordinates (x, y). \n");
    printf("--------------------\n");
    for(i = 0; i < sites.numSites; i++) {
        printf("%d. ", i + 1);
        printf("(%d,%d)\n", sites.coords[i][0], sites.coords[i][1]);
    }
    printf("--------------------\n");
    return 0;
}

/*/////////////////////////////////////////////////////////////////
/////////////////////// PRINT DIE ARRAY ///////////////////////////
/////////////////////////////////////////////////////////////////*/
int printDie(die) int ** die; {
    printf("--------------------\n");
    printf("DIE ARRAY: \n");
    
    for(i = 0; i < DIMY; i++) {
        printf("%-2d | ", DIMY - i - 1);
        for(j = 0; j < DIMX; j++) {
            if(die[i][j] == 0) {
                printf("  ");
            }
            else {
                printf("%d ", die[i][j]);
            }
        }
        printf("\n");
    }
    printf("    ");
    for(i = 0; i < DIMX; i++) {
        printf("--");
    }
    printf("\n");
    printf("\n\n");
    return 0;
}

/*//////////////////////////////////////////////////////////
//////////////// .WAF FILE READER FUNCTION ////////////////
//////////////////////////////////////////////////////////*/
int ** read(filename) char * filename; {
    start = time(NULL);
    file = filename; 
    dot = strrchr(filename, '.');
    if(strcmp(dot, ".waf") == 0) {   
        /* read in file using filename */
        fp = fopen(filename, "r");
        /* check if file exists */
        if (fp == NULL)
        {
            printf("Error: could not open file %s", filename);
            return NULL;
        }
        /* FIND DIMENSIONS OF DIE ARRAY */
        /* find number of lines */
        lineCounter = 0;
        while(1) {
            if(fgets(line, sizeof(line), fp) == NULL) {
                break;
            }
            lineCounter++;
        }
        /* reset counter and array counter */
        counter = 0; 
        arrCounter = 0;
        rewind(fp); 
        /* read in file again */
        while (fgets(line, sizeof(line), fp))
        {   
            /* split string by comma and store in xcoords and ycoords */ 
            /* increment line counter */

            if(counter > 25 && counter < lineCounter - 2) {
                toRead = line + 31; 
                
                sscanf(toRead, "%d,%d", &xcoords[arrCounter], &ycoords[arrCounter]);
                arrCounter++;
            }
            counter++;     
        }
        /* close file */
        fclose(fp);
        
        
        /* find max of xcoords and ycoords */
        DIMX = xcoords[0];
        DIMY = ycoords[0];
        for(i = 0; i < arrCounter; i++) {
            if(xcoords[i] > DIMX) {
                DIMX = xcoords[i];
            }
            if(ycoords[i] > DIMY) {
                DIMY = ycoords[i];
            }
        }
        /* add 1 to DIMX and DIMY */
        DIMX++;
        DIMY++;

        /* ///////////////////////////////////////////////// */
        /* make 2d array of die */
        die = malloc((size_t) (DIMY) * sizeof(int *));
        for(i = 0; i < DIMY; i++) {
            die[i] = malloc((size_t) (DIMX) * sizeof(int));
        }
        /* fill die array with 0s */
        for(i = 0; i < DIMY; i++) {
            for(j = 0; j < DIMX; j++) {
                die[i][j] = 0;
            }
        }
        VALID_SITES = 0;
        /*fill die array with 1s */
        for(i = 0; i < arrCounter; i++) {
            if(xcoords[i] != -1 && ycoords[i] != -1) {
                die[DIMY - 1 - ycoords[i]][xcoords[i]] = 1;
                VALID_SITES++;
            }
        } 
            return die;
    }
    else if(strcmp(dot, ".tpl") == 0) {
        /* read in file using filename */
        fp = fopen(filename, "r");
        /* check if file exists */
        if (fp == NULL)
        {
            printf("Error: could not open file %s", filename);
            return NULL;
        }
        /* FIND DIMENSIONS OF DIE ARRAY */
        /* find number of lines */
        lineCounter = 0;
        counter = 0;
        while(fgets(line, sizeof(line), fp)) {
            if(counter > 34 && strncmp(line, "$", strlen("$")) == 0) {
                break;
            }
            lineCounter++;
            counter++;
        }
        rewind(fp);
        arrCounter = 0;
        counter = 0; 
        while (fgets(line, sizeof(line), fp))
        {   
            /* split string by comma and store in xcoords and ycoords */ 
            /* increment line counter */

            if(counter > 34 && counter < lineCounter) {
                toRead = line + 31; 
                
                sscanf(toRead, "%d,%d", &xcoords[arrCounter], &ycoords[arrCounter]);
                arrCounter++;
            }
            counter++;     
        }
        /* close file */
        fclose(fp);
        /* find max of xcoords and ycoords */
        DIMY = xcoords[0];
        DIMX = ycoords[0];
        for(i = 0; i < arrCounter; i++) {
            if(xcoords[i] > DIMX) {
                DIMY = xcoords[i];
            }
            if(ycoords[i] > DIMY) {
                DIMX = ycoords[i];
            }
        }
        /* add 1 to DIMX and DIMY */
        DIMX++;
        DIMY++;
        /* make 2d array of die */
        die = malloc((size_t) (DIMX) * sizeof(int *));
        for(i = 0; i < DIMX; i++) {
            die[i] = malloc((size_t) (DIMY) * sizeof(int));
        }
        /* fill die array with 0s */
        for(i = 0; i < DIMX; i++) {
            for(j = 0; j < DIMY; j++) {
                die[i][j] = 0;
            }
        }
        VALID_SITES = 0;
        /*fill die array with 1s */
        for(i = 0; i < arrCounter; i++) {
            if(xcoords[i] != -1 && ycoords[i] != -1) {
                die[xcoords[i]][ycoords[i]] = 1;
                VALID_SITES++;
            }
        }
        return die;   
    }
    return NULL;
} 


/*/////////////////////////////////////////////////////////////////
/////////////////////// RANDOM SITE SELECTION /////////////////////
/////////////////////////////////////////////////////////////////*/
SITES randomSiteSelection(die, numSites) int ** die; int numSites; {
    if(VALID_SITES < numSites) {
        printf("ERROR: number of sites cannot be this big; die is too small!\n");
        exit(0);
    }
    /* get center of die array */ 
    centerX = DIMX / 2;
    centerY = DIMY / 2;

    if(numSites == 0) {
        printf("ERROR: numSites cannot be 0\n");
        exit(0);
    }
    else if(numSites == 1) {
        /* initialize struct for storing sites */
        randomSites.coords = (int **) malloc((size_t) 1 * sizeof(int *));
        randomSites.coords[0] = (int *) malloc((size_t) 2 * sizeof(int));
        /* pick a random site */
        randX1 = 0;
        randY1 = 0;
        while(die[randX1][randY1] != 1) {
            randX1 = rand() % DIMX;
            randY1 = rand() % DIMY;
        }
        die[randX1][randY1] = 8; 
        randomSites.coords[0][0] = randY1;
        randomSites.coords[0][1] = DIMY - 1 - randX1;
        randomSites.numSites = 1;
        printDie(die);
        printSites(randomSites);
        return randomSites;
        exit(0);
    }
    else if(numSites == 2) {
        /* Divide die into inner and outer circle and pick one element from each */
        /* initialize struct for storing sites */
        randomSites.coords = (int **) malloc((size_t) 2 * sizeof(int *));
        randomSites.coords[0] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.coords[1] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.numSites = 2;
        
        /* find radius of inner circle */
        if(DIMX > DIMY) {
            radius = DIMY / 4;
        }
        else {
            radius = DIMX / 4;
        }
        firstSectionXCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        firstSectionYCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        secondSectionXCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        secondSectionYCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        firstCounter = 0;
        secondCounter = 0;
        
        for(i = 0; i < DIMY; i++) {
            for(j = 0; j < DIMX; j++) {
                if((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) <= radius * radius) {
                    firstSectionXCoords[firstCounter] = i;
                    firstSectionYCoords[firstCounter] = j;
                    firstCounter++;
                }
                else {
                    secondSectionXCoords[secondCounter] = i;
                    secondSectionYCoords[secondCounter] = j;
                    secondCounter++;
                }
            }
        }
        /* pick random site from first section */
        randX1 = 0;
        randY1 = 0;
        while(die[randX1][randY1] != 1) {
            randIndex = rand() % firstCounter;
            randX1 = firstSectionXCoords[randIndex];
            randY1 = firstSectionYCoords[randIndex];
        }
        die[randX1][randY1] = 8;
        randomSites.coords[0][0] = randY1;
        randomSites.coords[0][1] = DIMY - 1 - randX1;
        /* pick random site from second section */
        randX2 = 0;
        randY2 = 0;
        while(die[randX2][randY2] != 1) {
            randIndex = rand() % secondCounter;
            randX2 = secondSectionXCoords[randIndex];
            randY2 = secondSectionYCoords[randIndex];
        }
        die[randX2][randY2] = 8;
        randomSites.coords[1][0] = randY2;
        randomSites.coords[1][1] = DIMY - 1 - randX2;
        printDie(die);
        printSites(randomSites);
        /* free arrs used */
        free(firstSectionXCoords);
        free(firstSectionYCoords);
        free(secondSectionXCoords);
        free(secondSectionYCoords);

        return randomSites;
        exit(0);
       
    }
    else if(numSites == 3) {
        /* initialize struct for storing sites */
        randomSites.coords = (int **) malloc((size_t) 3 * sizeof(int *));
        randomSites.coords[0] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.coords[1] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.coords[2] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.numSites = 3;

        /* find radius of inner circle */
        if(DIMX > DIMY) {
            radius = DIMY / 4;
        }
        else {
            radius = DIMX / 4;
        }
        firstSectionXCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        firstSectionYCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        secondSectionXCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        secondSectionYCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        thirdSectionXCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        thirdSectionYCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        firstCounter = 0;
        secondCounter = 0;
        thirdCounter = 0;

        /* pick random sites from first section */
        for(i = 0; i < DIMY; i++) {
            for(j = 0; j < DIMX; j++) {
                if((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) <= radius * radius) {
                    firstSectionXCoords[firstCounter] = i;
                    firstSectionYCoords[firstCounter] = j;
                    firstCounter++;
                }
                else if((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) > radius * radius && i <= centerX) {
                    secondSectionXCoords[secondCounter] = i;
                    secondSectionYCoords[secondCounter] = j;
                    secondCounter++;
                }
                else if((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) > radius * radius && i > centerX) {
                    thirdSectionXCoords[thirdCounter] = i;
                    thirdSectionYCoords[thirdCounter] = j;
                    thirdCounter++;
                }
            }
        }
        /* pick random site from first section */
        randX1 = 0;
        randY1 = 0;
        while(die[randX1][randY1] != 1) {
            randIndex = rand() % firstCounter;
            randX1 = firstSectionXCoords[randIndex];
            randY1 = firstSectionYCoords[randIndex];
        }
        die[randX1][randY1] = 8;
        randomSites.coords[0][0] = randY1;
        randomSites.coords[0][1] = DIMY - 1 - randX1;
        /* pick random site from second section */
        randX2 = 0;
        randY2 = 0;
        while(die[randX2][randY2] != 1) {
            randIndex = rand() % secondCounter;
            randX2 = secondSectionXCoords[randIndex];
            randY2 = secondSectionYCoords[randIndex];
        }
        die[randX2][randY2] = 8;
        randomSites.coords[1][0] = randY2;
        randomSites.coords[1][1] = DIMY - 1 - randX2;
        /* pick random site from third section */
        randX3 = 0;
        randY3 = 0;
        while(die[randX3][randY3] != 1) {
            randIndex = rand() % thirdCounter;
            randX3 = thirdSectionXCoords[randIndex];
            randY3 = thirdSectionYCoords[randIndex];
        }
        die[randX3][randY3] = 8;
        randomSites.coords[2][0] = randY3;
        randomSites.coords[2][1] = DIMY - 1 - randX3;
        printDie(die);
        printSites(randomSites);

        /* free arrs used*/
        free(firstSectionXCoords);
        free(firstSectionYCoords);
        free(secondSectionXCoords);
        free(secondSectionYCoords);
        free(thirdSectionXCoords);
        free(thirdSectionYCoords);

        return randomSites;
        exit(0);

    }
    else if(numSites == 4) {
        printf("Currently not working\n");
        exit(0); 
        /* initialize struct for storing sites */
        randomSites.coords = (int **) malloc((size_t) 4 * sizeof(int *));
        randomSites.coords[0] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.coords[1] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.coords[2] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.coords[3] = (int *) malloc((size_t) 2 * sizeof(int));
        randomSites.numSites = 4;
        /* find radius of inner circle */
        if(DIMX > DIMY) {
            radius = DIMY / 4;
        }
        else {
            radius = DIMX / 4;
        }
        firstSectionXCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        firstSectionYCoords = (int *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(int));
        
        /* get sites from first section */
        for(i = 0; i < DIMY; i++) {
            for(j = 0; j < DIMX; j++) {
                if((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) <= radius * radius) {
                    firstSectionXCoords[firstCounter] = i;
                    firstSectionYCoords[firstCounter] = j;
                    firstCounter++;
                }
            }
        }
        /* pick random site from first section */
        randX1 = 0;
        randY1 = 0;
        while(die[randX1][randY1] != 1) {
            randIndex = rand() % firstCounter;
            randX1 = firstSectionXCoords[randIndex];
            randY1 = firstSectionYCoords[randIndex];
        }
        die[randX1][randY1] = 8;
        randomSites.coords[0][0] = randY1;
        randomSites.coords[0][1] = DIMY - 1 - randX1;
        /*/////////////////////////////////////////////////////////*/
        /* POLAR COORDINATE TIME! */
        /*/////////////////////////////////////////////////////////*/
        /* initialize array for storing relative to center coords */
        relativeToCenterCoords = malloc((size_t) (DIMX) * (size_t) DIMY * sizeof(double *));
        for(i = 0; i < DIMX * DIMY; i++) {
            relativeToCenterCoords[i] = malloc(2 * sizeof(double));
        }
        
        /* get relative to center coords */
        counter = 0;
        for(i = 0; i < DIMX; i++) {

            for(j = 0; j < DIMY; j++) {
                if(die[i][j] == 1) {
                    relativeToCenterCoords[counter][0] = i - centerX;
                    relativeToCenterCoords[counter][1] = j - centerY;
                    counter++;
                }
            }
        }
        /* print out coords relative to center 
        for(i = 0; i < counter; i++) {
            printf("(%f, %f)\n", relativeToCenterCoords[i][0], relativeToCenterCoords[i][1]);
        }  */
        /* convert to polar coords */ 
        for(i = 0; i < counter; i++) {
            relativeToCenterCoords[i][0] = sqrt(relativeToCenterCoords[i][0] * relativeToCenterCoords[i][0] + relativeToCenterCoords[i][1] * relativeToCenterCoords[i][1]);
            relativeToCenterCoords[i][1] = atan(relativeToCenterCoords[i][1] / relativeToCenterCoords[i][0]) * 180 / M_PI;
        }
        /* sort by r */
        /* qsort(relativeToCenterCoords, (size_t) counter, sizeof(double *), compare); */
        /* print out polar coords */
        for(i = 0; i < counter; i++) {
            printf("r: %f, theta: %f\n", relativeToCenterCoords[i][0], relativeToCenterCoords[i][1]);
        }
        printf("---------------------------------\n"); 
        
        /* get sites from second section */
        secondSectionXCoordsRelativeToCenter = (double *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(double));
        secondSectionYCoordsRelativeToCenter = (double *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(double));
        thirdSectionXCoordsRelativeToCenter = (double *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(double));
        thirdSectionYCoordsRelativeToCenter = (double *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(double));
        fourthSectionXCoordsRelativeToCenter = (double *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(double));
        fourthSectionYCoordsRelativeToCenter = (double *) malloc((size_t)(DIMX) * (size_t)(DIMY) * sizeof(double));

        secondCounter = 0;
        thirdCounter = 0;
        fourthCounter = 0;/*
        for(i = 0; i < counter; i++) {
            if(relativeToCenterCoords[i][0] > radius && relativeToCenterCoords[i][1] >= -180 && relativeToCenterCoords[i][1] <= -60) {
                secondSectionXCoordsRelativeToCenter[secondCounter] = relativeToCenterCoords[i][0] * cos(relativeToCenterCoords[i][1] * M_PI / 180 / 4);
                secondSectionYCoordsRelativeToCenter[secondCounter] = relativeToCenterCoords[i][0] * sin(relativeToCenterCoords[i][1] * M_PI / 180 / 4);
                secondCounter++;
            } 
            else if(relativeToCenterCoords[i][0] > radius && relativeToCenterCoords[i][1] > -60 && relativeToCenterCoords[i][1] <= 60) {
                thirdSectionXCoords[secondCounter] = relativeToCenterCoords[i][0] * cos(relativeToCenterCoords[i][1] * M_PI / 180 / 4);
                thirdSectionYCoords[secondCounter] = relativeToCenterCoords[i][1] * sin(relativeToCenterCoords[i][1] * M_PI / 180 / 4);
                thirdCounter++;
            }
            else if(relativeToCenterCoords[i][0] > radius && relativeToCenterCoords[i][1] > 60 && relativeToCenterCoords[i][1] <= 180) {
                fourthSectionXCoords[secondCounter] = relativeToCenterCoords[i][0] * cos(relativeToCenterCoords[i][1] * M_PI / 180 / 4);
                fourthSectionYCoords[secondCounter] = relativeToCenterCoords[i][1] * sin(relativeToCenterCoords[i][1] * M_PI / 180 / 4);
                fourthCounter++;
            }  
        } */
        /* print out second section coords 
        for(i = 0; i < secondCounter; i++) {
            printf("x: %f, y: %f\n", secondSectionXCoordsRelativeToCenter[i], secondSectionYCoordsRelativeToCenter[i]);
        } */


        /* free arrs */
        free(firstSectionXCoords); 
        free(firstSectionYCoords);
        free(secondSectionXCoords);
        free(secondSectionYCoords);
        free(thirdSectionXCoords);
        free(thirdSectionYCoords);
        free(fourthSectionXCoords);
        free(fourthSectionYCoords);
        for(i = 0; i < DIMX * DIMY; i++) {
            free(relativeToCenterCoords[i]);
        }
        free(relativeToCenterCoords);
        return randomSites;
    }
    else if(numSites >= 5) {
        /* initialize struct for storing sites */
        SITES randomSites;
        randomSites.coords = (int **) malloc((size_t) numSites * sizeof(int *));
        for(i = 0; i < numSites; i++) {
            randomSites.coords[i] = (int *) malloc((size_t)2 * sizeof(int));
        }
        randomSites.numSites = numSites; 
        /* find radius of center quadrant */
        radius = 0;
        if (DIMY < DIMX) {
            radius = DIMY / 4;
        } else {
            radius = DIMX / 4;
        }
        firstSectionXCoords = (int *) malloc((size_t)VALID_SITES * sizeof(int));
        firstSectionYCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        secondSectionXCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        secondSectionYCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        thirdSectionXCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        thirdSectionYCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        fourthSectionXCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        fourthSectionYCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        fifthSectionXCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        fifthSectionYCoords = (int *) malloc((size_t) VALID_SITES * sizeof(int));
        /* iterate through die array and find elements within radius of center, i.e. first section */
        firstCounter = 0;
        secondCounter = 0; 
        thirdCounter = 0;
        fourthCounter = 0;
        fifthCounter = 0;
        for(i = 0; i < DIMY; i++) {
            for(j = 0; j < DIMX; j++) {
                /* check if element is within radius of center */ 
                if(die[i][j] == 1 && (i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) <= radius * radius) {
                      firstSectionXCoords[firstCounter] = i;
                      firstSectionYCoords[firstCounter] = j;
                      firstCounter += 1;
                }
                /* find elements in second section*/
                else if(die[i][j] == 1 && i < centerX && j < centerY && (i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) > radius * radius) { 
                        secondSectionXCoords[secondCounter] = i;
                        secondSectionYCoords[secondCounter] = j;
                        secondCounter += 1;
                }
                /* find elements in third section */
                else if(die[i][j] == 1 && i < centerX && j > centerY && (i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) > radius * radius) {
                        thirdSectionXCoords[thirdCounter] = i;
                        thirdSectionYCoords[thirdCounter] = j;
                        thirdCounter += 1;
                }
                /* find elements in fourth section */
                else if(die[i][j] == 1 && i > centerX && j < centerY && (i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) > radius * radius) {
                        fourthSectionXCoords[fourthCounter] = i;
                        fourthSectionYCoords[fourthCounter] = j;
                        fourthCounter += 1;
                }
                /* find elements in fifth section */
                else if(die[i][j] == 1 && i > centerX && j > centerY && (i - centerX) * (i - centerX) + (j - centerY) * (j - centerY) > radius * radius) {
                        fifthSectionXCoords[fifthCounter] = i;
                        fifthSectionYCoords[fifthCounter + 1] = j;
                        fifthCounter += 1;
                }
            }
        }
        /* randomly select 1 site from each section */
        i = 0; 

        while(i < numSites) {
            /* randomly select 1 site from first section */
            if(i < numSites) {
                randX1 = 0; 
                randY1 = 0;
                while(die[randX1][randY1] != 1) {
                    current = time(NULL);
                    if(current - start > 5) {
                        printf("Error: Program timed out. Please try again.\n");
                        exit(1);
                    }
                    randX1 = firstSectionXCoords[rand() % firstCounter];
                    randY1 = firstSectionYCoords[rand() % firstCounter];
                }
                die[randX1][randY1] = 8; 
                /* add to randomSites */
                randomSites.coords[i][0] = randY1;
                randomSites.coords[i][1] = DIMY - 1 - randX1;
                i += 1;
            }
            if(i < numSites) {
                /* randomly select 1 site from second section */
                randX2 = 0;
                randY2 = 0;
                while(die[randX2][randY2] != 1) {
                    current = time(NULL);
                    if(current - start > 5) {
                        printf("Error: Program timed out. Please try again.\n");
                        exit(1);
                    }
                    randX2 = secondSectionXCoords[rand() % secondCounter];
                    randY2 = secondSectionYCoords[rand() % secondCounter];
                }
                die[randX2][randY2] = 8; 
                /* add to randomSites */
                randomSites.coords[i][0] = randY2;
                randomSites.coords[i][1] = DIMY - 1 - randX2;
                i += 1;
            }
            if(i < numSites) {
                /* randomly select 1 site from third section */
                randX3 = 0;
                randY3 = 0;
                while(die[randX3][randY3] != 1) {
                    current = time(NULL);
                    if(current - start > 5) {
                        printf("Error: Program timed out. Please try again.\n");
                        exit(1);
                    }
                    randX3 = thirdSectionXCoords[rand() % thirdCounter];
                    randY3 = thirdSectionYCoords[rand() % thirdCounter];
                }
                die[randX3][randY3] = 8;
                /* add to randomSites */
                randomSites.coords[i][0] = randY3;
                randomSites.coords[i][1] = DIMY - 1 - randX3;
                i += 1;
            }
            if(i < numSites) {
                /* randomly select 1 site from fourth section */
                randX4 = 0; 
                randY4 = 0;
                while(die[randX4][randY4] != 1) {
                    current = time(NULL);
                    if(current - start > 5) {
                        printf("Error: Program timed out. Please try again.\n");
                        exit(1);
                    }
                    randX4 = fourthSectionXCoords[rand() % fourthCounter];
                    randY4 = fourthSectionYCoords[rand() % fourthCounter];
                }
                die[randX4][randY4] = 8;
                /* add to randomSites */
                randomSites.coords[i][0] = randY4;
                randomSites.coords[i][1] = DIMY - 1 - randX4;
                i += 1;
            }
            if(i < numSites) {
                /* randomly select 1 site from fifth section */
                randX5 = 0;
                randY5 = 0;
                while(die[randX5][randY5] != 1) {
                    current = time(NULL);
                    if(current - start > 5) {
                        printf("Error: Program timed out. Please try again.\n");
                        exit(1);
                    }
                    randX5 = fifthSectionXCoords[rand() % fifthCounter];
                    randY5 = fifthSectionYCoords[rand() % fifthCounter];
                }
                die[randX5][randY5] = 8;
                /* add to randomSites */
                randomSites.coords[i][0] = randY5;
                randomSites.coords[i][1] = DIMY - 1 - randX5;
                i += 1;
            }
        }
        printf("Freeing memory...\n");
        /* free memory */
        free(firstSectionXCoords);
        free(firstSectionYCoords);
        free(secondSectionXCoords);
        free(secondSectionYCoords);
        free(thirdSectionXCoords);
        free(thirdSectionYCoords);
        free(fourthSectionXCoords);
        free(fourthSectionYCoords);
        free(fifthSectionXCoords);
        free(fifthSectionYCoords);

        printDie(die); 
        printSites(randomSites);
        return randomSites;
    } 
    return randomSites; 
}

/*/////////////////////////////////////////////////////////////////
////////////////////// FIXED SITE SELECTION ///////////////////////
/////////////////////////////////////////////////////////////////*/
SITES fixedSiteSelection(die, numSites) int ** die; int numSites; {
    if(VALID_SITES < numSites) {
        printf("ERROR: number of sites requested exceeds number of valid sites\n");
        exit(1);
    }
    centerX = DIMX / 2; 
    centerY = DIMY / 2;
    
    if(numSites == 0) {
        printf("ERROR: number of sites requested is 0\n");
        exit(1); 
    }
    else if(numSites == 1) {
        fixedSites.coords = (int **) malloc(sizeof(int *) * 1);
        fixedSites.coords[0] = (int *) malloc(sizeof(int) * 2);
        die[centerX][centerY] = 8;
        fixedSites.coords[0][0] = centerY;
        fixedSites.coords[0][1] = DIMY - centerX - 1;
        fixedSites.numSites = 1;
        printDie(die); 
        printSites(fixedSites);
        return fixedSites;
        exit(1);
    }
    else if(numSites == 2) {
        fixedSites.coords = (int **) malloc(sizeof(int *) * 2);
        fixedSites.coords[0] = (int *) malloc(sizeof(int) * 2);
        fixedSites.coords[1] = (int *) malloc(sizeof(int) * 2);
        die[centerX][centerY] = 8;
        fixedSites.coords[0][0] = centerY;
        fixedSites.coords[0][1] = DIMX - centerX - 2;
        die[0][centerY] = 8;
        fixedSites.coords[1][0] = centerY;
        fixedSites.coords[1][1] = DIMX - 2;
        fixedSites.numSites = 2;
        printDie(die);
        printSites(fixedSites);
        return fixedSites;
    }
    else if(numSites == 3) {
        fixedSites.coords = (int **) malloc(sizeof(int *) * 3);
        fixedSites.coords[0] = (int *) malloc(sizeof(int) * 2);
        fixedSites.coords[1] = (int *) malloc(sizeof(int) * 2);
        fixedSites.coords[2] = (int *) malloc(sizeof(int) * 2);
        die[centerX][centerY] = 8;
        fixedSites.coords[0][0] = centerY;
        fixedSites.coords[0][1] = DIMX - centerX - 2;
        die[0][centerY] = 8;
        fixedSites.coords[1][0] = centerY;
        fixedSites.coords[1][1] = DIMX - 2;
        die[centerX][DIMY] = 8;
        fixedSites.coords[2][0] = DIMY;
        fixedSites.coords[2][1] = DIMX - centerX - 2;
        fixedSites.numSites = 3;
        printDie(die);
        printSites(fixedSites);
        return fixedSites;
    }
    else if(numSites == 4) {
        fixedSites.coords = (int **) malloc(sizeof(int *) * 4);
        fixedSites.coords[0] = (int *) malloc(sizeof(int) * 2);
        fixedSites.coords[1] = (int *) malloc(sizeof(int) * 2);
        fixedSites.coords[2] = (int *) malloc(sizeof(int) * 2);
        fixedSites.coords[3] = (int *) malloc(sizeof(int) * 2);
        die[centerX][centerY] = 8;
        fixedSites.coords[0][0] = centerY;
        fixedSites.coords[0][1] = DIMX - centerX - 2;
        die[0][centerY] = 8;
        fixedSites.coords[1][0] = centerY;
        fixedSites.coords[1][1] = DIMX - 2;
        die[centerX][DIMY] = 8;
        fixedSites.coords[2][0] = DIMY;
        fixedSites.coords[2][1] = DIMX - centerX - 2;
        die[DIMY - 1][centerY] = 8;
        fixedSites.coords[3][0] = centerY;
        fixedSites.coords[3][1] = 0;
        fixedSites.numSites = 4;
        printDie(die);
        printSites(fixedSites);
        return fixedSites;
    }
    else if(numSites == 5) {
        fixedSites.coords = (int **) malloc(sizeof(int *) * 5);
        for(i = 0; i < 5; i++) {
            fixedSites.coords[i] = (int *) malloc(sizeof(int) * 2);
        }
        die[centerX][centerY] = 8;
        fixedSites.coords[0][0] = centerY;
        fixedSites.coords[0][1] = DIMX - centerX - 2;
        die[0][centerY] = 8;
        fixedSites.coords[1][0] = centerY;
        fixedSites.coords[1][1] = DIMX - 2;
        die[centerX][DIMY] = 8;
        fixedSites.coords[2][0] = DIMY;
        fixedSites.coords[2][1] = DIMX - centerX - 2;
        die[DIMY - 1][centerY] = 8;
        fixedSites.coords[3][0] = centerY;
        fixedSites.coords[3][1] = 0;
        die[centerX][0] = 8;
        fixedSites.coords[4][0] = 0;
        fixedSites.coords[4][1] = DIMX - centerX - 2;
        fixedSites.numSites = 5;
        printDie(die);
        printSites(fixedSites);
        return fixedSites;
    }
    else if(numSites > 5) {
        fixedSites.coords = (int **) malloc(sizeof(int *) * (size_t) numSites);
        for(i = 0; i < numSites; i++) {
            fixedSites.coords[i] = (int *) malloc(sizeof(int) * 2);
        }
        die[centerX][centerY] = 8;
        fixedSites.coords[0][0] = centerY;
        fixedSites.coords[0][1] = DIMX - centerX - 2;
        die[0][centerY] = 8;
        fixedSites.coords[1][0] = centerY;
        fixedSites.coords[1][1] = DIMX - 2;
        die[centerX][DIMY] = 8;
        fixedSites.coords[2][0] = DIMY;
        fixedSites.coords[2][1] = DIMX - centerX - 2;
        die[DIMY - 1][centerY] = 8;
        fixedSites.coords[3][0] = centerY;
        fixedSites.coords[3][1] = 0;
        die[centerX][0] = 8;
        fixedSites.coords[4][0] = 0;
        fixedSites.coords[4][1] = DIMX - centerX - 2;
        fixedSites.numSites = numSites;
        /* calculate radius */
        if(DIMX > DIMY) {
            radius = DIMY / 2;
        }
        else {
            radius = DIMX / 2;
        }
        i = 5; 
        while(i < numSites) {
            radius = radius / 4 * 3; 
            if(radius < 1) {
                radius = 1;
            }
            if(i < numSites) {
                randX1 = centerX - radius; 
                randY1 = centerY + radius;
                die[randX1][randY1] = 8;
                fixedSites.coords[i][0] = randY1;
                fixedSites.coords[i][1] = DIMX - randX1 - 2;
                i++;
            }

            if(i < numSites) {
                randX2 = centerX + radius;
                randY2 = centerY + radius;
                die[randX2][randY2] = 8;
                fixedSites.coords[i][0] = randY2;
                fixedSites.coords[i][1] = DIMX - randX2 - 2;
                i++;
            }

            if(i < numSites) {
                randX3 = centerX + radius;
                randY3 = centerY - radius; 
                die[randX3][randY3] = 8;
                fixedSites.coords[i][0] = randY3;
                fixedSites.coords[i][1] = DIMX - randX3 - 2;
                i++;
            }
            
            if(i < numSites) {
                randX4 = centerX - radius;
                randY4 = centerY - radius; 
                die[randX4][randY4] = 8;
                fixedSites.coords[i][0] = randY4;
                fixedSites.coords[i][1] = DIMX - randX4 - 2;
                i++;
            }
            /*//////////////////////////////////////////////*/
            if(i < numSites) {
                randX1 = centerX;
                randY1 = centerY + radius;
                die[randX1][randY1] = 8;
                fixedSites.coords[i][0] = randY1;
                fixedSites.coords[i][1] = DIMX - randX1 - 2;
                i++;
            }
            if(i < numSites) {
                randX2 = centerX + radius;
                randY2 = centerY;
                die[randX2][randY2] = 8;
                fixedSites.coords[i][0] = randY2;
                fixedSites.coords[i][1] = DIMX - randX2 - 2;
                i++;
            }

            if(i < numSites) {
                randX3 = centerX;
                randY3 = centerY - radius;
                die[randX3][randY3] = 8;
                fixedSites.coords[i][0] = randY3;
                fixedSites.coords[i][1] = DIMX - randX3 - 2;
                i++;
            }

            if(i < numSites) {
                randX4 = centerX - radius;
                randY4 = centerY;
                die[randX4][randY4] = 8;
                fixedSites.coords[i][0] = randY4;
                fixedSites.coords[i][1] = DIMX - randX4 - 2;
                i++;
            }
        }
        printDie(die);
        printSites(fixedSites);
        return fixedSites;
    }
    return fixedSites; 
}

/* Output sites to file */
void outputSites(sites) SITES sites; {
    /* Determine output file name*/
    
    printf("Enter output file name (please include .waf extension): ");
    scanf("%s", outFile);

    if(sites.numSites == 0) {
        printf("No sites to output.\n");
        return;
    }

    source = fopen(file, "r");
    if( source == NULL )
    {
      printf("Could not open source file %s\n", file);
      exit(EXIT_FAILURE);
    }

    output = fopen(outFile, "w");
    if( output == NULL )
    {
      fclose(source);
      printf("Could not open output file %s\n", outFile);
      exit(EXIT_FAILURE);
    }
    /* delete lines in output file not in sites */
    ch = (char) fgetc(source);
    counter = 0; 
    while(fgets(line, sizeof(line), source) != NULL) {
        if(counter > 25 && counter < lineCounter - 2) {
            toRead = line + 31; 
            sscanf(toRead, "%d,%d", &x, &y);
            for(i = 0; i < sites.numSites; i++) {
                if(sites.coords[i][0] == x && sites.coords[i][1] == y) {
                    fputs(line, output);
                }
            }
        }
        else {
            fputs(line, output);
        }
        counter++;
    }
    

    printf("File output into %s\n", outFile);
    fclose(source);
    fclose(output);
}

/*////////////////////////////////////////////////////////////////////////////////*/
/*/////////////////////////////// MAIN FUNCTION //////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////////*/
int main(argc, argv) int argc; char* argv[]; {   
    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            printf("Usage: ./siteSelection -f <file> -n <# of sites> --fixed\n");
            printf("Example: ./siteSelection test.waf 5\n");
            exit(0);
        }
        else {
            printf("Wrong number of arguments. For more information, use the --help flag.\n");
            exit(0);
        }
    }  
    if (argc != 6) {
        printf("Wrong number of arguments. For more information, use the --help flag.\n");
        exit(0);
    } 
    srand((unsigned) time(NULL)); /* NEED this to truly randomize choices, else it just returns 
                                     the same result every time because rand() is pseudo-random */
    sites = (int) strtol(argv[4], NULL, 10);  /* change this to test different # of sites */
    die = read(argv[2]); /* change this to test .waf files */ 
    last = argv[argc - 1];
    if(strcmp(last, "--random") == 0) {
        if(VALID_SITES - 11 < sites) {
            printf("ERROR: number of sites cannot be this big; die is too small!\n");
            exit(1);
        }
        printf("Starting random site selection...\n");
        randomSites = randomSiteSelection(die, sites);  /* change this to test different # of sections */ 
        outputSites(randomSites, "test1.waf");
        /* free sites */ 
        for(i = 0; i < DIMY; i++) {
            free(die[i]); 
        }
        free(die); 
        /* free struct */ 
        for(i = 0; i < randomSites.numSites; i++) {
            free(randomSites.coords[i]);
        }
        free(randomSites.coords);
        return 0;
    }
    else if(strcmp(last, "--fixed") == 0) {
        if(VALID_SITES < sites) {
            printf("ERROR: number of sites cannot be this big; die is too small!\n");
            exit(1);
        }
        printf("Starting fixed site selection...\n");
        fixedSites = fixedSiteSelection(die, sites);  /* change this to test different # of sections */ 
        outputSites(fixedSites, "fixedSites.waf");
        /* free sites */ 
        for(i = 0; i < DIMY; i++) {
            free(die[i]); 
        }
        free(die);
        for(i = 0; i < fixedSites.numSites; i++) {
            free(fixedSites.coords[i]);
        }
        free(fixedSites.coords);
        return 0;
    } 
    else {
        printf("Wrong number of arguments. For more information, use the --help flag.\n");
        exit(0);
    }



    return 0;
}

