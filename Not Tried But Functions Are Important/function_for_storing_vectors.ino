char distx[6],disty[6],DIRECTX,DIRECTY;
char *main=gsm+51;
float DISTX=0,DISTY=0;
void Store(char *b)
{
  char *m=b,*a;
  
  while(*m!=' ')
  m++;
  if(*(m+1)==N||*(m+1)==S)
  {

    DIRECTX= *(m+1);  
    for(i=0,a=b;*a!='';a++,i++)
    distx[i]=*a;
    DISTX=atof(distx);
    if(*(a+1)=='+')
    {
    for(i=0,a=b+2;*a!=' ';a++;i++)
    disty[i]=*a;
    DIRECTY=*(a+1);
    }
    else
    DISTY=0;  
  }
  else
  {
    
    for(i=0,a=b;*a!='';a++,i++)
    disty[i]=*a;
    /*for(i=0,a=b+1;*a!=';';a++;i++)
    distx[i]=0;*/
    DIRECTX=0;
    DIRECTY=atof(disty);
     DIRECTY=*(a+1); 
  }
   main=a+1;
}
