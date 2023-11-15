#include "types.h"
#include "user.h"
#include "date.h"

int
main(int argc, char *argv[]){
	struct rtcdate r;

	//call date system call
	if(date(&r)){
		printf(2, "failed!\n");
		exit();
	}

	printf(1, "Current time : %d-%d-%d %d:%d:%d\n", r.year, r.month,
		       r.day, r.hour, r.minute, r.second);
	
	exit();
}
