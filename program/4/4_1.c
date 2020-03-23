#include<stdio.h>
int main()
{
	char *c;
	c="#include<stdio.h>%cint main()%c{%cchar *c;%cc=%c%s%c;%cprintf(c,10,10,10,10,34,c,34,10,10,10);%creturn 0;%c}";
	printf(c,10,10,10,10,34,c,34,10,10,10);return 0;
}
