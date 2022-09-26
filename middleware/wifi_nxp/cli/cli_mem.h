/** @file cli_mem.h
 *
 *  @brief Command Line Interface meory module
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#ifndef _CLI_MEM_H_
#define _CLI_MEM_H_
#define INBUF_SIZE 256

#define BUF_ALLOCATED '1'
#define BUF_AVAILABLE '0'
/** Malloc some memory from the CLI's byte pool
 */
void *cli_mem_malloc(int size);

/** Free previously allocated memory to the CLI's byte pool
 */
int cli_mem_free(char **buffer);

/** Initialize the CLI memory module
 *
 * \note This function is called in the CLI  initialization process
 * itself. Applications need not call this function.
 *
 * \param void
 * \return WM_SUCCESS on success, error code otherwise.
 */
int cli_mem_init(void);

/** Initialize the CLI memory module
 *
 * \note This function is called in the CLI  stop process
 * itself. Applications need not call this function.
 */
int cli_mem_cleanup(void);
#endif /* _CLI_MEM_H_ */
