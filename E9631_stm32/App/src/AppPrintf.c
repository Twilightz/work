/*=============================================
	Author: walter.chen
	Email : walnutcy@gmail.com
	Weibo : http://weibo.com/walnutcy
	Buid  : 2012.08.26 for RT Prj input
*/


#include "stdarg.h"
#include "math.h"
#include "AppPrintf.h"

#define APPDIM(array)  (sizeof(array)/sizeof(array[0]))

const unsigned int _applib_s_pow10[10] =
{
    1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000
};

const unsigned int _applib_s_pow16[8] =
{ 1, 0x10, 0x100, 0x1000, 0x10000, 0x100000, 0x1000000, 0x10000000};

const unsigned int _applib_s_pow8[11] =
{
    1, 010, 0100, 01000, 010000, 0100000,
    01000000, 010000000, 0100000000, 01000000000, 010000000000
};



#define LIBDEF_PRINTF_FLOATING  1
#define LIBDEF_PRINTF_LONG      1
#define __DOUBLE_IS_IEEE64__    // __DOUBLE_IS_IEEE32__
#define LIBDEF_PRINTF_ALLOW_OCTINT      0
#define LIBDEF_PRINTF_ALLOW_HEXINT      1
#define LIBDEF_PRINTF_FAR_PTR           0

//========================================================================


static void _out(char c, APPLIB_PRINT_ST *desc)
{
    desc->outc(c, (desc));
}

#define out(c) _out(c,desc); pbuf_i++





#if LIBDEF_PRINTF_LONG
#define INTEG long
#else
#define INTEG int
#endif

#define OCTINT  0x1
#define HEXINT  0x2
#define NOT_DEC (HEXINT | OCTINT)

#if LIBDEF_PRINTF_FLOATING
#define ETYPE 0x1
#define FTYPE 0x2
#define GTYPE (ETYPE | FTYPE)
#endif

#define BLANK      0x4
#define SIGNED     0x8
#define NEG       (SIGNED | BLANK)

#define UPPER     0x10
#define LEFT      0x20
#define ZEROPAD   0x40
#define ALT       0x80
#define LONGVAL  0x100
#define SHORTVAL 0x200
#define PREC     0x400
#define PTR      0x800

#define LIBDEF_CONST_DPTRQ


#if LIBDEF_PRINTF_FLOATING
const LIBDEF_CONST_DPTRQ static double fpow10[] = {1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e16, 1e24, 1e32};
const LIBDEF_CONST_DPTRQ static double fpow_10[] = {1e0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-16, 1e-24, 1e-32};

#ifdef __DOUBLE_IS_IEEE64__
#define EXPON int
#else
#define EXPON signed char
#endif
#endif

#if LIBDEF_PRINTF_FLOATING
static double Ten(EXPON e)
{
    const double *LIBDEF_CONST_DPTRQ arr = fpow10;

    if (e < 0)
    {
        arr = fpow_10;
        e = -e;
    }
    if (e <= 8)
    {
        return arr[e];
    }
    else
    {
        double res = 1;
        unsigned int i;
        i = e >> 3;
        if (i > 4)
        {
            i = 4;
        }
        e -= i << 3;
        res = arr[i+7];
        while (e > 8)
        {
            i = e >> 3;
            if (i > 4)
            {
                i = 4;
            }
            e -= i << 3;
            res *= arr[i+7];
        }
        return res * arr[e];
    }
}
#endif


#ifdef __DOUBLE_IS_IEEE32__
#define DIGITS  6
#define BOUND   1.0E-6F
#endif
#ifdef __DOUBLE_IS_IEEE64__
#define DIGITS  14
#define BOUND   1.0E-14F
#endif
#ifdef __DOUBLE_IS_DSP__
#define DIGITS  4
#define BOUND   1.0E-4F
#endif
#ifndef DIGITS
/* Just to catch V2.6 M68k compiler: doesn't set these flags!!!! */
#define DIGITS  14
#define BOUND   1.0E-14F
#endif

extern char AppLib_tolower(char /*c*/);
/* if c is an upper-case letter then return the corresponding */
/* lower-case letter, otherwise return c.                     */
char toupper(char c)
{
	char ch1;
	ch1 = c&0xDF;
	return ch1;/* c = c-'a'+'A';*/
}
/* if c is an lower-case letter then return the corresponding */
/* upper-case letter, otherwise return c.                     */
/*****************************************************/
#define p_abs(v)     ((v) < 0 ? -(v) : (v))
/*****************************************************/

#define F_DIGITS  (APPDIM(_applib_s_pow10)-2)

#define str _local._str
#define val _local._num._val
#define pwr _local._num._pow
#define fval  _local._num._flt
#define dig c
#define expon  _local._num._exp
#define exp0 _local._int



int AppLib_vprintf(APPLIB_ConstStrPtr format, va_list args, APPLIB_PRINT_ST *desc)
{
    unsigned int flags;
    unsigned short prec, len;
    int width;
    char c;
    union
    {
        char *_str;
        int _int;
        struct
        {
            unsigned INTEG _val;
            unsigned INTEG _pow;
#if LIBDEF_PRINTF_FLOATING
            double _flt;
            EXPON _exp;
#endif
        } _num;
    } _local;
    int pbuf_i = 0; // 2008.12.01

    while (*format)
    {
        if (*format != '%')
        {
            out(*format);
            ++format;
            continue;
        }
        else
        {
            flags = 0;
            prec = 0;
            width = 0;
            for (;;)
            {
                ++format;
                switch (*format)
                {
                case '-':
                    flags |= LEFT;
                    continue;
                case '+':
                    flags |= SIGNED;
                    continue;
                case ' ':
                    flags |= BLANK;
                    continue;
                case '0':
                    flags |= ZEROPAD;
                    continue;
                case '#':
                    flags |= ALT;
                    continue;
                }
                break;
            }
            if (flags & SIGNED)
            {
                flags &= ~BLANK;
            }
            if (flags & LEFT)
            {
                flags &= ~ZEROPAD;
            }

            /* field width */
            if (*format == '*')
            {
                width = va_arg(args, int);
                ++format;
            }
            else
            {
                c = *format;
                while (c >= '0' && c <= '9')
                {
                    width = 10 * width + (c - '0');
                    ++format;
                    c = *format;
                }
            }
            /* precision */
            if (*format == '.')
            {
                flags |= PREC;
                ++format;
                if (*format == '*')
                {
#if (_C_COMPILER_USE == _SOURCERY_GCC)
                    prec = va_arg(args, unsigned int); /* 2009.02.01 */
#else
                    prec = va_arg(args, unsigned short);
#endif
                    ++format;
                }
                else
                {
                    c = *format;
                    while (c >= '0' && c <= '9')
                    {
                        prec = 10 * prec + (c - '0');
                        ++format;
                        c = *format;
                    }
                }
            }
            /* long or short argument */
            if (toupper(*format) == 'L') /* support %lu --walter 20080826*/
            {
                flags |= LONGVAL;
                ++format;
            }
            else if (*format == 'h')
            {
                flags |= SHORTVAL;
                ++format;
            }

            /* conversions */

            switch (*format++)
            {
            case '%':
                out('%');
                continue;

#ifdef _N_OPTION
#if LIBDEF_PRINTF_FAR_PTR
            case 'N': /* return nof chars */
                if (flags & SHORTVAL)
                {
                    *va_arg(args, short int * FAR) = pbuf_i;
#if LIBDEF_PRINTF_LONG
                }
                else if (flags & LONGVAL)
                {
                    *va_arg(args, long int * FAR) = pbuf_i;
#endif
                }
                else
                {
                    *va_arg(args, int * FAR) = pbuf_i;
                }
                continue;
#endif
            case 'n': /* return nof chars */
                if (flags & SHORTVAL)
                {
                    *va_arg(args, short int *) = pbuf_i;
#if LIBDEF_PRINTF_LONG
                }
                else if (flags & LONGVAL)
                {
                    *va_arg(args, long int *) = pbuf_i;
#endif
                }
                else
                {
                    *va_arg(args, int *) = pbuf_i;
                }
                continue;
#endif

            case 'c': /* char */
                c = va_arg(args, int);
                str = (char *) &c;
                len = 1;
                goto string;

#if LIBDEF_PRINTF_FAR_PTR
            case 'S':
                flags |= FARVAL;
                str = va_arg(args, char * FAR);
                len = 0;
                if (str)
                {
                    while (str[len])
                    {
                        ++len;
                    }
                }
                goto string;
#endif
            case 's':  /* string */
                str = va_arg(args, char *);
                len = 0;
                if (str)
                {
                    while (str[len])
                    {
                        ++len;
                    }
                }
string:
                /* adjust length to precision */
                if (prec && (prec < len))
                {
                    len = prec;
                }
                if (width > len)
                {
                    width -= len;
                }
                else
                {
                    width = 0;
                }
                /* pad left */
                if (!(flags & LEFT))
                {
                    while (width)
                    {
                        out(' ');
                        --width;
                    }
                }
                /* output string */
                while (len)
                {
                    out(*str);
                    ++str;
                    --len;
                }
                /* pad right */
                while (width)
                {
                    out(' ');
                    --width;
                }
                continue;

#if LIBDEF_PRINTF_FAR_PTR
            case 'P':
                flags |= FARVAL;
#endif
            case 'p':
                if (sizeof(void *) != sizeof(unsigned int))
                {
                    flags |= PTR;
                }
#if LIBDEF_PRINTF_ALLOW_HEXINT
            case 'X': /* integer */
                flags |= UPPER;
            case 'x':
                flags |= HEXINT;
                goto unsig_int;
#endif
#if LIBDEF_PRINTF_ALLOW_OCTINT
            case 'o':
                flags |= OCTINT;
#endif
            case 'u':

unsig_int:
                /* get unsigned integer */
                flags &= ~NEG;
#if LIBDEF_PRINTF_FAR_PTR
                if (flags & FARVAL)
                {
                    val = (unsigned INTEG) va_arg(args, char * FAR);
                }
                else
#endif
                    if (flags & SHORTVAL)
                    {
#if (_C_COMPILER_USE == _SOURCERY_GCC)
                        val = va_arg(args, unsigned int); /* 2009.02.01 */
#else
                        val = va_arg(args, unsigned short);
#endif
#if LIBDEF_PRINTF_LONG
                    }
                    else if (flags & LONGVAL)
                    {
                        val = va_arg(args, unsigned long);
#endif
                    }
                    else
                    {
                        if (sizeof(void *) != sizeof(unsigned int) && (flags & PTR) != 0)
                        {
                            /* support for %p for targets with non int pointer sizes */
                            val = (unsigned INTEG)va_arg(args, void *);
                        }
                        else
                        {
                            val = va_arg(args, unsigned int);
                        }
                    }
                goto integer;

            case 'i':
            case 'd':
                /* get signed integer */
                if (flags & SHORTVAL)
                {
#if (_C_COMPILER_USE == _SOURCERY_GCC)
                    val = va_arg(args, int); /* 2009.02.01 */
#else
                    val = va_arg(args, short);
#endif
#if LIBDEF_PRINTF_LONG
                }
                else if (flags & LONGVAL)
                {
                    val = va_arg(args, long);
#endif
                }
                else
                {
                    val = va_arg(args, int);
                }

                if ((INTEG) val < 0)
                {
                    val = (-(INTEG)val);
                    flags |= NEG;
                }

integer:

                len = 1;
#if LIBDEF_PRINTF_ALLOW_OCTINT && LIBDEF_PRINTF_ALLOW_HEXINT
                if (flags & OCTINT)
                {
                    while (len < APPDIM(_applib_s_pow8) && val >= _applib_s_pow8[len])
                    {
                        ++len;
                    }
                }
                else if (flags & HEXINT)
                {
                    while (len < APPDIM(_applib_s_pow16) && val >= _applib_s_pow16[len])
                    {
                        ++len;
                    }
                }
                else
                {
                    while (len < APPDIM(_applib_s_pow10) && val >= _applib_s_pow10[len])
                    {
                        ++len;
                    }
                }
#elif LIBDEF_PRINTF_ALLOW_OCTINT
                if (flags & OCTINT)
                {
                    while (len < APPDIM(_applib_s_pow8) && val >= _applib_s_pow8[len])
                    {
                        ++len;
                    }
                }
                else
                {
                    while (len < APPDIM(_applib_s_pow10) && val >= _applib_s_pow10[len])
                    {
                        ++len;
                    }
                }
#elif LIBDEF_PRINTF_ALLOW_HEXINT
                if (flags & HEXINT)
                {
                    while (len < APPDIM(_applib_s_pow16) && val >= _applib_s_pow16[len])
                    {
                        ++len;
                    }
                }
                else
                {
                    while (len < APPDIM(_applib_s_pow10) && val >= _applib_s_pow10[len])
                    {
                        ++len;
                    }
                }
#else
                while (len < APPDIM(_applib_s_pow10) && val >= _applib_s_pow10[len])
                {
                    ++len;
                }
#endif

                /* width of integer */
                if (prec > len)
                {
                    prec -= len;
                }
                else
                {
                    prec = 0;
                }

                width -= (prec + len);

#if LIBDEF_PRINTF_ALLOW_OCTINT && LIBDEF_PRINTF_ALLOW_HEXINT
                if (flags & ALT)
                {
                    if (flags & OCTINT)
                    {
                        --width;
                    }
                    else if (flags & HEXINT)
                    {
                        width -= 2;
                    }
                }
#elif LIBDEF_PRINTF_ALLOW_OCTINT
                if ((flags & (ALT | OCTINT)) == (ALT | OCTINT))
                {
                    --width;
                }
#elif LIBDEF_PRINTF_ALLOW_HEXINT
                if ((flags & (ALT | HEXINT)) == (ALT | HEXINT))
                {
                    width -= 2;
                }
#endif
                if (flags & NEG)
                {
                    --width;
                }

                if (width < 0)
                {
                    width = 0;
                }

                /* pad left */
                if (width && !(flags & LEFT))
                {
                    if (!(flags & ZEROPAD))
                    {
                        /* pad with spaces */
                        do
                        {
                            out(' ');
                            --width;
                        }
                        while (width);
                    }
                    else
                    {
                        prec += width;
                        width = 0; /* pad with zeros */
                    }
                }

                /* alternate form of integers for oct and hex numbers */
#if LIBDEF_PRINTF_ALLOW_HEXINT && LIBDEF_PRINTF_ALLOW_OCTINT
                if ((flags & ALT) && (flags & NOT_DEC))
                {
                    out('0');
                    if (flags & HEXINT)
                    {
                        if (flags & UPPER)
                        {
                            out('X');
                        }
                        else
                        {
                            out('x');
                        }
                    }
                }
#elif LIBDEF_PRINTF_ALLOW_HEXINT
                if ((flags & (ALT | HEXINT)) == (ALT | HEXINT))
                {
                    out('0');
                    if (flags & UPPER)
                    {
                        out('X');
                    }
                    else
                    {
                        out('x');
                    }
                }
#elif LIBDEF_PRINTF_ALLOW_OCTINT
                if ((flags & (ALT | OCTINT)) == (ALT | OCTINT))
                {
                    out('0');
                }
#endif

                /* sign */
                if (flags & SIGNED)
                {
                    if (flags & BLANK)
                    {
                        out('-');
                    }
                    else
                    {
                        out('+');
                    }
                }
                else if (flags & BLANK)
                {
                    out(' ');
                }

                /* pad with zeros to precision */
                while (prec)
                {
                    out('0');
                    --prec;
                }

                /* output conversion. par: val, len; local: dig, pwr*/
                do
                {
                    dig = 0;
                    --len;
#if LIBDEF_PRINTF_ALLOW_OCTINT && LIBDEF_PRINTF_ALLOW_HEXINT
                    if (flags & OCTINT)
                    {
                        pwr = _applib_s_pow8[len];
                    }
                    else if (flags & HEXINT)
                    {
                        pwr = _applib_s_pow16[len];
                    }
                    else
                    {
                        pwr = _applib_s_pow10[len];
                    }
#elif LIBDEF_PRINTF_ALLOW_OCTINT
                    if (flags & OCTINT)
                    {
                        pwr = _applib_s_pow8[len];
                    }
                    else
                    {
                        pwr = _applib_s_pow10[len];
                    }
#elif LIBDEF_PRINTF_ALLOW_HEXINT
                    if (flags & HEXINT)
                    {
                        pwr = _applib_s_pow16[len];
                    }
                    else
                    {
                        pwr = _applib_s_pow10[len];
                    }
#else
                    pwr = _applib_s_pow10[len];
#endif

                    while (val >= pwr)
                    {
                        val -= pwr;
                        ++dig;
                    }
#if LIBDEF_PRINTF_ALLOW_HEXINT
                    if (dig < 10)
                    {
                        out(dig + '0');
                    }
                    else if (flags & UPPER)
                    {
                        out(dig + ('A' - 10));
                    }
                    else
                    {
                        out(dig + ('a' - 10));
                    }
#else
                    out(dig + '0');
#endif
                }
                while (len);

                /* append spaces to field width */
                while (width)
                {
                    out(' ');
                    --width;
                }
                continue;

#if LIBDEF_PRINTF_FLOATING
            case 'E': /* float */
                flags |= UPPER;
                /* fall through */
            case 'e':
                flags |= ETYPE;
                goto real;

            case 'f':
                flags |= FTYPE;
                goto real;

            case 'G':
                flags |= UPPER;
                /* fall through */
            case 'g':
                flags |= GTYPE;
                /* fall through */
real:
                if (!(flags & PREC))  /* default precision */
                {
                    prec = 6;
                }

                fval = va_arg(args, double); 
                if (fval < 0.0)
                {
                    fval = (-fval);
                    flags |= NEG;
                }

                expon = 0;
                if (fval != 0.0)
                {
#ifdef __DOUBLE_IS_IEEE32__
                    //            (void)frexpf(fval, &exp0);
                    frexpf(fval, &exp0);
#else
                    //            (void)frexp(fval, &exp0);
                    frexp(fval, &exp0);
#endif
                    expon = (exp0 * 3) / 10;  /*ln(2)/ln(10) = 0.30102999..*/
                }

                if (flags & ETYPE && (!(flags & FTYPE) || (expon < -4) || (expon >= (int) prec)))
                {
                    /* base conversion */
                    if (fval != 0.0)
                    {
                        fval *= Ten(-expon);
                        if (fval != 0.0)
                        {
                            while (fval >= 10.0)
                            {
                                fval *= 1e-1;
                                ++expon;
                            }
                            while (fval < 1.0)
                            {
                                fval *= 10.0;
                                --expon;
                            }
                        }
                    }
                    /* x = fval * 10 ^ expon; 1 <= fval < 10 */
                    /* e type conversion */
                    if (prec && flags & FTYPE)  /* for g type: prec = nof significant digits */
                    {
                        --prec;
                    }

                    if (prec < F_DIGITS)
                    {
                        fval += 0.5 * Ten(-(EXPON)prec);
                    }
                    /* if rounding causes overflow */
                    if (fval >= 10.0)
                    {
                        fval *= 1e-1;
                        ++expon;
                    }
                    /*adjust precision for g type conversion (remove trailing zeros)*/
                    if (prec && flags & FTYPE && !(flags & ALT))
                    {
                        if (prec > F_DIGITS)
                        {
                            prec = F_DIGITS;
                        }

                        val = fval * Ten(prec);

                        if (val)
                        {
                            while (prec && (val % 10 == 0))
                            {
                                val /= 10;
                                --prec;
                            }
                        }
                        else
                        {
                            prec = 0;
                        }
                    }
                    flags &= ~FTYPE;

                    /* width of e type conversion */
                    width -= (5 + prec);    /*minimum: dE+dd*/
                    if (prec || flags & ALT)   /*decimal point*/
                    {
                        --width;
                    }
                    if (flags & NEG)       /*sign or blank*/
                    {
                        --width;
                    }
                    if (p_abs(expon) >= 100)         /*additional digit for exponent*/
                    {
                        --width;
                    }

                    val = fval;
                    fval -= (double) val;
                    len = 1;
                }
                else     /* f type conversion */
                {
                    /*float = fval * 10 ^ expon; fval is not restricted to 1 <= fval < 10 !!!*/

                    /*adjust precision for g type conversion (trailing zeros)*/
                    if (prec && flags & ETYPE && !(flags & ALT))
                    {

                        prec -= (expon + 1); /*prec >= expon*/

                        if (expon < 0 || fval < 1.0)   /*leading 0 is not significant*/
                        {
                            ++prec;
                        }

                        if (prec > F_DIGITS)
                        {
                            prec = F_DIGITS;
                        }

                        val = fval * Ten(prec) + 0.5;

                        while (prec && !(val % 10))
                        {
                            val /= 10;
                            --prec;
                        }
                    }
                    flags &= ~ETYPE;

                    if ((EXPON) (expon - F_DIGITS) >= -(EXPON)prec)
                    {
                        fval += 0.5 * Ten(expon - F_DIGITS);
                    }
                    else
                    {
                        fval += 0.5 * Ten(-(EXPON)prec);
                    }

                    /* width of type f conversion */
                    if (expon >= 0)  /* (expon + 1) digits before dec pt */
                    {
                        if (expon > F_DIGITS)
                        {
                            expon -= F_DIGITS;
                            val = fval * Ten(-expon);
                            fval = 0.0;
                        }
                        else
                        {
                            val = fval;
                            fval -= (double) val;
                            expon = 0;
                        }
                    }
                    else
                    {
                        val = 0;
                        expon = 0;
                    }

                    len = 1;
                    while (len < APPDIM(_applib_s_pow10) && val >= _applib_s_pow10[len])
                    {
                        ++len;
                    }

                    width -= (prec + len + expon);

                    if (prec || flags & ALT)   /*decimal point*/
                    {
                        --width;
                    }
                    if (flags & NEG)       /*sign or blank*/
                    {
                        --width;
                    }

                }

                if (width < 0)
                    width = 0;

                /* pad left */
                if (!(flags & LEFT))
                {
                    if (!(flags & ZEROPAD))  /*pad with spaces before sign*/
                    {
                        while (width)
                        {
                            out(' ');
                            --width;
                        }
                    }
                }
                /*!(flags & LEFT) && !(flags & ZEROPAD) => (width == 0)*/

                /*sign*/
                if (flags & SIGNED)
                {
                    if (flags & BLANK)
                    {
                        out('-');
                    }
                    else
                    {
                        out('+');
                    }
                }
                else if (flags & BLANK)
                {
                    out(' ');
                }

                if (!(flags & LEFT))   /*pad with zeros after sign (if width still > 0)*/
                {
                    while (width)
                    {
                        out('0');
                        --width;
                    }
                }

                /* float conversion */

                /*print all digits before decimal point*/
                do
                {
                    --len;
                    dig = '0';
                    pwr = _applib_s_pow10[len];
                    while (val >= pwr)
                    {
                        val -= pwr;
                        ++dig;
                    }
                    out(dig);
                }
                while (len);

                if (flags & FTYPE)  /* add zeros before decimal point */
                {
                    while (expon > 0)
                    {
                        out('0');
                        --expon;
                    }
                }

                if (prec || flags & ALT)
                {
                    out('.');
                    if(prec > 0)
                    {
                        if (prec > F_DIGITS)
                        {
                            len = F_DIGITS;
                        }
                        else
                        {
                            len = prec;
                        }

                        prec -= len;

                        if (flags & FTYPE)
                        {
                            val = fval * Ten(len - expon); /*10^(-expon) --> 1 <= fval < 10 */
                        }
                        else
                        {
                            val = fval * Ten(len);
                        }

                        /*out int val */
                        do
                        {
                            --len;
                            dig = '0';
                            pwr = _applib_s_pow10[len];
                            while (val >= pwr)
                            {
                                val -= pwr;
                                ++dig;
                            }
                            out(dig);
                        }
                        while (len);

                        while (prec)
                        {
                            out('0');
                            --prec;
                        }
                    } /* if prec > 0 */
                }
                if (flags & ETYPE)   /*exponent*/
                {
                    if (flags & UPPER)
                    {
                        out('E');
                    }
                    else
                    {
                        out('e');
                    }
                    if (expon < 0)
                    {
                        expon = (-expon);
                        out('-');
                    }
                    else
                    {
                        out('+');
                    }

                    if (expon >= 100)
                    {
                        out(expon / 100 + '0');
                        expon %= 100;
                    }
                    out(expon / 10 + '0');
                    out(expon % 10 + '0');
                }

                /* pad right */
                while (width)
                {
                    out(' ');
                    --width;
                }
#else
            case 'E':
            case 'e':
            case 'f':
            case 'G':
            case 'g':
                va_arg(args, double); /* drop it */
#endif
            default:
                continue;

            } /* switch (*format++) */
        } /* if (*format != '%') */
    } /* while (*format) */
    return (pbuf_i);
} /* vprintf */


static void deposit_char(char c, APPLIB_PRINT_ST *p)
{
    p->s[p->i] = c;
    p->i++;
}

int AppLib_sprintf(char *s, const char *format, ...)
{
    va_list args;
    APPLIB_PRINT_ST p;

    p.i = 0;
    p.s = s;
    p.outc = (Printf_CallBack)deposit_char;
    va_start(args, format);
    AppLib_vprintf(format, args, (APPLIB_PRINT_ST *)&p);
    p.s[p.i] = 0;
    va_end(args);

    return p.i;
}


int AppLib_vsprintf(char *s, const char *format, va_list args)
{
    APPLIB_PRINT_ST p;

    p.i = 0;
    p.s = s;
    p.outc = (Printf_CallBack)deposit_char;
    AppLib_vprintf(format, args, (APPLIB_PRINT_ST *)&p);
    p.s[p.i] = 0;

    return p.i;
}



