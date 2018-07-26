/*********************************************************************
  �ֳt�֧Q���ܴ�C���
  ���²���G����ƬO�q�Ϊ��ֳt���ظ��ܴ�C�y����ơA���өʱj�A�H�U��������
  ��w��C����ƪ����p�X�骺�Φ���ܤ@�Ӵ_�ơA��J���۵M���Ǫ��_
  �ơ]��J��ƬO�i�O�_�Ƶ곡��0�^�A��X���g�LFFT�ܴ����۵M���Ǫ�
  �_��
  �ϥλ����G�ϥΦ���ƥu�ݧ�炙�w�qFFT_N���ȧY�i��{�I�ƪ����ܡAFFT_N��
  ���Ӭ�2��N����A����������������b�᭱��0
  ��ƽեΡGFFT(s);
  �@    �̡G�N�Ӫ�
  ��    ���G2010-2-20
  ��    ���GVer1.0
  �ѦҤ��m�G    

 **********************************************************************/
//#include <reg52.h>    //AT89C52
//#include <iom128.h> //atmeg128
//#include <intrinsics.h>
#include <math.h>

#define  PI     3.1415926535897932384626433832795028841971               //�w�q��P�v��
#define  FFT_N  128                                                      //�w�q�֧Q���ܴ����I��

struct compx {float real,imag;};                                    //�w�q�@�Ӵ_�Ƶ��c
struct compx s[FFT_N];                                              //FFT��J�M��X�G�qS[1]�}�l�s��A�ھڤj�p�ۤv�w�q

/*******************************************************************
  ��ƭ쫬�Gstruct compx EE(struct compx b1,struct compx b2)  
  ��ƥ\��G���Ӵ_�ƶi�歼�k�B��
  ��J�ѼơG��ӥH�p�X��w�q���_��a,b
  ��X�ѼơGa�Mb�����n�A�H�p�X�骺�Φ���X
 *******************************************************************/
struct compx EE(struct compx a,struct compx b)      
{
	struct compx c;

	c.real=a.real*b.real-a.imag*b.imag;
	c.imag=a.real*b.imag+a.imag*b.real;
	
	return(c);
}
/*****************************************************************
  ��ƭ쫬�Gvoid FFT(struct compx *xin,int N)
  ��ƥ\��G���J���_�Ʋնi��ֳt���ظ��ܴ��]FFT�^
  ��J�ѼơG*xin�_�Ƶ��c��ժ����a�}���w�Astruct��
 *****************************************************************/
void FFT(struct compx *xin)
{
	int f , m, nv2, nm1, i, k, l, j = 0;
	struct compx u,w,t;

	nv2 = FFT_N / 2;                   //�ܧ}�B��A�Y��۵M�����ܦ��˦�ǡA���ιp�w��kADVERTISEMENT



 

	nm1 = FFT_N - 1;  
	for(i = 0; i < nm1; i++)        
	{
		if(i < j)                      //�p�Gi<j,�Y�i���ܧ}
		{
			t = xin[j];           
			xin[j] = xin[i];
			xin[i] = t;
		}
		k = nv2;                       //�Dj���U�@�ӭ˦��
		while( k <= j)                 //�p�Gk<=j,���j���̰��쬰1   
		{           
			j = j - k;                 //��̰����ܦ�0
			k = k / 2;                 //k/2�A���������A�̦������A�v�Ӥ���A����Y�Ӧ쬰0
		}
		j = j + k;                     //��0�אּ1
	}

	{
		int le , lei, ip;                            //FFT�B��֡A�ϥν��ιB�⧹��FFT�B��
		
		f = FFT_N;
		for(l = 1; (f=f/2)!=1; l++)                  //�p��l���ȡA�Y�p�⽺�ίż�
			;
		for( m = 1; m <= l; m++)                         // ����ε��ż�
		{                                        //m��ܲ�m�Ž��ΡAl�����ί��`��l=log�]2�^N
			le = 2 << (m - 1);                            //le���ε��Z���A�Y��m�Ž��Ϊ����ε��۶Zle�I
			lei = le / 2;                               //�P�@���ε����ѥ[�B�⪺���I���Z��
			u.real = 1.0;                             //u�����ε��B��t�ơA��l�Ȭ�1
			u.imag = 0.0;
			w.real = cos(PI / lei);                     //w���t�ưӡA�Y��e�t�ƻP�e�@�Өt�ƪ���
			w.imag = -sin(PI / lei);
			for(j = 0;j <= lei - 1; j++)                   //����p�⤣�P�ؽ��ε��A�Y�p��t�Ƥ��P�����ε�
			{
				for(i = j; i <= FFT_N - 1; i = i + le)            //����P�@���ε��B��A�Y�p��t�ƬۦP���ε�
				{
					ip = i + lei;                           //i�Aip���O��ܰѥ[���ιB�⪺��Ӹ`�I
					t = EE(xin[ip], u);                    //���ιB��A�Ԩ�����
					xin[ip].real = xin[i].real - t.real;
					xin[ip].imag = xin[i].imag - t.imag;
					xin[i].real = xin[i].real + t.real;
					xin[i].imag = xin[i].imag + t.imag;
				}
				u = EE(u, w);                           //���ܨt�ơA�i��U�@�ӽ��ιB��
			}
		}
	}
}
/************************************************************
  ��ƭ쫬�Gvoid main() 
  ��ƥ\��G����FFT�ܴ��A�t�ܨ�ƨϥΤ�k
  ��J�ѼơG�L
  ��X�ѼơG�L
 ************************************************************/
int main()   
{  
	int i;

	for(i = 0; i < FFT_N; i++)                                        //�����c����
	{
		s[i].real = 1 + 2*sin(2*3.141592653589793*i / FFT_N);         //�곡�������iFFT_N�I���ˡA��Ȭ�1
		s[i].imag = 0;                                                //�곡��0
	}

	FFT(s);                                                //�i��ֳt�֧Q���ܴ�

	for(i = 0; i < FFT_N; i++)                             //�D�ܴ��ᵲ�G���ҭȡA�s�J�_�ƪ��곡����
		s[i].real = sqrt(s[i].real*s[i].real + s[i].imag * s[i].imag);

	while(1);
}
