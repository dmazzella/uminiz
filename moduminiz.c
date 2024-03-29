/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Damiano Mazzella
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if defined(MICROPY_PY_UMINIZ)

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "py/objstr.h"
#include "py/objint.h"
#include "py/runtime.h"

#include "miniz.h"

STATIC mp_obj_t uminiz_compress(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    enum
    {
        ARG_level,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_level, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 6}},
    };

    mp_arg_val_t vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, vals);

    mp_buffer_info_t bufinfo_data;
    mp_get_buffer_raise(args[0], &bufinfo_data, MP_BUFFER_READ);
    mp_int_t level = vals[ARG_level].u_int;

    if (level < 0 || level > 9)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("level must be in range 0-9"));
    }

    vstr_t vstr_out;
    vstr_init_len(&vstr_out, compressBound(bufinfo_data.len));

    mp_int_t ret = 0;
    if ((ret = compress2((byte *)vstr_out.buf, (mz_ulong *)&vstr_out.len, bufinfo_data.buf, bufinfo_data.len, level)) != Z_OK)
    {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("compress: %d"), ret);
    }

    mp_obj_t out = mp_obj_new_bytes((const byte *)vstr_out.buf, vstr_out.len);
    vstr_clear(&vstr_out);
    return out;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_KW(fastlz_compress_obj, 1, uminiz_compress);

STATIC mp_obj_t uminiz_decompress(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    enum
    {
        ARG_maxout
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_maxout, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 1024}},
    };

    mp_arg_val_t vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, vals);

    mp_buffer_info_t bufinfo_data;
    mp_get_buffer_raise(args[0], &bufinfo_data, MP_BUFFER_READ);

    vstr_t vstr_out;
    vstr_init_len(&vstr_out, vals[ARG_maxout].u_int);

    mp_int_t ret = 0;
    if ((ret = uncompress((byte *)vstr_out.buf, (mz_ulong *)&vstr_out.len, bufinfo_data.buf, bufinfo_data.len)) != Z_OK)
    {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("decompress: %d"), ret);
    }

    mp_obj_t out = mp_obj_new_bytes((const byte *)vstr_out.buf, vstr_out.len);
    vstr_clear(&vstr_out);
    return out;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_KW(fastlz_decompress_obj, 1, uminiz_decompress);

STATIC const mp_rom_map_elem_t mp_module_uminiz_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR__fastlz)},
    {MP_ROM_QSTR(MP_QSTR_compress), MP_ROM_PTR(&fastlz_compress_obj)},
    {MP_ROM_QSTR(MP_QSTR_decompress), MP_ROM_PTR(&fastlz_decompress_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_uminiz_globals, mp_module_uminiz_globals_table);

const mp_obj_module_t mp_module_uminiz = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&mp_module_uminiz_globals,
};

// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR__miniz, mp_module_uminiz);

#endif // MICROPY_PY_UMINIZ
