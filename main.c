//ECE 198 Project: "Guess it, Diffuse it"
//  The user must guess the correct third sequence to the pattern to recieve a green light (diffuse the bomb)
//  Failure to guess the correct sequence will result in red light

// Written by Nandita Lohani and Sabirah Alam, Novemeber 2021

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h"

//pattern - first sqeuence 
void pattern1(); 

//pattern - second sequence 
void pattern2(); 

int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...
    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED


    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)
    SerialSetup(9600);

    //initialize the led pins 
    InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //red led
    InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //yellow led
    InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //blue led
    InitializePin(GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //green led

    //initializine the led pins for guessing the correct/incorrect pattern sequence
    InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //"correct" led
    InitializePin(GPIOA, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //"incorrect" led

    //initialize the button pins
    InitializePin(GPIOA, GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_NOPULL, 0); //red button
    InitializePin(GPIOC, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, 0); //yellow button
    InitializePin(GPIOB, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, 0); //blue button
    InitializePin(GPIOA, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, 0); //green button

    //Sample array for the sequence 
    bool pattern[4] = {false, false, false, false};
    bool correct_pattern = true;

    //display the first two sequences 
    pattern1(); 
    pattern2();
    
    while (1) { 

        /////////////////////////////////
        ////TURN LEDs ON WITH BUTTONS////
        ////////////////////////////////
        
        //turn on red led if the red button is pressed 
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);               
        } else { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
        }
        
        //turn on yellow led if the yellow button is pressed 
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)) { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);              
        } else { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
        }

        //turn on blue led if the blue button is pressed  
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1); 
        } else { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
        }
    
        //turn on green led if the green button is pressed 
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) { 
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1); 
        } else { 
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0); 
        }
        

       //////////////////////////////////
       ////INITIALIZE CORRECT PATTERN////
       //////////////////////////////////

        for(int i = 0; i < 4; ++i) { 

            //Green button
            if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) && pattern[i] == false){
                pattern[i] = true;
            }
            
            if (i == 1) { 

                //Blue button
                if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6) && pattern[i] == false){
                pattern[i] = true;
                }
            } 
            
            if ( i == 2) { 
                //Yellow button
                if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) && pattern[i] == false){
                pattern[i] = true;
                }
            } 
            
            if ( i == 3 ) { 
                //Red button
                if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9) && pattern[i] == false){
                pattern[i] = true;
                }
            } 
        }

       
       //////////////////////////////////////////////
       ////ACCOUNTING FOR INCORRECT PATTERN INPUT////
       /////////////////////////////////////////////

       for (int i = 0; i < 4; ++i) { 
           if((pattern[i]== false) && (pattern[i+1] == false) && (pattern[i+2] == false) && (pattern[i+3] == false)){
                correct_pattern = true;
           }
           else if((pattern[i]== true) && (pattern[i+1] == false) && (pattern[i+2] == false) && (pattern[i+3] == false)){
                correct_pattern = true;  
           }
           else if((pattern[i]== true) && (pattern[i+1] == true) && (pattern[i+2] == false) && (pattern[i+3] == false)){
                correct_pattern = true;   
           }
           else if((pattern[i]== true) && (pattern[i+1] == true) && (pattern[i+2] == true) && (pattern[i+3] == false)){
                correct_pattern = true;
           }
           else if((pattern[i]== true) && (pattern[i+1] == true) && (pattern[i+2] == true) && (pattern[i+3] == true)){
                correct_pattern = true;
                //Display "correct" led - light the green led
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,1);      
           }
           else{
                correct_pattern = false;
                //Display "incorrect" led - light the red led
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,1);
           }       
       }
    } 
    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}


//Displaying pattern 1
void pattern1() { 
    HAL_Delay(3000); //3 second

    //RED LED 
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);  
    HAL_Delay(1000); //1 second
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0); 

    //YELLOW LED
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);  
    HAL_Delay(1000); 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0); 

    //BLUE LED
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);  
    HAL_Delay(1000); 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0); 

    //GREEN LED
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); 
    HAL_Delay(1000); 
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0); 
}


//Displaying pattern 2
void pattern2() { 
    HAL_Delay(3000); //3 second

    //YELLOW LED 
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);  
    HAL_Delay(1000); 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0); 

    //GREEN LED 
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); 
    HAL_Delay(1000); 
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0); 
        
    //BLUE LED 
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10); 
    HAL_Delay(1000); 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0); 

    //RED LED
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);  
    HAL_Delay(1000); //1 second
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0); 

}

