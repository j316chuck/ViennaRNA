/*
 *                partiton function for RNA secondary structures
 *
 *                Ivo L Hofacker
 *                Stephan Bernhart
 *                Ronny Lorenz
 *                ViennaRNA package
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>    /* #defines FLT_MAX ... */
#include <limits.h>

#include "ViennaRNA/utils.h"
#include "ViennaRNA/structure_utils.h"
#include "ViennaRNA/energy_par.h"
#include "ViennaRNA/fold_vars.h"
#include "ViennaRNA/PS_dot.h"
#include "ViennaRNA/params.h"
#include "ViennaRNA/loop_energies.h"
#include "ViennaRNA/part_func.h"
#include "ViennaRNA/part_func_co.h"

#ifdef  VRNA_BACKWARD_COMPAT
#ifdef _OPENMP
#include <omp.h>
#endif
#endif

/*
 #################################
 # GLOBAL VARIABLES              #
 #################################
 */

#ifdef  VRNA_BACKWARD_COMPAT

int     mirnatog      = 0;
double  F_monomer[2]  = {
  0, 0
};                             /* free energies of the two monomers */

#endif

/*
 #################################
 # PRIVATE VARIABLES             #
 #################################
 */

#ifdef  VRNA_BACKWARD_COMPAT

/* some backward compatibility stuff */
PRIVATE vrna_fold_compound_t  *backward_compat_compound = NULL;
PRIVATE int                   backward_compat           = 0;

#ifdef _OPENMP

#pragma omp threadprivate(backward_compat_compound, backward_compat)

#endif

#endif

/*
 #################################
 # PRIVATE FUNCTION DECLARATIONS #
 #################################
 */
PRIVATE void    pf_co(vrna_fold_compound_t *vc);


PRIVATE void    pf_co_bppm(vrna_fold_compound_t *vc,
                           char                 *structure);


PRIVATE double *Newton_Conc(double  ZAB,
                            double  ZAA,
                            double  ZBB,
                            double  concA,
                            double  concB,
                            double  *ConcVec);


#ifdef  VRNA_BACKWARD_COMPAT

PRIVATE vrna_dimer_pf_t wrap_co_pf_fold(char              *sequence,
                                        char              *structure,
                                        vrna_exp_param_t  *parameters,
                                        int               calculate_bppm,
                                        int               is_constrained);


#endif

/*
 #################################
 # BEGIN OF FUNCTION DEFINITIONS #
 #################################
 */
PUBLIC vrna_dimer_pf_t
vrna_pf_dimer(vrna_fold_compound_t  *vc,
              char                  *structure)
{
  int               n;
  FLT_OR_DBL        Q;
  vrna_dimer_pf_t   X;
  double            free_energy;
  char              *sequence;
  vrna_md_t         *md;
  vrna_exp_param_t  *params;
  vrna_mx_pf_t      *matrices;

  if (!vrna_fold_compound_prepare(vc, VRNA_OPTION_PF | VRNA_OPTION_HYBRID)) {
    vrna_message_warning("vrna_pf_dimer@part_func_co.c: Failed to prepare vrna_fold_compound");
    X.FA = X.FB = X.FAB = X.F0AB = X.FcAB = 0;
    return X;
  }

  params    = vc->exp_params;
  n         = vc->length;
  md        = &(params->model_details);
  matrices  = vc->exp_matrices;
  sequence  = vc->sequence;

#ifdef _OPENMP
  /* Explicitly turn off dynamic threads */
  omp_set_dynamic(0);
#endif

#ifdef SUN4
  nonstandard_arithmetic();
#else
#ifdef HP9
  fpsetfastmode(1);
#endif
#endif

  /* call user-defined recursion status callback function */
  if (vc->stat_cb)
    vc->stat_cb(VRNA_STATUS_PF_PRE, vc->auxdata);

  pf_co(vc);

  /* call user-defined recursion status callback function */
  if (vc->stat_cb)
    vc->stat_cb(VRNA_STATUS_PF_POST, vc->auxdata);

  if (md->backtrack_type == 'C')
    Q = matrices->qb[vc->iindx[1] - n];
  else if (md->backtrack_type == 'M')
    Q = matrices->qm[vc->iindx[1] - n];
  else
    Q = matrices->q[vc->iindx[1] - n];

  /* ensemble free energy in Kcal/mol */
  if (Q <= FLT_MIN)
    vrna_message_warning("pf_scale too large");

  free_energy = (-log(Q) - n * log(params->pf_scale)) * params->kT / 1000.0;
  /* in case we abort because of floating point errors */
  if (n > 1600)
    vrna_message_info(stderr, "free energy = %8.2f", free_energy);

  /* probability of molecules being bound together */

  /* Computation of "real" Partition function */
  /* Need that for concentrations */
  if (vc->cutpoint > 0) {
    double kT, QAB, QToT, Qzero;
    kT    = params->kT / 1000.0;
    Qzero = matrices->q[vc->iindx[1] - n];
    QAB   =
      (matrices->q[vc->iindx[1] - n] - matrices->q[vc->iindx[1] - (vc->cutpoint - 1)] *
       matrices->q[vc->iindx[vc->cutpoint] - n]) * params->expDuplexInit;
    /*correction for symmetry*/
    if ((n - (vc->cutpoint - 1) * 2) == 0)
      if ((strncmp(sequence, sequence + vc->cutpoint - 1, vc->cutpoint - 1)) == 0)
        QAB /= 2;

    QToT = matrices->q[vc->iindx[1] - (vc->cutpoint - 1)] *
           matrices->q[vc->iindx[vc->cutpoint] - n] + QAB;
    X.FAB   = -kT * (log(QToT) + n * log(params->pf_scale));
    X.F0AB  = -kT * (log(Qzero) + n * log(params->pf_scale));
    X.FcAB  = (QAB > 1e-17) ? -kT * (log(QAB) + n * log(params->pf_scale)) : 999;
    X.FA    = -kT *
              (log(matrices->q[vc->iindx[1] - (vc->cutpoint - 1)]) + (vc->cutpoint - 1) *
               log(params->pf_scale));
    X.FB = -kT *
           (log(matrices->q[vc->iindx[vc->cutpoint] - n]) + (n - vc->cutpoint + 1) *
            log(params->pf_scale));

    /* printf("QAB=%.9f\tQtot=%.9f\n",QAB/scale[n],QToT/scale[n]); */
  } else {
    X.FA    = X.FB = X.FAB = X.F0AB = free_energy;
    X.FcAB  = 0;
  }

  /* backtracking to construct binding probabilities of pairs */
  if (md->compute_bpp) {
    pf_co_bppm(vc, structure);

#ifdef  VRNA_BACKWARD_COMPAT

    /*
     *  Backward compatibility:
     *  This block may be removed if deprecated functions
     *  relying on the global variable "pr" vanish from within the package!
     */
    pr = vc->exp_matrices->probs;
    /*
     * {
     * if(pr) free(pr);
     * pr = (FLT_OR_DBL *) vrna_alloc(sizeof(FLT_OR_DBL) * ((n+1)*(n+2)/2));
     * memcpy(pr, probs, sizeof(FLT_OR_DBL) * ((n+1)*(n+2)/2));
     * }
     */

#endif
  }

#ifdef SUN4
  standard_arithmetic();
#else
#ifdef HP9
  fpsetfastmode(0);
#endif
#endif

  return X;
}


PUBLIC void
vrna_pf_dimer_probs(double                  FAB,
                    double                  FA,
                    double                  FB,
                    vrna_ep_t               *prAB,
                    const vrna_ep_t         *prA,
                    const vrna_ep_t         *prB,
                    int                     Alength,
                    const vrna_exp_param_t  *exp_params)
{
  /* computes binding probabilities and dimer free energies */
  int             i, j;
  double          pAB;
  double          mykT;
  const vrna_ep_t *lp2;
  vrna_ep_t       *lp1;
  int             offset;

  mykT = exp_params->kT / 1000.;

  /* pair probabilities in pr are relative to the null model (without DuplexInit) */

  /* Compute probabilities pAB, pAA, pBB */

  pAB = 1. - exp((1 / mykT) * (FAB - FA - FB));

  /* compute pair probabilities given that it is a dimer */
  /* AB dimer */
  offset  = 0;
  lp2     = prA;
  if (pAB > 0) {
    for (lp1 = prAB; lp1->j > 0; lp1++) {
      float pp = 0;
      i = lp1->i;
      j = lp1->j;
      while (offset + lp2->i < i && lp2->i > 0)
        lp2++;
      if (offset + lp2->i == i)
        while ((offset + lp2->j) < j && (lp2->j > 0))
          lp2++;

      if (lp2->j == 0) {
        lp2     = prB;
        offset  = Alength;
      }                                          /* jump to next list */

      if ((offset + lp2->i == i) && (offset + lp2->j == j)) {
        pp = lp2->p;
        lp2++;
      }

      lp1->p = (lp1->p - (1 - pAB) * pp) / pAB;
      if (lp1->p < 0.) {
        vrna_message_warning(
          "vrna_co_pf_probs: numeric instability detected, probability below zero!");
        lp1->p = 0.;
      }
    }
  }

  return;
}


PUBLIC vrna_dimer_conc_t *
vrna_pf_dimer_concentrations(double                 FcAB,
                             double                 FcAA,
                             double                 FcBB,
                             double                 FEA,
                             double                 FEB,
                             const double           *startconc,
                             const vrna_exp_param_t *exp_params)
{
  /*
   * takes an array of start concentrations, computes equilibrium concentrations of dimers,
   * monomers, returns array of concentrations in strucutre vrna_dimer_conc_t
   */
  double            *ConcVec;
  int               i;
  vrna_dimer_conc_t *Concentration;
  double            KAA, KAB, KBB, kT;

  kT            = exp_params->kT / 1000.;
  Concentration = (vrna_dimer_conc_t *)vrna_alloc(20 * sizeof(vrna_dimer_conc_t));
  /* Compute equilibrium constants */
  /* again note the input free energies are not from the null model (without DuplexInit) */

  KAA = exp((2.0 * FEA - FcAA) / kT);
  KBB = exp((2.0 * FEB - FcBB) / kT);
  KAB = exp((FEA + FEB - FcAB) / kT);
  /* printf("Kaa..%g %g %g\n", KAA, KBB, KAB); */
  for (i = 0; ((startconc[i] != 0) || (startconc[i + 1] != 0)); i += 2) {
    ConcVec = Newton_Conc(KAB,
                          KAA,
                          KBB,
                          startconc[i],
                          startconc[i + 1],
                          ConcVec);
    Concentration[i / 2].Ac_start = startconc[i];
    Concentration[i / 2].Bc_start = startconc[i + 1];
    Concentration[i / 2].ABc      = ConcVec[0];
    Concentration[i / 2].AAc      = ConcVec[1];
    Concentration[i / 2].BBc      = ConcVec[2];
    Concentration[i / 2].Ac       = ConcVec[3];
    Concentration[i / 2].Bc       = ConcVec[4];

    if (!(((i + 2) / 2) % 20))
      Concentration =
        (vrna_dimer_conc_t *)vrna_realloc(Concentration,
                                          ((i + 2) / 2 + 20) * sizeof(vrna_dimer_conc_t));

    free(ConcVec);
  }

  return Concentration;
}


/* forward recursion of pf cofolding */
PRIVATE void
pf_co(vrna_fold_compound_t *vc)
{
  unsigned char       *hard_constraints;
  int                 n, d, i, j, ij, turn, *my_iindx, *jindx;
  FLT_OR_DBL          temp, Qmax, qbt1, *q, *qb, *qm, *qm1;
  double              max_real;
  vrna_md_t           *md;
  vrna_hc_t           *hc;
  vrna_exp_param_t    *pf_params;
  vrna_mx_pf_t        *matrices;
  vrna_mx_pf_aux_el_t *aux_mx_el;
  vrna_mx_pf_aux_ml_t *aux_mx_ml;

  n                 = vc->length;
  my_iindx          = vc->iindx;
  jindx             = vc->jindx;
  pf_params         = vc->exp_params;
  md                = &(pf_params->model_details);
  hc                = vc->hc;
  matrices          = vc->exp_matrices;
  turn              = md->min_loop_size;
  hard_constraints  = hc->matrix;
  Qmax              = 0.;

  q   = matrices->q;
  qb  = matrices->qb;
  qm  = matrices->qm;
  qm1 = matrices->qm1;

  max_real = (sizeof(FLT_OR_DBL) == sizeof(float)) ? FLT_MAX : DBL_MAX;

  /* hard code min_loop_size to 0, since we can not be sure yet that this is already the case */
  turn = 0;

  /* init auxiliary arrays for fast exterior/multibranch loops */
  aux_mx_el = vrna_exp_E_ext_fast_init(vc);
  aux_mx_ml = vrna_exp_E_ml_fast_init(vc);

  /*
   * array initialization ; qb,qm,q
   * qb,qm,q (i,j) are stored as ((n+1-i)*(n-i) div 2 + n+1-j
   */

  for (d = 0; d <= turn; d++)
    for (i = 1; i <= n - d; i++) {
      j       = i + d;
      ij      = my_iindx[i] - j;
      qb[ij]  = 0.0;
    }

  for (j = turn + 2; j <= n; j++) {
    for (i = j - turn - 1; i >= 1; i--) {
      ij    = my_iindx[i] - j;
      qbt1  = 0;

      if (hard_constraints[jindx[j] + i]) {
        /* process hairpin loop(s) */
        qbt1 += vrna_exp_E_hp_loop(vc, i, j);
        /* process interior loop(s) */
        qbt1 += vrna_exp_E_int_loop(vc, i, j);
        /* process multibranch loop(s) */
        qbt1 += vrna_exp_E_mb_loop_fast(vc, i, j, aux_mx_ml->qqm1);
      }

      qb[ij] = qbt1;

      /* Multibranch loop */
      qm[ij] = vrna_exp_E_ml_fast(vc, i, j, aux_mx_ml);

      if (qm1)
        qm1[jindx[j] + i] = aux_mx_ml->qqm[i]; /* for stochastic backtracking */

      /* Exterior loop */
      q[ij] = temp = vrna_exp_E_ext_fast(vc, i, j, aux_mx_el);

      if (temp > Qmax) {
        Qmax = temp;
        if (Qmax > max_real / 10.)
          vrna_message_warning("Q close to overflow: %d %d %g", i, j, temp);
      }

      if (temp >= max_real)
        vrna_message_error("overflow in co_pf_fold while calculating q[%d,%d]\n"
                           "use larger pf_scale", i, j);
    }

    /* rotate auxiliary arrays */
    vrna_exp_E_ext_fast_rotate(vc, aux_mx_el);
    vrna_exp_E_ml_fast_rotate(vc, aux_mx_ml);
  }

  /* free memory occupied by auxiliary arrays for fast exterior/multibranch loops */
  vrna_exp_E_ml_fast_free(vc, aux_mx_ml);
  vrna_exp_E_ext_fast_free(vc, aux_mx_el);
}


/* backward recursion of pf cofolding */
PRIVATE void
pf_co_bppm(vrna_fold_compound_t *vc,
           char                 *structure)
{
  unsigned int      *sn;
  int               n, i, j, k, l, ij, kl, ii, ll, lj, u1, u2, type, type_2, tt, turn, ov = 0,
                    *my_iindx, *jindx, cp;
  FLT_OR_DBL        temp, Qmax = 0, prm_MLb, tmp2, ppp;
  FLT_OR_DBL        prmt, prmt1, *expMLbase;
  FLT_OR_DBL        *tmp;
  FLT_OR_DBL        expMLclosing, *probs, *q1k, *qln, *q, *qb, *qm, *scale;
  double            max_real;
  vrna_exp_param_t  *pf_params;
  vrna_md_t         *md;
  short             *S, *S1, s5, s3;
  char              *ptype;
  vrna_hc_t         *hc;
  vrna_sc_t         *sc;
  vrna_mx_pf_t      *matrices;
  char              *sequence;
  unsigned char     *hard_constraints;
  int               *hc_up_ext;
  int               *hc_up_hp;
  int               *hc_up_int;
  int               *hc_up_ml;
  int               *rtype;

  sequence      = vc->sequence;
  n             = vc->length;
  cp            = vc->cutpoint;
  pf_params     = vc->exp_params;
  md            = &(pf_params->model_details);
  expMLclosing  = pf_params->expMLclosing;
  S             = vc->sequence_encoding2;
  S1            = vc->sequence_encoding;
  sn            = vc->strand_number;
  jindx         = vc->jindx;
  my_iindx      = vc->iindx;
  ptype         = vc->ptype;
  rtype         = &(md->rtype[0]);
  turn          = md->min_loop_size;

  matrices  = vc->exp_matrices;
  probs     = matrices->probs;
  scale     = matrices->scale;
  q1k       = matrices->q1k;
  qln       = matrices->qln;
  q         = matrices->q;
  qb        = matrices->qb;
  qm        = matrices->qm;
  expMLbase = matrices->expMLbase;

  hc  = vc->hc;
  sc  = vc->sc;

  hard_constraints  = hc->matrix;
  hc_up_ext         = hc->up_ext;
  hc_up_hp          = hc->up_hp;
  hc_up_int         = hc->up_int;
  hc_up_ml          = hc->up_ml;

  /* hard code min_loop_size to 0, since we can not be sure yet that this is already the case */
  turn = 0;

  max_real = (sizeof(FLT_OR_DBL) == sizeof(float)) ? FLT_MAX : DBL_MAX;

  /* backtracking to construct binding probabilities of pairs*/
  if ((S != NULL) && (S1 != NULL)) {
    FLT_OR_DBL  *Qlout, *Qrout;
    FLT_OR_DBL  *prm_l  = (FLT_OR_DBL *)vrna_alloc(sizeof(FLT_OR_DBL) * (n + 2));
    FLT_OR_DBL  *prm_l1 = (FLT_OR_DBL *)vrna_alloc(sizeof(FLT_OR_DBL) * (n + 2));
    FLT_OR_DBL  *prml   = (FLT_OR_DBL *)vrna_alloc(sizeof(FLT_OR_DBL) * (n + 2));

    Qmax  = 0;
    Qrout = (FLT_OR_DBL *)vrna_alloc(sizeof(FLT_OR_DBL) * (n + 2));
    Qlout = (FLT_OR_DBL *)vrna_alloc(sizeof(FLT_OR_DBL) * (cp + 2));

    for (k = 1; k <= n; k++) {
      q1k[k]  = q[my_iindx[1] - k];
      qln[k]  = q[my_iindx[k] - n];
    }
    q1k[0]      = 1.0;
    qln[n + 1]  = 1.0;

    /* 1. exterior pair i,j and initialization of pr array */
    for (i = 1; i <= n; i++) {
      for (j = i; j <= MIN2(i + turn, n); j++)
        probs[my_iindx[i] - j] = 0;

      for (j = i + turn + 1; j <= n; j++) {
        ij = my_iindx[i] - j;
        if ((hard_constraints[jindx[j] + i] & VRNA_CONSTRAINT_CONTEXT_EXT_LOOP) && (qb[ij] > 0.)) {
          type = ptype[jindx[j] + i];

          if (type == 0)
            type = 7;

          s5        = ((i > 1) && (sn[i] == sn[i - 1])) ? S1[i - 1] : -1;
          s3        = ((j < n) && (sn[j + 1] == sn[j])) ? S1[j + 1] : -1;
          probs[ij] = q1k[i - 1] * qln[j + 1] / q1k[n];
          probs[ij] *= exp_E_ExtLoop(type, s5, s3, pf_params);
          if (sc)
            if (sc->exp_f)
              probs[ij] *= sc->exp_f(1, n, i, j, VRNA_DECOMP_EXT_STEM_OUTSIDE, sc->data);
        } else {
          probs[ij] = 0;
        }
      }
    }

    for (l = n; l > turn + 1; l--) {
      /* 2. bonding k,l as substem of 2:loop enclosed by i,j */
      for (k = 1; k < l - turn; k++) {
        kl      = my_iindx[k] - l;
        type_2  = (unsigned char)ptype[jindx[l] + k];
        type_2  = rtype[type_2];

        if (qb[kl] == 0.)
          continue;

        if (hard_constraints[jindx[l] + k] & VRNA_CONSTRAINT_CONTEXT_INT_LOOP_ENC) {
          if (type_2 == 0)
            type_2 = 7;

          for (i = MAX2(1, k - MAXLOOP - 1); i <= k - 1; i++) {
            u1 = k - i - 1;
            if (hc_up_int[i + 1] < u1)
              continue;

            for (j = l + 1; j <= MIN2(l + MAXLOOP - k + i + 2, n); j++) {
              u2 = j - l - 1;
              if (hc_up_int[l + 1] < u2)
                break;

              ij = my_iindx[i] - j;
              if (hard_constraints[jindx[j] + i] & VRNA_CONSTRAINT_CONTEXT_INT_LOOP) {
                if ((sn[k] == sn[i]) && (sn[j] == sn[l])) {
                  type = (unsigned char)ptype[jindx[j] + i];
                  if (probs[ij] > 0) {
                    if (type == 0)
                      type = 7;

                    tmp2 = probs[ij]
                           * scale[u1 + u2 + 2]
                           * exp_E_IntLoop(u1,
                                           u2,
                                           type,
                                           type_2,
                                           S1[i + 1],
                                           S1[j - 1],
                                           S1[k - 1],
                                           S1[l + 1],
                                           pf_params);

                    if (sc) {
                      if (sc->exp_energy_up)
                        tmp2 *= sc->exp_energy_up[i + 1][u1]
                                * sc->exp_energy_up[l + 1][u2];

                      if (sc->exp_energy_bp)
                        tmp2 *= sc->exp_energy_bp[ij];

                      if (sc->exp_energy_stack) {
                        if ((i + 1 == k) && (j - 1 == l)) {
                          tmp2 *= sc->exp_energy_stack[i]
                                  * sc->exp_energy_stack[k]
                                  * sc->exp_energy_stack[l]
                                  * sc->exp_energy_stack[j];
                        }
                      }

                      if (sc->exp_f)
                        tmp2 *= sc->exp_f(i, j, k, l, VRNA_DECOMP_PAIR_IL, sc->data);
                    }

                    probs[kl] += tmp2;
                  }
                }
              }
            }
          }
        }
      }

      /* 3. bonding k,l as substem of multi-loop enclosed by i,j */
      prm_MLb = 0.;
      if ((l < n) && (sn[l + 1] == sn[l])) {
        for (k = 2; k < l - turn; k++) {
          kl    = my_iindx[k] - l;
          i     = k - 1;
          prmt  = prmt1 = 0.0;

          ii  = my_iindx[i];        /* ii-j=[i,j]     */
          ll  = my_iindx[l + 1];    /* ll-j=[l+1,j] */
          tt  = (unsigned char)ptype[jindx[l + 1] + i];
          tt  = rtype[tt];
          if (sn[k] == sn[i]) {
            if (hard_constraints[jindx[l + 1] + i] & VRNA_CONSTRAINT_CONTEXT_MB_LOOP) {
              if (tt == 0)
                tt = 7;

              prmt1 = probs[ii - (l + 1)]
                      *expMLclosing
                      *exp_E_MLstem(tt,
                                    S1[l],
                                    S1[i + 1],
                                    pf_params);


              if (sc) {
                /* which decompositions are covered here? => (i, l+1) -> enclosing pair, (k,l) -> enclosed pair, */
                if (sc->exp_energy_bp)
                  prmt1 *= sc->exp_energy_bp[ii - (l + 1)];

                /*
                 *              if(sc->exp_f)
                 *                prmt1 *= sc->exp_f(i, l+1, k, l, , sc->data);
                 */
              }
            }

            ij  = my_iindx[i] - (l + 2);
            lj  = my_iindx[l + 1] - (l + 1);

            for (j = l + 2; j <= n; j++, ij--, lj--) {
              if (hard_constraints[jindx[j] + i] & VRNA_CONSTRAINT_CONTEXT_MB_LOOP) {
                if (sn[j] == sn[j - 1]) {
                  /*??*/
                  tt  = (unsigned char)ptype[jindx[j] + i];
                  tt  = rtype[tt];

                  if (tt == 0)
                    tt = 7;

                  /* which decomposition is covered here? =>
                   * i + 1 = k < l < j:
                   * (i,j)       -> enclosing pair
                   * (k, l)      -> enclosed pair
                   * (l+1, j-1)  -> multiloop part with at least one stem
                   */
                  ppp = probs[ii - j]
                        * exp_E_MLstem(tt, S1[j - 1], S1[i + 1], pf_params)
                        * qm[ll - (j - 1)];

                  if (sc) {
                    if (sc->exp_energy_bp)
                      ppp *= sc->exp_energy_bp[ij];

                    /*
                     *                  if(sc->exp_f)
                     *                    ppp *= sc->exp_f(i, j, l+1, j-1, , sc->data);
                     */
                  }

                  prmt += ppp;
                }
              }
            }
          }

          prmt *= expMLclosing;

          tt = ptype[jindx[l] + k];

          prml[i] = prmt;

          /* l+1 is unpaired */
          if (hc->up_ml[l + 1]) {
            ppp = prm_l1[i] * expMLbase[1];
            if (sc) {
              if (sc->exp_energy_up)
                ppp *= sc->exp_energy_up[l + 1][1];

              /*
               *            if(sc_exp_f)
               *              ppp *= sc->exp_f(, sc->data);
               */
            }

            prm_l[i] = ppp + prmt1;
          } else {
            prm_l[i] = prmt1;
          }

          /* i is unpaired */
          if (hc->up_ml[i]) {
            ppp = prm_MLb * expMLbase[1];
            if (sc) {
              if (sc->exp_energy_up)
                ppp *= sc->exp_energy_up[i][1];

              /*
               *            if(sc->exp_f)
               *              ppp *= sc->exp_f(, sc->data);
               */
            }

            prm_MLb = ppp + prml[i];
            /*
             *  same as:    prm_MLb = 0;
             * for (i=1; i<=k-1; i++) prm_MLb += prml[i]*expMLbase[k-i-1];
             */
          } else {
            prm_MLb = prml[i];
          }

          prml[i] = prml[i] + prm_l[i];

          if (qb[kl] == 0.)
            continue;

          if (hard_constraints[jindx[l] + k] & VRNA_CONSTRAINT_CONTEXT_MB_LOOP_ENC) {
            temp = prm_MLb;

            for (i = 1; i <= k - 2; i++)
              if ((sn[i + 1] == sn[i]) && (sn[k] == sn[k - 1]))
                temp += prml[i] * qm[my_iindx[i + 1] - (k - 1)];

            if (tt == 0)
              tt = 7;

            s5        = ((k > 1) && (sn[k] == sn[k - 1])) ? S1[k - 1] : -1;
            s3        = ((l < n) && (sn[l + 1] == sn[l])) ? S1[l + 1] : -1;
            temp      *= exp_E_MLstem(tt, s5, s3, pf_params) * scale[2];
            probs[kl] += temp;

            if (probs[kl] > Qmax) {
              Qmax = probs[kl];
              if (Qmax > max_real / 10.)
                vrna_message_warning("P close to overflow: %d %d %g %g",
                                     i, j, probs[kl], qb[kl]);
            }

            if (probs[kl] >= max_real) {
              ov++;
              probs[kl] = FLT_MAX;
            }
          }
        } /* end for (k=..) multloop*/
      } else {
        /* set prm_l to 0 to get prm_l1 to be 0 */
        for (i = 0; i <= n; i++)
          prm_l[i] = 0;
      }

      tmp     = prm_l1;
      prm_l1  = prm_l;
      prm_l   = tmp;

      /* computation of .(..(...)..&..). type features? */
      if (cp <= 0)
        continue;                     /* no .(..(...)..&..). type features */

      if ((l == n) || (l <= 2))
        continue;                     /* no .(..(...)..&..). type features */

      /*new version with O(n^3)??*/
      if (l > cp) {
        if (l < n) {
          int t, kt;
          for (t = n; t > l; t--) {
            for (k = 1; k < cp; k++) {
              int samestrand;
              kt = my_iindx[k] - t;

              samestrand  = (sn[k + 1] == sn[k]) ? 1 : 0;
              type        = rtype[(unsigned char)ptype[jindx[t] + k]];

              if (type == 0)
                type = 7;

              temp = probs[kt]
                     * exp_E_ExtLoop(type, S1[t - 1], samestrand ? S1[k + 1] : -1, pf_params)
                     * scale[2];

              if (l + 1 < t)
                temp *= q[my_iindx[l + 1] - (t - 1)];

              if (samestrand)
                temp *= q[my_iindx[k + 1] - (cp - 1)];

              Qrout[l] += temp;
            }
          }
        }

        for (k = l - 1; k >= cp; k--) {
          if (qb[my_iindx[k] - l]) {
            kl    = my_iindx[k] - l;
            type  = ptype[jindx[l] + k];
            temp  = Qrout[l];

            if (type == 0)
              type = 7;

            temp *= exp_E_ExtLoop(type,
                                  (k > cp) ? S1[k - 1] : -1,
                                  (l < n) ? S1[l + 1] : -1,
                                  pf_params);
            if (k > cp)
              temp *= q[my_iindx[cp] - (k - 1)];

            probs[kl] += temp;
          }
        }
      } else if (l == cp) {
        int t, sk, s;
        for (t = 2; t < cp; t++) {
          for (s = 1; s < t; s++) {
            for (k = cp; k <= n; k++) {
              sk = my_iindx[s] - k;
              if (qb[sk]) {
                int samestrand;
                samestrand  = (sn[k] == sn[k - 1]) ? 1 : 0;
                type        = rtype[(unsigned char)ptype[jindx[k] + s]];

                if (type == 0)
                  type = 7;

                temp = probs[sk]
                       * exp_E_ExtLoop(type, samestrand ? S1[k - 1] : -1, S1[s + 1], pf_params)
                       * scale[2];
                if (s + 1 < t)
                  temp *= q[my_iindx[s + 1] - (t - 1)];

                if (samestrand)
                  temp *= q[my_iindx[cp] - (k - 1)];

                Qlout[t] += temp;
              }
            }
          }
        }
      } else if (l < cp) {
        for (k = 1; k < l; k++) {
          if (qb[my_iindx[k] - l]) {
            type  = ptype[jindx[l] + k];
            temp  = Qlout[k];

            if (type == 0)
              type = 7;

            temp *= exp_E_ExtLoop(type,
                                  (k > 1) ? S1[k - 1] : -1,
                                  (l < (cp - 1)) ? S1[l + 1] : -1,
                                  pf_params);
            if (l + 1 < cp)
              temp *= q[my_iindx[l + 1] - (cp - 1)];

            probs[my_iindx[k] - l] += temp;
          }
        }
      }
    }  /* end for (l=..)   */
    free(Qlout);
    free(Qrout);
    for (i = 1; i <= n; i++)
      for (j = i + turn + 1; j <= n; j++) {
        ij        = my_iindx[i] - j;
        probs[ij] *= qb[ij];
      }

    if (structure != NULL) {
      char *s = vrna_db_from_probs(probs, (unsigned int)n);
      memcpy(structure, s, n);
      structure[n] = '\0';
      free(s);
    }

    /* clean up */
    free(prm_l);
    free(prm_l1);
    free(prml);
  }   /* end if (do_backtrack) */

  if (ov > 0)
    vrna_message_warning("%d overflows occurred while backtracking;\n"
                         "you might try a smaller pf_scale than %g\n",
                         ov, pf_params->pf_scale);
}


PRIVATE double *
Newton_Conc(double  KAB,
            double  KAA,
            double  KBB,
            double  concA,
            double  concB,
            double  *ConcVec)
{
  double  TOL, EPS, xn, yn, det, cA, cB;
  int     i;

  i = 0;
  /* Newton iteration for computing concentrations */
  cA      = concA;
  cB      = concB;
  TOL     = 1e-6;                                     /* Tolerance for convergence */
  ConcVec = (double *)vrna_alloc(5 * sizeof(double)); /* holds concentrations */
  do {
    /* det = (4.0 * KAA * cA + KAB *cB + 1.0) * (4.0 * KBB * cB + KAB *cA + 1.0) - (KAB *cB) * (KAB *cA); */
    det = 1 + 16. * KAA * KBB * cA * cB + KAB * (cA + cB) + 4. * KAA * cA + 4. * KBB * cB + 4. *
          KAB * (KBB * cB * cB + KAA * cA * cA);
    /*
     * xn  = ( (2.0 * KBB * cB*cB + KAB *cA *cB + cB - concB) * (KAB *cA) -
     *       (2.0 * KAA * cA*cA + KAB *cA *cB + cA - concA) * (4.0 * KBB * cB + KAB *cA + 1.0) ) / det;
     */
    xn = ((2.0 * KBB * cB * cB + cB - concB) * (KAB * cA) - KAB * cA * cB * (4. * KBB * cB + 1.) -
          (2.0 * KAA * cA * cA + cA - concA) * (4.0 * KBB * cB + KAB * cA + 1.0)) / det;
    /*
     * yn  = ( (2.0 * KAA * cA*cA + KAB *cA *cB + cA - concA) * (KAB *cB) -
     *       (2.0 * KBB * cB*cB + KAB *cA *cB + cB - concB) * (4.0 * KAA * cA + KAB *cB + 1.0) ) / det;
     */
    yn = ((2.0 * KAA * cA * cA + cA - concA) * (KAB * cB) - KAB * cA * cB * (4. * KAA * cA + 1.) -
          (2.0 * KBB * cB * cB + cB - concB) * (4.0 * KAA * cA + KAB * cB + 1.0)) / det;
    EPS = fabs(xn / cA) + fabs(yn / cB);
    cA  += xn;
    cB  += yn;
    i++;
    if (i > 10000) {
      vrna_message_warning("Newton did not converge after %d steps!!", i);
      break;
    }
  } while (EPS > TOL);

  ConcVec[0]  = cA * cB * KAB;  /* AB concentration */
  ConcVec[1]  = cA * cA * KAA;  /* AA concentration */
  ConcVec[2]  = cB * cB * KBB;  /* BB concentration */
  ConcVec[3]  = cA;             /* A concentration */
  ConcVec[4]  = cB;             /* B concentration */

  return ConcVec;
}


#if 0
/*
 * stochastic backtracking in pf_fold arrays
 * returns random structure S with Boltzman probabilty
 * p(S) = exp(-E(S)/kT)/Z
 */
PRIVATE void
backtrack_qm1(vrna_fold_compound_t  *vc,
              int                   i,
              int                   j,
              char                  *pstruc)
{
  /* i is paired to l, i<l<j; backtrack in qm1 to find l */
  int               ii, l, type, *jindx, *my_iindx, *rtype, turn;
  double            qt, r;
  FLT_OR_DBL        *qm, *qm1, *qb, *expMLbase;
  short             *S1;
  char              *ptype;
  vrna_md_t         *md;

  vrna_exp_param_t  *pf_params;
  vrna_mx_pf_t      *matrices;

  pf_params = vc->exp_params;
  md        = &(pf_params->model_details);
  S1        = vc->sequence_encoding;
  ptype     = vc->ptype;
  rtype     = &(md->rtype[0]);
  turn      = md->min_loop_size;

  matrices  = vc->exp_matrices;
  qb        = matrices->qb;
  qm        = matrices->qm;
  qm1       = matrices->qm1;
  expMLbase = matrices->expMLbase;

  jindx     = vc->jindx;
  my_iindx  = vc->iindx;

  r   = vrna_urn() * qm1[jindx[j] + i];
  ii  = my_iindx[i];
  for (qt = 0., l = i + turn + 1; l <= j; l++) {
    type = ptype[jindx[l] + i];
    if (type)
      qt += qb[ii - l] * exp_E_MLstem(type, S1[i - 1], S1[l + 1], pf_params) * expMLbase[j - l];

    if (qt >= r)
      break;
  }
  if (l > j)
    vrna_message_error("backtrack failed in qm1");

  backtrack(vc, i, l, pstruc);
}


PRIVATE void
backtrack(vrna_fold_compound_t  *vc,
          int                   i,
          int                   j,
          char                  *pstruc)
{
  int               *jindx, *my_iindx, *rtype, turn;
  FLT_OR_DBL        *qm, *qm1, *qb, *expMLbase, *scale;
  vrna_exp_param_t  *pf_params;
  vrna_mx_pf_t      *matrices;
  short             *S1;
  char              *ptype, *sequence;
  int               noGUclosure;
  vrna_md_t         *md;

  sequence  = vc->sequence;
  pf_params = vc->exp_params;
  md        = &(pf_params->model_details);
  S1        = vc->sequence_encoding;
  ptype     = vc->ptype;
  rtype     = &(md->rtype[0]);
  turn      = md->min_loop_size;

  matrices    = vc->exp_matrices;
  qb          = matrices->qb;
  qm          = matrices->qm;
  qm1         = matrices->qm1;
  expMLbase   = matrices->expMLbase;
  scale       = matrices->scale;
  jindx       = vc->jindx;
  my_iindx    = vc->iindx;
  noGUclosure = pf_params->model_details.noGUclosure;

  do {
    double  r, qbt1;
    int     k, l, type, u, u1;

    pstruc[i - 1] = '(';
    pstruc[j - 1] = ')';

    r     = vrna_urn() * qb[my_iindx[i] - j];
    type  = ptype[jindx[j] + i];
    u     = j - i - 1;
    /*hairpin contribution*/
    if (((type == 3) || (type == 4)) && noGUclosure)
      qbt1 = 0;
    else
      qbt1 =
        exp_E_Hairpin(u, type, S1[i + 1], S1[j - 1], sequence + i - 1, pf_params) * scale[u + 2];

    if (qbt1 > r)
      return;           /* found the hairpin we're done */

    for (k = i + 1; k <= MIN2(i + MAXLOOP + 1, j - turn - 2); k++) {
      u1 = k - i - 1;
      for (l = MAX2(k + turn + 1, j - 1 - MAXLOOP + u1); l < j; l++) {
        int type_2;
        type_2 = ptype[jindx[l] + k];
        if (type_2) {
          type_2  = rtype[type_2];
          qbt1    += qb[my_iindx[k] - l] *
                     exp_E_IntLoop(u1, j - l - 1, type, type_2,
                                   S1[i + 1], S1[j - 1], S1[k - 1], S1[l + 1],
                                   pf_params) * scale[u1 + j - l + 1];
        }

        if (qbt1 > r)
          break;
      }
      if (qbt1 > r)
        break;
    }
    if (l < j) {
      i = k;
      j = l;
    } else {
      break;
    }
  } while (1);

  /* backtrack in multi-loop */
  {
    double  r, qt;
    int     k, ii, jj;

    i++;
    j--;
    /* find the first split index */
    ii  = my_iindx[i];  /* ii-j=[i,j] */
    jj  = jindx[j];     /* jj+i=[j,i] */
    for (qt = 0., k = i + 1; k < j; k++)
      qt += qm[ii - (k - 1)] * qm1[jj + k];
    r = vrna_urn() * qt;
    for (qt = 0., k = i + 1; k < j; k++) {
      qt += qm[ii - (k - 1)] * qm1[jj + k];
      if (qt >= r)
        break;
    }
    if (k >= j)
      vrna_message_error("backtrack failed, can't find split index ");

    backtrack_qm1(vc, k, j, pstruc);

    j = k - 1;
    while (j > i) {
      /* now backtrack  [i ... j] in qm[] */
      jj  = jindx[j];
      ii  = my_iindx[i];
      r   = vrna_urn() * qm[ii - j];
      qt  = qm1[jj + i];
      k   = i;
      if (qt < r)
        for (k = i + 1; k <= j; k++) {
          qt += (qm[ii - (k - 1)] + expMLbase[k - i]) * qm1[jj + k];
          if (qt >= r)
            break;
        }

      if (k > j)
        vrna_message_error("backtrack failed in qm");

      backtrack_qm1(vc, k, j, pstruc);

      if (k < i + turn)
        break;             /* no more pairs */

      r = vrna_urn() * (qm[ii - (k - 1)] + expMLbase[k - i]);
      if (expMLbase[k - i] >= r)
        break;                        /* no more pairs */

      j = k - 1;
    }
  }
}


#endif

#ifdef  VRNA_BACKWARD_COMPAT

/*
 *****************************************
 * BEGIN backward compatibility wrappers *
 *****************************************
 */
PRIVATE vrna_dimer_pf_t
wrap_co_pf_fold(char              *sequence,
                char              *structure,
                vrna_exp_param_t  *parameters,
                int               calculate_bppm,
                int               is_constrained)
{
  int                   length;
  char                  *seq;
  vrna_fold_compound_t  *vc;
  vrna_md_t             md;

  vc      = NULL;
  length  = strlen(sequence);

  seq = (char *)vrna_alloc(sizeof(char) * (length + 2));
  if (cut_point > -1) {
    int i;
    for (i = 0; i < cut_point - 1; i++)
      seq[i] = sequence[i];
    seq[i] = '&';
    for (; i < (int)length; i++)
      seq[i + 1] = sequence[i];
  } else {
    /* this ensures the allocation of all cofold matrices via vrna_fold_compound_t */
    free(seq);
    seq = strdup(sequence);
  }

  /*
   *  if present, extract model details from provided parameters variable,
   *  to properly initialize the fold compound. Otherwise use default
   *  settings taken from deprecated global variables
   */
  if (parameters)
    vrna_md_copy(&md, &(parameters->model_details));
  else
    set_model_details(&md);

  /* set min_loop_size and backtracing options */
  md.compute_bpp = calculate_bppm;
  md.min_loop_size  = 0;

  vc = vrna_fold_compound(seq, &md, VRNA_OPTION_DEFAULT);

  /*
   *  if present, attach a copy of the parameters structure instead of the
   *  default parameters but take care of re-setting it to (initialized)
   *  model details
   */
  free(vc->exp_params);
  if(parameters) {
    vrna_md_copy(&(parameters->model_details), &(vc->params->model_details));
    vc->exp_params = vrna_exp_params_copy(parameters);
  } else {
    vc->exp_params = vrna_exp_params(&(vc->params->model_details));
  }

  /* propagate global pf_scale into vc->exp_params */
  vc->exp_params->pf_scale = pf_scale;

  if (is_constrained && structure) {
    unsigned int constraint_options = 0;
    constraint_options |= VRNA_CONSTRAINT_DB
                          | VRNA_CONSTRAINT_DB_PIPE
                          | VRNA_CONSTRAINT_DB_DOT
                          | VRNA_CONSTRAINT_DB_X
                          | VRNA_CONSTRAINT_DB_ANG_BRACK
                          | VRNA_CONSTRAINT_DB_RND_BRACK;

    vrna_constraints_add(vc, (const char *)structure, constraint_options);
  }

  if (backward_compat_compound)
    vrna_fold_compound_free(backward_compat_compound);

  backward_compat_compound  = vc;
  backward_compat           = 1;
  iindx                     = backward_compat_compound->iindx;

  free(seq);
  return vrna_pf_dimer(vc, structure);
}


/*
 *****************************************
 * END backward compatibility wrappers   *
 *****************************************
 */


/*###########################################*/
/*# deprecated functions below              #*/
/*###########################################*/

PUBLIC vrna_dimer_pf_t
co_pf_fold(char *sequence,
           char *structure)
{
  return wrap_co_pf_fold(sequence, structure, NULL, do_backtrack, fold_constrained);
}


PUBLIC vrna_dimer_pf_t
co_pf_fold_par(char             *sequence,
               char             *structure,
               vrna_exp_param_t *parameters,
               int              calculate_bppm,
               int              is_constrained)
{
  return wrap_co_pf_fold(sequence, structure, parameters, calculate_bppm, is_constrained);
}


PUBLIC vrna_ep_t *
get_plist(vrna_ep_t *pl,
          int       length,
          double    cut_off)
{
  int i, j, n, count, *my_iindx;

  my_iindx = backward_compat_compound->iindx;
  /* get pair probibilities out of pr array */
  count = 0;
  n     = 2;
  for (i = 1; i < length; i++) {
    for (j = i + 1; j <= length; j++) {
      if (pr[my_iindx[i] - j] < cut_off)
        continue;

      if (count == n * length - 1) {
        n   *= 2;
        pl  = (vrna_ep_t *)vrna_realloc(pl, n * length * sizeof(vrna_ep_t));
      }

      pl[count].i   = i;
      pl[count].j   = j;
      pl[count++].p = pr[my_iindx[i] - j];
      /* printf("gpl: %2d %2d %.9f\n",i,j,pr[my_iindx[i]-j]); */
    }
  }
  pl[count].i   = 0;
  pl[count].j   = 0; /* ->?? */
  pl[count++].p = 0.;
  pl            = (vrna_ep_t *)vrna_realloc(pl, (count) * sizeof(vrna_ep_t));
  return pl;
}


PUBLIC void
compute_probabilities(double    FAB,
                      double    FA,
                      double    FB,
                      vrna_ep_t *prAB,
                      vrna_ep_t *prA,
                      vrna_ep_t *prB,
                      int       Alength)
{
  if (backward_compat_compound && backward_compat) {
    vrna_pf_dimer_probs(FAB,
                        FA,
                        FB,
                        prAB,
                        (const vrna_ep_t *)prA,
                        (const vrna_ep_t *)prB,
                        Alength,
                        (const vrna_exp_param_t *)backward_compat_compound->exp_params);
  }
}


PUBLIC vrna_dimer_conc_t *
get_concentrations(double FcAB,
                   double FcAA,
                   double FcBB,
                   double FEA,
                   double FEB,
                   double *startconc)
{
  return vrna_pf_dimer_concentrations(FcAB, FcAA, FcBB, FEA, FEB, (const double *)startconc,
                                      (const vrna_exp_param_t *)backward_compat_compound->exp_params);
}


PUBLIC void
init_co_pf_fold(int length)
{
  /* DO NOTHING */
}


PUBLIC void
free_co_pf_arrays(void)
{
  if (backward_compat_compound && backward_compat) {
    vrna_fold_compound_free(backward_compat_compound);
    backward_compat_compound  = NULL;
    backward_compat           = 0;
  }
}


PUBLIC FLT_OR_DBL *
export_co_bppm(void)
{
  if (backward_compat_compound)
    return backward_compat_compound->exp_matrices->probs;
  else
    return NULL;
}


PUBLIC void
update_co_pf_params(int length)
{
  if (backward_compat_compound && backward_compat) {
    vrna_md_t md;
    set_model_details(&md);
    vrna_exp_params_reset(backward_compat_compound, &md);

    /* compatibility with RNAup, may be removed sometime */
    pf_scale = backward_compat_compound->exp_params->pf_scale;
  }
}


PUBLIC void
update_co_pf_params_par(int               length,
                        vrna_exp_param_t  *parameters)
{
  if (backward_compat_compound && backward_compat) {
    vrna_md_t md;
    if (parameters) {
      vrna_exp_params_subst(backward_compat_compound, parameters);
    } else {
      set_model_details(&md);
      vrna_exp_params_reset(backward_compat_compound, &md);
    }

    /* compatibility with RNAup, may be removed sometime */
    pf_scale = backward_compat_compound->exp_params->pf_scale;
  }
}


#endif
