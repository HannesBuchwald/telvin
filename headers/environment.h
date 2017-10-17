/*
 * environment.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */


#ifndef HEADERS_ENVIRONMENT_H_
#define HEADERS_ENVIRONMENT_H_

#include "global.h"

#define INIT_ENVIRONMENT_SIZE    3


/*
 * allocete sms oject whit T_ENVIRONMENT
 */
extern TEL_OBJ alloc_environment(unsigned int size);

/*
 * initilese  topEnvironment global value
 */
extern void  init_environment(void);

/*
 * function for adding methods witch TELWIN will support
 */
extern void add_binding(TEL_OBJ env, TEL_OBJ key, TEL_OBJ value);

/*
 * guess witch method key in enviroment stack
 */
extern TEL_OBJ get_binding(TEL_OBJ env, TEL_OBJ key);


#endif /* HEADERS_ENVIRONMENT_H_ */
