#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <vector>
#include "R2.h"
//------------------------------------
template <class T> T Min(T a,T b){return a >= b ? b : a;}
template <class T> T Max(T a,T b){return a >= b ? a : b;}
#define FOR(ixxx,iyyy) for(iyyy=0;iyyy<Height;iyyy++)for(ixxx=0;ixxx<Width;ixxx++)
//------------------------------------
inline char **allocate_2D_Arrayx(int m,int n, int size_of_element,int set_clear=0,void *NULL_VALUE=NULL)
{int i;char **p;size_t size;
 if(m <= 0 || n <= 0) return NULL;
 size = (size_t)(m+1)*sizeof(char*) + (size_t)(m+1)*n*size_of_element;
 try{
  p=(char**)new char[size];
 }catch(...)
 {fprintf(stderr,"Can't allocate memory: %uM (%zub)\n",(unsigned)(size/1000/1000),size); return NULL;}
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
 if(!v) throw std::bad_alloc();
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
{FILE *f=NULL;int err=0,StringSize;
 if(sizeof(int)!=4){fprintf(stderr,"Fatal error: sizeof(int)!=4\n");return -1;}
 f=fopen(sf,"rb");
 if(f==NULL){fprintf(stderr,"Can't open file '%s'\n",sf);return -1;}
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
 if(Width<=0||Height<=0||SizeImage<=0||(BitsPerPixel!=24&&BitsPerPixel!=32)){err=-1;goto le;}
 StringSize=SizeImage/Height;
 if(v){delete[] (char*)v; v=NULL;}
 if(g){delete[] (char*)g; g=NULL;}
 if(bg){delete[] (char*)bg; bg=NULL;}
 v=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);
 if(!v){err=-1;goto le;}
 //--
 fseek(f,Ptr,SEEK_SET);
 {std::vector<unsigned char> row(StringSize);
 for(int i=0;i<(int)Height;i++)
 {
  if(fread(&row[0],1,StringSize,f)!=(size_t)StringSize){fprintf(stderr,"Can't read pixel data\n");err=-2;goto le;}
  int bpp=BitsPerPixel/8;
  for(int j=0;j<(int)Width;j++)
  {
   c[i][j][0]=row[j*bpp+0];
   c[i][j][1]=row[j*bpp+1];
   c[i][j][2]=row[j*bpp+2];
   c[i][j][3]=0;
  }
 }
 }
 //--
 if(k!=1)
 {
  int w=(int)(Width*k),h=(int)(Height*k),i0,i1,j0,j1,i,j,ii,jj,kk; unsigned int **vv=NULL,s[4];
  vv=(unsigned int**)allocate_2D_Arrayx(h,w,4,1);
  if(!vv){err=-1;goto le;}
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
 }
 //--
 MakeGrayFromColor();
 le:;
 fclose(f);
 return err;
}

int CBMP32x::Save(const char *sf)
{FILE *f=NULL;int err=0,StringSize;
 if(sizeof(int)!=4){fprintf(stderr,"Fatal error: sizeof(int)!=4\n");return -1;}
 if(!v||Width<=0||Height<=0){fprintf(stderr,"Nothing to save\n");return -1;}
 SizeImage=(Width+3)/4*4*Height*3;
 FileLength=SizeImage+54;
 f=fopen(sf,"wb");
 if(f==NULL){fprintf(stderr,"Can't open file '%s'\n",sf);return -1;}
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
 //--
 fseek(f,Ptr,SEEK_SET);
 {std::vector<unsigned char> row(StringSize,0);
 for(int i=0;i<(int)Height;i++)
 {
  int bpp=BitsPerPixel/8;
  for(int j=0;j<(int)Width;j++)
  {
   row[j*bpp+0]=c[i][j][0];
   row[j*bpp+1]=c[i][j][1];
   row[j*bpp+2]=c[i][j][2];
  }
  if(fwrite(&row[0],1,StringSize,f)!=(size_t)StringSize){fprintf(stderr,"Can't write\n");err=-2;goto le;}
 }
 }
 //--
 le:;
 fclose(f);
 return err;
}

void CBMP32x::MakeGrayFromColor()
{int ix,iy;
 if(!v) return;
 if(g) delete[] (char*)g;
 g=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);
 if(!g) return;
 for(ix=0;ix<Width;ix++)for(iy=0;iy<Height;iy++)
  g[iy][ix]=c[iy][ix][1];
}

void CBMP32x::MakeColorFromGray()
{int ix,iy; unsigned int gmax=0;
 if(!v||!g) return;
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
 Signature=b.Signature;
 FileLength=b.FileLength;
 Zero=b.Zero;
 Ptr=b.Ptr;
 Version=b.Version;
 Width=b.Width;
 Height=b.Height;
 Planes=b.Planes;
 BitsPerPixel=b.BitsPerPixel;
 Compression=b.Compression;
 SizeImage=b.SizeImage;
 XPelsPerMeter=b.XPelsPerMeter;
 YPelsPerMeter=b.YPelsPerMeter;
 ClrUsed=b.ClrUsed;
 ClrImportant=b.ClrImportant;
 memcpy(pal,b.pal,sizeof(pal));
 if(b.v)
 {v=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);if(v)FOR(ix,iy)v[iy][ix]=b.v[iy][ix];}
 if(b.g)
 {g=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);if(g)FOR(ix,iy)g[iy][ix]=b.g[iy][ix];}
 if(b.bg)
 {bg=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);if(bg)FOR(ix,iy)bg[iy][ix]=b.bg[iy][ix];}
}
