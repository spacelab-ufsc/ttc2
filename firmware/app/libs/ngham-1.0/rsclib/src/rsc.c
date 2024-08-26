/* 
 * rsc.c
 * 
 * Copyright The RSCLib Contributors.
 * 
 * This file is part of RSCLib.
 * 
 * RSCLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RSCLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with RSCLib. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Reed-Solomon C library implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 1.0.1
 *
 * \date 2024/05/09
 * 
 * \addtogroup rsclib
 * \{
 */

#include <string.h>
#include "rsc/rsc.h"

/**
 * \brief Computes the modulo of a given number.
 *
 * \param[in] rs is the Reed-Solomon scheme used to compute the modulo.
 *
 * \param[in] x is the integer to compute the modulo.
 *
 * \return The computed modulo value of the given number.
 */

static int modnn(reed_solomon_t *rs, int num);

static int min(int a, int b);

int rsc_init(uint32_t symsize, int gfpoly, uint32_t fcr, uint32_t prim, uint32_t nroots, uint32_t pad, reed_solomon_t *rs)
{
    int err = 0;

    uint32_t i;
    uint32_t j;
    uint32_t sr;
    uint32_t root;
    uint32_t iprim;

    /* Check parameter ranges */
    if (symsize <= (8U * sizeof(uint8_t)))
    {
        if (fcr < (1UL << symsize))
        {
            if ((prim > 0U) && ( prim < (1U << symsize)))
            {
                if (( nroots < (1U << symsize)))
                {
                    if ((pad < ((1U << symsize) - 1U - nroots)))
                    {
                        rs->mm = symsize;
                        rs->nn = (1U << symsize) - 1U;
                        rs->pad = pad;

                        /* Generate Galois field lookup tables */
                        rs->index_of[0] = rs->nn;   /* log(zero) = -inf */
                        rs->alpha_to[rs->nn] = 0;   /* alpha**-inf = 0 */
                        sr = 1U;
                        for(i = 0; i < rs->nn; i++)
                        {
                            rs->index_of[sr] = i;
                            rs->alpha_to[i] = sr;
                            sr <<= 1;
                            if ((sr & (1U << symsize)) != 0U)
                            {
                                sr ^= gfpoly;
                            }
                            sr &= rs->nn;
                        }
                        if (sr == 1U)
                        {
                            rs->fcr = fcr;
                            rs->prim = prim;
                            rs->nroots = nroots;

                            /* Find prim-th root of 1, used in decoding */
                            for(iprim = 1U; (iprim % prim) != 0U; iprim += rs->nn)
                            {
                            }
                            rs->iprim = iprim / prim;

                            rs->genpoly[0] = 1U;

                            root = fcr * prim;
                            for(i = 0U; i < nroots; i++)
                            {
                                if (i != 0U)
                                {
                                    root += prim;
                                }

                                rs->genpoly[i + 1U] = 1U;

                                /* Multiply rs->genpoly[] by  @**(root + x) */
                                for(j = i; j > 0U; j--)
                                {
                                    if (rs->genpoly[j] != 0U)
                                    {
                                        rs->genpoly[j] = rs->genpoly[j - 1U] ^ rs->alpha_to[modnn(rs, rs->index_of[rs->genpoly[j]] + (uint32_t)root)];
                                    }
                                    else
                                    {
                                        rs->genpoly[j] = rs->genpoly[j - 1U];
                                    }
                                }
                                /* rs->genpoly[0] can never be zero */
                                rs->genpoly[0] = rs->alpha_to[modnn(rs, rs->index_of[rs->genpoly[0]] + (uint32_t)root)];
                            }
                            /* convert rs->genpoly[] to index form for quicker encoding */
                            for(i = 0U; i <= nroots; i++)
                            {
                                rs->genpoly[i] = rs->index_of[rs->genpoly[i]];
                            }

                            err = 0;
                        }
                        else
                        {
                            /* field generator polynomial is not primitive! */
                            (void)memset(rs, 0, sizeof(reed_solomon_t));

                            err = -1;
                        }
                    }
                    else
                    {
                        err = 0;    /* Too much padding */
                    }
                }
                else
                {
                    err = -1;   /* Can't have more roots than symbol values! */
                }
            }
            else
            {
                err = -1;
            }
        }
        else
        {
            err = -1;
        }
    }
    else
    {
        err = -1;
    }

    return err;
}

void rsc_encode(reed_solomon_t *rs, uint8_t *data, uint8_t *parity, uint8_t *parity_len)
{
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t feedback = 0;

    (void)memset(parity, 0, rs->nroots);

    for(i = 0U; i < (rs->nn - rs->nroots - rs->pad); i++)
    {
        feedback = rs->index_of[data[i] ^ parity[0]];
        if (feedback != rs->nn)  /* feedback term is non-zero */
        {
            for(j = 0U; j < rs->nroots; j++)
            {
	            parity[j] ^= rs->alpha_to[modnn(rs, feedback + rs->genpoly[rs->nroots - j])];
            }
        }
        /* Shift */
        (void)memmove(&parity[0], &parity[1], rs->nroots - 1U);
        if (feedback != rs->nn)
        {
            parity[rs->nroots - 1U] = rs->alpha_to[modnn(rs, feedback + rs->genpoly[0])];
        }
        else
        {
            parity[rs->nroots - 1U] = 0U;
        }
    }

    *parity_len = rs->nroots;
}

int rsc_decode(reed_solomon_t *rs, uint8_t *data, int *err_pos, int *num_err)
{
    int err                 = -1;

    int deg_lambda          = 0;
    int el                  = 0;
    int deg_omega           = 0;

    int r                   = 0;
    int i                   = 0;
    int j                   = 0;
    int k                   = 0;

    uint8_t u               = 0;
    uint8_t q               = 0;
    uint8_t tmp             = 0;
    uint8_t num1            = 0;
    uint8_t num2            = 0;
    uint8_t den             = 0;
    uint8_t discr_r         = 0;
    uint8_t lambda[33]      = {0};
    uint8_t s[32]           = {0};
    uint8_t b[33]           = {0};
    uint8_t t[33]           = {0};
    uint8_t omega[33]       = {0};
    uint8_t root[32]        = {0};
    uint8_t reg[33]         = {0};
    uint8_t loc[32]         = {0};

    int syn_error           = 0;
    int count               = 0;

    /* Form the syndromes; i.e., evaluate data(x) at roots of g(x) */
    for (i=0;i<(int)rs->nroots;i++)
    {
        s[i] = data[0];
    }

    for(j = 1; j < ((int)(rs->nn) - (int)(rs->pad)); j++)
    {
        for(i = 0; i < (int)rs->nroots; i++)
        {
            if (s[i] == 0U)
            {
                s[i] = data[j];
            }
            else
            {
                s[i] = data[j] ^ rs->alpha_to[modnn(rs, ((int)(rs->index_of[s[i]]) + ((int)(rs->fcr) + i) * (int)(rs->prim)))];
            }
        }
    }

    for(i = 0; i < (int)rs->nroots; i++)
    {
        syn_error |= s[i];
        s[i] = rs->index_of[s[i]];

    }


    if (syn_error == 0)
    {
        /* if syndrome is zero, data[] is a codeword and there are no errors
         to correct. So return data[] unmodified */
        count = 0;
        err = 0;
    }
    else
    {
        (void)memset(&lambda[1],0,rs->nroots*sizeof(lambda[0]));
        lambda[0] = 1U;

        if (*num_err > 0)
        {
            /* Init lambda to be the erasure locator polynomial */
            lambda[1] = rs->alpha_to[modnn(rs, rs->prim * (rs->nn - 1U - (uint32_t)(err_pos[0])))];
            for(i = 1U; i < *num_err; i++)
            {
                u = modnn(rs, rs->prim * (rs->nn - 1U - (uint32_t)(err_pos[i])));
                for(j = i + 1; j > 0; j--)
                {
                    tmp = rs->index_of[lambda[j - 1]];
                    if (tmp != rs->nn)
                    {
                        lambda[j] ^= rs->alpha_to[modnn(rs, u + tmp)];
                    }
                }
            }
        }

        for(i = 0; i < ((int)(rs->nroots) + 1); i++)
        {
            b[i] = rs->index_of[lambda[i]];
        }

        /* Begin Berlekamp-Massey algorithm to determine error+erasure locator polynomial */
        r = *num_err;
        el = *num_err;
        while(++r <= rs->nroots)    /* r is the step number */
        {
            /* Compute discrepancy at the r-th step in poly-form */
            discr_r = 0;
            for(i = 0U; i < r; i++)
            {
                if ((lambda[i] != 0U) && ((uint32_t)(s[r - i - 1]) != rs->nn))
                {
                    discr_r ^= rs->alpha_to[modnn(rs, rs->index_of[lambda[i]] + s[r - i - 1])];
                }
            }
            discr_r = rs->index_of[discr_r];    /* Index form */
            if ((uint32_t)discr_r == rs->nn)
            {
                /* 2 lines below: B(x) <-- x*B(x) */
                (void)memmove(&b[1],b,rs->nroots*sizeof(b[0]));
                b[0] = (uint8_t) rs->nn;
            }
            else
            {
                /* 7 lines below: T(x) <-- lambda(x) - discr_r*x*b(x) */
                t[0] = lambda[0];
                for(i = 0U; i < (int) rs->nroots; i++)
                {
                    if (((uint32_t) b[i]) != rs->nn)
                    {
                        t[i + 1] = lambda[i + 1] ^ rs->alpha_to[modnn(rs, discr_r + b[i])];
                    }
                    else
                    {
                        t[i + 1] = lambda[i + 1];
                    }
                }
                if ((2 * el) <= (r + *num_err - 1))
                {
                    el = r + *num_err - el;
                    /* 2 lines below: B(x) <-- inv(discr_r) * lambda(x) */
                    for(i = 0; i <= (int)rs->nroots; i++)
                    {
                        if (lambda[i] == 0U)
                        {
                            b[i] = (uint8_t)rs->nn;
                        }
                        else
                        {
                            b[i] = modnn(rs, rs->index_of[lambda[i]] - discr_r + rs->nn);
                        }
                    }
                }
                else
                {
                    /* 2 lines below: B(x) <-- x*B(x) */
                      (void)memmove(&b[1],b,(rs->nroots)*sizeof(b[0]));
                      b[0] = 255U;
                }

                (void)memcpy(lambda,t,(rs->nroots)*sizeof(t[0]));
            }
        }

        /* Convert lambda to index form and compute deg(lambda(x)) */
        deg_lambda = 0;

        for(i = 0; i < ((int)(rs->nroots) + 1); i++)
        {
            lambda[i] = rs->index_of[lambda[i]];
            if (lambda[i] != rs->nn)
            {
                deg_lambda = i;
            }
        }
        /* Find roots of the error+erasure locator polynomial by Chien search */
        (void)memcpy(&reg[1],&lambda[1],rs->nroots*sizeof(reg[0]));
        count = 0;  /* Number of roots of lambda(x) */
        k =(int)(rs->iprim) - 1;
        for(i = 1; i <= (int)(rs->nn); i++)
        {
            if (i != 1) 
            {
                k = modnn(rs, k + (int)(rs->iprim));
            }

            q = 1;  /* lambda[0] is always 0 */
            for(j = deg_lambda; j > 0; j--)
            {
                if (reg[j] != rs->nn)
                {
                    reg[j] = modnn(rs, reg[j] + (uint8_t)j);
                    q ^= rs->alpha_to[reg[j]];
                }
            }

            if (q != 0U)
            {
                continue;   /* Not a root */
            }   
            /* store root (index-form) and error location number */
            root[count] = i;
            loc[count] = k;
            /* If we've already found max possible roots, abort the search to save time */
            if (++count == deg_lambda)
            {
                break;
            }

        }
        if (deg_lambda != count)
        {
            /* deg(lambda) unequal to number of roots => uncorrectable error detected */
            count = -1;
            err = -1;
        }
        else
        {
            /* Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo x**rs.nroots). in index form. Also find deg(omega) */
            deg_omega = deg_lambda - 1;
            for(i = 0; i <= deg_omega; i++)
            {
                tmp = 0;
                for(j = i; j >= 0; j--)
                {
                    if ((s[i - j] != rs->nn) && (lambda[j] != rs->nn))
                    {
                        tmp ^= rs->alpha_to[modnn(rs, s[i - j] + lambda[j])];
                    }
                }
                omega[i] = rs->index_of[tmp];
            }

            /* Compute error values in poly-form. num1 = omega(inv(X(l))), num2 = inv(X(l))**(rs.fcr-1) and den = lambda_pr(inv(X(l))) all in poly-form */
            for(j = (count - 1); j >= 0; j--)
            {
                num1 = 0;
                for(i = deg_omega; i >= 0; i--)
                {
                    if (omega[i] != rs->nn)
                    {
                        num1 ^= rs->alpha_to[modnn(rs, (int)omega[i] + (i * (int)root[j]))];
                    }
                }
                num2 = rs->alpha_to[modnn(rs, (int)(root[j]) * ((int)(rs->fcr) - 1) + (int)(rs->nn))];
                den = 0;

                /* lambda[i+1] for i even is the formal derivative lambda_pr of lambda[i] */
                for (i = min(deg_lambda, ((int)(rs->nroots)-1)) & ~1; i>=0; i -= 2)
                {
                    if (lambda[i + 1] != rs->nn)
                    {
                        den ^= rs->alpha_to[modnn(rs, (int)lambda[i + 1] + (i * (int)root[j]))];
                    }
                }
                /* Apply error to data */
                if ((num1 != 0U) && (loc[j] >= rs->pad))
                {
                    data[loc[j] - rs->pad] ^= rs->alpha_to[modnn(rs, rs->index_of[num1] + rs->index_of[num2] + rs->nn - rs->index_of[den])];
                }
            }
        err = 0;
        }
    }

    if (err_pos != NULL)
    {
        for(i = 0; i < count; i++)
        {
            err_pos[i] = loc[i];
        }
    }

    if (num_err != NULL)
    {
        *num_err = count;
    }

    return err;
}

static int modnn(reed_solomon_t *rs, int num)
{
    uint32_t x;
    x = (uint32_t)num;

    while(x >= rs->nn)
    {
        x -= rs->nn;
        x = (x >> rs->mm) + (x & rs->nn);
    }

    return (int)x;
}

static int min(int a, int b)
{
    int min;
    if (a <= b)
    {
        min = a;
    }
    else
    {
        min = b;
    }

    return min;
}

/**< \} End of rsclib group */
