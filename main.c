/*
NAME: Lana Mahmoud Ali Musaffer
ID: 1210455
INSTRUCTOR: Dr. Bashar Tahayna
SECTION: 4
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_PROCESSES 10
#define NUM_OF_RESOURCES 5

// A METHOD THAT COUNTS THE NUMBERS OF ROWS & COLUMNS IN THE REQUEST & ALLOCATION FILES
int countDimensions(FILE *file, int *numRows, int *numCols) {
    char line[200];
    *numRows = 0;
    *numCols = 0;

    int skipFirstRow = 1;  // INITIALIZE A FLAG TO SKIP THE FIRST ROW (STRING ROW)

    while (fgets(line, sizeof(line), file)) {
        // FOR SKIPPING THE FIRST ROW
        if (skipFirstRow) {
            skipFirstRow = 0;
            continue;
        }
        (*numRows)++;

        char *token = strtok(line, ",");
        int colCount = 0;

        int skipFirstColumn = 1; // INITIALIZE A FLAG TO SKIP THE FIRST COLUMN (STRING COLUMNS)

        while (token != NULL) {
            // FOR SKIPPING THE FIRST COLUMNS
            if (skipFirstColumn) {
                skipFirstColumn = 0;
                token = strtok(NULL, ",");
                continue;
            }

            colCount++;
            token = strtok(NULL, ",");
        }

        // COUNT THE NUMBER OF COLUMNS FOR CHECKING THE CONSISTENCY
        if (*numCols == 0) {
            *numCols = colCount;
        } else if (*numCols != colCount) {
            printf("INCONSISTENT NUMBER OF COLUMNS IN THE FILE.\n");
            return 0;
        }
    }
    return 1;
}

// A METHOD THAT COUNTS THE NUMBERS OF ROWS & COLUMNS IN THE AVAILABLE FILE
int countDimensionsWithFirstColumn(FILE *file, int *numRows, int *numCols) {
    char line[200];
    *numRows = 0;
    *numCols = 0;

    int skipFirstRow = 1;  // INITIALIZE A FLAG TO SKIP THE FIRST ROW (STRING ROW)

    while (fgets(line, sizeof(line), file)) {
        // FOR SKIPPING THE FIRST ROW
        if (skipFirstRow) {
            skipFirstRow = 0;
            continue;
        }

        (*numRows)++;

        char *token = strtok(line, ",");
        int colCount = 0;

        while (token != NULL) {
            colCount++;
            token = strtok(NULL, ",");
        }

        (*numCols) = colCount; // SET ALL COLUMNS TO THE COUNT

        break; // IGNORE SUBSEQUENT ROWS
    }

    return 1;
}

// A METHOD FOR PRINTING THE DATA OF EACH FILE
void printFileContent(FILE *file) {
    char line[200];

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");

        while (token != NULL) {
            printf("%-12s", token); // TO ADD SPACES
            token = strtok(NULL, ",");
        }
        printf("\n");
    }
}

// A METHOD FOR READING FROM THE FILE
int readMatrixFromFile(FILE *file, int matrix[NUM_OF_PROCESSES][NUM_OF_RESOURCES], int numRows, int numCols) {
    char line[200];
    int row = 0;

    // FOR SKIPPING THE FIRST ROW
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("ERROR IN READING FROM FILES.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), file) && row < numRows) {
        int col = 0;
        char *token = strtok(line, ",");

        while (token != NULL && col < numCols) {
            matrix[row][col] = atoi(token); // CONVERT TO INTEGER TO STORE IT IN THE MATRIX
            col++;
            token = strtok(NULL, ",");
        }

        // TO HANDLE THE INCONSISTENCY CASES
        if (col != numCols) {
            printf("ERROR; INCONSISTENT NUMBER OF COLUMNS IN THE FILE.\n");
            return 0;
        }

        row++;
    }

    // TO HANDLE THE INCONSISTENCY CASES
    if (row != numRows) {
        printf("ERROR; INCONSISTENT NUMBER OF ROWS IN THE FILE.\n");
        return 0;
    }
    return 1;
}

void detectionAlgorithm(int available[NUM_OF_RESOURCES], int allocation[NUM_OF_PROCESSES][NUM_OF_RESOURCES], int request[NUM_OF_PROCESSES][NUM_OF_RESOURCES]) {
    int safeSequence[NUM_OF_PROCESSES];  //FOR KEEP TRAKING OF THE SAFE SEQUENCE
    int index = 0;  //INTIALIZE AN INDEX FOR THE SAFE STATE CASE

    //ASSIGN THE AVAILABLE VECTOR TO THE WORK VECTOR
    int work[NUM_OF_RESOURCES];
    for (int i = 0; i < NUM_OF_RESOURCES; i++) {
        work[i] = available[i];
    }

    //TO KEEP TRAK OF THE FINISHED AND UNFINISHED PROCESSES
    int finish[NUM_OF_PROCESSES];
    for (int i = 0; i < NUM_OF_PROCESSES; i++) {
        finish[i] = 0;
    }

    //IF ALL PROCESSES FINISHED --> SAFE STATE. OTHERWISE,  DEADLOCK IS DETECTED
    int deadlock = 0;
    for (int i = 0; i < NUM_OF_PROCESSES; i++) {
        if (!finish[i]) {
            int canExecute = 1;
            for (int j = 0; j < NUM_OF_RESOURCES; j++) {
                if (request[i][j] > work[j]) {
                    canExecute = 0;
                    break;
                }
            }
            if (canExecute) {
                //RELEASE ALLOCATED RESOURCES
                for (int j = 0; j < NUM_OF_RESOURCES; j++) {
                    work[j] += allocation[i][j];
                }
                finish[i] = 1; //PROCESS IS FINISHED
                safeSequence[index++] = i;  //ADD IT TO THE SAFE SEQUENCE
                i = -1; //GO TO THE FIRST STEP!
            }
        }
    }

    //CHECK IF ALL PROCESSES ARE FINISHED
    for (int i = 0; i < NUM_OF_PROCESSES; i++) {
        if (!finish[i]) {
            deadlock = 1;
            printf("P%d IS DEADLOCKED.\n", i + 1);
        }
    }

    // DEADLOCK IS DETECTED
    if (deadlock) {
        printf("\nDEADLOCK IS DETECTED.\n");
    } else {
        // SAFE STATE
        printf("NO DEADLOCK, THE SAFE SEQUENCE IS: ");
        for (int i = 0; i < NUM_OF_PROCESSES; i++) {
            printf("P%d", safeSequence[i] + 1);

            if (i < NUM_OF_PROCESSES - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
}


int main() {
    int allocation[NUM_OF_PROCESSES][NUM_OF_RESOURCES];
    int request[NUM_OF_PROCESSES][NUM_OF_RESOURCES];
    int available[NUM_OF_RESOURCES];

    FILE *allocFile = fopen("Allocation.csv", "r");
    FILE *avaiFile = fopen("Available.csv", "r");
    FILE *reqFile = fopen("Request.csv", "r");

    if (allocFile == NULL || avaiFile == NULL || reqFile == NULL) {
        printf("ERROR IN OPENING THE FILE.\n");
        return 1;
    }

    int numRows1, numCols1, numRows2, numCols2, numRows3, numCols3;

    // CHECK THE DIMENSIONS FOR THE ALLOCATION FILE
    if (!countDimensions(allocFile, &numRows1, &numCols1)) {
        fclose(allocFile);
        fclose(avaiFile);
        fclose(reqFile);
        return 1;
    }

    // CHECK THE DIMENSIONS FOR THE AVAILABLE FILE
    if (!countDimensionsWithFirstColumn(avaiFile, &numRows3, &numCols3) || numCols3 != 5 ||  numRows3 != 1) {
        fclose(allocFile);
        fclose(avaiFile);
        fclose(reqFile);
        return 1;
    }

    // CHECK THE DIMENSIONS FOR THE REQUEST FILE
    if (!countDimensions(reqFile, &numRows2, &numCols2)) {
        fclose(allocFile);
        fclose(avaiFile);
        fclose(reqFile);
        return 1;
    }

    // IF THE FILES ARE CONSISTENT THEN START THE DETECTION ALGORITHM
    if (numRows1 == numRows2 && numCols1 == numCols2 && numCols1 == numCols3 && numRows3 == 1) {
        printf("CONSISTENT IN ROWS ACROSS ALLOCATION & REQUEST FILES, AND CONSISTENT IN COLUMNS ACROSS ALL FILES.\n\n");
        printf("* Allocation.csv FILE HAVE %d ROWS & %d COLUMNS\n", numRows1, numCols1);
        printf("* Request.csv FILE HAVE %d ROWS & %d COLUMNS\n", numRows2, numCols2);
        printf("* Available.csv FILE HAVE %d ROWS & %d COLUMNS\n\n", numRows3, numCols3);
        printf("*************************************************************************\n\n");

        // TO PRINT THE ALLOCATION DATA
        printf("\nAllocation.csv DATA:\n");
        rewind(allocFile); // TO REST THE FILE POINTER TO THE BEGINNING
        printFileContent(allocFile);

        // TO PRINT THE REQUEST DATA
        printf("\nRequest.csv DATA:\n");
        rewind(reqFile); // TO REST THE FILE POINTER TO THE BEGINNING
        printFileContent(reqFile);

        // TO PRINT THE AVAILABLE DATA
        printf("\nAvailable.csv DATA:\n");
        rewind(avaiFile); // TO REST THE FILE POINTER TO THE BEGINNING
        printFileContent(avaiFile);

        //RESET FILE POINTERS BEFORE READING MATRICES
        fseek(allocFile, 0, SEEK_SET);
        fseek(reqFile, 0, SEEK_SET);
        fseek(avaiFile, 0, SEEK_SET);

        printf("***************************************************************************\n\n");

        if (!readMatrixFromFile(allocFile, allocation, numRows1, numCols1) || !readMatrixFromFile(reqFile, request, numRows2, numCols2) ||
                !readMatrixFromFile(avaiFile, available, numRows3, numCols3)) {
            printf("ERROR IN READING MATRICES FROM THE FILES.\n");
                fclose(allocFile);
                fclose(avaiFile);
                fclose(reqFile);
            return 1;
        }

        detectionAlgorithm(available, allocation, request); //CALL THE DEADLOCK ALGORITHM FUNTION
    } else {
        printf("INCONSISTENT DIMENSIONS ACROSS FILES.\n");
    }

    //CLOSE ALL FILES
    fclose(allocFile);
    fclose(avaiFile);
    fclose(reqFile);
    return 0;
}