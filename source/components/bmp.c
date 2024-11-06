#include "headers/components/bmp.h"

/* Reades bmp file on the given path and stores data in BMP f pointer. Data must be freed when no longer needed. */
void readBMP(BMP *f, const char path[]) {
    int emvadon;
    FILE* fp;
    fp = fopen(path, "rb");
    if (!fp) {
        printf("Could not open file < %s >! readHeightmap() -- fopen().\n", path);
        return;
    } else {
        fread(&f->header, sizeof(BMP_Header), 1, fp);
        fseek(fp, 14, SEEK_SET);
        fread(&f->info, sizeof(BMP_Info), 1, fp);
        fseek(fp, f->header.Res2, SEEK_SET);

        emvadon = f->info.Height * f->info.Width;
        if (!emvadon) {
            printf("Zero value for Height map Size: %d. createTerrain() --> ERROR 1\n", emvadon);
            fclose(fp);
            return;
        }
        f->data = malloc(emvadon * 4);
        if (!f->data) {
            fclose(fp);
            return;
        }
        fread(f->data, emvadon, 1, fp);
    }
    fclose(fp);
}
/* Loggs BMP file header. */
void logBMP_Header(const BMP_Header bmp_h) {
    printf("header.Type  : %d\n", bmp_h.Type);
    printf("header.Size  : %d\n", bmp_h.Size);
    printf("header.Res1  : %d\n", bmp_h.Res1);
    printf("header.Res2  : %d\n", bmp_h.Res2);
    printf("header.Offset: %d\n\n", bmp_h.OffSet);
}
/* Loggs BMP file EndPoints. */
void logBMP_EndPoints(const CS_EndPoints cse) {
    printf("Endpoints: {\n");
    printf("    redX  : %d    redY  : %d    redZ  : %d\n", cse.red_coords.RedX, cse.red_coords.RedY, cse.red_coords.RedZ);
    printf("    GreenX: %d    GreenY: %d    GreenZ: %d\n", cse.green_coords.GreenX, cse.green_coords.GreenY, cse.green_coords.GreenZ);
    printf("    BLueX : %d    BLueY : %d    BLueZ : %d\n", cse.blue_coords.BlueX, cse.blue_coords.BlueY, cse.blue_coords.BlueZ);
    printf("}\n");
}
/* Loggs BMP file info. */
void logBMP_Info(const BMP_Info bmp_i) {
    printf("info.Size            : %d\n", bmp_i.Size);
    printf("info.Width           : %d\n", bmp_i.Width);
    printf("info.Height          : %d\n", bmp_i.Height);
    printf("info.Planes          : %d\n", bmp_i.Planes);
    printf("info.BitsPerPixel    : %d\n", bmp_i.BitsPerPixel);
    printf("info.compression     : %d\n", bmp_i.compression);
    printf("info.SizeOfBitmap    : %d\n", bmp_i.SizeOfBitmap);
    printf("info.HorzResolution  : %ld\n", bmp_i.HorzResolution);
    printf("info.VertResolution  : %ld\n", bmp_i.VertResolution);
    printf("info.ColorsUsed      : %d\n", bmp_i.ColorsUsed);
    printf("info.ColorsImportand : %d\n", bmp_i.ColorsImportant);
    printf("info.RedMask         : %d\n", bmp_i.RedMask);
    printf("info.GreenMask       : %d\n", bmp_i.GreenMask);
    printf("info.BlueMask        : %d\n", bmp_i.BlueMask);
    printf("info.AlphaMask       : %d\n", bmp_i.AlphaMask);
    printf("info.CSType          : %d\n", bmp_i.CSType);
    logBMP_EndPoints(bmp_i.CSEndPoints);
    printf("info.GammaRed        : %d\n", bmp_i.GammaRed);
    printf("info.GammaGreen      : %d\n", bmp_i.GammaGreen);
    printf("info.GammaBlue       : %d\n", bmp_i.GammaBlue);
    printf("info.Intent          : %d\n", bmp_i.Intent);
    printf("info.ProfileData     : %d\n", bmp_i.ProfileData);
    printf("info.ProfileSize     : %d\n", bmp_i.ProfileSize);
    printf("info.Reserved        : %d\n", bmp_i.Reserved);
}
/* Loggs BMP file. */
void logBMP(const BMP bmp) {
    logBMP_Header(bmp.header);
    logBMP_Info(bmp.info);

}


