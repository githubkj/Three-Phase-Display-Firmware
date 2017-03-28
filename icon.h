#ifndef __ICON_HANDLER_H
#define __ICON_HANDLER_H

extern const char Pentair_Logo[];
extern const char compIcon[];
extern const char evapIcon[];
extern const char resisterIcon[];
extern const char condensorIcon[];
extern const char fanIcon[];
extern const char snowFlakeIcon[];
extern const char sunIcon[];
extern const char bellIcon[];
extern const char degreeF[];
extern const char degreeC[];
extern const char attentionIcon[];
extern const char decimal_point [];
extern const char negativeSign[];

#define NEGATIVE_SIGN_WIDTH		12
#define DECIMAL_POINT_WIDTH     4

/* Function Prototypes */
void showIcon(const char icon[], int width, unsigned char pageNum, unsigned char page, unsigned char col);
void hideIcon(int width, unsigned char page, unsigned char col);

#endif /* __ICON_HANDLER_H */
