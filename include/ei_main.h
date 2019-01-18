/**
 *  @file	ei_main.h
 *  @brief	Declares the "ei_main" function: the main function of programs built with the
 *		libei.
 *
 *  \author 
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
**/
#ifndef EI_MAIN_H
#define EI_MAIN_H

/**
 * \brief	The main function of the program.
 *
 *		Programmers must not define their main function in a function called "main",
 *		because the "main" function is defined by Allegro and linked with in the libeibase
 *		library.
 *
 * @param	argc, argv	The parameters that were passed the the "main" function.
 *
 * @return			An error code: 0 means ok, 1 means error.
 */
int ei_main (int argc, char* argv[]);

#endif
