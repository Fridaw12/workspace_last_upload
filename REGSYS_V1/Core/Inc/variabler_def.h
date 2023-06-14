/*
 * variabler_def.h
 *
 *  Created on: 21. feb. 2023
 *      Author: fridawagner
 *
 *  Definition associates the variable with a type and allocates memory
 *  A definition actually instantiates/implements this identifier.
 *  It's what the linker needs in order to link references to those entities.
 *
 *  However, it must be defined exactly once.
 *
 *   If you forget to define something that's
 *  been declared and referenced somewhere, then the linker doesn't know what to link references
 *  to and complains about a missing symbols. If you define something more than once, then the
 *  linker doesn't know which of the definitions to link references to and complains about
 *  duplicated symbols.
 *
 *  These are definitions corresponding to the declarations in filename_dek.h:
 */




#ifndef INC_VARIABLER_DEK_H_
#define INC_VARIABLER_DEK_H_


#endif /* INC_VARIABLER_DEK_H_ */
