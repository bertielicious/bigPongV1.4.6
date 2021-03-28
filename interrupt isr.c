#include "config.h"
#include "arrays.h"
#include "sendSPIbyte.h"
#include "sendNoSPIbyte.h"
#include "writeMatrix.h"
#include "splitScore.h"
#include "selectDigit.h"
#include "drawPaddle.h"
#include "splashScreen.h"
#include "random.h"
//static volatile char icol = 1;
//static volatile char irow = 5;
static volatile char xSpeed = 1;
static volatile char ySpeed = 1;
static volatile bool ballInt = HI;
static volatile bool eraseInt = LO;
__interrupt() void MY_ISR(void)
{
     if(INTCONbits.TMR0IF == HI && drawFlg == HI)
     {
        icol = icol + xSpeed;
        irow = irow + ySpeed;
        gameArray[irow][icol]= 1;
        
      
        if((gameArray[irow][0] && gameArray[irow][1] == 1) || (gameArray[irow][14] && gameArray[irow][15] == 1))
        {
          xSpeed = -xSpeed;
          if(xSpeed == 1)
          {
            SOUND = LO;
            __delay_ms(2);
            SOUND = HI;
          }
          else
          {
            SOUND = LO;
            __delay_ms(2);
            SOUND = HI;
          }
        }
        
        else if ((icol == 1 && gameArray[irow][0]==0) || (icol == 14 && gameArray[irow][15] ==0))
        {
           // tic();
            gameArray[irow][icol] = 0;
            INTCONbits.TMR0IE = LO;
            xSpeed = 0;
            ySpeed = 0;
            if(icol == 1)
            {
                rightScore++;
                icol = 14;  // ball start position and direction after point scored
                random();
               // irow = 1;
                xSpeed = -1;
                ySpeed = 1;
                
            }
            else if(icol == 14)
            {
              //  tic();
                leftScore++;
                icol = 1;       // ball start position and direction after point scored
                random();
                //irow = 5;
                xSpeed = 1;
                ySpeed = 1;
                
            }
            splitScore(rightScore);  
            selectDigit(units,6,8, matrixR);
            selectDigit(tens,2,4, matrixR);
            splitScore(leftScore);
            selectDigit(units,6,8, matrixL);
            selectDigit(tens,2,4, matrixL);
            __delay_ms(500);
            
            if(rightScore < 11  || leftScore < 11)
            {
                writeMatrix(0,7,0,15);
                INTCONbits.TMR0IE = HI;
                TMR0 = 0;
               
               
              
            }
            if(rightScore > 10 || leftScore > 10)
            {
                INTCONbits.TMR0IE = LO; // turn off TMR0 interrupts
                                    // print game winner & final scores
                if(rightScore > 10)
                {
                       splashScreen(cross, tick); // print right player winner
                     __delay_ms(1000);
                }
                if(leftScore > 10)
                {
                        // print left player winner
                     splashScreen(tick, cross);
                      __delay_ms(1000);
                }
            }
           
        }
        if(irow <1 || irow>=7)
        {
            ySpeed = -ySpeed;   
        }
        
        
        drawFlg = LO;
        DIAGNOSTIC_BLUE_LED = ~DIAGNOSTIC_BLUE_LED;
     }
      else if(INTCONbits.TMR0IF == HI && drawFlg == LO)
        {
          if(gameArray[irow][icol]==1)
                    { 
                        gameArray[irow][icol] = 0x00;// clear row 0 of previous paddle position data
                        
                    }
        DIAGNOSTIC_GREEN_LED = ~DIAGNOSTIC_GREEN_LED;
        drawFlg = HI;
        }
     
      writeMatrix(0,7,0,15);
      TMR0 = 0;
      INTCONbits.TMR0IF = LO; 
}    