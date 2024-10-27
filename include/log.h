#ifndef __LOG_H__
#define __LOG_H__

/**
 * @brief Print error message in red color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_error(const char *format, ...);

/**
 * @brief Print info message in green color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_info(const char *format, ...);

/**
 * @brief Print warning message in yellow color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_warning(const char *format, ...);

#endif // __LOG_H__
