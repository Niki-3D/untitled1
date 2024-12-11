#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MovieNode {
    int movieID;
    struct MovieNode* next;
} MovieNode;

typedef struct User {
    int userID;
    int parentID;
    MovieNode* movies;
    struct User* next;
} User;

User* userList = NULL;

User* findUser(int userID) {
    User* current = userList;
    while (current) {
        if (current->userID == userID)
            return current;
        current = current->next;
    }
    return NULL;
}

void freeMovies(MovieNode* movie) {
    while (movie) {
        MovieNode* temp = movie;
        movie = movie->next;
        free(temp);
    }
}

void freeUser(User* user) {
    freeMovies(user->movies);
    free(user);
}

void addUser(int parentUserID, int userID) {
    if (findUser(userID) || (!findUser(parentUserID) && userID != 0)) {
        printf("ERROR\n");
        return;
    }

    User* newUser = (User*)malloc(sizeof(User));
    newUser->userID = userID;
    newUser->parentID = parentUserID;
    newUser->movies = NULL;
    newUser->next = userList;
    userList = newUser;
    if (userID != 0) {
        printf("OK\n");
    }
}

void delUser(int userID) {
    if (userID == 0 || !findUser(userID)) {
        printf("ERROR\n");
        return;
    }

    User* current = userList;
    User* prev = NULL;
    while (current) {
        if (current->userID == userID) {
            if (prev)
                prev->next = current->next;
            else
                userList = current->next;

            User* iter = userList;
            while (iter) {
                if (iter->parentID == userID)
                    iter->parentID = current->parentID;
                iter = iter->next;
            }

            freeUser(current);
            printf("OK\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("ERROR\n");
}

void addMovie(int userID, int movieID) {
    User* user = findUser(userID);
    if (!user) {
        printf("ERROR\n");
        return;
    }

    MovieNode* current = user->movies;
    while (current) {
        if (current->movieID == movieID) {
            printf("ERROR\n");
            return;
        }
        current = current->next;
    }

    MovieNode* newMovie = (MovieNode*)malloc(sizeof(MovieNode));
    newMovie->movieID = movieID;
    newMovie->next = user->movies;
    user->movies = newMovie;
    printf("OK\n");
}

void delMovie(int userID, int movieID) {
    User* user = findUser(userID);
    if (!user) {
        printf("ERROR\n");
        return;
    }

    MovieNode* current = user->movies;
    MovieNode* prev = NULL;
    while (current) {
        if (current->movieID == movieID) {
            if (prev)
                prev->next = current->next;
            else
                user->movies = current->next;

            free(current);
            printf("OK\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("ERROR\n");
}

void printUserMovies(int userID) {
    User* user = findUser(userID);
    if (!user) {
        printf("ERROR\n");
        return;
    }

    int* movies = (int*)malloc(1000 * sizeof(int));
    int count = 0;

    MovieNode* current = user->movies;
    while (current) {
        movies[count++] = current->movieID;
        current = current->next;
    }

    User* parent = findUser(user->parentID);
    if (parent) {
        current = parent->movies;
        while (current) {
            movies[count++] = current->movieID;
            current = current->next;
        }
    }

    if (count == 0) {
        printf("\n");
        free(movies);
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (movies[i] > movies[j]) {
                int temp = movies[i];
                movies[i] = movies[j];
                movies[j] = temp;
            }
        }
    }

    printf("%d", movies[0]);
    for (int i = 1; i < count; i++) {
        if (movies[i] != movies[i - 1])
            printf(" %d", movies[i]);
    }
    printf("\n");

    free(movies);
}

void freeAllUsers() {
    while (userList) {
        User* temp = userList;
        userList = userList->next;
        freeUser(temp);
    }
}

int main() {
    addUser(0, 0); // Ensure the root user exists without printing anything

    char command[20];
    int arg1, arg2;

    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "addUser") == 0) {
            scanf("%d %d", &arg1, &arg2);
            addUser(arg1, arg2);
        } else if (strcmp(command, "delUser") == 0) {
            scanf("%d", &arg1);
            delUser(arg1);
        } else if (strcmp(command, "addMovie") == 0) {
            scanf("%d %d", &arg1, &arg2);
            addMovie(arg1, arg2);
        } else if (strcmp(command, "delMovie") == 0) {
            scanf("%d %d", &arg1, &arg2);
            delMovie(arg1, arg2);
        } else if (strcmp(command, "printUserMovies") == 0) {
            scanf("%d", &arg1);
            printUserMovies(arg1);
        } else {
            printf("ERROR\n");
        }
    }

    fflush(stdout); // Ensure all output is flushed before exiting
    freeAllUsers();
    return 0;
}
