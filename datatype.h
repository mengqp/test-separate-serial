/*******************************************************************************
 * 文件名:datatype.h
 * 文件描述:一些基本的数据定义
 * 创建日期:2018/04/18 09:11:15
 * 版本：Ver1.0
 *
 * Copyright © 2018 - 2018 mengqp.
 *
 * 历史版本：
 * 修改人                修改时间                修改版本        修改内容
 *
 *
 ******************************************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H

////////////////////////////////////////////////////////////////////////////////
// 全局数据类型及数据结构定义
typedef signed char				s08_t;
typedef signed short				s16_t;
typedef signed int				s32_t;
typedef unsigned char			u08_t;
typedef unsigned short			u16_t;
typedef unsigned int				u32_t;
typedef long long int				s64_t;
typedef unsigned long long int		u64_t;
typedef float					f32_t;
typedef double					f64_t;

typedef enum {SUCCESS, FAIL = !SUCCESS} status_t;
typedef enum {INVALID, VALID = !INVALID} item_status_t;


#endif /* DATATYPE_H */
