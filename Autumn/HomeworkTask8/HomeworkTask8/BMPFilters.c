#include <stdio.h>
#include <math.h>
#include "BMPFilters.h"

int main()
{
	printf("This program applies filter to image.\n");
	printf("Source and target file must .BMP.\n\n");
	printf("Available filters:\n0 = Average3x3 (default)\n");
	printf("1 = Gaussian3x3\n2 = Sobel X\n3 = Sobel Y\n4 = GrayScale\n\n");

	do {

		FILE *sourceFile;
		BITMAPFILEHEADER bmFileHeader;
		BITMAPINFOHEADER bmInfoHeader;
		UBYTE *image;

		{
			char* pathsource = malloc(sizeof(char) * 64);
			printf("Enter path to source file: \n");
			scanf("%s", pathsource);
			sourceFile = fopen(pathsource, "rb");
			free(pathsource);
		}

		if (sourceFile == NULL)
		{
			printf("Wrong path!\n");
			continue;
		}

		fread(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, sourceFile);
		if (bmFileHeader.bfType != 0x4D42)
		{
			printf("Wrong file header type!\n");
			continue;
		}

		fread(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, sourceFile);
		if (bmInfoHeader.biSize != 0x28)
		{
			printf("Wrong info header size!\n");
			continue;
		}
		fseek(sourceFile, bmFileHeader.bfOffBits, SEEK_SET);

		image = (UBYTE*)malloc(bmInfoHeader.biSizeImage);
		if (!image)
		{
			printf("Not enough memory!\n");
			free(image);
			continue;
		}
		
		fread(image, bmInfoHeader.biSizeImage, 1, sourceFile);
		if (image == NULL)
		{
			printf("Image is NULL!\n");
			continue;
		}

		// choosing filter
		do {

			int filter = 0;
			FILE *targetFile;
			int width = bmInfoHeader.biWidth;
			int height = bmInfoHeader.biHeight;

			{
				char* pathtarget = malloc(sizeof(char) * 64);
				printf("Enter path to target file : \n");
				scanf("%s", pathtarget);

				targetFile = fopen(pathtarget, "wb");
				free(pathtarget);
			}

			if (targetFile == NULL)
			{
				printf("Wrong path!\n");
				continue;
			}

			// new image with RGB
			PIXEL **newImage = (PIXEL**)malloc(sizeof(PIXEL*) * width);
			if (!newImage)
			{
				printf("Not enough memory!\n");
				continue;
			}

			for (int i = 0; i < width; i++)
			{
				newImage[i] = (PIXEL*)malloc(sizeof(PIXEL) * height);
				if (!newImage[i])
				{
					printf("Not enough memory!\n");
					free(newImage);
					continue;
				}
			}

			// for writing into file
			UBYTE pad[3] = { 0, 0, 0 };
			int padSize = (4 - (width * 3) % 4) % 4;
			fwrite(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, targetFile);
			fwrite(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, targetFile);
			// fseek(targetFile, bmFileHeader.bfOffBits, SEEK_SET);

			// pixel array
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					int offset = (x + y*width) * 3 + y * padSize;

					newImage[x][y].red = image[offset + 2];
					newImage[x][y].green = image[offset + 1];
					newImage[x][y].blue = image[offset];
				}
			}

			{
				do {

					printf("Choose filter: ");
					scanf("%d", &filter);
					if (filter < 0 || filter > 4)
					{
						printf("Wrong filter index!\n");
						continue;
					}
					break;

				} while (1);
			}

			switch (filter)
			{
			case 1: // gaussian3x3
			{
				float sigma = 1;
				sigma = 2 * sigma * sigma;
				float **kernel = (float**)malloc(sizeof(float*) * 3);

				for (char i = -1; i <= 1; i++)
				{
					kernel[i + 1] = (float*)malloc(sizeof(float) * 3);
					for (char j = -1; j <= 1; j++)
					{
						{
							kernel[i + 1][j + 1] = (exp(-(i*i + j*j) / sigma)) / (PI * sigma);
						}
					}
				}

				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						float sum = 0;
						float r = 0, g = 0, b = 0;

						for (int i = -1; i <= 1; i++)
						{
							for (int j = -1; j <= 1; j++)
							{
								if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
								{
									PIXEL pix = newImage[x + i][y + j];

									r += pix.red * kernel[i + 1][j + 1];
									g += pix.green * kernel[i + 1][j + 1];
									b += pix.blue * kernel[i + 1][j + 1];

									sum += kernel[i + 1][j + 1];
								}
							}
						}

						if (sum != 0)
						{
							newImage[x][y].red = round(r / sum);
							newImage[x][y].green = round(g / sum);
							newImage[x][y].blue = round(b / sum);
						}
					}
				}

				break;
			}
			case 2: // sobelx
			{
				char mask[3][3] =
				{
					{ -1, 0, 1 },
					{ -2, 0, 2 },
					{ -1, 0, 1 }
				};
				//sobel(width, height, newImage, masky);

				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						short g = 0;

						for (int i = -1; i <= 1; i++)
						{
							for (int j = -1; j <= 1; j++)
							{
								if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
								{
									UBYTE cur = pixelGrayscaled(newImage[x + i][y + j]);
									g += cur * mask[i + 1][j + 1];
								}
							}
						}

						clampToByte(&g);
						newImage[x][y] = *(pixelNewSame(g));
					}
				}

				break;
			}
			case 3: // sobely
			{
				char mask[3][3] =
				{
					{ -1, -2, -1 },
					{ 0, 0, 0 },
					{ 1, 2, 1 }
				};

				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						short g = 0;

						for (int i = -1; i <= 1; i++)
						{
							for (int j = -1; j <= 1; j++)
							{
								if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
								{
									UBYTE cur = pixelGrayscaled(newImage[x + i][y + j]);
									g += cur * mask[i + 1][j + 1];
								}
							}
						}

						clampToByte(&g);
						newImage[x][y] = *(pixelNewSame(g));
					}
				}

				break;
			}
			case 4: // grayscale
			{
				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						/*PIXEL pix = newImage[x][y];
						WORD grayscale = 0; // WORD used, because 255*9 > 255
						grayscale = pix.blue + pix.green + pix.red;
						grayscale /= 3;*/

						UBYTE grayscale = pixelGrayscaled(newImage[x][y]);
						newImage[x][y] = *(pixelNewSame(grayscale));
					}
				}
				break;
			}
			default: // avg3x3
			{
				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						WORD r = 0, g = 0, b = 0; // WORD used, because 255*9 > 255
						for (int i = -1; i <= 1; i++)
						{
							for (int j = -1; j <= 1; j++)
							{
								if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
								{
									r += newImage[x + i][y + j].red;
									g += newImage[x + i][y + j].green;
									b += newImage[x + i][y + j].blue;
								}
							}
						}
						newImage[x][y] = *(pixelNew(r / 9, g / 9, b / 9));
					}
				}
				break;
			}
			}

			// write new image to file
			// fseek(targetFile, bmFileHeader.bfOffBits, SEEK_SET);
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					UBYTE b = newImage[x][y].blue;
					UBYTE g = newImage[x][y].green;
					UBYTE r = newImage[x][y].red;

					fwrite((UBYTE*)&b, 1, 1, targetFile);
					fwrite((UBYTE*)&g, 1, 1, targetFile);
					fwrite((UBYTE*)&r, 1, 1, targetFile);
				}
				fwrite((char*)pad, padSize, 1, targetFile);
			}

			free(image);
			fclose(targetFile);

			printf("Done!\n");

			break;

		} while (1);

		fclose(sourceFile);
		break;

	} while (1);

	return 0;
}

PIXEL* pixelNew(UBYTE r, UBYTE g, UBYTE b)
{
	PIXEL* p = malloc(sizeof(PIXEL));
	p->red = r;
	p->green = g;
	p->blue = b;
	return p;
}

PIXEL* pixelNewSame(UBYTE v)
{
	PIXEL* p = malloc(sizeof(PIXEL));
	p->red = v;
	p->green = v;
	p->blue = v;
	return p;
}

UBYTE* pixelGrayscaled(PIXEL p)
{
	return (p.blue + p.green + p.red) / 3;
}

/*void pixelMultiply(PIXEL *p, float v)
{
	short value = p->red * v;
	clampToByte(value);
	p->red = value;

	value = p->green * v;
	clampToByte(value);
	p->green = value;

	value = p->blue * v;
	clampToByte(value);
	p->blue = value;
}*/

void clampToByte(short *value)
{
	*value = 
		*value < 0 ? 0 : 
		*value > 255 ? 255 : 
		*value;
}