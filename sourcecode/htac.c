#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512]; //just to follow cat.c buffer size
char *lines_arr[1024]; //maximum 1024 lines can be stored
int line = 0; //counts stored lines

int is_valid_integer(const char *str) {
    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0;  //not valid integer
        }
        str++;
    }
    return 1;  //valid integer
}

void htac(int fd) {
    int n; // number of bytes read in read()
    int total = 0; // total number of bytes currently in buf
    int last_newline = 0; // current line's starting index

    //Reading file
    while((n = read(fd, buf + total, sizeof(buf) - total - 1)) > 0) {
	//read data from file, store in buf start from the position 'buf+total'(not to overwrite data)    
        //process the data in current buf
	total += n; //total(+num of bytes read) 
        for(int i = 0; i < total; i++) {//iterate every character in buf
            if(buf[i] == '\n') {//check end of line
                int length = buf + i - (buf + last_newline);//length of current line
                lines_arr[line] = malloc(length + 1);//store line(allocate memory)
                memmove(lines_arr[line], buf + last_newline, length);
		//copy line from buf to allocate memory
                lines_arr[line][length] = 0; //add '\0'
                line++; //stored lines+1
                last_newline = i + 1;  // next line's starting index
                
                if(line == 1024) { //since only maximum 1024 can be stored
                    free(lines_arr[0]);//free first line
                    for(int j = 0; j < 1023; j++) { //move every line up
                        lines_arr[j] = lines_arr[j + 1];
                    }
                    line--; 
                }
            }
        }

	//preparing buffer for next read
        memmove(buf, buf + last_newline, total - last_newline);//move left data to start of buf
        total -= last_newline; //left data
        last_newline = 0; // reset last_newline(since buf is updated to left data)
    }
    if(total > 0) { // check unprocessed characters still left in buffer 
        lines_arr[line] = malloc(total + 1); //allocate memory(store left characters in buf)
        memmove(lines_arr[line], buf, total); //copy left charters from buf to line_arr[line] 
        lines_arr[line][total] = 0; //add '\0'
        line++; 
    }
}

int main(int argc, char *argv[]) {
    int n; //requested number of lines from user
    n = atoi(argv[1]); //convert argv[1] to integer(to check range)
    
    if(argc!=3){ //check if it consists only 3 arguments(if not exit())
        printf(1, "Usage: htac <lines> <file name>\n");
        exit();
    }

    if (!is_valid_integer(argv[1])) {//check if it's valid integer(if not exit())  
        printf(1, "Type a valid integer between 1-1024\n");
        exit();
    }
    
    if (n > 1024 || n < 1) { //check if requested number is within range(if not exit())
        printf(1, "Type between 1-1024\n");
        exit();
    }
	
    //Read file
    int fd;
    if((fd = open(argv[2], 0)) < 0){
        printf(1, "Can't open %s.\n", argv[2]);
        exit();
    }
    htac(fd);
    close(fd);
    
    //Determine the start index of line for printing(among lines_arr[])
    int start_idx; //going to be last index of line to be printed  
    if (line - n >= 0) { //case)number of stored lines >= number of requested lines 
        start_idx = line - n; // set the index line-n
    } else { 
        start_idx = 0; //set start index 0(will print all lines stored)
    }
    
    // Print the lines in reverse order
    for(int i = line - 1; i >= start_idx; i--) { 
        printf(1, "%s\n", lines_arr[i]);
    }

    // Free allocated memory
    for(int i = 0; i < line; i++) {
        free(lines_arr[i]); //free each lines
    }
    exit();
}
