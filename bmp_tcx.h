#pragma once
class CBMP32x
{public:
	unsigned short int Signature;
	unsigned int	   FileLength;
	unsigned int	   Zero;
	unsigned int	   Ptr;
	unsigned int 	   Version;
	 int 	   Width;
	 int 	   Height;
	unsigned short int Planes;
	unsigned short int BitsPerPixel;
	unsigned int 	   Compression;
	unsigned int 	   SizeImage;
	unsigned int 	   XPelsPerMeter;
	unsigned int      YPelsPerMeter;
	unsigned int      ClrUsed;
	unsigned int      ClrImportant;
	unsigned char pal[256][4];
        union
        {
	 unsigned int **v;
	 unsigned char (**c)[4];
        };
        unsigned int **g;
        unsigned int **bg;
 CBMP32x(){SetZero();}
 CBMP32x(int nx,int ny,unsigned char InitialValueR=255,unsigned char InitialValueG=255,unsigned char InitialValueB=255);
 CBMP32x(const char *sf){SetZero();Load(sf);}
 CBMP32x(const CBMP32x &b){SetZero();CopyOnly(b);}
 ~CBMP32x(){Clean();}
 void SetZero(){v = 0;g = 0;bg = 0;Width = 0;Height = 0;}
 void Clean(){
  if(v) delete[] (char*)v;
  if(g) delete[] (char*)g;
  if(bg) delete[] (char*)bg;
  SetZero();
 }
 int Load(const char *sf,double k=1);
 int Save(const char *sf);
 void MakeGrayFromColor();
 void MakeColorFromGray();
 CBMP32x &operator=(const CBMP32x &b);
 void CopyOnly(const CBMP32x &b);
};
