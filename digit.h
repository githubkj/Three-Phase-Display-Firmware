#ifndef __DIGIT_H
#define __DIGIT_H

/* Font tables */

extern const char digitCalibri4x28[];
extern const char digitCalibriLight4x28[];
extern const char digitCalibriBold5x30[];
extern const char digitCalibriLight5x30[];
extern const char digitCalibri5x30[];
extern const char digitCourier2x7[];
extern const char digitSevenSegment6x24[];

/* Function Prototypes */

void showDigit(const char font[], unsigned char digit, unsigned char heightInPages, unsigned char widthInRows, unsigned char page, unsigned char column);
void clearDigit(unsigned char widthInRows, unsigned char heightInPages, unsigned char page, unsigned char column);
void showIntegerRightAligned(const char font[], int number, unsigned char heightInPages, unsigned char widthInRows, unsigned char page, unsigned char column);
void showIntegerLeftAligned(const char font[], int number, unsigned char heightInPages, unsigned char widthInRows, unsigned char page, unsigned char column);

#endif /* __DIGIT_H */
