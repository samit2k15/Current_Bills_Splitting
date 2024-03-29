/*
############################################################################
# 0. This program can solve Electricity Bills calculaton                   #
#     # At ANY_TIME        # WITH_OUT_PER/UNIT COST                        #
# 1. When we calculate Many Meters Bills NO NEED OF PER_UNIT COST .        #
# 2. If you have Billing Amount @ (No need of Per/Unit Cost).              #
# 3. Know Your Electricity cost at ANY_TIME (U must know PER_UNIT COST)    #
#                                                                          #
############################################################################
*/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
void one_meter();
void endline(){printf("\n---------------------------End of this Calculation---------------------------");}
void longline(){printf("\n----------------------------------------------------------------------------");}
void shortline(){printf("\n-------------------------------------------------");}
void hashline()
 {
  printf("\n##########################################################################");
 }			       /*Global Variables declerations*/
int n;
//float t_units=0.0,t_amount=0.0,t_extra_charges=0.0,per_unit_cost=0.0;
//float bills=0.0,t_bill=0.0;
float t_units,t_amount,t_extra_charges,per_unit_cost,bills,t_bill;
      struct bill              /*declaring a structure to create a node*/
       {
	 float m_no,e_cost;
	 int pres,prev,e_unit;
       }b[100];

void d_units_cost()        /*End of calculation and displaying the result*/
  {
   clrscr();
   printf("\n\t\t\tWELCOME TO ELECTRICITY DEPARTMENT ");
   printf("\n\tELECTRICITY TARRIF FOR HOUSEHOLDS MEDAK DISTRICT (Rates/units)");
   longline();
   printf("\n|      Upto 50 Units/Month.............................Rs_ 1.45/Unit       |");
   printf("\n|      51  - 100 Units/Month...........................Rs_ 2.60/Unit       |");
   printf("\n|      101 - 200 Units/Month...........................Rs_ 4.30/Unit       |");
   printf("\n|      201 - 300 Units/Month...........................Rs_ 7.20/Unit       |");
   printf("\n|      301 - 400 Units/Month...........................Rs_ 8.50/Unit       |");
   printf("\n|      401 - 800 Units/Month...........................Rs_ 9/Unit          |");
   printf("\n|      Above 800+ units................................Rs_ 9.50/Unit       |");
   longline();
  }
void display(int n)               /*Displaing all calcuated bills */
{
int i,k=0,ok;
FILE *fp;                        /*Pointer  to a file */       //char ch;
k=n;
clrscr();
fp=fopen("C:\\TURBOC3\\current.txt","w");
printf("\n\t\t\tYOUR BILL IS HERE ");
fprintf(fp,"\n\t\t\tYOUR BILLS ARE HERE ");
fprintf(fp,"\n--------------------------------------------------------------------------");
longline();                            /*It will print a long --- line */
printf("\nS.NO - METER.NO - UNITS_CONSUMED - P_UNIT_COST - EXT_CHARGES - COST");
fprintf(fp,"\nS.NO - METER.NO  - UNITS_CONSUMED - P_UNIT_COST - EXT_CHARGES - COST");
longline();
fprintf(fp,"\n--------------------------------------------------------------------------");
 for(i=1;i<=k;i++)
  {
   printf("\n  %d   - %f    - %d           - %f - %f - %f ",i,b[i].m_no,b[i].e_unit,per_unit_cost,t_extra_charges/k,b[i].e_cost);
   fprintf(fp,"\n  %d   - %f    - %d           - %f - %f - %f ",i,b[i].m_no,b[i].e_unit,per_unit_cost,t_extra_charges/k,b[i].e_cost);
  }
longline();
fprintf(fp,"\n--------------------------------------------------------------------------");
printf("\n  %d -\t\t -     %d           - %f - %f - %f ",k,t_units,per_unit_cost,t_extra_charges,bills);
fprintf(fp,"\n  %d -\t\t -     %d           - %f - %f - %f ",k,t_units,per_unit_cost,t_extra_charges,bills);
longline();
fprintf(fp,"\n--------------------------------------------------------------------------");
printf("\n\t\t\t\t\t\t%f",t_bill);
fprintf(fp,"\n\t\t\t\t\t\t\t%f",t_bill);
longline();
fprintf(fp,"\n--------------------------------------------------------------");
printf("\n\t\t\t End of this calculaton ");
fprintf(fp,"\n\t\t\t End of this calculaton ");
printf("\nEnter 1 for more choise :");
scanf("%d",&ok);
fclose(fp);
}
void bill(int n)
  {
   int i,s;
   s=n;
   clrscr();
   hashline();
   printf("\n#----------------------------REMEMBER---------------------------#");
   printf("\n# There is a different between :                                #");
   printf("\n# 1. Electricity Consumed Units Amount :                        #");
   printf("\n# 2. Extra Charges Amount                                       #");
   hashline();
   t_amount:
   printf("\nEnter Your Electricity Consumed Bill Amont : ");
   scanf("%f",&t_amount);
   if(t_amount>=0)
   {
   extra_charges:
   printf("\n Enter Total Extra charges :");
   scanf("%f",&t_extra_charges);
   if(t_extra_charges>=0)
   {
    for(i=1;i<=s;i++)  /* here 's' is a globle declaration (Count of Meters) */
      {
       longline();
       printf("\n\tS.no %d calculation started :",i);
       longline();
       printf("\nEnter Meter No: ");
       scanf("%f",&b[i].m_no);
       printf("Enter Present meter reading of %d  : ",i);
       scanf("%d",&b[i].pres);
       printf("Enter previous meter reading of %d : ",i);
       scanf("%d",&b[i].prev);
       b[i].e_unit=b[i].pres-b[i].prev;          /* calculating each unit */ /*shortline();printf("\n-- Meter_no - Present - Previou - Unit --- Extra_charges ");*/
       t_units+=b[i].e_unit;
       per_unit_cost=(t_amount/t_units);         /*shortline();/* calculating total units *//*printf("\n-- %f --- %d --- %d --- %d --- %f",b[i].m_no,b[i].pres,b[i].prev,b[i].e_unit,t_extra_charges/n);shortline();*/
       b[i].e_cost=(per_unit_cost*b[i].e_unit);  /* calculating each meter cost */
       bills+=b[i].e_cost;                       /* calculating total Amount */
       t_bill+=bills+t_extra_charges;
       }
       printf("\nYour all Done !");
     display(n);
    }else
	{
	  printf("\n Alphabits are Prohebited :");
	  goto extra_charges;
	}
   }else {
	  printf("\n Alphabits are Prohebited :");
	  goto t_amount;
	 }
    printf("\n YOUR DONE .....\n");
  }
  void Per_unit_cost(int n)             /*To calculate many meters bills */
  {
  int i,s;
  s=n;	                                /*int k=0;k=s;*/
  clrscr();
  hashline();
  printf("\n#-------------------------- REMEMBER ----------------------------#");
  printf("\n# There is a different between :                                 #");
  printf("\n# 1. Electricity Consumed Units Amount :                         #");
  printf("\n# 2. Extra Charges Amount                                        #");
  hashline();
  p_unit:
  printf("\nEnter your Per_unit cost  :");
  scanf("%f",&per_unit_cost);
  if(per_unit_cost>=0)
  {
  tot_ext:
  printf("\n Enter Total Extra charges :");
  scanf("%f",&t_extra_charges);
    if(t_extra_charges>=0)
    {
    for(i=1;i<=s;i++)  /* here 's' is a globle declaration (Count of Meters) */
      {
       longline();
       printf("\n\tS.no %d calculation started :",i);
       longline();
       printf("\nEnter Meter No: ");
       scanf("%f",&b[i].m_no);
       printf("Enter Present meter reading of %f  : ",b[i].m_no);
       scanf("%d",&b[i].pres);
       printf("Enter previous meter reading of %f : ",b[i].m_no);
       scanf("%d",&b[i].prev);
       b[i].e_unit=b[i].pres-b[i].prev;          /* calculating each unit */ /*shortline();printf("\n-- Meter_no - Present - Previou - Unit --- Extra_charges ");*/
       t_units+=b[i].e_unit;                     /*per_unit_cost=(t_amount/t_units); /*shortline();/* calculating total units *//*printf("\n-- %f --- %d --- %d --- %d --- %f",b[i].m_no,b[i].pres,b[i].prev,b[i].e_unit,t_extra_charges/n);shortline();*/
       b[i].e_cost=(per_unit_cost*b[i].e_unit);  /* calculating each meter cost */
       bills+=b[i].e_cost;                       /* calculating total Amount */
       t_bill+=(bills+t_extra_charges);           //b[i].e_unit=b[i].pres-b[i].prev;           /* calculating each unit *//*shortline();       printf("\n-- Meter_no - Present - Previou - Unit --- Extra_charges ");t_units+=b[i].e_unit;                      /*shortline();/* calculating total units *//*printf("\n-- %f --- %d --- %d --- %d --- %f",b[i].m_no,b[i].pres,b[i].prev,b[i].e_unit,t_extra_charges/n); shortline();*/
       b[i].e_cost=per_unit_cost*b[i].e_unit;     /* calculating each meter cost */
       t_amount+=b[i].e_cost;	                  /* calculating total Amount */
       }
     display(n);
     } else
	 {
	 printf("\nStrings are prohebited ");
	 goto  tot_ext;
	 }
    }else
	 {
	  printf("\nStrings are prohebited ");
	  goto p_unit;
	 }
    printf("\n YOUR DONE.....\n");
   }
int main()
{
 int ch,h;
 clrscr();                                   //d_units_cost();
 do {
 hashline();
 printf("\n\t\t\tWELCOME TO ELECTRICITY DEPARTMENT");
 printf("\n\tELECTRICITY TARRIF FOR HOUSEHOLDS MEDAK DISTRICT (Rates/units)");
 longline();
 printf("\n NOTE :");
 printf("\n I.This program can solve Electricity Bill calcultaion,using Total Units:");
 printf("\n   It can also calculate using Ecectricity Bill (Amount) :");
 printf("\n\t\t\t\t(OR)");
 printf("\n   Meter Readings (Present - Previous meter Readings) to get Units :");
 printf("\nII.Either One Main-Meter or Many Sub_Meters Connected to One Main-Meter:");
 longline();
 printf("\n############ PLEASE DO NOT USE NEGTIVE NUMBERS OR CHARACTERS #############");
 longline();
 printf("\n\t ==>CHOOSE [1 or 2] TO START CALCULATIONS :");
 printf("\n\t1 -> Do You Have an Electricity Bill :");
 printf("\n\t2 -> Do You Know the Per_uint Cost (It varies on District) :");
 printf("\n\t3 -> Default Per_unit Cost (only one meter at a time):");
 printf("\n\t4 -> Exit and End ");
 longline();
 printf("\nEnter your choise here ->  ");
 scanf("%d",&ch);
 longline();
 switch(ch)
  {
   case 1: up:
	   printf("\nHow many Meter's do you have :");
	   printf("\nPlese enter here-> ");
	   scanf("%d",&n);
	   if(n>0)
	   bill(n);
	   else {
		 shortline();
		 printf("\n* Meter Number's must be Greater than 0 :");
		 printf("\n* Characters not allowed : ");
		 shortline();
		 printf("\n Want to continue : press -> 1 ");
		 scanf("%d",&h);
		 if(h==1)
		  {
		   goto up;
		  }
		  else
		   printf("\n Wrong choise ");
		}
	   break;
   case 2: start:
	   printf("\nHow many Meter's do you have :");
	   printf("\nPlese enter here-> ");
	   scanf("%d",&n);
	   if(n>0)
	   Per_unit_cost(n);
	   else {
		 shortline();
		 printf("\n* Meter Number must be Greater than 0 :");
		 printf("\n* Characters not allowed :");
		 shortline();
		 printf("\n Want to continue : press -> 1 ");
		 scanf("%d",&h);
		 if(h==1)
		  {
		  goto start;
		  }
		  else
		   printf("\n Wrong choise ");
		 }
	   break;
   case 3:  one_meter();
	   break;
   case 4: printf("\n\t\t\tProgram Ends..... :");
	   break;
   default:printf("\n\n\t\t\tHuman's do mistakes ......-:)");
	   printf("\n\t-------No Problem! Select any [1 or 2] Again:------\n");
	   break;
   }
  } while(ch!=4);
getch();
return 0;
}

void one_meter()               /* Calculating only Main meter bill */
 {
  float meter_no;
  clrscr();
  d_units_cost();
  longline();
  printf("\t\t You seems to have Only 1 Main Meter :\n");
  longline();
  d_units_cost();              /* Display each units cost */
   m_no:
  printf("\n\n\tTo identify Meter, Enter your Meter-Number:");
  printf("\n\tEnter here -> ");
  scanf("%f",&meter_no);
  if(meter_no>0)
  {
   t_unts:
  printf("Enter your Total units Consumed here  ->");
  scanf("%f",&t_units);
  if(t_units>0)
  {
  t_x_chr:
  printf("\nPlease Include (SUM OF ALL) Extra Charges (Ex:Vat,Service) ==>");
  printf("Enter Here ->");
  scanf("%f",&t_extra_charges);
  if(t_extra_charges)
  {
    if(t_units<=50)
     {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 1.45");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs-> .... %f",t_units*1.45+t_extra_charges);
     shortline();
     endline();
     }
    else if(t_units>=51&&t_units<=100)
    {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 2.60");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs->..... %f",t_units*2.60+t_extra_charges);
     shortline();
     printf("\n\n\t\tThanking you, hope u satisfied. If not !");
     endline();
     }
     else if(t_units>=101&&t_units<=200)
     {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 4.30");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs->..... %f",t_units*4.30+t_extra_charges);
     shortline();
     endline();
     }
     else if(t_units>=201&&t_units<=300)
     {
      printf("\nYour Meter-Number is ......... %f",meter_no);
      shortline();
      printf("\nTotal Units is ............... %f",t_units);
      printf("\nPer Unit cost is ............. 7.20");
      printf("\nSum of all other taxes........ %f",t_extra_charges);
      shortline();
      printf("\nTotal Bill Amount is Rs->..... %f",t_units*7.20+t_extra_charges);
      shortline();
	endline();
     }
    else if(t_units>=301&&t_units<=400)
     {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 8.50");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs->..... %f",t_units*8.50+t_extra_charges);
     shortline();
       endline();
     }
    else if(t_units>=401&&t_units<=800)
     {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 9.00");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs->..... %f",t_units*9.00+t_extra_charges);
     shortline();
     endline();
     }
    else if(t_units>=401&&t_units<=800)
     {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 9.50");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs->..... %f",t_units*9.50+t_extra_charges);
     shortline();
     endline();
     }
   else if(t_units>=801)
   {
     printf("\nYour Meter-Number is ......... %f",meter_no);
     shortline();
     printf("\nTotal Units is ............... %f",t_units);
     printf("\nPer Unit cost is ............. 9.00");
     printf("\nSum of all other taxes........ %f",t_extra_charges);
     shortline();
     printf("\nTotal Bill Amount is Rs->..... %f",t_units*9.00+t_extra_charges);
     shortline();
     endline();
    }
   }
    else
    {
    printf("\nString & Negative values are not allowed :");
    goto m_no;
    }
  }
   else
    {
    printf("\nString & Negative values are not allowed :");
    goto t_unts;
    }
 }
  else
     {
      printf("\nString & Negative values are not allowed :");
      goto t_x_chr;
     }
 printf("\n YOUR DONE .....");
}
