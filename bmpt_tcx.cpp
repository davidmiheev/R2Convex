#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "R2.h"
//------------------------------------

 //*/
//------------------------------------
template <class T> T Min(T a,T b){return a >= b ? b : a;}
template <class T> T Max(T a,T b){return a >= b ? a : b;}
#define FOR(ixxx,iyyy) for(iyyy=0;iyyy<Height;iyyy++)for(ixxx=0;ixxx<Width;ixxx++)
//------------------------------------
template <class Type> class AutoFree
{
 public:
  AutoFree(): ptr(NULL) {};
  AutoFree(Type *p): ptr(p) {};
  ~AutoFree()
   {
    if(ptr)//&&*ptr)
     {
      delete [] ptr;
      ptr=NULL;
     };
   };
  void own(Type *p)
   {
    ptr=p;
   };
 private:
  Type *ptr;
};
//------------------------------------
template <class Type> class AAutoFree
{
 public:
  AAutoFree():ptr(NULL){};
  AAutoFree(Type *&p){ptr=&p;};
  ~AAutoFree()
   {
    if(ptr&&*ptr)
     {
      delete [] (*ptr);
      *ptr=NULL;
      ptr=NULL;
     };
   };
  void clean(){if(ptr){delete [] (*ptr); *ptr=NULL;}}
  void own(Type *&p)
   {
    ptr=&p;
   };
 private:
  Type **ptr;
};
//------------------------------------


inline char **allocate_2D_Arrayx(int m,int n, int size_of_element,int set_clear=0,void *NULL_VALUE=NULL)
{int i;char **p;int size;//size_of_element/=sizeof(int);
 if((m+1)*n*size_of_element>1000000)
 {
  try{
  p=(char**)new char[size=(m+1)*sizeof(char*)+(m+1)*n*size_of_element];
  }catch(...)
  {char s[256]; sprintf(s,"(1221823)Can't allocate memory: %dM (%db)",size/1000/1000,size); return NULL;}
 }
 else
 p=(char**)new char[size=(m+1)*sizeof(char*)+(m+1)*n*size_of_element];
 //p=(char**)((((int)p)+31)/32*32);
 if(set_clear)memset(p,0,size);
	p[0]=((char*)p)+(m+1)*sizeof(char*);
	for(int i=1;i<m;i++)
	{
		p[i]=p[i-1]+n*size_of_element;
	}
 if(NULL_VALUE)
  for(i=0;i<m*n;i++)
   memcpy(p[0]+i*size_of_element,NULL_VALUE,size_of_element);
return p;
}



CBMP32x::CBMP32x(int nx,int ny,unsigned char InitialValueR,unsigned char InitialValueG,unsigned char InitialValueB)
{int InitialValue=(InitialValueR<<16)|(InitialValueG<<8)|(InitialValueB);
 SetZero();Width=nx;Height=ny;v=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1,&InitialValue);
     //---
     Signature=0x4d42;
     Zero=0;
     Ptr=54;
     Version=0x28;
     Planes=1;
     BitsPerPixel=24;
     Compression=0;
     SizeImage=(Width+3)/4*4*Height*3;
     XPelsPerMeter=2834;
     YPelsPerMeter=2834;
     ClrUsed=0;
     ClrImportant=0;
     FileLength=SizeImage+54;
     //---
}

int CBMP32x::Load(const char *sf,double k)
{FILE *f=NULL;int err=0,StringSize; //union UIC{int i;unsigned char c[4];}x;
 if(sizeof(int)!=4){printf("Fatal error (59431024)\n");exit(-1);}
 delete[] v; v=NULL;
 f=fopen(sf,"rb");
 if(f==NULL){char s[256]; sprintf(s,"Can't open file '%s'\n",sf); err=-1;return -1;}
 //--
#define RD(v) {if(fread(&v,sizeof(v),1,f)!=1){err=-1;goto le;}}
	RD(Signature);
	RD(FileLength);
	RD(Zero);
	RD(Ptr);
	RD(Version);
	RD(Width);
	RD(Height);
	RD(Planes);
	RD(BitsPerPixel);
	RD(Compression);
	RD(SizeImage);
	RD(XPelsPerMeter);
	RD(YPelsPerMeter);
	RD(ClrUsed);
	RD(ClrImportant);
 StringSize=SizeImage/Height;
 if(0)if(k==1)printf("==================Ptr=%d SizeImage=%d WidthxHeight=%dx%d %d %d\n",Ptr,SizeImage,Width,Height, BitsPerPixel,StringSize);
 v=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);
 //--
 fseek(f,Ptr,SEEK_SET);
 for(int i=0;i<(int)Height;i++)
 {unsigned char *s=new unsigned char[StringSize]; AutoFree <unsigned char> AF_s(s);
  if((int)fread(s,1,StringSize,f)!=(int)StringSize){printf("(6253)Can't read\n");err=-2;goto le;}
  if(BitsPerPixel==24)
  {
   for(int j=0;j<(int)Width;j++)
   {
    c[i][j][0]=s[j*3+0];
    c[i][j][1]=s[j*3+1];
    c[i][j][2]=s[j*3+2];
    c[i][j][3]=0;
   }
  }
  else if(BitsPerPixel==32)
  {
   for(int j=0;j<(int)Width;j++)
   {
    c[i][j][0]=s[j*4+0];
    c[i][j][1]=s[j*4+1];
    c[i][j][2]=s[j*4+2];
    c[i][j][3]=0;
   }
  }
  else
  {printf("Not supported bpp (123341044)\n");exit(-1);}
 }
 //--
 if(k!=1)
 {
  int w=(int)(Width*k),h=(int)(Height*k),i0,i1,j0,j1,i,j,ii,jj,kk; unsigned int **vv=NULL,s[4];
  vv=(unsigned int**)allocate_2D_Arrayx(h,w,4,1);
  for(i=0;i<h;i++)
  {
   i0=(int)(i/k);i1=(int)((i+1)/k); i1=Min(i1,(int)Height);
   for(j=0;j<w;j++)
   {
    j0=(int)(j/k);j1=(int)((j+1)/k); j1=Min(j1,(int)Width);
    for(s[0]=s[1]=s[2]=s[3]=0,ii=i0;ii<i1;ii++)for(jj=j0;jj<j1;jj++)for(kk=0;kk<4;kk++)s[kk]+=((unsigned char*)(&(v[ii][jj])))[kk];
    for(kk=0;kk<4;kk++)s[kk]/=(Max(1,(j1-j0)*(i1-i0)));
    for(kk=0;kk<4;kk++)((unsigned char*)(&(vv[i][j])))[kk]=s[kk];
   }
  }
  delete[] (char*)v; v=NULL;
  v=vv; Width=w; Height=h;
if(0)printf("==================Ptr=%d SizeImage=%d WidthxHeight=%dx%d %d %d\n",Ptr,SizeImage,Width,Height, BitsPerPixel,StringSize);
 }
 //--
 le:;if(err)return err;
 fclose(f);
 MakeGrayFromColor();
 return err;
}

int CBMP32x::Save(const char *sf)
{FILE *f=NULL;int err=0,StringSize; //union UIC{int i;unsigned char c[4];}x;
 if(sizeof(int)!=4){printf("Fatal error (11131024)\n");exit(-1);}
 //delete[] v; v=NULL;
 f=fopen(sf,"wb");
 if(f==NULL){char s[256]; sprintf(s,"Can't open file '%s'\n",sf); err=-1;return -1;}
 //--
#define WR(v) {if(fwrite(&v,sizeof(v),1,f)!=1){err=-1;goto le;}}
	WR(Signature);
	WR(FileLength);
	WR(Zero);
	WR(Ptr);
	WR(Version);
	WR(Width);
	WR(Height);
	WR(Planes);
	WR(BitsPerPixel);
	WR(Compression);
	WR(SizeImage);
	WR(XPelsPerMeter);
	WR(YPelsPerMeter);
	WR(ClrUsed);
	WR(ClrImportant);
 StringSize=SizeImage/Height;
 //if(0)if(k==1)printf("==================Ptr=%d SizeImage=%d WidthxHeight=%dx%d %d %d\n",Ptr,SizeImage,Width,Height, BitsPerPixel,StringSize);
// v=(unsigned int**)allocate_2D_Array(Height,Width,4,1);
 //--
 fseek(f,Ptr,SEEK_SET);
 for(int i=0;i<(int)Height;i++)
 {unsigned char *s=new unsigned char[StringSize]; AutoFree <unsigned char> AF_s(s);memset(s,0,StringSize);
  if(BitsPerPixel==24)
  {
   for(int j=0;j<(int)Width;j++)
   {
    s[j*3+0]=c[i][j][0];
    s[j*3+1]=c[i][j][1];
    s[j*3+2]=c[i][j][2];
   }
  }
  else if(BitsPerPixel==32)
  {
   for(int j=0;j<(int)Width;j++)
   {
    s[j*4+0]=c[i][j][0];
    s[j*4+1]=c[i][j][1];
    s[j*4+2]=c[i][j][2];
   }
  }
  else
  {printf("Not supported bpp (123341044)\n");exit(-1);}
  if((int)fwrite(s,1,StringSize,f)!=(int)StringSize){printf("(6254)Can't write\n");err=-2;goto le;}
 }
 //--
 le:;if(err)return err;
 fclose(f);
 return err;
}

void CBMP32x::MakeGrayFromColor()
{int ix,iy;
 delete[] g;
 g=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);
 for(ix=0;ix<Width;ix++)for(iy=0;iy<Height;iy++)
  g[iy][ix]=c[iy][ix][0]*0+c[iy][ix][1]+c[iy][ix][2]*0;
}

void CBMP32x::MakeColorFromGray()
{int ix,iy; unsigned int gmax=0;
 for(ix=0;ix<Width;ix++)for(iy=0;iy<Height;iy++)gmax=Max(gmax,g[iy][ix]);
 if(gmax>255)
 for(ix=0;ix<Width;ix++)for(iy=0;iy<Height;iy++)
 {c[iy][ix][0]=c[iy][ix][1]=c[iy][ix][2]=0;c[iy][ix][1]=(unsigned char)((int)(0.4999+g[iy][ix]*255./gmax));}
 else
 for(ix=0;ix<Width;ix++)for(iy=0;iy<Height;iy++)
 {c[iy][ix][0]=c[iy][ix][1]=c[iy][ix][2]=0;c[iy][ix][1]=(unsigned char)g[iy][ix];}
}

CBMP32x &CBMP32x::operator=(const CBMP32x &b)
{
 if(this!=&b)
 {
  Clean();
  CopyOnly(b);
 }
 return *this;
}

void CBMP32x::CopyOnly(const CBMP32x &b)
{int ix,iy;
 memcpy(this,&b,((char*)&CheckTmp)-((char*)this));
 if(b.v)
 {v=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);FOR(ix,iy)v[iy][ix]=b.v[iy][ix];}
 if(b.g)
 {g=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);FOR(ix,iy)g[iy][ix]=b.g[iy][ix];}
 if(b.bg)
 {bg=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);FOR(ix,iy)bg[iy][ix]=b.bg[iy][ix];}
}

