/*
 * common_macros.h
 *
 * Created: 19-Apr-20 1:26:11 PM
 * Author: Ahmed Gamal
 */
#ifndef COMMON_MACROS
#define COMMON_MACROS

#define SET_BIT(REG,BIT)		(REG|=(1<<BIT))
#define CLEAR_BIT(REG,BIT)		(REG&=(~(1<<BIT)))
#define TOGGLE_BIT(REG,BIT)		(REG^=(1<<BIT))
#define BIT_IS_SET(REG,BIT)		(REG&(1<<BIT))
#define BIT_IS_CLEAR(REG,BIT)	(!(REG&(1<<BIT)))

#endif
