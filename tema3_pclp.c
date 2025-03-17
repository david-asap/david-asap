#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef struct {
int R, G, B;
}pixel;

pixel** aloca_mem_ppm(int lungime, int inaltime) {
pixel** aux = (pixel **)malloc(inaltime * sizeof(pixel *));
if(!aux) {
	//printf("Eroare la alocare\n");
	return NULL;
}
for(int i = 0; i < inaltime; i++) {
	aux[i] = (pixel *)malloc(lungime * sizeof(pixel));
	if(!aux[i]) {
		for(int k = 0; k < i; k++)
			free(aux[k]);
		free(aux);
		//printf("Eroare la alocare\n");
		return NULL;
	}
}
return aux;
}

int** aloca_mem_pgm(int lungime, int inaltime) {
int** aux = (int **)malloc(inaltime * sizeof(int *));
if(!aux) {
	//printf("Eroare la alocare\n");
	return NULL;
}
for(int i = 0; i < inaltime; i++) {
	aux[i] = (int *)malloc(lungime * sizeof(int));
	if(!aux[i]) {
		for(int k = 0; k < i; k++)
			free(aux[k]);
		free(aux);
		//printf("Eroare la alocare\n");
		return NULL;
	}
}
return aux;
}

int** load_pgm_txt(int lungime, int inaltime, FILE* in) {
int** b = aloca_mem_pgm(lungime, inaltime);
for(int i = 0; i < inaltime; i++)
	for(int j = 0; j < lungime; j++)
		fscanf(in, "%d", &b[i][j]);
return b;
}

int** load_pgm_bin(int lungime, int inaltime, FILE* in, char *p) {
int** b = aloca_mem_pgm(lungime, inaltime);
int dis = ftell(in);
fclose(in);
in = fopen(p, "rb");
fseek(in, dis + 1, SEEK_SET);
unsigned char c;
for(int i = 0; i < inaltime; i++)
	for(int j = 0; j < lungime; j++) {
		fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
		b[i][j] = (int)c;
	}
return b;
}

pixel** load_ppm_txt(int lungime, int inaltime, FILE* in) {
pixel** a = aloca_mem_ppm(lungime, inaltime);
for(int i = 0; i < inaltime; i++)
	for(int j = 0; j < lungime; j++)
		fscanf(in, "%d%d%d", &a[i][j].R, &a[i][j].G, &a[i][j].B);
return a;
}

pixel** load_ppm_bin(int lungime, int inaltime, FILE* in, char *p) {
pixel** a = aloca_mem_ppm(lungime, inaltime);
int dis = ftell(in);
fclose(in);
in = fopen(p, "rb");
fseek(in, dis + 1, SEEK_SET);
unsigned char c;
for(int i = 0; i < inaltime; i++)
	for(int j = 0; j < lungime; j++) {
		fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
		a[i][j].R = (int)c;
		fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
		a[i][j].G = (int)c;
		fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
		a[i][j].B = (int)c;
	}
return a;
}

void swap_int(int v[4], int coord1, int coord2) {
int aux = v[coord1];
v[coord1] = v[coord2];
v[coord2] = aux;
}

int atribuiri_select(char *p, int v[4], int w, int h) {
int nr = 0;
while(p) {
	for(int i = 0; i < (int)strlen(p); i++)
		if(isalpha(p[i]))
			return 0;
	v[nr++] = atoi(p);
	p = strtok(NULL, " ");
}
if(nr != 4)
	return 0;
if(v[0] > v[2])
	swap_int(v, 0, 2);
if(v[1] > v[3])
	swap_int(v, 1, 3);
if(v[0] < 0 || v[1] < 0 || v[2] > w || v[3] > h || v[1] == v[3] || v[0] == v[2])
	return 2;
return 1;
}

void save_txt_ppm(char tip[], pixel** a, int w, int h, int inte, char d[]) {
FILE* out = fopen(d, "wt");
if(!out) {
	printf("Failed to load\n");
	return;
}
strcpy(tip, "P3");
fprintf(out, "%s\n", tip);
fprintf(out, "%d %d\n", w, h);
fprintf(out, "%d\n", inte);
for(int i = 0; i < h; i++) {
	for(int j = 0; j < w; j++)
		fprintf(out, "%d %d %d ", a[i][j].R, a[i][j].G, a[i][j].B);
	fprintf(out, "\n");
}
fclose(out);
}

void save_txt_pgm(char tip[], int** b, int w, int h, int inte, char d[]) {
FILE* out = fopen(d, "wt");
if(!out) {
	printf("Failed to load\n");
	return;
}
strcpy(tip, "P2");
fprintf(out, "%s\n", tip);
fprintf(out, "%d %d\n", w, h);
fprintf(out, "%d\n", inte);
for(int i = 0; i < h; i++) {
	for(int j = 0; j < w; j++)
		fprintf(out, "%d ", b[i][j]);
	fprintf(out, "\n");
}
fclose(out);
}

void save_bin_ppm(char tip[], pixel** a, int w, int h, int inte, char d[]) {
FILE* out = fopen(d, "wb");
if(!out) {
	printf("Failed to load\n");
	return;
}
strcpy(tip, "P6");
fprintf(out, "%s\n", tip);
fprintf(out, "%d %d\n", w, h);
fprintf(out, "%d\n", inte);
for(int i = 0; i < h; i++)
	for(int j = 0; j < w; j++) {
		fwrite(&a[i][j].R, sizeof(char), 1, out);
		fwrite(&a[i][j].G, sizeof(char), 1, out);
		fwrite(&a[i][j].B, sizeof(char), 1, out);
	}
fclose(out);
}

void save_bin_pgm(char tip[], int** b, int w, int h, int inte, char d[]) {
FILE* out = fopen(d, "wb");
if(!out) {
	printf("Failed to load\n");
	return;
}
strcpy(tip, "P5");
fprintf(out, "%s\n", tip);
fprintf(out, "%d %d\n", w, h);
fprintf(out, "%d\n", inte);
for(int i = 0; i < h; i++)
	for(int j = 0; j < w; j++)
		fwrite(&b[i][j], sizeof(char), 1, out);
fclose(out);
}

pixel** copy_image_ppm(pixel** a, int w_start, int h_start, int w_end, int h_end) {
pixel** cpy = aloca_mem_ppm(w_end - w_start, h_end - h_start);
if(cpy != NULL)
	for(int i = h_start; i < h_end; i++)
		for(int j = w_start; j < w_end; j++) {
			cpy[i - h_start][j - w_start].R = a[i][j].R;
			cpy[i - h_start][j - w_start].G = a[i][j].G;
			cpy[i - h_start][j - w_start].B = a[i][j].B;
		}
return cpy;
}

int** copy_image_pgm(int** b, int w_start, int h_start, int w_end, int h_end) {
int** cpy = aloca_mem_pgm(w_end - w_start, h_end - h_start);
if(cpy != NULL)
for(int i = h_start; i < h_end; i++)
	for(int j = w_start; j < w_end; j++) {
		cpy[i - h_start][j - w_start] = b[i][j];
	}
return cpy;
}

void histogram(char *p, int** b, int lungime, int inaltime, char tip_img[]) {
p = strtok(NULL, " ");
int coord[11], nr = 0;
while(p) {
	for(int i = 0; i < (int)strlen(p); i++)
		if(isalpha(p[i])) {
			printf("Invalid set of paramteres\n");
			return;
		}
	coord[nr++] = atoi(p);
	p = strtok(NULL, " ");
}
if(nr != 2)
	printf("Invalid command\n");
else 
	if(strcmp(tip_img, "P2") == 0 || strcmp(tip_img, "P5") == 0) {
		int x = coord[0], y = coord[1];
		int *bins = calloc(y, sizeof(int));
		int bin_size = 256 / y;
		for(int i = 0; i < inaltime; i++)
			for(int j = 0; j < lungime; j++) {
				unsigned char pix_val = b[i][j];
				int bin_index = pix_val / bin_size;
				if(bin_size >= y)
					bin_index = y - 1;
				bins[bin_index]++;
		}
		int max_cnt = 0;
		for(int i = 0; i < y; i++)
			if(bins[i] > max_cnt)
				max_cnt = bins[i];
		for(int i = 0; i < y; i++) {
			int star_count = (bins[i] * x) / max_cnt;
			printf("%d\t|\t", star_count);
			for(int k = 0; k < star_count; k++)
				printf("*");
			printf("\n");
		}
		free(bins);
	} else 
		printf("Black and white image needed\n");
}

void free_memory(void** a, int inaltime) {
if(a == NULL)
	return;
for(int i = 0; i < inaltime; i++)
	if(a[i] != NULL) {
		free(a[i]);
		a[i] = NULL;
	}
free(a);
}

int clamp(double x, int min_val, int max_val) {
if(x < min_val)
	return min_val;
if(x > max_val)
	return max_val;
return (unsigned int)x;
}

void edge(int mat[3][3]) {
for(int i = 0; i < 3; i++) {
	mat[0][i] = -1;
	mat[i][0] = -1;
	mat[2][i] = -1;
	mat[i][2] = -1;
}
mat[1][1] = 8;
}

void sharpen(int mat[3][3]) {
for(int i = 0; i < 3; i++)
	for(int j = 0; j < 3; j++)
		mat[i][j] = 0;
mat[0][1] = -1;
mat[2][1] = -1;
mat[1][0] = -1;
mat[1][2] = -1;
mat[1][1] = 5;
}

void blur(int mat[3][3]) {
for(int i = 0; i < 3; i++)
	for(int j = 0; j < 3; j++)
		mat[i][j] = 1;
}

void gauss_blur(int mat[3][3]) {
for(int i = 0; i < 3; i++)
	for(int j = 0; j < 3; j++)
		mat[i][j] = 1;
mat[0][1] = 2;
mat[2][1] = 2;
mat[1][0] = 2;
mat[1][2] = 2;
mat[1][1] = 4;
}

void apply_met(pixel** a, int mat[3][3], int xi, int xf, int yi, int yf, int num) {
//verify if it touches the edges
if (xi == 0)
	xi = xi + 1;
if(yi == 0)
	yi = yi + 1;
pixel** copie = aloca_mem_ppm(xf - xi, yf - yi);
for(int i = yi; i < yf; i++)
	for(int j = xi; j < xf; j++) {
		double sR = 0, sG = 0, sB = 0;
		for(int k = i - 1; k <= i + 1; k++) {
			for(int l = j - 1; l <= j + 1; l++) {
				sR += ((double)a[k][l].R * (double)mat[k - i + 1][l - j + 1]);
				sG += ((double)a[k][l].G * (double)mat[k - i + 1][l - j + 1]);
				sB += ((double)a[k][l].B * (double)mat[k - i + 1][l - j + 1]);
			}
		}
		sR /= num, sG /= num, sB /= num;
		sR = clamp(sR, 0, 255);
		sG = clamp(sG, 0, 255);
		sB = clamp(sB, 0, 255);
		copie[i - yi][j - xi].R = (unsigned int)sR;
		copie[i - yi][j - xi].G = (unsigned int)sG;
		copie[i - yi][j - xi].B = (unsigned int)sB;
	}
//copy intial matrix
for(int i = yi; i < yf; i++)
	for(int j = xi; j < xf; j++) {
		a[i][j].R = copie[i - yi][j - xi].R;
		a[i][j].G = copie[i - yi][j - xi].G;
		a[i][j].B = copie[i - yi][j - xi].B;
	}
free_memory((void **)copie, yf - yi);
copie = NULL;
}

int** rotate_pgm(int** b, int xi, int yi, int xf, int yf, int new_w, int new_h, int angle) {
	int** new_image = aloca_mem_pgm(new_w, new_h);
	if (angle == 90 || angle == -270) {
		for (int i = 0; i < new_w; i++)
			for(int j = 0; j < new_h; j++)
				new_image[j][new_w - i - 1] = b[yi + i][xi + j];
	} else {
		if(angle == 180 || angle == -180) {
			for(int i = 0; i < new_h; i++)
				for(int j = 0; j < new_w; j++)
					new_image[i][j] = b[yf - 1 - i][xf - 1 - j];
		} else {
			if(angle == 270 || angle == -90) {
				for (int i = 0; i < new_w; i++)
					for(int j = 0; j < new_h; j++)
						new_image[new_h - j - 1][i] = b[yi + i][xi + j];
			}
		}
	}
	return new_image;
}

pixel** rotate_ppm(pixel** a, int xi, int yi, int xf, int yf, int new_w, int new_h, int angle) {
	pixel** new_image = aloca_mem_ppm(new_w, new_h);
	if (angle == 90 || angle == -270) {
		for (int i = 0; i < new_w; i++)
			for(int j = 0; j < new_h; j++) {
				new_image[j][new_w - i - 1].R = a[yi + i][xi + j].R;
				new_image[j][new_w - i - 1].G = a[yi + i][xi + j].G;
				new_image[j][new_w - i - 1].B = a[yi + i][xi + j].B;
			}
	} else {
		if(angle == 180 || angle == -180) {
			for(int i = 0; i < new_h; i++)
				for(int j = 0; j < new_w; j++) {
					new_image[i][j].R = a[yf - 1 - i][xf - 1 - j].R;
					new_image[i][j].G = a[yf - 1 - i][xf - 1 - j].G;
					new_image[i][j].B = a[yf - 1 - i][xf - 1 - j].B;
				}
		} else {
			if(angle == 270 || angle == -90) {
				for (int i = 0; i < new_w; i++)
					for(int j = 0; j < new_h; j++) {
						new_image[new_h - j - 1][i].R = a[yi + i][xi + j].R;
						new_image[new_h - j - 1][i].G = a[yi + i][xi + j].G;
						new_image[new_h - j - 1][i].B = a[yi + i][xi + j].B;
					}
			}
		}
	}
	return new_image;
}

int** rotate_all_pgm(int** b, int** new_image, int *xf, int *yf, int new_h, int new_w, int angle) {
	free_memory((void**)b, *yf);
	int** new_b = aloca_mem_pgm(new_w, new_h);
	for(int i = 0 ; i < new_h; i++)
		for(int j = 0; j < new_w; j++)
			new_b[i][j] = new_image[i][j];
	*xf = new_w;
	*yf = new_h;
	printf("Rotated %d\n", angle);
	return new_b;
}

pixel** rotate_all_ppm(pixel** a, pixel** new_image, int *xf, int *yf, int new_h, int new_w, int angle) {
	free_memory((void**)a, *yf);
	pixel** new_a = aloca_mem_ppm(new_w, new_h);
	for(int i = 0 ; i < new_h; i++)
		for(int j = 0; j < new_w; j++) {
			new_a[i][j].R = new_image[i][j].R;
			new_a[i][j].G = new_image[i][j].G;
			new_a[i][j].B = new_image[i][j].B;
		}
	*xf = new_w;
	*yf = new_h;
	printf("Rotated %d\n", angle);
	return new_a;
}

int verify_angle(int angle) {
	int v[9] = {0, 90, 180, 270, 360, -90, -180, -270, -360};
	for(int i = 0; i <= 8; i++)
		if(v[i] == angle)
			return 1;
	return 0;
}

//functie pentru copierea submatricei rotite in imaginea originala
void verify_square_pgm(int** b, int** new_image, int xi, int yi, int xf, int yf, int angle) {
	if((xf - xi) == (yf - yi)) {
		for(int i = yi; i < yf; i++)
			for(int j = xi; j < xf; j++)
				b[i][j] = new_image[i - yi][j - xi];
		printf("Rotated %d\n", angle);
	} else printf("The selection must be square\n");
}

//functie pentru copierea submatricei rotite in imaginea originala
void verify_square_ppm(pixel** a, pixel** new_image, int xi, int yi, int xf, int yf, int angle) {
	if((xf - xi) == (yf - yi)) {
		for(int i = yi; i < yf; i++)
			for(int j = xi; j < xf; j++) {
				a[i][j].R = new_image[i - yi][j - xi].R;
				a[i][j].G = new_image[i - yi][j - xi].G;
				a[i][j].B = new_image[i - yi][j - xi].B;
			}
		printf("Rotated %d\n", angle);
	} else printf("The selection must be square\n");
}

void equalize(int** b , int lungime, int inaltime) {
int *v_ap = calloc(256, sizeof(int));
for(int i = 0; i < inaltime; i++)
	for(int j = 0; j < lungime; j++)
		v_ap[b[i][j]]++;
for(int i = 0; i < inaltime; i++)
	for(int j = 0; j < lungime; j++) {
		double sum = 0;
		for(int k = 0; k <= b[i][j]; k++)
			sum += (double)v_ap[k];
		double value = 255.0 * ((double)sum / (double)(inaltime * lungime));
		value = clamp(value, 0 , 255);
		b[i][j] = (unsigned int)round(value);
	}
free(v_ap);
}

int main(void) {
pixel** a = NULL;
int** b = NULL;
int lungime, inaltime, intensitate, load = 0;
char linie[51], tip_imagine[4];
int xi = 0, xf, yi = 0,yf;
fgets(linie, 50, stdin);
linie[strcspn(linie, "\n")] = '\0';
char *p = strtok(linie, " ");
while(strcmp(p, "EXIT") != 0) {
	if (strcmp (p, "LOAD") == 0) {
		p = strtok(NULL, " ");
		FILE* in = fopen(p, "rt");
		if(in && p != NULL) {
			if (a != NULL) {
				free_memory((void **)a, inaltime);
				a = NULL;
			}
			if (b != NULL) {
				free_memory((void **)b, inaltime);
				b = NULL;
			}
			fscanf(in, "%s", tip_imagine);
			fscanf(in, "%d%d", &lungime, &inaltime);
			xf = lungime, yf = inaltime, xi = 0, yi = 0;
			fscanf(in, "%d", &intensitate);
			//printf("%s\n%d %d\n%d\n", tip_imagine, lungime, inaltime, intensitate);
			switch (tip_imagine[1])
			{
			case '2':
			b = load_pgm_txt(lungime, inaltime, in);
				break;
			case '3':
			a = load_ppm_txt(lungime, inaltime, in);
				break;
			case '5':
			b = load_pgm_bin(lungime, inaltime, in, p);
				break;
			case '6':
			a = load_ppm_bin(lungime, inaltime, in, p);
				break;
			}
			load = 1;
			printf("Loaded %s\n", p);
			fclose(in);
		} else {
			printf("Failed to load %s\n", p);
			load = 0;
		}
	} else
	if (strcmp (p, "SELECT") == 0) {
		if(load == 0)
			printf("No image loaded\n");
		else{
			p = strtok(NULL, " ");
			if(strcmp(p, "ALL") == 0) {
				xi = 0, yi = 0, xf = lungime, yf = inaltime;
				printf("Selected ALL\n");
			} else {
				int v[4];
				int verif = atribuiri_select(p, v, lungime, inaltime);
				//printf("verif = %d\n", verif);
				if(verif == 1) {
					xi = v[0], xf = v[2], yi = v[1], yf = v[3];
					printf("Selected %d %d %d %d\n", xi, yi, xf, yf);
				} else 
					if(verif == 2)
						printf("Invalid set of coordinates\n");
					else printf("Invalid command\n");
			}
		}
	} else
	if (strcmp (p, "SAVE") == 0) {
		if(load == 0)
			printf("No image loaded\n");
		else{
			char dest[21];
			p = strtok(NULL, " ");
			strcpy(dest, p);
			p = strtok(NULL, " ");
			if(p != NULL) {
				if(strcmp(tip_imagine, "P3") == 0 || strcmp(tip_imagine, "P6") == 0)
					save_txt_ppm(tip_imagine, a, lungime, inaltime, intensitate, dest);
				else
					save_txt_pgm(tip_imagine, b, lungime, inaltime, intensitate, dest);
			} else {
				if(strcmp(tip_imagine, "P3") == 0 || strcmp(tip_imagine, "P6") == 0)
					save_bin_ppm(tip_imagine, a, lungime, inaltime, intensitate, dest);
				else
					save_bin_pgm(tip_imagine, b, lungime, inaltime, intensitate, dest);
			}
			printf("Saved %s\n", dest);
		}
	} else
	if (strcmp (p, "CROP") == 0) {
		if(load == 0)
			printf("No image loaded\n");
		else {
			if(strcmp(tip_imagine, "P3") == 0 || strcmp(tip_imagine, "P6") == 0) {
				pixel** cpy = copy_image_ppm(a, xi, yi, xf, yf);
				free_memory((void **)a, inaltime);
				a = NULL;
				xf = xf - xi, yf = yf - yi, xi = 0, yi = 0;
				inaltime = yf, lungime = xf;
				a = cpy;
			}
			else {
				int** cpy = copy_image_pgm(b, xi, yi, xf, yf);
				free_memory((void **)b, inaltime);
				b = NULL;
				xf = xf - xi, yf = yf - yi, xi = 0, yi = 0;
				inaltime = yf, lungime = xf;
				b = cpy;
			}
			printf("Image cropped\n");
		}
	} else
	if (strcmp (p, "HISTOGRAM") == 0) {
		if (load == 0)
			printf("No image loaded\n");
		else
			histogram(p, b, lungime, inaltime, tip_imagine);
	} else
	if (strcmp (p, "APPLY") == 0) {
		if (load == 0)
			printf("No image loaded\n");
		else {
			p = strtok(NULL, " ");
			if (p) {
				if (strcmp(tip_imagine, "P2") == 0 || strcmp(tip_imagine, "P5") == 0)
					printf("Easy, Charlie Chaplin\n");
				else {
					int mat[3][3], auxxf = xf, auxyf = yf;
					if (auxxf == lungime)
						auxxf--;
					if (auxyf == inaltime)
						auxyf--;
					if (strcmp(p, "SHARPEN") == 0) {
						sharpen(mat);
						apply_met(a, mat, xi, auxxf, yi, auxyf, 1);
						printf("APPLY %s done\n", p);
					} else
					if (strcmp(p, "EDGE") == 0) {
						edge(mat);
						apply_met(a, mat, xi, auxxf, yi, auxyf, 1);
						printf("APPLY %s done\n", p);
					} else
					if (strcmp(p, "BLUR") == 0) {
						blur(mat);
						apply_met(a, mat, xi, auxxf, yi, auxyf, 9);
						printf("APPLY %s done\n", p);
					} else
					if(strcmp(p, "GAUSSIAN_BLUR") == 0) {
						gauss_blur(mat);
						apply_met(a, mat, xi, auxxf, yi, auxyf, 16);
						printf("APPLY %s done\n", p);
					} else
						printf("APPLY parameter invalid\n");
				}
			} else
				printf("Invalid command\n");
		}
	} else
	if (strcmp (p, "EQUALIZE") == 0) {
		if (load == 0)
			printf("No image loaded\n");
		else {
			if (strcmp(tip_imagine, "P3") == 0 || strcmp (tip_imagine, "P6") == 0)
				printf("Black and white image needed\n");
			else {
				equalize(b, lungime, inaltime);
				printf("Equalize done\n");
			}
		}
	} else
	if (strcmp(p, "ROTATE") == 0) {
		if (load == 0 )
			printf("No image loaded\n");
		else {
			p = strtok(NULL, " ");
			int angle = atoi(p);
			if (!verify_angle(angle))
				printf("Unsupported rotation angle\n");
			else {
				if (angle == 360 || angle == -360 || angle == 0)
					printf("Rotated %d\n", angle);
				else {
				int new_w = xf - xi, new_h = yf - yi;
				if (angle == 90 || angle == -270 || angle == -90 || angle == 270) {
					new_w = yf - yi;
					new_h = xf - xi;
				}
				if( tip_imagine[1] == '2' || tip_imagine[1] == '5') {
					int** new_image = rotate_pgm(b, xi, yi, xf, yf, new_w, new_h, angle);
					//verificam daca avem vreo selectie
					if (xi == 0 && yi == 0 && xf == lungime && yf == inaltime) {
						b = rotate_all_pgm(b, new_image, &xf, &yf, new_h, new_w, angle);
						inaltime = yf, lungime = xf;
					} else
						verify_square_pgm(b, new_image, xi, yi, xf, yf, angle);
					free_memory((void**)new_image, new_h);
				} else {
					pixel** new_image = rotate_ppm(a, xi, yi, xf, yf, new_w, new_h, angle);
					//verificam daca avem vreo selectie
					if (xi == 0 && yi == 0 && xf == lungime && yf == inaltime) {
						a = rotate_all_ppm(a, new_image, &xf, &yf, new_h, new_w, angle);
						inaltime = yf, lungime = xf;
					}
					else 
						verify_square_ppm(a, new_image, xi, yi, xf, yf, angle);
					free_memory((void**)new_image, new_h);
				}
				}
			}
				
		}
	} else
		printf("Invalid command\n");
	fgets(linie, 50, stdin);
	linie[strcspn(linie, "\n")] = '\0';
	p = strtok(linie, " ");
}
if (load == 0)
	printf("No image loaded\n");
else load = 0;
if (a) {
	free_memory((void **)a, inaltime);
	a = NULL;
}

if (b) {
	free_memory((void **)b, inaltime);
	b = NULL;
}
return 0;
}
