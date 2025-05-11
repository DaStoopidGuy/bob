#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} Bob_StringBuilder;

Bob_StringBuilder bob_sb_create();
Bob_StringBuilder bob_sb_from_parts(const char *data, size_t len);
Bob_StringBuilder bob_sb_from_cstr(const char *str);
void bob_sb_destroy(Bob_StringBuilder *sb);
const char *bob_sb_get_cstr(Bob_StringBuilder *sb);
void bob_sb_append(Bob_StringBuilder *primary, Bob_StringBuilder *secondary);
void bob_sb_append_parts(Bob_StringBuilder *sb, const char *data, size_t len);
void bob_sb_append_cstr(Bob_StringBuilder *sb, const char* str);

typedef struct {
    Bob_StringBuilder *items;
    size_t len;
    size_t cap;
} Bob_Cmd;

void bob_cmd_append(Bob_Cmd *cmd, ...);
void bob_cmd_destroy(Bob_Cmd *cmd);
void bob_cmd_run(Bob_Cmd *cmd);

//----------------------//
// Dynamic Array Macros //
//----------------------//

/*
 * The `da` argument should be a struct like following:
 * typedef struct {
 *  <TypeName> *items;
 *  size_t len;
 *  size_t cap;
 * } Dynamic_Array_Struct;
 *
 * NOTE - initialize as follows:
 *  Dynamic_Array_Struct da = {0};  // to make sure all fields are zero-initialized
 */

#define BOB_DA_INITIAL_CAP 8

#define bob_da_free(da)    \
    do {                   \
        free((da)->items); \
        (da)->len = 0;     \
        (da)->cap = 0;     \
    } while(0)             \

#define bob_da_append(da, item)                                                     \
    do {                                                                            \
        if ((da)->len >= (da)->cap)                                                 \
        {                                                                           \
            (da)->cap = (da)->cap==0 ? BOB_DA_INITIAL_CAP : (da)->cap*2;            \
            (da)->items = realloc((da)->items, (da)->cap * sizeof(*(da)->items));   \
            assert((da)->items != NULL);                                            \
        }                                                                           \
        (da)->items[(da)->len++] = item;                                            \
    } while(0)

#define bob_da_reserve(da, needed_size)                                             \
    do {                                                                            \
        if((da)->cap < needed_size)                                                 \
        {                                                                           \
            (da)->cap = needed_size;                                                \
            (da)->items = realloc((da)->items, (da)->cap * sizeof(*(da)->items));   \
            assert((da)->items != NULL);                                            \
        }                                                                           \
    } while(0)

#ifdef BOB_IMPL
//----------------//
// String Builder //
//----------------//
Bob_StringBuilder bob_sb_create() {
    Bob_StringBuilder sb = {0};
    sb.cap = 16;
    sb.data = malloc(sb.cap);
    sb.len = 0;
    return sb;
}

Bob_StringBuilder bob_sb_from_parts(const char *data, size_t len) {
    Bob_StringBuilder sb = {0};
    sb.cap = len;
    sb.data = malloc(sb.cap);
    memcpy(sb.data, data, len);
    sb.len = len;
    return sb;
}

Bob_StringBuilder bob_sb_from_cstr(const char *str) {
    return bob_sb_from_parts(str, strlen(str));
}

void bob_sb_destroy(Bob_StringBuilder *sb) {
    free(sb->data);
    sb->cap = 0;
    sb->len = 0;
}

void bob_sb_realloc_if_required(Bob_StringBuilder *sb, size_t required_size) {
    while (sb->cap < required_size) {
        sb->cap *= 2;
        sb->data = realloc(sb->data, sb->cap);
    }
}

const char *bob_sb_get_cstr(Bob_StringBuilder *sb) {
    bob_sb_realloc_if_required(sb, sb->len+1);

    if (sb->data[sb->len] != '\0')
        sb->data[sb->len] = '\0';

    return sb->data;
}

void bob_sb_append(Bob_StringBuilder *primary, Bob_StringBuilder *secondary) {
    bob_sb_append_parts(primary, secondary->data, secondary->len);
}

void bob_sb_append_parts(Bob_StringBuilder *sb, const char *data, size_t len) {
    size_t required_len = sb->len + len;
    bob_sb_realloc_if_required(sb, required_len);
    memcpy(sb->data + sb->len, data, len);
    sb->len = required_len;
}

void bob_sb_append_cstr(Bob_StringBuilder *sb, const char* str) {
    bob_sb_append_parts(sb, str, strlen(str));
}

//------------------------//
// Bob Command or Bob_Cmd //
//------------------------//

void bob_cmd_append(Bob_Cmd *cmd, ...) {
    va_list args;
    va_start(args, cmd);

    const char *arg = va_arg(args, const char *);
    while(arg) {
        // make string builder from argument
        // and append to cmd 'args'
        Bob_StringBuilder sb_arg = bob_sb_from_cstr(arg);
        bob_da_append(cmd, sb_arg);

        // do this at the end of iteration
        arg = va_arg(args, const char *);
    }
    va_end(args);
}

void bob_cmd_destroy(Bob_Cmd *cmd) {
    for (size_t i=0; i<cmd->len; i++) {
        bob_sb_destroy(&cmd->items[i]);
    }
    bob_da_free(cmd);
}

void bob_cmd_run(Bob_Cmd *cmd) {
    Bob_StringBuilder complete_cmd = bob_sb_create();

    for (size_t i=0; i<cmd->len; i++) {
        Bob_StringBuilder *arg = &cmd->items[i];
        bob_sb_append_cstr(&complete_cmd, " ");
        bob_sb_append(&complete_cmd, arg);
    }

    int ret = system(bob_sb_get_cstr(&complete_cmd));
    if (ret != 0) {
        printf("Following command could not be run successfully:\n>   %s\n", bob_sb_get_cstr(&complete_cmd));
    }

    bob_sb_destroy(&complete_cmd);
}
#endif // BOB_IMPL
