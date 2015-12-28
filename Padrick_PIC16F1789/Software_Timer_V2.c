/* 
 * File:   Software_Timer.c
 * Author: Florian
 *
 * Created on 14. Oktober 2015, 15:32
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Prototypes_V2.h"
#include "Variables_V2.h"

//              *************Timer_Subroutines*************

void delay_10us (void)
{
    TMR1IF = 0;
    TMR1 = 65280;
    while (TMR1IF == 0);
}

void delay_80us (void)
{
    TMR1IF = 0;
    TMR1 = 63040;
    while (TMR1IF == 0);
}

void delay_90us (void)
{
    TMR1IF = 0;
    TMR1 = 62720;
    while (TMR1IF == 0);
}

void delay_150us (void)
{
    TMR1IF = 0;
    TMR1 = 60800;
    while (TMR1IF == 0);
}

void delay_450us (void)
{
    TMR1IF = 0;
    TMR1 = 51200;
    while (TMR1IF == 0);
}

void delay_720us (void)
{
    TMR1IF = 0;
    TMR1 = 42496;
    while (TMR1IF == 0);
}

void delay_50ms (void)
{
    for (loop = 0; loop < 24; loop++)
    {
        TMR1IF = 0;
        TMR1 = 0;
        while (TMR1IF == 0)
        {
            TMR0IE = 1;
        }    
    }
    TMR0IF = 0;
    TMR0IE = 0;
}

void delay_200ms (void)
{
    for (loop = 0; loop < 97; loop++)
    {
        TMR1IF = 0;
        TMR1 = 0;
        while (TMR1IF == 0)
        {
            TMR0IE = 1;
        }    
    }
    TMR0IF = 0;
    TMR0IE = 0;
}



