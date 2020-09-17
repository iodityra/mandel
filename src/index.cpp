#include <iostream>
#include <math.h>
using namespace std;

class Complex {
public:
	double real_part;
	double complex_part;
	void squared(double *x, double *i) {
		*x = (real_part * real_part) - (complex_part * complex_part);
		*i = 2 * real_part * complex_part;
	}
	double magnitude() {
		return sqrt(real_part * real_part + complex_part * complex_part);
	}
};

class equation_vars {
public:
	int res_x 	= 1000;
	int res_y 	= 1000;

	double zoom 	= -1;
	double pos_x 	= 0.27632656411703;
	double pos_y 	= 0.0079368622318991;
};

int stays_bounded(double x, double i, int iter) {
	Complex c;

	c.real_part = x;
	c.complex_part = i;

	Complex z;
	z.real_part = 0;
	z.complex_part = 0;

	equation_vars a;
	//cout << iter;

	int count = 0;

	while (count < 1000) {
		z.squared(&x, &i);
		//cout << "x = " << x << " i = " << i << endl;
		z.real_part = x + c.real_part;
		z.complex_part = i + c.complex_part;
		count++;
		if (z.magnitude() >= 2)
		{

			return count;
		}
	}
	return 0;
}

void output (int iter) {
	equation_vars a;
	double d = iter;
	double zoom = (d - 10) / 10;
	cout << zoom << endl;
	a.zoom = zoom;
	//a.zoom = a.zoom + 0.5;
	//cout << a.zoom << endl;

	unsigned char pix[2000000];
	int iterations[a.res_x][a.res_y];

	for (int x = -a.res_x / 2; x <= a.res_x / 2; x++)
	{
		for (int i = -a.res_y / 2; i <= a.res_y / 2; i++)
		{
			double d = x;
			double dx = d / (a.res_x * pow(10, a.zoom)) + a.pos_x;
			d = i;
			double di = d / (a.res_y * pow(10, a.zoom)) + a.pos_y;
			int in = 0;
			//if (iterations[x][i] < iter) {
				in = stays_bounded(dx, di, iter);
			//}
			if (in > 0) {
				//cout << "x = " << dx << ", i = " << di << " Position = " << x + a.res_x / 2 + (i + a.res_y / 2) * a.res_x << "in = " << (in%16)*16 << endl;
				pix[x + a.res_x / 2 + (i + a.res_y / 2) * a.res_x] = ((in%16)*16);
				iterations[x][i] = in;
			}
			else {
				//cout << "x = " << dx << ", i = " << di << " Position = " << x + a.res_x / 2 + (i + a.res_y / 2) * a.res_x << "in = " << (in%16)*16 << endl;
				pix[x + a.res_x / 2 + (i + a.res_y / 2) * a.res_x] = ((in%16)*16);

			}
		}
	}

	char buf[0x100];
	snprintf(buf, sizeof(buf), "../assets/set%d.pgm", iter);
	FILE *output = fopen(buf,"wb");;

	if(output==NULL){
		perror("ERROR: Cannot open output file");
		exit(EXIT_FAILURE);
	}


	fprintf(output,"P5\n");
	fprintf(output,"%d %d\n",a.res_x,a.res_y);
	fprintf(output,"255\n");


	fwrite(pix,1,16000000,output);


	fclose(output);
}


auto main() -> int {
	equation_vars a;
	//output(100);
	for (int x = 0; x < 100; x++) {
		cout << x << ": ";
		output(x);
	}
	return 0;
}
