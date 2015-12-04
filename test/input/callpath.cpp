
void leafOne(){}
void leafTwo(){}


void callsLeafOne(){
	leafOne();
}

void callsLeavesOneAndTwo(){
	leafOne();
	leafTwo();
}

void callsLeavesTwoAndOne(){
	leafTwo();
	leafOne();
}

int main(int argc, char **argv){
	callsLeafOne();
	callsLeavesOneAndTwo();
	callsLeavesTwoAndOne();
	return 0;
}
