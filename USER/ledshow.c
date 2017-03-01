#include "leddriver.h"
#include "ledshow.h"

unsigned char buf[2*32];

void delay(uint16_t temp)
{
	uint16_t di,dj;
	for(di=0;di<temp;di++)
	  for(dj=0;dj<4;dj++);
}
void delay_100(uint16_t temp)
{
	uint16_t di,dj;
	for(di=0;di<temp;di++)
	  for(dj=0;dj<7200;dj++);
}

void Display(unsigned char *table,uint8_t Px)
{
     uint16_t i,j,l;
     for(l=0;l<4;l++)                  //显示的四行
    {   
        for(j=0;j<2;j++)          //显示3、4列
        {
             for(i=0;i<2;i++)      // 显示1、2列
            {
                 SerialPut(table[j*32+3*8+2*l+i],Px);
                 SerialPut(table[j*32+2*8+2*l+i],Px);
                 SerialPut(table[j*32+1*8+2*l+i],Px);
                 SerialPut(table[j*32+0*8+2*l+i],Px);
                
                
         	}
        }	
    Scan(l,Px);
    }
}

void RollLeft(unsigned char *table,uint8_t Px)
{	
   uint16_t i,j,l,k,add,shan_1,x;
   uint32_t	buf_0,buf_1,buf_2,buf_3;
   uint32_t roll[16]={0};
   for(add=0;add<32;add++)
  	{
   	  for(shan_1=0;shan_1<100;shan_1++)
	  {
	   for(k=0;k<16;k++)                  
	    {   
		 if(add>=16)
			{
				roll[k]=table[(x+1)*32+2*k];
				roll[k]=(roll[k]<<8)+table[(x+1)*32+2*k+1];
				roll[k]=(roll[k]<<8)+table[x*32+2*k];
				roll[k]=(roll[k]<<8)+table[x*32+2*k+1];
				buf_0=roll[k];
				buf[0*32+2*k]=(buf_0>>(24-add%16))&0xff;
				buf_1=(buf_0<<8)+table[(x+1)*32+2*k];
				buf[0*32+2*k+1]=(buf_1>>(24-add%16))&0xff;
				buf_2=(buf_1<<8)+table[(x+1)*32+2*k+1];
				buf[1*32+2*k]=(buf_2>>(24-add%16))&0xff;
				buf_3=(buf_2<<8)+table[x*32+2*k];
				buf[1*32+2*k+1]=(buf_3>>(24-add%16))&0xff;
			}
			else
			{     
			    roll[k]=table[x*32+2*k];
				roll[k]=(roll[k]<<8)+table[x*32+2*k+1];
				roll[k]=(roll[k]<<8)+table[(x+1)*32+2*k];
				roll[k]=(roll[k]<<8)+table[(x+1)*32+2*k+1];
				buf_0=roll[k];
				buf[0*32+2*k]=(buf_0>>(24-add))&0xff;
				buf_1=(buf_0<<8)+table[x*32+2*k];
				buf[0*32+2*k+1]=(buf_1>>(24-add))&0xff;
				buf_2=(buf_1<<8)+table[x*32+2*k+1];
				buf[1*32+2*k]=(buf_2>>(24-add))&0xff;
				buf_3=(buf_2<<8)+table[(x+1)*32+2*k];
				buf[1*32+2*k+1]=(buf_3>>(24-add))&0xff;
			}	  
	    }
		for(l=0;l<4;l++)                  //显示的四行
	    {   
	        for(j=0;j<2;j++)          //显示3、4列
	        {
	             for(i=0;i<2;i++)      // 显示1、2列
	            {
	                 SerialPut(buf[j*32+3*8+2*l+i],Px);
	                 SerialPut(buf[j*32+2*8+2*l+i],Px);
	                 SerialPut(buf[j*32+1*8+2*l+i],Px);
	                 SerialPut(buf[j*32+0*8+2*l+i],Px);
	         	}
	        }	
	    Scan(l,Px);
	   }
	  }
	}
}

void RollLeftShow(unsigned char *table,uint8_t Px)
{													
   uint16_t i,j,l,k,add,shan_1,shun;
   uint32_t	buf_0,buf_1,buf_2,buf_3;
   uint32_t roll[16]={0};
   for(shun=0;shun<12;shun++)
     {
	  for(add=0;add<16;add++)
	  	{
	   	  for(shan_1=0;shan_1<100;shan_1++)
		  {
			   for(k=0;k<16;k++)                  //显示的四行
			    {      
				    roll[k]=table[shun*32+2*k];
					roll[k]=(roll[k]<<8)+table[shun*32+2*k+1];
					roll[k]=(roll[k]<<8)+table[(shun+1)*32+2*k];
					roll[k]=(roll[k]<<8)+table[(shun+1)*32+2*k+1];
					buf_0=roll[k];						 
					buf[0*32+2*k]=(buf_0>>(24-add))&0xff;
					buf_1=(buf_0<<8)+table[(shun+2)*32+2*k];
					buf[0*32+2*k+1]=(buf_1>>(24-add))&0xff;
					buf_2=(buf_1<<8)+table[(shun+2)*32+2*k+1];
					buf[1*32+2*k]=(buf_2>>(24-add))&0xff;
					buf_3=(buf_2<<8)+table[(shun+3)*32+2*k];
					buf[1*32+2*k+1]=(buf_3>>(24-add))&0xff;	  
			    }
			
				for(l=0;l<4;l++)                  //显示的四行
			    {   
			        for(j=0;j<2;j++)          //显示3、4列
			        {
			             for(i=0;i<2;i++)      // 显示1、2列
			            {
			                 SerialPut(buf[j*32+3*8+2*l+i],Px);
			                 SerialPut(buf[j*32+2*8+2*l+i],Px);
			                 SerialPut(buf[j*32+1*8+2*l+i],Px);
			                 SerialPut(buf[j*32+0*8+2*l+i],Px);
			         	}
			        }	
			    Scan(l,Px);
			   }
		 	  }
		}
		   
	 }
}




