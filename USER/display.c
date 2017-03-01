#include "stm32f10x.h"
#include <string.h>
#include <ascii.h>
#include "math.h"
#define uchar unsigned char  
#define uint unsigned int 

#define OE(a)	if (a)	\
					GPIOB->BSRR = GPIO_Pin_8;\
					else		\
					GPIOB->BRR = GPIO_Pin_8;
#define A1(a)	if (a)	\
					GPIOB->BSRR = GPIO_Pin_6;\
					else		\
					GPIOB->BRR = GPIO_Pin_6;
#define B1(a)	if (a)	\
					GPIOA->BSRR = GPIO_Pin_10;\
					else		\
					GPIOA->BRR = GPIO_Pin_10;
#define SHCP(a)	if (a)	\
					GPIOA->BSRR = GPIO_Pin_8;\
					else		\
					GPIOA->BRR = GPIO_Pin_8;                   
#define STCP(a)	if (a)	\
					GPIOC->BSRR = GPIO_Pin_8;\
					else		\
					GPIOC->BRR = GPIO_Pin_8;
#define DATA(a)	if (a)	\
					GPIOC->BSRR = GPIO_Pin_6;\
					else		\
					GPIOC->BRR = GPIO_Pin_6;
#define NOP     ;                    
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

#define BEEP  ;
//#define OE(1)  OE(1)
//#define OE(0)  OE(0)
//#define A1(1)  A1(1)
//#define A1(0)  A1(0)
//#define B1=1  B1(1)
//#define B1=0  B1(0)
//#define SHCP=1  SHCP(1)
//#define SHCP=0  SHCP(0)
//#define STCP=1  STCP(1)
//#define STCP=0  STCP(0)
//#define DATA(1)  DATA(1)
//#define DATA(0)  DATA(0)
#define BUF_SIZE 64
unsigned char ip,tp,min,sec;
u16 time;
unsigned char PLAYBUF[BUF_SIZE];
u8 yi_dong[31] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
u8 tab[10] = {"0123456789"};
const uchar table[160]=
{
0x10,0x00,0x08,0x0E,0x7F,0x70,0x22,0x40,0x14,0x40,0xFF,0x7E,0x08,0x48,0x08,0x48,
0xFF,0x48,0x08,0x48,0x2C,0x48,0x2A,0x88,0x4A,0x88,0x89,0x08,0x2A,0x08,0x10,0x08,
0x00,0x00,0x00,0x40,0x78,0x40,0x0B,0xF8,0x10,0x48,0x17,0xFE,0x20,0x48,0x7B,0xF8,
0x08,0x40,0x4B,0xFC,0x48,0x40,0x28,0x40,0x17,0xFC,0x28,0x40,0x46,0x40,0x81,0xFE,
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 工
;  宽×高（像素）: 15×
------------------------------------------------------------------------------*/
0x00,0x00,0x00,0x00,0x3F,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,
0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00,
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 作
;  宽×高（像素）: 15×16
------------------------------------------------------------------------------*/
0x08,0x80,0x0C,0x80,0x09,0x00,0x13,0xFE,0x12,0x80,0x34,0x88,0x50,0xFC,0x90,0x80,
0x10,0x80,0x10,0x84,0x10,0xFE,0x10,0x80,0x10,0x80,0x10,0x80,0x10,0x80,0x10,0x80,
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 室
;  宽×高（像素）: 15×16
------------------------------------------------------------------------------*/
0x00,0x00,0x02,0x00,0x01,0x00,0x3F,0xFE,0x20,0x04,0x40,0x08,0x3F,0xFC,0x04,0x00,
0x08,0x20,0x1F,0xF0,0x01,0x10,0x01,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0xFF,0xFE
}; 
/*******************************************************************************************/  
       
/*******************************************************************************************/ 
/*******************************************************************************************/ 
//数据串行输入  
void serial_input(uchar dat)     
{ 
uint m; 
for(m=0;m<8;m++) 
{
  if(dat&0x80)
  { DATA(0);}
  else
  {DATA(1);}
  
  SHCP(0);
  SHCP(1);
        NOP;
  NOP;
  SHCP(0);
  NOP;
  NOP;
  dat=dat<<1;
}
} 
void delay()
{
unsigned int i = 50000;
while(i--);
}
void delay_us(uint32_t i)
{
i*=14;
while(i--);
}
/*******************************************************************************************/ 
//并出
void serial_output() 
{

STCP(1);
delay_us(1);
STCP(0);
}
/*******************************************************************************************/ 
/*******************************************************************************************/ 
//行显控制 
void HC138_scan(uchar temp) 
{
   OE(1);
   A1(0x01 & temp);
   B1(0x01 &(temp>>1));
} 
/*******************************************************************************************/ 
/*******************************************************************************************/ 
//整屏显示 
void display() 
{
uint i,j,k;
unsigned char buf;
serial_output();
for(k=0;k<4;k++)              //显示的四行 
{

  for(j=0;j<2;j++)         //显示3、4列 
  {
   for(i=0;i<2;i++)      // 显示1、2列 
   {
    buf = PLAYBUF[24+(k<<1)+i+(j<<5)];
    serial_input(buf);
    buf = PLAYBUF[16+(k<<1)+i+(j<<5)];
    serial_input(buf);
    buf = PLAYBUF[8+(k<<1)+i+(j<<5)];
    serial_input(buf);
    buf = PLAYBUF[(k<<1)+i+(j<<5)];
    serial_input(buf);
   }
  }
  
  HC138_scan(k);
  serial_output();
}
}
void point(unsigned char x,unsigned char y,uint8_t c)
{
unsigned char i;
if(x>=32) return;
if(y>=16) return;
i=0x80;
i>>=x-((x>>3)<<3);
if(c)
  PLAYBUF[(y<<1)+((x>>4)<<5)+((x>>3)&0x01)] |= i;
else 
  PLAYBUF[(y<<1)+((x>>4)<<5)+((x>>3)&0x01)] &= ~i;
}
//画线
void line(u8  x1, u8  y1, u8  x2, u8  y2, uint8_t color)
{
u8  x, y, t;
if((x1 == x2) && (y1 == y2))
  point(x1, y1, color);
else if(fabs(y2 - y1) > fabs(x2 - x1))
{
  if(y1 > y2)
  {
   t = y1;
   y1 = y2;
   y2 = t;
   t = x1;
   x1 = x2;
   x2 = t;
  }
  for(y = y1; y <= y2; y ++)
  {
   x = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
   point(x, y, color);
  }
}
else
{
  if(x1 > x2)
  {
   t = y1;
   y1 = y2;
   y2 = t;
   t = x1;
   x1 = x2;
   x2 = t;
  }
  for(x = x1; x <= x2; x ++)
  {
   y = (x - x1) * (y2 - y1) / (x2 - x1) + y1;
   point(x, y, color);
  }
}
}
void rectangle(u8 x1,u8 y1,u8 x2,u8 y2, uint8_t color)
{
line(x1,y1,x2,y1, color);
line(x1,y1,x1,y2, color);
line(x1,y2,x2,y2, color);
line(x2,y1,x2,y2, color);
}
void filled_rectangle(u8 x1,u8 y1,u8 x2,u8 y2, uint8_t color)
{
u8 i;
for(i=y1;i<=y2;i++)
{
  line(x1,i,x2,i, color);
}
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void circle(u8 x0,u8 y0,u8 r, uint8_t color)
{
int a,b;
int di;
a=0;
b=r;
di=3-2*r;             //判断下个点位置的标志
while(a<=b)
{
  point(x0-b,y0-a, color);             //3           
  point(x0+b,y0-a, color);             //0           
  point(x0-a,y0+b, color);             //1       
  point(x0-b,y0-a, color);             //7           
  point(x0-a,y0-b, color);             //2             
  point(x0+b,y0+a, color);             //4               
  point(x0+a,y0-b, color);             //5
  point(x0+a,y0+b, color);             //6 
  point(x0-b,y0+a, color);             
  a++;
  /***使用Bresenham算法画圆**/     
  if(di<0)di +=4*a+6;   
  else
  {
   di+=10+4*(a-b);   
   b--;
  } 
  point(x0+a,y0+b, color);
}
}
void load_playbuff(unsigned char *p)
{
unsigned char i;
for(i=0;i<BUF_SIZE;i++)
{
  PLAYBUF[i]= *p++;
}
}
void clear_playbuff(unsigned char n)
{
unsigned char i;
for(i=0;i<BUF_SIZE;i++)
{
  PLAYBUF[i] = n;
}
}
unsigned char op;
/*******************************************************************************************/ 
/*******************************************************************************************/ 
void put_char(u8 x,u8 y,u8 c,u8 color)
{
  unsigned char tmp_char=0,i,j;
  for (i=0;i<16;i++)
  {
    tmp_char=ascii16x8[((c)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
      {
        point(x+j,y+i,color); // 字符颜色
      }
      else
      {
        point(x+j,y+i,!color); // 背景颜色
      }
    }
  }
}
void put_chinese(u8 x,u8 y,u8 *c,u8 color)
{
u8 i=0;
u8 j=0;
u8 tmp_char=0;
u8 tmp_char2=0;
  for (i=0;i<16;i++)
  {
    tmp_char=c[i*2];
tmp_char2=c[i*2+1];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
      {
        point(x+j,y+i,color); // 字符颜色
      }
      else
      {
        point(x+j,y+i,!color); // 背景颜色
      }
   
   if ( (tmp_char2 >> 7-j) & 0x01 == 0x01)
      {
        point(x+j+8,y+i,color); // 字符颜色
      }
      else
      {
        point(x+j+8,y+i,!color); // 背景颜色
      }
    }
  }
}
void display_move(unsigned int c)
{
unsigned char i;

for(i=0;i<16;i++)
{
  PLAYBUF[i*2] = (PLAYBUF[i*2]<<1)+(PLAYBUF[i*2+1]>>7);
  PLAYBUF[i*2+1] = (PLAYBUF[i*2+1]<<1)+(PLAYBUF[i*2+32]>>7);
  PLAYBUF[i*2+32] = (PLAYBUF[i*2+32]<<1)+(PLAYBUF[i*2+1+32]>>7);
  PLAYBUF[i*2+32+1] = (PLAYBUF[i*2+32+1]<<1)+((c>>i)&0x01); 
}
}
void delay_100(uint16_t temp)
{
	uint16_t di,dj;
	for(di=0;di<temp;di++)
	  for(dj=0;dj<7200;dj++);
}
/*******************************************************************************************/ 
/*******************************************************************************************/ 
void main() 
{ 
unsigned int i=1000;
GPIO_Config_P1();
min=0;
sec=0;
time=0;
rectangle(0,0,31,15,1);
circle(15,8,4,1);
while(i--)
{
  display();
}
//load_playbuff((uint8_t *)&table[0]);
put_char(0,0,tab[min/10%10],1);
put_char(8,0,tab[min%10],1);
point(15,6,1);
point(15,9,1);
point(16,6,1);
point(16,9,1);
put_char(16,0,tab[sec/10%10],1);
put_char(24,0,tab[sec%10],1);
while(1) 
{
  //  
  display();
    
  time++;
  if(time>=1000)
{
   // display_move(1);
  time=0;
  sec++;
  if(sec>=60)
  {
   sec=0;
   min++;
   if(min>=60)
   {
    min=0;
   }
   put_char(0,0,tab[min/10%10],1);
   put_char(8,0,tab[min%10],1);
  }
  put_char(16,0,tab[sec/10%10],1);
  put_char(24,0,tab[sec%10],1);
  point(15,5,1);
  point(15,10,1);
  point(16,5,1);
  point(16,10,1);
  point(15,6,1);
  point(15,9,1);
  point(16,6,1);
  point(16,9,1);
}
}
}