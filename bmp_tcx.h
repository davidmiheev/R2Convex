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
 int CheckTmp;
 //do not include pointers up to this line
        union
        {
	 unsigned int **v;
	 unsigned char (**c)[4];
        };
        unsigned int **g;
        unsigned int **bg;
        unsigned int *t,*t2;
 CBMP32x(){SetZero();}
 CBMP32x(int nx,int ny,unsigned char InitialValueR=255,unsigned char InitialValueG=255,unsigned char InitialValueB=255);//{SetZero();Width=nx;Height=ny;v=(unsigned int**)allocate_2D_Arrayx(Height,Width,4,1);}
 CBMP32x(const char *sf){SetZero();Load(sf);}
 CBMP32x(const CBMP32x &b){SetZero();CopyOnly(b);}
 ~CBMP32x(){Clean();}
 void SetZero(){v = 0;g = 0;t = 0;t2 = 0;bg = 0;}
 void Clean(){if(v != 0) { delete[] v; delete[] g; delete[] t; delete[] t2; delete[] bg; SetZero(); } }
 int Load(const char *sf,double k=1);
 int Save(const char *sf);
 void MakeGrayFromColor();
 void MakeColorFromGray();
 CBMP32x &operator=(const CBMP32x &b);
 void CopyOnly(const CBMP32x &b);
};
