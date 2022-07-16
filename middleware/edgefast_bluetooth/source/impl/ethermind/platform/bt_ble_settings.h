/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BT_BLE_SETTINGS_H__
#define __BT_BLE_SETTINGS_H__

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
#include "porting.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SETTINGS_KEY_MAX 256

/**
 * Function used to read the data from the settings storage in
 * h_set handler implementations.
 *
 * @param[in] cb_arg  arguments for the read function. Appropriate cb_arg is
 *                    transferred to h_set handler implementation by
 *                    the backend.
 * @param[out] data  the destination buffer
 * @param[in] len    length of read
 *
 * @return positive: Number of bytes read, 0: key-value pair is deleted.
 *                   On error returns -ERRNO code.
 */
typedef ssize_t (*settings_read_cb)(void *cb_arg, void *data, size_t len);

/**
 * @struct settings_handler_static
 * Config handlers without the node element, used for static handlers.
 * These are registered using a call to SETTINGS_REGISTER_STATIC().
 */
struct settings_handler_static {

	const char *name;
	/**< Name of subtree. */

	int (*h_get)(const char *key, char *val, int val_len_max);
	/**< Get values handler of settings items identified by keyword names.
	 *
	 * Parameters:
	 *  - key[in] the name with skipped part that was used as name in
	 *    handler registration
	 *  - val[out] buffer to receive value.
	 *  - val_len_max[in] size of that buffer.
	 *
	 * Return: length of data read on success, negative on failure.
	 */

	int (*h_set)(const char *key, size_t len, settings_read_cb read_cb,
		     void *cb_arg);
	/**< Set value handler of settings items identified by keyword names.
	 *
	 * Parameters:
	 *  - key[in] the name with skipped part that was used as name in
	 *    handler registration
	 *  - len[in] the size of the data found in the backend.
	 *  - read_cb[in] function provided to read the data from the backend.
	 *  - cb_arg[in] arguments for the read function provided by the
	 *    backend.
	 *
	 * Return: 0 on success, non-zero on failure.
	 */

	int (*h_commit)(void);
	/**< This handler gets called after settings has been loaded in full.
	 * User might use it to apply setting to the application.
	 */

	int (*h_export)(int (*export_func)(const char *name, const void *val,
					   size_t val_len));
	/**< This gets called to dump all current settings items.
	 *
	 * This happens when @ref settings_save tries to save the settings.
	 * Parameters:
	 *  - export_func: the pointer to the internal function which appends
	 *   a single key-value pair to persisted settings. Don't store
	 *   duplicated value. The name is subtree/key string, val is the string
	 *   with value.
	 *
	 * @remarks The User might limit a implementations of handler to serving
	 * only one keyword at one call - what will impose limit to get/set
	 * values using full subtree/key name.
	 *
	 * Return: 0 on success, non-zero on failure.
	 */
};

/**
 * Define a static handler for settings items
 *
 * @param _hname handler name
 * @param _tree subtree name
 * @param _get get routine (can be NULL)
 * @param _set set routine (can be NULL)
 * @param _commit commit routine (can be NULL)
 * @param _export export routine (can be NULL)
 *
 * This creates a variable _hname prepended by settings_handler_.
 *
 */

#define SETTINGS_STATIC_HANDLER_DEFINE(_hname, _tree, _get, _set, _commit,   \
				       _export)				     \
	const STRUCT_SECTION_ITERABLE(settings_handler_static,	     \
					settings_handler_ ## _hname) = {     \
		.name = _tree,						     \
		.h_get = _get,						     \
		.h_set = _set,						     \
		.h_commit = _commit,					     \
		.h_export = _export,					     \
	}

/**
 * Initialization of settings and backend
 *
 * Can be called at application startup.
 * In case the backend is a FS Remember to call it after the FS was mounted.
 * For FCB backend it can be called without such a restriction.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_init(void);


/**
 * Load serialized items from registered persistence sources. Handlers for
 * serialized item subtrees registered earlier will be called for encountered
 * values.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_load(void);

/**
 * Save currently running serialized items. All serialized items which are
 * different from currently persisted values will be saved.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_save(void);

/**
 * Write a single serialized value to persisted storage (if it has
 * changed value).
 *
 * @param name Name/key of the settings item.
 * @param value Pointer to the value of the settings item. This value will
 * be transferred to the @ref settings_handler::h_export handler implementation.
 * @param val_len Length of the value.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_save_one(const char *name, const void *value, size_t val_len);

/**
 * Delete a single serialized in persisted storage.
 *
 * Deleting an existing key-value pair in the settings mean
 * to set its value to NULL.
 *
 * @param name Name/key of the settings item.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_delete(const char *name);

/**
 * Read a single serialized value to persisted storage (if it has
 * changed value).
 *
 * @param name Name/key of the settings item.
 * @param value Pointer to the value of the settings item. This value will
 * be transferred to the @ref settings_handler::h_export handler implementation.
 * @param val_len Length of the value.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_load_one(const char *name, void *value, size_t val_len);

/**
 * Load limited set of serialized items from registered persistence sources.
 * Handlers for serialized item subtrees registered earlier will be called for
 * encountered values that belong to the subtree.
 *
 * @param[in] subtree name of the subtree to be loaded.
 * @return 0 on success, non-zero on failure.
 */
int settings_load_subtree(const char *subtree);

/**
 * Callback function used for direct loading.
 * Used by @ref settings_load_subtree_direct function.
 *
 * @param[in]     key     the name with skipped part that was used as name in
 *                        handler registration
 * @param[in]     len     the size of the data found in the backend.
 * @param[in]     read_cb function provided to read the data from the backend.
 * @param[in,out] cb_arg  arguments for the read function provided by the
 *                        backend.
 * @param[in,out] param   parameter given to the
 *                        @ref settings_load_subtree_direct function.
 *
 *  - key[in] the name with skipped part that was used as name in
 *    handler registration
 *  - len[in] the size of the data found in the backend.
 *  - read_cb[in] function provided to read the data from the backend.
 *  - cb_arg[in] arguments for the read function provided by the
 *    backend.
 *
 * @return When nonzero value is returned, further subtree searching is stopped.
 *         Use with care as some settings backends would iterate through old
 *         values, and the current value is returned last.
 */
typedef int (*settings_load_direct_cb)(
	const char      *key,
	size_t           len,
	settings_read_cb read_cb,
	void            *cb_arg,
	void            *param);

/**
 * Load limited set of serialized items using given callback.
 *
 * This function bypasses the normal data workflow in settings module.
 * All the settings values that are found are passed to the given callback.
 *
 * @note
 * This function does not call commit function.
 * It works as a blocking function, so it is up to the user to call
 * any kind of commit function when this operation ends.
 *
 * @param[in]     subtree subtree name of the subtree to be loaded.
 * @param[in]     cb      pointer to the callback function.
 * @param[in,out] param   parameter to be passed when callback
 *                        function is called.
 * @return 0 on success, non-zero on failure.
 */
int settings_load_subtree_direct(
	const char             *subtree,
	settings_load_direct_cb cb,
	void                   *param);

/**
 * Compares the start of name with a key
 *
 * @param[in] name in string format
 * @param[in] key comparison string
 * @param[out] next pointer to remaining of name, when the remaining part
 *             starts with a separator the separator is removed from next
 *
 * Some examples:
 * settings_name_steq("bt/btmesh/iv", "b", &next) returns 1, next="t/btmesh/iv"
 * settings_name_steq("bt/btmesh/iv", "bt", &next) returns 1, next="btmesh/iv"
 * settings_name_steq("bt/btmesh/iv", "bt/", &next) returns 0, next=NULL
 * settings_name_steq("bt/btmesh/iv", "bta", &next) returns 0, next=NULL
 *
 * REMARK: This routine could be simplified if the settings_handler names
 * would include a separator at the end.
 *
 * @return 0: no match
 *         1: match, next can be used to check if match is full
 */
int settings_name_steq(const char *name, const char *key, const char **next);

/**
 * determine the number of characters before the first separator
 *
 * @param[in] name in string format
 * @param[out] next pointer to remaining of name (excluding separator)
 *
 * @return index of the first separator, in case no separator was found this
 * is the size of name
 *
 */
int settings_name_next(const char *name, const char **next);


/**
 * Call commit for all settings handler. This should apply all
 * settings which has been set, but not applied yet.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_commit(void);

/**
 * Call commit for settings handler that belong to subtree.
 * This should apply all settings which has been set, but not applied yet.
 *
 * @param[in] subtree name of the subtree to be committed.
 *
 * @return 0 on success, non-zero on failure.
 */
int settings_commit_subtree(const char *subtree);

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_BT_SETTINGS */
#endif /* __BT_BLE_SETTINGS_H__ */