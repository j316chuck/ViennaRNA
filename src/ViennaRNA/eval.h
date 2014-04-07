#ifndef __VIENNA_RNA_PACKAGE_EVAL_H__
#define __VIENNA_RNA_PACKAGE_EVAL_H__

#include <ViennaRNA/data_structures.h>

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#else
#define DEPRECATED(func) func
#endif

/**
 *  \defgroup eval Energy evaluation
 *  @{
 *    \brief This module contains all functions and variables related to energy evaluation
 *    of sequence/structure pairs.
 *
 *
 *  @}
 */

/** \brief set to first pos of second seq for cofolding  */
extern  int cut_point;

/**
 *  \brief verbose info from energy_of_struct
 *  \ingroup eval
 */
extern  int eos_debug;

/**
 *  \addtogroup eval Energy evaluation
 *  \ingroup folding_routines
 *  @{
 *    \brief This module contains all functions and variables related to energy evaluation
 *    of sequence/structure pairs.
 *  @}
 */

/**
 *  \brief Calculate the free energy of an already folded RNA using global model detail settings
 *
 *  If verbosity level is set to a value >0, energies of structure elements are printed to stdout
 *
 *  \note OpenMP: This function relies on several global model settings variables and thus is
 *        not to be considered threadsafe. See energy_of_struct_par() for a completely threadsafe
 *        implementation.
 *
 *  \ingroup eval
 *
 *  \see energy_of_struct_par(), energy_of_circ_structure()
 *
 *  \param string     RNA sequence
 *  \param structure  secondary structure in dot-bracket notation
 *  \param verbosity_level a flag to turn verbose output on/off
 *  \return          the free energy of the input structure given the input sequence in kcal/mol
 */
float energy_of_structure(const char *string,
                          const char *structure,
                          int verbosity_level);

/**
 *  \brief Calculate the free energy of an already folded RNA
 *
 *  If verbosity level is set to a value >0, energies of structure elements are printed to stdout
 *
 *  \ingroup eval
 *
 *  \see energy_of_circ_structure(), energy_of_structure_pt(), get_scaled_parameters()
 *
 *  \param string           RNA sequence in uppercase letters
 *  \param structure        Secondary structure in dot-bracket notation
 *  \param parameters       A data structure containing the prescaled energy contributions and the model details.
 *  \param verbosity_level  A flag to turn verbose output on/off
 *  \return                The free energy of the input structure given the input sequence in kcal/mol
 */
float energy_of_struct_par( const char *string,
                            const char *structure,
                            paramT *parameters,
                            int verbosity_level);

/**
 *  \brief Calculate the free energy of an already folded  circular RNA
 *
 *  \note OpenMP: This function relies on several global model settings variables and thus is
 *        not to be considered threadsafe. See energy_of_circ_struct_par() for a completely threadsafe
 *        implementation.
 *
 *  If verbosity level is set to a value >0, energies of structure elements are printed to stdout
 *
 *  \ingroup eval
 *
 *  \see energy_of_circ_struct_par(), energy_of_struct_par()
 *
 *  \param string           RNA sequence
 *  \param structure        Secondary structure in dot-bracket notation
 *  \param verbosity_level  A flag to turn verbose output on/off
 *  \return                The free energy of the input structure given the input sequence in kcal/mol
 */
float energy_of_circ_structure( const char *string,
                                const char *structure,
                                int verbosity_level);

/**
 *  \brief Calculate the free energy of an already folded circular RNA
 *
 *  If verbosity level is set to a value >0, energies of structure elements are printed to stdout
 *
 *  \ingroup eval
 *
 *  \see energy_of_struct_par(), get_scaled_parameters()
 *
 *  \param string           RNA sequence
 *  \param structure        Secondary structure in dot-bracket notation
 *  \param parameters       A data structure containing the prescaled energy contributions and the model details.
 *  \param verbosity_level  A flag to turn verbose output on/off
 *  \return                The free energy of the input structure given the input sequence in kcal/mol
 */
float energy_of_circ_struct_par(const char *string,
                                const char *structure,
                                paramT *parameters,
                                int verbosity_level);


float energy_of_gquad_structure(const char *string,
                                const char *structure,
                                int verbosity_level);

float energy_of_gquad_struct_par( const char *string,
                                  const char *structure,
                                  paramT *parameters,
                                  int verbosity_level);


/**
 *  \brief Calculate the free energy of an already folded RNA
 *
 *  If verbosity level is set to a value >0, energies of structure elements are printed to stdout
 *
 *  \note OpenMP: This function relies on several global model settings variables and thus is
 *        not to be considered threadsafe. See energy_of_struct_pt_par() for a completely threadsafe
 *        implementation.
 *
 *  \ingroup eval
 *
 *  \see make_pair_table(), energy_of_struct_pt_par()
 *
 *  \param string     RNA sequence
 *  \param ptable     the pair table of the secondary structure
 *  \param s          encoded RNA sequence
 *  \param s1         encoded RNA sequence
 *  \param verbosity_level a flag to turn verbose output on/off
 *  \return          the free energy of the input structure given the input sequence in 10kcal/mol
 */
int energy_of_structure_pt( const char *string,
                            short *ptable,
                            short *s,
                            short *s1,
                            int verbosity_level);

/**
 *  \brief Calculate the free energy of an already folded RNA
 *
 *  If verbosity level is set to a value >0, energies of structure elements are printed to stdout
 *
 *  \ingroup eval
 *
 *  \see make_pair_table(), energy_of_struct_par(), get_scaled_parameters()
 *
 *  \param string           RNA sequence in uppercase letters
 *  \param ptable           The pair table of the secondary structure
 *  \param s                Encoded RNA sequence
 *  \param s1               Encoded RNA sequence
 *  \param parameters       A data structure containing the prescaled energy contributions and the model details.
 *  \param verbosity_level  A flag to turn verbose output on/off
 *  \return                The free energy of the input structure given the input sequence in 10kcal/mol
 */
int energy_of_struct_pt_par(const char *string,
                            short *ptable,
                            short *s,
                            short *s1,
                            paramT *parameters,
                            int verbosity_level);



/** 
 * \brief Calculate energy of a move (closing or opening of a base pair)
 *
 *  If the parameters m1 and m2 are negative, it is deletion (opening)
 *  of a base pair, otherwise it is insertion (opening).
 *
 *  \see              make_pair_table(), energy_of_move()
 *  \param string     RNA sequence
 *  \param structure  secondary structure in dot-bracket notation
 *  \param m1         first coordinate of base pair
 *  \param m2         second coordinate of base pair
 *  \returns          energy change of the move in kcal/mol
 */
float energy_of_move( const char *string,
                      const char *structure,
                      int m1,
                      int m2);


/**
 * 
 * \brief Calculate energy of a move (closing or opening of a base pair)
 *
 *  If the parameters m1 and m2 are negative, it is deletion (opening)
 *  of a base pair, otherwise it is insertion (opening).
 *
 *  \see              make_pair_table(), energy_of_move()
 *  \param pt         the pair table of the secondary structure
 *  \param s          encoded RNA sequence
 *  \param s1         encoded RNA sequence
 *  \param m1         first coordinate of base pair
 *  \param m2         second coordinate of base pair
 *  \returns          energy change of the move in 10cal/mol
 */
int energy_of_move_pt(short *pt,
                   short *s,
                   short *s1,
                   int m1,
                   int m2);

/**
 * \brief Calculate energy of a loop
 *
 *  \param ptable     the pair table of the secondary structure
 *  \param s          encoded RNA sequence
 *  \param s1         encoded RNA sequence
 *  \param i          position of covering base pair
 *  \returns          free energy of the loop in 10cal/mol
 */
int   loop_energy(short *ptable,
                  short *s,
                  short *s1,
                  int i);

/**
 *  Calculate the free energy of an already folded RNA
 * 
 *  \note This function is not entirely threadsafe! Depending on the state of the global
 *  variable \ref eos_debug it prints energy information to stdout or not...\n
 * 
 *  \deprecated This function is deprecated and should not be used in future programs!
 *  Use \ref energy_of_structure() instead!
 * 
 *  \see              energy_of_structure, energy_of_circ_struct(), energy_of_struct_pt()
 *  \param string     RNA sequence
 *  \param structure  secondary structure in dot-bracket notation
 *  \return          the free energy of the input structure given the input sequence in kcal/mol
 */
DEPRECATED(float energy_of_struct(const char *string,
                                  const char *structure));

/**
 *  Calculate the free energy of an already folded RNA
 * 
 *  \note This function is not entirely threadsafe! Depending on the state of the global
 *  variable \ref eos_debug it prints energy information to stdout or not...\n
 * 
 *  \deprecated This function is deprecated and should not be used in future programs!
 *  Use \ref energy_of_structure_pt() instead!
 * 
 *  \see              make_pair_table(), energy_of_structure()
 *  \param string     RNA sequence
 *  \param ptable     the pair table of the secondary structure
 *  \param s          encoded RNA sequence
 *  \param s1         encoded RNA sequence
 *  \return          the free energy of the input structure given the input sequence in 10kcal/mol
 */
DEPRECATED(int energy_of_struct_pt( const char *string,
                                    short *ptable,
                                    short *s,
                                    short *s1));

/**
 *  Calculate the free energy of an already folded  circular RNA
 * 
 *  \note This function is not entirely threadsafe! Depending on the state of the global
 *  variable \ref eos_debug it prints energy information to stdout or not...\n
 * 
 *  \deprecated This function is deprecated and should not be used in future programs
 *  Use \ref energy_of_circ_structure() instead!
 * 
 *  \see              energy_of_circ_structure(), energy_of_struct(), energy_of_struct_pt()
 *  \param string     RNA sequence
 *  \param structure  secondary structure in dot-bracket notation
 *  \return          the free energy of the input structure given the input sequence in kcal/mol
 */
DEPRECATED(float energy_of_circ_struct( const char *string,
                                        const char *structure));

#endif