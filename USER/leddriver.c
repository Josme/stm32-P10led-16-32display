#include "leddriver.h"

#define uint unsigned int
#define uchar unsigned char
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
#define plong 8
#define dispbuf_mul 4

uchar disp_buf[plong*dispbuf_mul];
uchar scan_temp;//显示内容标志
uchar scan_mod;
uchar num_dotmatrix[10][16]={
/*-- 文字: 0 --*/ 
/*-- 宋体12; 此字体下对应的点阵为：宽x高=8x16 --*/ 
0xFF,0xFF,0xFF,0xE7,0xDB,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xDB,0xE7,0xFF,0xFF,/*"0",0*/
0xFF,0xFF,0xFF,0xEF,0x8F,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0x83,0xFF,0xFF,/*"1",1*/
0xFF,0xFF,0xFF,0xC3,0xBD,0xBD,0xBD,0xFB,0xFB,0xF7,0xEF,0xDF,0xBD,0x81,0xFF,0xFF,/*"2",2*/
0xFF,0xFF,0xFF,0xC3,0xBD,0xBD,0xFB,0xE7,0xFB,0xFD,0xFD,0xBD,0xBB,0xC7,0xFF,0xFF,/*"3",3*/
0xFF,0xFF,0xFF,0xFB,0xF3,0xEB,0xDB,0xDB,0xBB,0xBB,0x81,0xFB,0xFB,0xE1,0xFF,0xFF,/*"4",4*/
0xFF,0xFF,0xFF,0x81,0xBF,0xBF,0xBF,0xA7,0x9B,0xFD,0xFD,0xBD,0xBB,0xC7,0xFF,0xFF,/*"5",5*/
0xFF,0xFF,0xFF,0xE3,0xDB,0xBF,0xBF,0xA7,0x9B,0xBD,0xBD,0xBD,0xDB,0xE7,0xFF,0xFF,/*"6",6*/
0xFF,0xFF,0xFF,0x81,0xBB,0xBB,0xF7,0xF7,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xFF,0xFF,/*"7",7*/
0xFF,0xFF,0xFF,0xC3,0xBD,0xBD,0xBD,0xDB,0xE7,0xDB,0xBD,0xBD,0xBD,0xC3,0xFF,0xFF,/*"8",8*/
0xFF,0xFF,0xFF,0xE7,0xDB,0xBD,0xBD,0xBD,0xD9,0xE5,0xFD,0xFD,0xDB,0xC7,0xFF,0xFF/*"9",9*/
};
void GPIO_Config_P1(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 															   
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_8 ; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8 ; 
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8 ; 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

void Showline(uchar line_num);  //行扫
void delay(uchar ms); 
void num_todisp();
void display1p();
void moveleft(uchar sspp);  //左移显示效果
void leftoned();//左移一点
void onedisp(uchar ttime); 
void delay(uchar ms)
{
	uchar i,j;
	for(i=0;i<ms;i++)
	{
		for(j=0;j<20;j++);
	}
}

void cldispb(void)		//清显示缓存区
{
	uchar *disbuff;
	uint i; 	
	disbuff=&disp_buf[0];
	for(i=0;i<plong*dispbuf_mul;i++)
	{
		*disbuff=0xff;
		disbuff++;
	}
}
void onedisp(unsigned char ttime)		
{
	uint i,stopt;
		  num_todisp();                       //计算缓存直接送入显示缓存
	stopt=ttime*50;           //显示时间
	for(i=0;i<stopt;i++)
	{
	  display1p();                 //调用扫描子程序
	}
  cldispb();
 display1p();
}

void flicker(unsigned char sspeed,unsigned char stop)//闪烁显示 	
{
	uint i,kk,stopt,sped;
	uchar j;
	sped=sspeed*10;
   	stopt=stop*10;
	for(j=0;j<4;j++)
	{
		num_todisp();  
		for(i=0;i<sped;i++)
		{
			display1p();
		}	
		cldispb();
		for(i=0;i<sped;i++)
		{
			display1p();
		}
		
	}
		  num_todisp();  
		 	for(kk=0;kk<stopt;kk++)
	{
		display1p();
	}
}
void Showline(uchar line_num) 
{
	switch(line_num)
	{
	case 3:PIAN_B(1);PIAN_A(1);
			break;
	case 2:PIAN_B(1);PIAN_A(0);
			break;
	case 1:PIAN_B(0);PIAN_A(1);
			break;
	case 0:PIAN_B(0);PIAN_A(0);
			break;
	default:break;
	}
}
void num_todisp(void)			//数据到显示缓存区
{
  unsigned char i,k;
  unsigned char *point,*temp1,*disp_temp;
  point=&num_dotmatrix[0][0];      //汉字库首地址
  disp_temp=&disp_buf[0];           //取计算缓存首地址
 for(k=0;k<plong;k++)                     //4个字
 {
      point=&num_dotmatrix[k][0]; //从ROM中的下一个汉字的首地址
	  for(i=0;i<16;i++)
	  {	   
	      	temp1=disp_temp+(plong*i); //对应行地址=汉字数*2(字节数)*行号
			*temp1=*(point+i+1+1);         //汉字对应行的第二个字节
	  }
	  disp_temp=disp_temp+1;     //下一个数字
  }
}
void display1p()
{
  uchar temp,j,k,i,m; 
  unsigned char *point,*point1;
  uchar hangxu=1;
  point1=&disp_buf[0];
  point = point1;
  for(m=0;m<4;m++)
  {
	 for(j=0;j<4;j++)
	 	{
			 	 for(k=0;k<plong;k++)
			 {
	            temp=*point;
	            for(i=0;i<8;i++)
					{
				      
				      DATA_R(disp_buf[0]&0x80);
					 if((temp&0x80)==0x00)
					   {
						DATA_R(0);
					   }	
                                           
				   	  CLK_L(1);
                       delay(1);
				      temp>>=1;
					} 
			 }
			 OE(0);
			 CLK_S(0);
             delay(1);
	 		 CLK_S(1);
             delay(1);
			 CLK_S(0);
	 		 Showline(j);
	 		 OE(1);
			 delay(9);
	         OE(0);
	       point=point1+plong*(j);
	    }
	 }
}
void disp_test()   //测试显示屏
{
  uchar temp,j,k,i,m; 
  unsigned char *point,*point1; 
  unsigned char disp_test[16]={0x0,0xff,0,0xff,0,0xee,0,0x23,0,0,0,0,0,0,0,0x10};
  point1=&disp_test[0];
  point = point1;
	 for(j=0;j<4;j++)
	 	{	 
	            //temp=*point;
					temp=disp_test[1];
	            for(i=0;i<8;i++)
					{	 
					 DATA_R(temp&0x80);
				      CLK_S(1);
				        ;
				   	  CLK_S(0);
				     	;
						temp<<=1;
					}
					for(i=0;i<8;i++)
					{
					 CLK_L(0);
					 delay(1);
			 		 CLK_L(1);
					 delay(1) ;
					 CLK_L(0);
					} 
			  
			  OE(1);
			  delay(1) ;
	 		  Showline(j);
			  delay(1);
	 		  OE(0)	;		
			  delay(1);
	    
		 }
	 
}
void leftoned()
{
	uchar qq,ww;
	uchar bb2,bb3,bb4;
//	uchar  *buff1;
	uchar  *buff2;
		for(qq=0;qq<16;qq++)
		{
		/*	buff1=&disp_buf[0]+(plong*qq);		//
			temdat1=*buff1;
			bb1=temdat1&0x01;
			if(bb1==1)
			{
				bb2=0x80;
			}
			else
			{
				bb2=0x00;
			}
			temdat1>>=1;
			*buff1=temdat1;*/
			buff2 = &disp_buf[0] + qq *plong;		//显示
//            buff2 = &disp_buf[0] + qq *plong;		//显示
			for(ww=0;ww<plong;ww++)	
			{
				//if(ww<plong-1)bb3=*buff2&0x01;
                //else bb3=1; 
                bb3=*buff2&0x01;
				bb4=*buff2;
				bb4>>=1;
				bb4=(bb4&0x7f)|bb2;   //超过4个字
				*buff2=bb4;
				buff2++;
				if(bb3==1)	
				{
					bb2=0x80;
				}
				else
				{
					bb2=0x00;
				}
               if(ww==plong-1)bb2=0x80;
			}

		}
}
void moveleft(unsigned char sspp)
{
	uchar i,j,k;
    num_todisp();  
	for(i=0;i<plong;i++)		//ilong
	{
		for(j=0;j<8;j++)
		{
			leftoned();			//左移1点
				for(k=0;k<sspp;k++)
				{
					display1p();
                    if(scan_mod != scan_temp){scan_temp=scan_mod;goto exit;} 
				}
            
		}
	}
exit:; 
   cldispb();
}
void main()
{
//	uint data_r,i;
   // SP = 0X30;
//	 
    GPIO_Config_P1();
   num_todisp();
     onedisp(20);
	 while(1)
	 {	 
          
    disp_test();
      display1p();
      cldispb();
      moveleft(20);
      flicker(50,50);
//delay(50);
		 }
	 }
 


