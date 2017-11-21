/** Programmer: Daniel J EAgy
 ** Assignment: EagyBoxBlur
 ** Version: 0.9
 *
 */

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

#pragma warning(disable: 4996)

//problem assumptions
#define BMP_HEADER_SIZE_BYTES 14
#define BMP_DIB_HEADER_SIZE_BYTES 40
#define MAXIMUM_IMAGE_SIZE 256

//bmp compression methods
//none:
#define BI_RGB 0

//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

struct BMP_Header {
    char signature[2];        //ID field
    int size;        //Size of the BMP file
    short reserved1;        //Application specific
    short reserved2;        //Application specific
    int offset_pixel_array;  //Offset where the pixel array (bitmap data) can be found
};

//TODO: finish me

struct DIB_Header {
    int dibHeaderSize;
    int width;
    int height;
    short planes;
    short bitsPerPixel;
    int compression;
    int imageSize;
    int xPixPerMeter;
    int yPixPerMeter;
    int colorsInTable;
    int importantColors;
};

struct Pixel {
    unsigned int red;
    unsigned int blue;
    unsigned int green;
};

typedef struct {
    struct BMP_Header header;
    struct DIB_Header dib;
    struct Pixel **pixels;
    int padding;
}BMP;
////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

//GLOBAL VARIABLES
struct Pixel **origPixelMatrix;
struct Pixel **newPixelMatrix;
struct BMP_Header *header;
struct DIB_Header *dib;
BMP *in_bmp;
BMP *out_bmp;

//TODO: finish me
void write_pixel_data(FILE *file);

void read_header_info(FILE *file, struct BMP_Header *header, struct DIB_Header *dib);

void load_bmp(FILE *file);

void blur(int blur_level);

void main(int argc, char *argv[]) {
    //TODO: finish me
    header = (struct BMP_Header *) malloc(sizeof(struct BMP_Header));
    dib = (struct DIB_Header *) malloc(sizeof(struct DIB_Header));
    in_bmp = (BMP*)malloc(sizeof(BMP));
    out_bmp = (BMP*)malloc(sizeof(BMP));

    //User Prompt
    char *buff;
    printf("Enter input filename: ");
    scanf("%s", buff);

    //sample code to read first 14 bytes of BMP file format
    FILE *file = fopen(buff, "rb");

    //Read in header info
    read_header_info(file, header, dib);

    printf("1\n");
    //Setting BMP pixel matrix
    in_bmp->pixels = (struct Pixel **) malloc(sizeof(struct Pixel*) * in_bmp->dib.height);
    int i;
    for (i = 0; i < in_bmp->dib.height; i++)
        in_bmp->pixels[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * in_bmp->dib.width);

    //Setting BMP pixel matrix
    printf("1\n");
    out_bmp->pixels = (struct Pixel **) malloc(sizeof(struct Pixe*) *out_bmp->dib.height);
    for (i = 0; i < in_bmp->dib.height; i++)
        out_bmp->pixels[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * out_bmp->dib.width);

    //initialize origPixel matrix
    origPixelMatrix = (struct Pixel **) malloc(sizeof(struct Pixel *) * dib->height);

    for (i = 0; i < dib->height; i++)
        origPixelMatrix[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * dib->width);

    //initialize newPixel matrix
    newPixelMatrix = (struct Pixel **) malloc(sizeof(struct Pixel *) * dib->height);

    for (i = 0; i < dib->height; i++)
        newPixelMatrix[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * dib->width);

/*    //Print Header
    printf("signature: %c%c\n", header->signature[0], header->signature[1]);
    printf("size: %d\n", header->size);
    printf("reserved1: %d\n", header->reserved1);
    printf("reserved2: %d\n", header->reserved2);
    printf("offset_pixel_array: %d\n", header->offset_pixel_array);
    printf("\n\nDIB Header\n");

    //Print DIB Header
    printf("DIB Header Size: %d\n", dib->dibHeaderSize);
    printf("Image Width: %d\n", dib->width);
    printf("Image Height: %d\n", dib->height);
    printf("Planes: %d\n", dib->planes);
    printf("Bits Per Pixel: %d\n", dib->bitsPerPixel);
    printf("Compression: %d\n", dib->compression);
    printf("Image Size: %d\n", dib->imageSize);
    printf("X Pixels Per Meter: %d\n", dib->xPixPerMeter);
    printf("Y Pixels Per Meter: %d\n", dib->yPixPerMeter);
    printf("Colors in Color Table: %d\n", dib->colorsInTable);
    printf("Important Color Count: %d\n", dib->importantColors);*/

    short row, col;
    short value;
    short pixel[dib->height][dib->width];
    short blue = 0, green = 0, red = 0;


    load_bmp(file);
    blur(3);

    //output filename
    printf("Enter output filename: ");
    scanf("%s", buff);

    FILE *out = fopen(buff, "wb");
    write_pixel_data(file);


    free(header);
    free(dib);
    free(origPixelMatrix);
    free(newPixelMatrix);
    fclose(out);
    fclose(file);
}

void write_pixel_data(FILE *file) {
    fwrite(&in_bmp, sizeof(BMP), 1, file);
/*    fwrite(&header->signature[0], sizeof(char), 1, file);
    fwrite(&header->signature[1], sizeof(char), 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);

    //read DIB header
    fwrite(&dib->dibHeaderSize, sizeof(int), 1, file);
    fwrite(&dib->width, sizeof(int), 1, file);
    fwrite(&dib->height, sizeof(int), 1, file);
    fwrite(&dib->planes, sizeof(short), 1, file);
    fwrite(&dib->bitsPerPixel, sizeof(short), 1, file);
    fwrite(&dib->compression, sizeof(int), 1, file);
    fwrite(&dib->imageSize, sizeof(int), 1, file);
    fwrite(&dib->xPixPerMeter, sizeof(int), 1, file);
    fwrite(&dib->yPixPerMeter, sizeof(int), 1, file);
    fwrite(&dib->colorsInTable, sizeof(int), 1, file);
    fwrite(&dib->importantColors, sizeof(int), 1, file);

    int row, col, value = 00;
    for (row = 0; row < (dib->height); row++) {
        for (col = 0; col < (dib->width); col++) {
            fwrite(&newPixelMatrix[row][col].blue, 1, 1, file);
            fwrite(&newPixelMatrix[row][col].green, 1, 1, file);
            fwrite(&newPixelMatrix[row][col].red, 1, 1, file);
        }
        fwrite(&value, 1, 2, file);
    }*/
}

void read_header_info(FILE *file, struct BMP_Header *header, struct DIB_Header *dib) {
    //read bitmap file header (14 bytes)
    fread(&in_bmp->header.signature, sizeof(char) * 2, 1, file);
    fread(&in_bmp->header.size, sizeof(int), 1, file);
    fread(&in_bmp->header.reserved1, sizeof(short), 1, file);
    fread(&in_bmp->header.reserved2, sizeof(short), 1, file);
    fread(&in_bmp->header.offset_pixel_array, sizeof(int), 1, file);

    //read DIB header
    fread(&in_bmp->dib.dibHeaderSize, sizeof(int), 1, file);
    fread(&in_bmp->dib.width, sizeof(int), 1, file);
    fread(&in_bmp->dib.height, sizeof(int), 1, file);
    fread(&in_bmp->dib.planes, sizeof(short), 1, file);
    fread(&in_bmp->dib.bitsPerPixel, sizeof(short), 1, file);
    fread(&in_bmp->dib.compression, sizeof(int), 1, file);
    fread(&in_bmp->dib.imageSize, sizeof(int), 1, file);
    fread(&in_bmp->dib.xPixPerMeter, sizeof(int), 1, file);
    fread(&in_bmp->dib.yPixPerMeter, sizeof(int), 1, file);
    fread(&in_bmp->dib.colorsInTable, sizeof(int), 1, file);
    fread(&in_bmp->dib.importantColors, sizeof(int), 1, file);



}

void load_bmp(FILE *file) {

    int row, col;
    unsigned int padding;
    for (row = 0; row < in_bmp->dib.height; row++) {
        for (col = 0; col < (in_bmp->dib.width); col++) {

            fread(&in_bmp->pixels[row][col].blue, 1, 1, file);
            fread(&in_bmp->pixels[row][col].green, 1, 1, file);
            fread(&in_bmp->pixels[row][col].red, 1, 1, file);

            //printf("%x, %x, %x, ", blue, green, red);

        }
        fread(&padding, 1, 2, file);
    }
}

void blur(int blur_level) {
    int row, col, blur_row, blur_col, count;
    unsigned int red, green, blue;

#pragma omp parallel for
    for (row = 0; row < dib->height; row++) {
#pragma omp parallel for
        for (col = 0; col < dib->width; col++) {
            red = 0;
            green = 0;
            blue = 0;
            count = 0;

            //printf("%d, %d\n", row, col);
#pragma omp parallel for
            for (blur_row = row - (blur_level); blur_row <= row + blur_level; blur_row++) {
#pragma omp parallel for
                for (blur_col = col - (blur_level); blur_col <= col + blur_level; blur_col++) {
                    //printf("%d, %d\n", blur_row, blur_col);
                    if (blur_row < 0 || blur_row >= dib->height || blur_col < 0 || blur_col >= dib->width)
                        continue;

                    blue += origPixelMatrix[blur_row][blur_col].blue;
                    green += origPixelMatrix[blur_row][blur_col].green;
                    red += origPixelMatrix[blur_row][blur_col].red;

                    count++;
                }
            }
            //printf("count: %d\n", count);
            blue = blue / count;
            green /= count;
            red /= count;

            newPixelMatrix[row][col].blue = blue;
            newPixelMatrix[row][col].green = green;
            newPixelMatrix[row][col].red = red;

        }
    }
}