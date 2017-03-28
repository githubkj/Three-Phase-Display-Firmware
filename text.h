#ifndef __TEXT_HANDLER_H
#define __TEXT_HANDLER_H

#include "PE_Types.h"

/*****************************************************************************/
/*                           arial_bold14 								     */
/*                           menu Title 								     */
/*****************************************************************************/
extern const char arial_bold14[];

/*****************************************************************************/
/*                              arial14 								     */
/*                           menu content 								     */
/*****************************************************************************/
extern const char arial14[];

/*****************************************************************************/
/*                           courier7x14 								     */
/*                    ID number on main screen 						   	     */
/*****************************************************************************/
extern const char courier7x14[];

void displayChar(char ch, const char fontTable[], uint8 page, uint8 col);
void displayHighlightedChar(char ch, const char fontTable[], uint8 page, uint8 col);
void displayString (const char string[], const char fontTable[], uint8 page, uint8 col);
void displayHighlightedString (const char string[], const char fontTable[], uint8 page, uint8 col);

#endif /* __TEXT_HANDLER_H */
