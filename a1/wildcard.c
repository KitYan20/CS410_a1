#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
bool wildcard(const char* str, char* pattern){
    
    char ch = *pattern;
    char pattern_copy[strlen(pattern)+1];
    strcpy(pattern_copy,pattern);
    //Make a copy of the pattern string
    char str_copy[strlen(str)+1];
    strcpy(str_copy,str);
    if (strstr(pattern,".") != NULL){
        //Copy the pattern string 
        //printf("%s ",str);
        //Separate the pattern string into tokens by the "."
        char* token = strtok(pattern_copy,".");
        //Get the token from the left side
        char* token1 = token;
        //Get the token from the right side
        char *token2;
        //Standard function to get the token
        while (token != NULL) {
            token2 = token;
            token = strtok(NULL, ".");
        }
        //Dereference the first and second token into chars because 'C' 
        char token3 = *token1;
        char token4 = *token2;
        int i = 0;
        while (str_copy[i] != '\0'){
            //Checks if the current character in the string matches the first character in the pattern string
            // This will be the intial state
            if (token3 == str_copy[i]){
                //Increment the pointer to move to the next character
                i++;
                char next_state = str_copy[i];
                //If the next character is the second token and the second character is also a second token, it will be a match 
                if (next_state == token4 && str_copy[i+1] == token4){
                    //printf("Pattern matched\n");
                    return true;
                    break;
                //Else case will be if the next character isn't the second token
                //Will have to increment to the next character given the "."
                //Requirement to have any other character between the second token and after the first token
                }else if(next_state == token4){
                    i++;
                }else{
                    //Have a nested while loop to continue iterating the string
                    while(str_copy[i] != '\0'){
                        //When it finally finds a match of the second token, it will break out of the loop
                        if (str_copy[i] == token4){
                            //printf("Pattern matched\n");
                            return true;
                            break;
                            
                        }else{
                        //Continue to next character
                        //Eventually, this loop will exit out and will also exit out the outer while loop
                        //Since we have iterated throught the entire string already
                            i++;
                        }
                    }
                }
            }
            //Move on to the next character in the string
            else{
                i++;
            }
        }
        //Return false if no pattern is found
        //printf("No Pattern Found\n");
        return false;
    }
    else if ((strstr(pattern,"(") != NULL) && (strstr(pattern,")")) != NULL){ //Parentheses
        printf("%s\n",pattern);
        int count = 0;
        for (int i = 0; i < strlen(pattern); i++){
            // printf("%c\n",pattern_copy[i]);
            if (pattern_copy[i] == '('){
                count++;
            }
            else if (pattern_copy[i] == ')' && count == 1){
                count = 0;
                //printf("closing parenthesis found\n");
            }else if (pattern_copy[i] == ')' && count > 1){
                //printf("Invalid parenthesis\n");
                return false;
            }
        }
        if (strstr(pattern,"*") != NULL && strstr(pattern,"?") != NULL){
            printf("Wildcard with () and * and ?\n");
        }
        else if (strstr(pattern,"?") != NULL){
            char literal_start[256];
            char literal_end[256];
            printf("Wildcard with () and ?\n");
            int i = 0;

            while(pattern_copy[i] != '(' && pattern_copy[i] != '\0'){
                literal_start[i] = pattern_copy[i];
                
                i++;
            }
            literal_start[i] = '\0';
            printf("%s",literal_start);
        }else if (strstr(pattern,"*") != NULL){
            char literal_start[256];
            char literal_end[256];
            char substring[256];
            //printf("Wildcard with () and *\n");
            int i = 0;
            //Extracting characters before (
            while(pattern_copy[i] != '(' && pattern_copy[i] != '\0'){
                literal_start[i] = pattern_copy[i];
                i++;
            }
            literal_start[i] = '\0';
            
            //Extracting substrings within ()
            int j = 0;
            if (pattern_copy[i] == '('){
                i++;
                while(pattern_copy[i] != ')' && pattern_copy[i] != '\0'){
                    substring[j] = pattern_copy[i];
                    i++;
                    j++;
                }
            }
            substring[j] = '\0';

            int k = 0;
            if (pattern_copy[i] == ')'){
                i++;
                if (pattern_copy[i] == '*'){
                    i++;
                }
                while(pattern_copy[i] != '\0'){
                    literal_end[k] = pattern_copy[i];
                    i++;
                    k++;
                }
            }
            literal_end[k] = '\0';
            i = 0;
            j = 0;
            k = 0;
            printf("%s %s %s %d %d %d\n",literal_start ,substring,literal_end,i,j,k);

            while(str_copy[i] != '\0'){
                char current_char[2];
                memset(current_char,0,sizeof(current_char));
                current_char[0] =str_copy[i];
                current_char[1] = '\0';
                //No character before the parenthesis
                // if (strcmp(" ",literal_start)){
                    
                // }
                
                if (strcmp(literal_start,current_char) == 0 || strcmp(" ",literal_start)){
                    j = i;
                    j++;
                    char group_char[sizeof(substring)];
                    memset(group_char,0,sizeof(group_char));
                    
                    while(k < strlen(substring)){
                        group_char[k] = str_copy[j];
                        k++;
                        j++;

                    }
                    group_char[k] = '\0';
                    k = 0;
                    //printf("%s",group_char);
                    if (strcmp(substring,group_char) == 0){
                        if (strcmp(" ",literal_end) == 0){
                            printf("Pattern found\n");
                            return true;
                        }else {
                            char next_char[sizeof(literal_end)];
                            memset(next_char,0,sizeof(next_char));
                            while(k < strlen(literal_end)){
                                next_char[k] = str_copy[j];
                                k++;
                                j++;
                            }
                            
                            next_char[k] = '\0';
                            k = 0;
                            if (strcmp(literal_end,next_char) == 0){
                                printf("Pattern found\n for %s %s", group_char,next_char);
                                return true;
                            }else{
                                i++;
                            }
                        }
                    }else{
                        i++;
                    }
                    
                }else{
                    i++;
                }    
            }
            
        }else{
            printf("Unknown delimeter\n");
        }
        printf("No Pattern Found\n");
        return false;
    }
    else if (strstr(pattern,"*") != NULL) {//Checks for the "*" wildcard
        //Separate the pattern string into tokens by the "."
        char* token = strtok(pattern_copy,"*");
        //Get the token from the left side of the wildcard
        char* token1 = token; 
        //Get the token from the right side of the card
        char *token2;
        //Standard function to get the tokens from each side
        while (token != NULL) {
            token2 = token;
            token = strtok(NULL, "*");
        }
        //Get the last n-1 characters
        //str1 will get the substring of character excluding the previous 
        //character of the wildcard -> eg aba*b 
        //                                / \
        //                               ab  a
        char str1[256];
        //str2 will get the character of the single character before the wildcard
        char str2[2];
        str2[0] = token1[strlen(token1)-1];
        str2[1] = '\0';
        //if the string on the right side has only more than one character,
        //separate n - 1 characters from the right substring
        if (strlen(token1) > 1){
            strncpy(str1,token1, strlen(token1)-1);
            str1[strlen(token1) - 1] = '\0';
        //However if its only character on the left, store str1 with that one character
        }else{
            strcpy(str1,token1);
            str1[strlen(str1)] = '\0';
        }
        printf("%s %s %s\n",str1,str2,token2);
        int i = 0;
        //get the length of each substring from each side of the wildcard excluding the last character in the left substring
        int len_token1 = strlen(str1);
        int len_token2 = strlen(token2);
        //Case 1 to account for just the character b in a string
        //IF the wildcard pattern previous and next character is length 1, just need to check if the character b exist in the string
        if (strlen(token1) == 1 && strlen(token2) == 1 && strstr(str_copy,token2) != NULL){
            printf("Pattern Matched\n");
            return true;
        }
        //Case 2 to account if str1 (left side of the wildcard) or token2 (right side of the wildcard)
        //exist in the string but need to still iterate through it to see if its in sequence for pattern matching
        else if(strstr(str_copy,str1) != NULL || strstr(str_copy,token2) != NULL){
            //Iterate through the string character by character
            while(str_copy[i] != '\0'){
                //use malloc to allocate string storage for the length of left side substring
                char *temp = (char*)malloc(len_token1+1);
                //Initialize a nested pointer to be used for later when we iterate through the string
                //multiple times
                int j = i;
                //Intialize another counter to store our left side substring with k characters
                int k = 0;
                if (temp == NULL){
                    //printf("Memory Allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                //This is where we essentially do sliding window to get a substring of k characters from the main string
                while(str_copy[j] != '\0' && k < len_token1){
                    temp[k] = str_copy[j];
                    j++;
                    k++;
                }
                //End the string with a null terminated character
                temp[k] = '\0';
                // printf("Temp %s\n",temp);
                //This essentially checks the whole string and see if the length of the 
                //temporary string is less than the left side of the wildcard
                //This usually happens when it reaches to a state where it's at the end of the string and 
                //still hasn't found a match
                if (strlen(temp) < len_token1){
                    // printf("Temp %s\n",temp);
                    printf("No pattern found\n");
                    return false;
                }
                //Reset j back to wherever i was pointing to in the string
                j = i;
                int l = 0;
                //if the temporary string finds a match with the left side substring
                //we now check to see if there are occurences of the next character 
                //or occurences of the rightside string and if it does find it, then its a match
                if (strcmp(str1,temp) == 0){
                    //Allocate enough memory for one character string
                    char *next_char = (char*)malloc(strlen(str2)+1);
                    if (next_char == NULL){
                        //printf("Memory Allocation failed\n");
                        exit(EXIT_FAILURE);
                    }
                    //Fill in the next character string with chars
                    while(l < len_token2){
                            next_char[l] = str_copy[j+len_token1];
                            j++;
                            l++;
                    }
                    //end the next character with a null terminated character
                    next_char[l] = '\0';
                    //printf("hi %s %s",next_char,temp);
                    //If the next character does indeed match with the rightside, then we can stop iterating through
                    //As it found a pattern
                    if (strcmp(token2,next_char) == 0){
                            // printf("%s\n",next_char);
                            printf("Pattern Matched\n");
                            free(temp);
                            free(next_char);
                            return true;
                    //Otherwise, we check to see if the next character matches with the same instance of
                    //the single character before the wildcard
                    }else if (strcmp(str2,next_char) == 0){
                        //Allocte enough memory for the next char
                        char *next_char2 = (char*)malloc(strlen(str2)+1);
                        //Fill in the character string with the next character
                        next_char2[0] = str_copy[j+strlen(str1)];
                        next_char2[1] = '\0';
                        //printf("hi %s %s",next_char2,temp);
                        //Now we check again to see if it matches with the rightside of the wildcard
                        //If it does, make sure to free up memory from the heap and return a match
                        if (strcmp(token2,next_char2) == 0){
                                printf("Pattern Matched\n");
                                free(temp);
                                free(next_char);
                                free(next_char2);
                                return true;
                        }else{
                        //If that is not the case, we need to keep iterating through the string
                        //And check continuosly to see if the next character matches the previous character
                        //Essentially checking the final state here as well 
                            while(strcmp(str2,next_char2) == 0){
                                next_char2[0] = str_copy[j+strlen(str1)];
                                next_char2[1] = '\0';
                                // printf("%s",next_char2);
                                if (strcmp(token2,next_char2) == 0){
                                    printf("Pattern Matched\n");
                                    free(temp);
                                    free(next_char);
                                    free(next_char2);
                                    return true;
                                }else{
                                    j++;
                                }
                            }
                        }            
                    }          
                    else{
                        free(next_char);
                    }              
                }
                //Else case is we just reset the string back to where i was in the string
                //and make sure to free memory from the heap
                free(temp);
                i++;
            }
        //If it satisfies none of the cases, we just return false meaning no pattern was found
        }else{
            printf("No Pattern found\n");
            return false;
        }
        // //strncmp
        // //strstr
        //Main case of not finding other cases that wasn't accounted for
        printf("No pattern found\n");
        return false;
        
    }else if (strstr(pattern,"?") != NULL){ //Checks for the "?" wildcard
        //Separate the pattern string into tokens by the "."
        char* token = strtok(pattern_copy,"?");
        //Get the token from the left side aka leftside of the wildcard
        char* token1 = token;
        //Get the token from the right side aka rightside of the wildcard
        char *token2;
        //Standard function to get the token
        while (token != NULL) {
            token2 = token;
            token = strtok(NULL, "?");
        }
        //Get the length of both side of the wildcard string
        int len_token1 = strlen(token1);
        int len_token2 = strlen(token2);
        int i = 0;
        //printf("%s %d %s %d %s\n",token1,len_token1,token2,len_token2, str_copy);
        
        /*Accounts for the case of the wildcard pattern have only one character before the wildcard and after the wildcard "?"
         *Looking at the "?" wildcard, as long as there's a instance of the character after the "?" in the string
         *And the preceding the character is only of length one, you just need to check if the character after the "?"
         *Exist in the string
        */
        if (strlen(token1) == 1 && strlen(token2) == 1 && strstr(str_copy,token2) != NULL){
            printf("Pattern Matched\n");
            return true;
        }//Check if both the preceding string and string before and after the wildcard exist in the string
        //Does not check if its in order of the pattern
        //Cases where the lenght of the leftside string of the wildcard is greater than 1
        else if(strstr(str_copy,token1) != NULL && strstr(str_copy,token2) != NULL){
            //Iterate through the whole string character by character
            while(str_copy[i] != '\0'){
                //Allocate enough memory on the heap for one character string
                char *temp = (char*)malloc(len_token1+1);
                if (temp == NULL){
                    //printf("Memory Allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                //Initialize a pointer j to be a nested pointer when iterating through the string
                //To perform other conditional checks
                int j = i;
                int k = 0;
                //Fill in the temporary substring of the string until its the same length of the left side substring
                //This will change throughout iterations of the string
                while(str_copy[j] != '\0' && k < len_token1){
                    temp[k] = str_copy[j];
                    j++;
                    k++;
                }
                //End the temporary substring with a null terminated character
                temp[k] = '\0';
                //Same reason from *
                if (strlen(temp) < len_token1){
                    //printf("No pattern found");
                    return false;
                    
                }
                char *next_char =  (char*)malloc(len_token2+1);
                if (next_char == NULL){
                    //printf("Memory Allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                //Reassign j to be the previous character before adding length of right side of the wildcard substring
                j = i;
                int l = 0;
                //Getting the next character in the string
                while(l < len_token2){
                    next_char[l] = str_copy[j+strlen(token1)];
                    j++;
                    l++;
                }
                //End the string with a null terminated character
                next_char[l] = '\0';
                //printf("%s %s\n",temp,next_char);
                //Check to see if the leftside substring and next character both match the pattern of the wildcard string
                //Make sure to free the memory before exiting the program
                if (strcmp(token1,temp) == 0 && strcmp(token2,next_char) == 0){
                    printf("Pattern Found %s %s with string %s\n",temp,next_char,str_copy);
                    free(next_char);
                    free(temp);
                    return true;    
                }
                //Continue iterating the string and free the memory for both the strings
                free(next_char);
                free(temp);
                i++;
           
            }
        }
        //No pattern found if the whole string has been iterated and none of the conditions are satisfied
        printf("No Pattern Found with %s\n",str_copy);
        return false;

    }else if (isalnum(ch)){
        if(strstr(str,pattern) != NULL){
            return true;
        }   

    }else{
        printf("Unknown delimeter\n");
        exit(EXIT_FAILURE);
    }
}