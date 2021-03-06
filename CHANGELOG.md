# Changelog

Below, you'll find a list of notable changes for each version of the ViennaRNA Package.

## Version 2.4.x

### [Unreleased](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.9...HEAD)


### [v2.4.9](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.8...v2.4.9) (2018-07-11)

#### Programs
  * Fix interactive mode behavior for multiple sequence alignment input in `RNAalifold`, `RNALalifold`
  * Allow for Stockholm formatted multiple sequence alignment input in `RNAeval` and `RNAplot`
  * Allow for multiple input files in `RNAeval` and `RNAplot`
  * Allow for parallel processing of input batch jobs in `RNAeval` and `RNAplot`
  * Add `-g` option to activate G-Quadruplex support in `RNAheat`
  * Warn on unsatisfiable hard constraints from dot-bracket string input in `RNAfold`, `RNAcofold`, and `RNAalifold`

#### Library
  * Fix parameter order bug in `vrna_path_findpath*` functions that resulted in too large search widths
  * Fix wrong application of base pair soft constraints in partition function computations
  * Fix position ruler string in EPS alignment output files
  * Fix MFE backtracking errors that might appear under specific hard constrained base pair patterns
  * Refrain from reading anything other than `#=GC SS_cons` to retrieve structures when parsing Stockholm 1.0 format
  * Complete soft constraints additions to Boltzmann sampling implementation for single sequences
  * Allow for disabling alignment wrapping in `vrna_file_PS_aln*` functions
  * Do not remove G-Quadruplex annotation from WUSS formatted structure strings upon calls to `vrna_db_from_WUSS`
  * Enable G-Quadruplex related average loop energy correction terms in verbose output of `vrna_eval_*` functions
  * Speed-up backward compatibility layer for energy evaluation functions that unnecessarily slowed down third-party tools using the old API
  * Allow for passing dot-bracket strings with `'&'` strand-end identifier to simple `vrna_eval_*` functions
  * Remove implicit `exit()` calls from global MFE backtracking implementation.


### [v2.4.8](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.7...v2.4.8) (2018-06-23)

#### Programs
  * Fix compilation of RNAforester with C++17 standard
  * Fix tty input detection in RNAcofold
  * Fix bad memory access with RNAcofold -p

#### Library
  * API: Fix incorrect unpaired probability computations in vrna_probs_window()
  * API: Fix potential out-of-bounds access situations (for circular RNA folding) in eval.c
  * API: Fix comparative exterior internal loop partition function computation for circfold
  * SWIG: Fix false-positive use of uninitialized value in Python3/file_py3.i

#### Package
  * TESTS: Add tests for special features in RNAalifold
  * TESTS: Add test case for RNAcofold -p


### [v2.4.7](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.6...v2.4.7) (2018-06-13)
  * Allow for parallel processing across multiple input files in RNAfold
  * Allow for arbitrary number of input files in RNAalifold
  * Allow for parallel processing of input data in RNAalifold
  * Allow for arbitrary number of input files in RNAcofold
  * Allow for parallel processing of input data in RNAcofold
  * Enable parallel processing in RNAfold, RNAcofold, RNAalifold for MS Windows build
  * Add centroid and MEA structure computation to RNAcofold
  * Add configure time check for LTO capabilities of the linker
  * Include ligand binding energies in centroid and MEA structure output of RNAfold
  * Refactor ct2db program to process multiple structures from single .ct file
  * API: Enable processing of comparative fold_compound with vrna_pr_*() functions
  * API: Refactor vrna_ostream_t to enable NULL input in vrna_ostream_provide()
  * API: Major refactoring in loop energy evaluations (MFE and PF)
  * API: Make vrna_mx_pf_aux_el_t and vrna_mx_pf_aux_ml_s opaque pointers
  * API: Make fold_compound field `type` a const attribute
  * API: Refactor MFE post-processing for circular RNAs
  * API: Add motif name/id support for unstructured domains
  * API: Remove major part of implicit exit() calls in RNAlib
  * API: Add implementations of Boyer-Moore-Horspool search algorithm
  * API: Add implementations to determine number of rotational symmetry for strings (of objects)
  * API: Make vrna_cmd_t an opaque pointer
  * API: Move headers for constraints, datastructures, io, loop energy evaluation, energy parameters, plotting, search, and utilities into separate subdirectories (backward compatibility is maintained)
  * API: Add hash table data structure
  * API: Fix discrepancy between comparative and single sequence --noLP predictions
  * API: Add functions to replace 'old API' interface of RNAstruct.h
  * API: Add functions to replace 'old API' interface of aln_util.h
  * API: Add generic soft constraints support to suboptimal structure prediction sensu Wuchty et al.
  * SWIG: Refactor callback execution for Python 2 / 3 interface to reduce overhead
  * SWIG: Fix configure-time check for Python 3 interface build
  * SWIG: Fix Python 3 IO file stream to C FILE * conversion
  * Cosmetic changes in final configure notice
  * Major changes in source tree structure of the library
  * Add autoconf checks for maintainer tools
  * Generate C strings from static PostScript files at configure time (for structure- and dot plots)
  * REFMAN: Large updates in API documentation and structure of reference manual


### [v2.4.6](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.5...v2.4.6) (2018-04-19)
  * Stabilize rounding of free energy output in RNAalifold
  * API: Fix potential rounding errors for comparative free energies in eval.c and mfe.c
  * API: Fix regression in exterior loop dangling end contributions for comparative base pair probabilities and Boltzmann sampling (introduced with v2.4.4)
  * API: Fix regression with hard constrained base pairs for comparative structure prediction (introduced with v2.4.4)
  * TESTS: Add basic tests for RNAalifold executable
  * TESTS: Ignore 'frequency of MFE structure' in RNAcofold partition function checks


### [v2.4.5](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.4...v2.4.5) (2018-04-17)
  * Allow for arbitrary number of input files in RNAfold
  * Allow for parallel processing of input data in RNAfold (UNIX only, no Windows support yet)
  * Add SHAPE reactivity support through commandline options for RNAplfold
  * Fix unstructured domain motif detection in MFE, centroid, and MEA structures computed by RNAfold
  * Limit allowed set of commands in command file for RNAcofold to hard and soft constraints
  * API: Add functions to compute equilibrium probability of particular secondary structures
  * API: Add dynamic string stream data type and associated functions
  * API: Add priority-queue like data structure with unordered fill capability and ordered output callback execution
  * API: Add functions to detect unstructured domain motifs in MFE, centroid, and MEA structures
  * API: Fix bug in sliding-window partition function computation with SHAPE reactivity and Deigan et al. conversion method
  * API: Fix application of '<' and '>' constraint symbols in dot-bracket provided constraints (was broken since v2.4.2)
  * API: Fix MEA structure computation in the presence of unstructured domains
  * API: Stabilize order of probability entries in EPS dot-plot files
  * Fix compiler warnings on wrong type of printf() in naview.c
  * Define VRNA_VERSION macro as string literal and add macros for major, minor, and patch numbers
  * Stabilize parallel make of Mac OS X installer
  * Add energy parameter set from Langdon et al. 2018
  * Add autoconf checks for POSIX threads compiler/linker support
  * SWIG: Fix 'next' is a perl keyword warnings for Perl5 wrapper
  * SWIG: Catch errors and throw execptions whenever scripting language provided callback functions are not applicable or fail
  * SWIG: Add keyword arguments and autodoc feature for Python/Python3 wrappers


### [v2.4.4](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.3...v2.4.4) (2018-03-06)
  * Change verbose output for soft-constraints derived ligand binding motifs in RNAfold
  * Allow for lowercase letters in ct2db input
  * Fix bug in interior-like G-Quadruplex MFE computation for single sequences
  * Fix autoconf switch to enable deprecation warnings
  * Fix bug in eval_int_loop() that prevented propagation of energy evaluation for loops with nick in strands
  * Fix several bugs for SHAPE reactivity related comparative partition function computations
  * Fix annotation of PostScript output for soft-constraint derived ligand binding motifs in RNAfold
  * Fix constraint indices for multibranch loops in unpaired probability computations of LPfold.c
  * Fix dangling end contributions in comparative partition function for exterior loops
  * API: Add simplified interface for vrna_pf_dimer()
  * API: Move concentraton dependent implementation for co-folding to separate compile unit
  * API: Add new API functions for exterior loop evaluations
  * API: Add simplified interfaces for energy evaluation with G-Quadruplexes and circular RNAs
  * API: Add findpath functions that allow for specification of an upper bound for the saddle point
  * Add configure-time linker check for Python3 interface
  * Add automatic CPP suggestions for deprecated function substitutes
  * Major restucturing and constraints feature additions in loop type dependent energy evaluation functions
  * Major restructuring in MFE implementations
  * Major restructuring in PF implementations
  * Minor fixes in Boltzmann sampling implementation
  * SWIG: Fix wrappers for findpath() implementation
  * SWIG: Add tons of energy evaluation wrappers
  * SWIG: Fix configure-time check of Perl5 interface build capabilities
  * SWIG: Wrap functions from walk.c and neighbor.c
  * DOC: Add some missing references to manpages of executable programs
  * REFMAN: Heavy re-ordering of the RNAlib reference manual


### [v2.4.3](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.2...v2.4.3) (2017-11-14)
  * Fix handling of dangling end contribution at sequence boundaries for sliding window base pair probability computations
  * Fix handling of base pair hard constraints in sliding-window implementations
  * Fix sliding-window pair probability computations with multibranch-loop unpaired constraints
  * Fix sliding-window non-specific base pair hard constraint implementation
  * Fix probability computation for stochastic backtracking in RNAsubopt --stochBT_en output
  * Fix regression in comparative structure prediction for circular RNAs
  * Fix LDFLAGS for scripting language interfaces in corresponding Makefiles
  * Stabilize partition function scaling by always using sfact scaling factor from model details
  * Add --pf_scale commandling parameter to RNAplfold
  * Add constraint framework for single sequence circular RNA structure prediction
  * Add RNAfold test suite to check for working implementation of constraints for circular RNAs
  * Add a brief contribution guideline CONTRIBUTING.md
  * Prevent RNAplfold from creating inf/-inf output when solution set is empty with particular hard constraints
  * Include RNAforester v2.0.1


### [v2.4.2](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.1...v2.4.2) (2017-10-13)
  * Fix G-Quadruplex energy corrections in comparative structure energy evaluations
  * Fix discrepancy in comparative exterior loop dangling end contribution of eval vs. MFE predictions
  * Fix regression in RNAup unstructuredness and interaction energy computations
  * Fix sequence length confusions when FASTA input contains carriage returns
  * Fix build problems of RNAlocmin with older compilers
  * Fix sliding-window hard constraints where single nucleotides are prohibited from pairing
  * Fix dot-bracket output string length in sliding-window MFE with G-Quadruplexes
  * Fix unpaired probability computations for separate individual loop types in LPfold.c
  * Fix bad memory access in RNAsubopt with dot-bracket constraint
  * Add full WUSS support for --SS_cons constraint option in RNAalifold
  * Add commandline option to RNALalifold that enables splitting of energy contributions into separate parts
  * Add missing hard constraint cases to sliding-window partition function implementation
  * Add CSV output option to RNAcofold
  * Use the same model details for SCI computations in RNAalifold
  * Abort computations in vrna_eval_structure_v() if structure has unexpected length
  * Use original MSA in all output generated by RNAalifold and RNALalifold
  * API: Add new functions to convert dot-bracket like structure annotations
  * API: Add various new utility functions for alignment handling and comparative structure predictions
  * API: Add function vrna_strsplit() to split string into tokens
  * API: Do not convert sequences of input MSA to uppercase letters in vrna_file_msa_read_record()
  * API: Rename vrna_annotate_bp_covar() and vrna_annotate_pr_covar()
  * API: Add new noLP neighbor generation
  * SWIG: Add wrapper for functions in file_utils_msa.h
  * SWIG: Add wrappers for vrna_pbacktrack() and vrna_pbacktrack5()
  * SWIG: Add vrna_db_to_element_string() to scripting language interface
  * REFMAN: Fix formula to image conversion in HTML output


### [v2.4.1](https://github.com/ViennaRNA/ViennaRNA/compare/v2.4.0...v2.4.1) (2017-08-23)
  * Fix memory leak in fold_compound methods of SWIG interface
  * Fix memory leaks in double ** returning functions of SWIG Perl5 interface
  * Fix memory leak in vrna_ep_t to-string() function of SWIG interface
  * Regression: Fix reverting pf_scale to defaults after vrna_exp_params_rescale()
  * Regression: Fix homo-dimer partition function computation in RNAcofold
  * Add unit tests for RNAcofold executable
  * Add SHAPE reactivity support to RNAcofold
  * Add SHAPE reactivity support to RNALalifold


### [v2.4.0](https://github.com/ViennaRNA/ViennaRNA/compare/v2.3.5...v2.4.0) (2017-08-01)
  * Bump libsvm to version 3.22
  * Print G-Quadruplex corrections in verbose mode of RNAeval
  * Change behavior of RNAfold --outfile option to something more predictable
  * Unify max_bp_span usage among sliding window prediction algorithms: RNAplfold, RNALfold, and RNALalifold now consider any base pair (i,j) with (j - i + 1) <= max_bp_span
  * Add SHAPE reactivity data support to RNALfold
  * Add commands-file support for RNALfold, RNAplfold (hard/soft constraints)
  * Add RNAlocmin - Calculate local minima from structures via gradient walks
  * Add RNA Bioinformatics tutorial (PDF version)
  * Add hard constraints to sliding-window MFE implementations (RNALfold, RNALalifold)
  * Add hard constraints to sliding-window PF implementations (RNAplfold)
  * Add soft constraints to sliding-window MFE implementation for single sequences (RNALfold)
  * Add soft constraints to sliding-window PF implementations (RNAplfold)
  * Add SWIG interfaces for sliding-window MFE/PF implementations
  * Add proper SWIG interface for alignment and structure plotting functions
  * Add proper SWIG interface for duplexfold, duplex_subopt, and its comparative variants
  * Add SWIG wrapper for vrna_exp_params_rescale()
  * Add explicit destructor for SWIG generated vrna_md_t objects
  * Add SWIG perl5 typemap for simple nested STL vectors
  * Add dummy field in vrna_structured_domains_s
  * Add note about SSE optimized code in reference manual
  * Add SWIG interface for findpath implementation
  * Add prepare() functions for ptypes-arrays and vrna_(exp_)param_t
  * Add warnings for ignored commands in function vrna_commands_apply()
  * Add callback featured functions for sliding window MFE and PF implementations
  * Change default behavior of adding soft constraints to a vrna_fold_compound_t (store only)
  * Several fixes with respect to G-Quadruplex prediction in sliding-window MFE recursions (single sequence and comparative implementation)
  * Replace comparative sliding-window MFE recursions (All hits are reported to callback and can be filtered in a post-processing step)
  * API: Remove E_mb_loop_stack() and introduce new function vrna_E_mb_loop_stack() as a replacement
  * API: change data type of all constraint bit-flags from `char` to `unsigned char`
  * API: change data type of a2s array in comparative structure prediction from `unsigned short` to `unsigned int`
  * API: Change function parameter order in vrna_probs_window() to follow the style of other callback-aware functions in RNAlib
  * Move sliding-window MFE implementations to new file mfe_window.c
  * Fix building PDF Reference manual with non-standard executable paths
  * Fix redefinition of macro ON_SAME_STRAND() in subopt.c
  * Fix dangling end issues in sliding-window MFE implementations
  * Fix regression for --canonicalBPonly switch in RNAfold/RNAcofold/RNAsubopt
  * Fix building sliding-window MFE implementation without SVM support
  * Fix parsing of STOCKHOLM 1.0 MSA files that contain MSA spanning multiple blocks
  * Fix Alidot link in RNAalifold manpage
  * Fix wrong pre-processor flags when enabling single-precision PF computations
  * Fix unit testing perl5 interface by including builddir/tests in PERL5LIB path
  * Fix buffer overflow in hairpin loop sequence motif extraction for circular RNAs
  * Fix out-of-bounds memory access in neighbor.c
  * Restore capability to compile stand-alone findpath utility
  * Restore capability to use non-standard alphabets for structure prediction
  * Restore old-API random number functions in SWIG interface
  * Allow additional control characters in MAF MSA input that do not end a block
  * Improve reference manual
  * Make functions in pair_mat.h static inline
  * Prevent users from adding out-of-range base pair soft constraints
  * Inline print functions in color_output.inc
  * Start documenting callback features in reference manual
  * Re-write large portions of sliding-window PF implementation
  * Introduce soft-constraint state flag
  * Clean-up SWIG unit test framework
  * Remove obsolete scripts ct2b.pl and colorrna.pl from src/Utils directory
  * Remove old RNAfold tutorial


## Version 2.3.x

### [v2.3.5](https://github.com/ViennaRNA/ViennaRNA/compare/v2.3.4...v2.3.5) (2017-04-14)
  * Fix duplication of output filename prefix in RNAfold
  * Add V3.0 API for sliding window partition function (a.k.a. RNAPLfold)
  * Add G-Quadruplex prediction to RNALalifold
  * Add SWIG wrappers for callback-based sliding window comparative MFE prediction
  * Add SSE4.1 multiloop decomposition for single sequence MFE prediction
  * Enable RNAfold unit tests to run in paralllel
  * Enable users to turn-off base pair probability computations in RNAcofold with -a option
  * Split move set in neighbor.c


### [v2.3.4](https://github.com/ViennaRNA/ViennaRNA/compare/v2.3.3...v2.3.4) (2017-03-10)
  * Fix G-Quadruplex probability computation for single sequences
  * Fix double-free when using SHAPE reactivity data in RNAalifold
  * Fix out-of-bounds access in strand_number array
  * Fix weighting of SHAPE reactivity data in consensus structure prediction when fewer data than sequences are present
  * Fix z-score output in RNALfold
  * Substitute field name 'A0'/'B0' in data structure vrna_dimer_conc_s by 'Ac_start'/'Bc_start' to avoid clashes with termios.h (Mac OSX Python wrapper bug)
  * Minimize usage of 'unsafe' sprintf() calls
  * Enhance auto-id feature in executable programs
  * Always sanitize output file names to avoid problems due to strange FASTA headers
  * Lift restrictions of FASTA header length in RNAfold, RNAcofold, and RNAeval
  * Add ViennaRNA/config.h with pre-processor definitions of configure time choices
  * Add test-suite for RNAfold
  * Add functions to procude colored EPS structure alignments
  * Add function to write Stockholm 1.0 formatted alignments
  * Add function to sanitize file names
  * Add callback based implementation for sliding-window MFE prediction (single sequences, comparative structure prediction)
  * Add fast API 3.0 implementations to generate structural neighbors and perform steepest descent / random walks (Thanks to Gregor!)
  * Add parameter option to RNALalifold for colored EPS structure alignment and structure plot output
  * Add parameter option to RNALalifold to write hits into Stockholm file
  * Add parameter option to RNAalifold to write Stockholm 1.0 formatted output
  * Add parameter option to RNAalifold to suppress stderr spam
  * Add auto-id feature to RNAplot, RNALfold, RNAsubopt, RNAplfold, RNAheat
  * Add SHAPE reactivity derived pseudo-energies as separate output in RNAalifold
  * Add colored output to RNA2Dfold, RNALalifold, RNALfold, RNAduplex, RNAheat, RNAinverse, RNAplfold, and RNAsubopt
  * Add command line parameters to RNAsubopt to allow for specification of input/output files


### [v2.3.3](https://github.com/ViennaRNA/ViennaRNA/compare/v2.3.2...v2.3.3) (2017-01-24)
  * Fix multiloop contributions for comparative partition function
  * Fix building python2 extension module for OSX


### [v2.3.2](https://github.com/ViennaRNA/ViennaRNA/compare/v2.3.1...v2.3.2) (2017-01-18)
  * Fix pair probability plist creation with G-Quadruplexes
  * Allow for specification of python2/3-config at configure time
  * Fix init of vrna_md_t data structure after call to set_model_details()
  * Fix bug in consensus partition function with hard constraints that force nucleotides to be paired
  * Fix compilation of functions that use ellipsis/va_list
  * Enable generic hard constraints by default
  * Fix init of partition function DP matrices for unusually short RNAs
  * Fix behavior of RNAplfold for unusually short RNAs
  * Report SCI of 0 in RNAalifold when sum of single sequence MFEs is 0
  * Avoid multiple includes of pair_mat.h
  * Add configure flag to build entirely static executables


### [v2.3.1](https://github.com/ViennaRNA/ViennaRNA/compare/v2.3.0...v2.3.1) (2016-11-15)
  * Add description for how to use unstructured domains through command files to reference manual and RNAfold manpage
  * Fix compilation issue for Windows platforms with MingW
  * Add missing newline in non-TTY-color output of vrna_message_info()
  * Fix regression in vrna_md_update() that resulted in incomplete init of reverse-basepair type array
  * Extend coverage of generic hard constraints for partition function computations
  * Fix scaling of secondary structure in EPS plot such that it always fits into bounding box
  * Several fixes and improvements for SWIG generated scripting language interface(s)


### [v2.3.0](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.10...v2.3.0) (2016-11-01)
  * Add grammar extension with structured and unstructured domains
  * Add default implementation for unstructured domains to allow for ligand/protein binding to unpaired structure segments (MFE and PF for single sequences)
  * Introduced command files that subsume constraint definition files (currently used in RNAfold and RNAcofold)
  * Replace explicit calls to asprintf() with portable equivalent functions in the library
  * Fix configure script to deal with situations where Perl module can't be build
  * Fix bug in doc/Makefile.am that prevented HTML installation due to long argument list
  * Added utility functions that deal with conversion between different units
  * Bugfix in SWIG wrapped generic soft constraint feature
  * Add subopt() and subopt_zuker() methods to SWIG wrapped fold_compound objects
  * Bugfix multiloop decomposition in MFE for circular RNAs
  * Add separate function to compute pscore for alignments
  * Renamed VRNA_VC_TYPE_* macros to VRNA_FC_TYPE_*
  * Bugfix regression that prevented programs to fail on too long input sequences
  * Extend EPS dot-plot in RNAfold to include motif/binding probabilities from unstructured domains
  * Add variadic functions for error/warning/info message
  * Add ID manipulation feature to RNAeval
  * Extend API for soft constraint feature for more fine-grained control
  * Add section on SWIG wrapped functions in reference manual
  * Fix bug in interior loop computations when hard constraints result in non-canonical base pairs


## Version 2.2.x

### [v2.2.10](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.9...v2.2.10) (2016-09-06)
  * Do not 'forget' subopt results when output is not written to file handle and sorting is switched off
  * Fix bad memory access in vrna_subopt() with sorted output
  * Add SWIG wrappers for vrna_subopt_cb()
  * Correctly show if C11 features are activated in configure status
  * Fix autoconf checks to allow for cross compilation again


### [v2.2.9](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.8...v2.2.9) (2016-09-01)
  * Fix bug in partition function scaling for backward compatibility of ali_pf_fold()
  * Stabilize v3.0 API when building RNAlib and third party program linking against it with compilers that use different C/C++ standards
  * Add details on how to link against RNAlib to the reference manual
  * Fix RNAlib2.pc
  * Fix bug for temperature setting in RNAplfold
  * Use -fflat-lto-objects for static RNAlib library to allow linking without LTO
  * Fix interpretation of 'P' hard constraint for single nucleotides in constraint definition files
  * Add 'A' command for hard constraints
  * Fix several hard constraint corner-cases in MFE and partition function computation when nucleotides must not be unpaired
  * Fix order of hard constraints when read from input file
  * Allow for non-canonical base pairs in MFE and partition function computations if hard constraints demand it
  * Fix behavior of --without-swig configure script option
  * Fix bug in hard constraints usage of exterior loop MFE prediction with odd dangles
  * Add parsers for Clustal, Stockholm, FASTA, and MAF formatted alignment files
  * Enable RNAalifold to use Clustal, Stockholm, FASTA, or MAF alignments as input
  * Lift restriction of sequence number in alignments for RNAalifold
  * Enable ANSI colors for TTY output in RNAfold, RNAcofold, RNAalifold, RNAsubopt, and warnings/errors issued by RNAlib
  * Add various new commandline options to manipulate sequence/alignment IDs in RNAfold, RNAcofold and RNAalifold


### [v2.2.8](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.7...v2.2.8) (2016-08-01)
  * Fix bad memory access in RNAalifold
  * Fix regression in RNAalifold to restore covariance contribution ratio determination for circular RNA alignments
  * Changed output of RNAsubopt in energy-band enumeration mode to print MFE and energy range in kcal/mol instead of 10cal/mol
  * Include latest Kinfold sources that make use of v3.0 API, therefore speeding up runtime substantially
  * Re-activate warnings in RNAeval when non-canonical base pairs are encountered
  * Fix syntactic incompatibilities that potentially prevented compilation with compilers other than gcc
  * dd function to compare nucleotides encoded in IUPAC format
  * Fix regression in energy evaluation for circular RNA sequences
  * Fix regression in suboptimal structure enumeration for circular RNAs
  * Allow for P i-j k-l commands in constraint definition files
  * Make free energy evaluation functions polymorphic
  * Add free energy evaluation functions that allow for specifying verbosity level
  * Secure functions in alphabet.c against NULL pointer arguments
  * Fix incompatibility with swig >= 3.0.9
  * Fix memory leak in swig-generated scripting language interface(s) for user-provided target language soft-constraint callbacks
  * Expose additional functions to swig-generated scripting language interface(s)
  * Build Python3 interface by default
  * Start of more comprehensive scripting language interface documentation
  * Fix linking of python2/python3 interfaces when libpython is in non-standard directory
  * Restructured viennarna.spec for RPM based distributions
  * Several syntactic changes in the implementation to minimize compiler warnings
  * Fix --with-*/--without-* and --enable-*/--disable-* configure script behavior


### [v2.2.7](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.6...v2.2.7) (2016-06-30)
  * Fix partition function scaling for long sequences in RNAfold, RNAalifold, and RNAup
  * Fix backtracking issue in RNAcofold when --noLP option is activated
  * Fix hard constraints issue for circular RNAs in generating suboptimal structures
  * Rebuild reference manual only when actually required


### [v2.2.6](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.5...v2.2.6) (2016-06-19)
  * Plugged memory leak in RNAcofold
  * Fixed partition function rescaling bug in RNAup
  * Fixed bug in RNALfold with window sizes larger than sequence length
  * Re-added SCI parameter for RNAalifold
  * Fixed backtracking issue for large G-quadruplexes in RNAalifold
  * Fixed missing FASTA id in RNAeval output
  * Added option to RNAalifold that allows to specify prefix for output files
  * Several fixes and additional functions/methods in scripting language interface(s)
  * Added version information for scripting language interface(s)
  * Some changes to allow for compilation with newer compilers, such as gcc 6.1


### [v2.2.5](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.4...v2.2.5) (2016-04-09)
  * Fixed regression in RNAcofold that prohibited output of concentration computations
  * Fixed behavior of RNAfold and RNAcofold when hard constraints create empty solution set (programs now abort with error message)
  * Added optional Python 3 interface
  * Added RNA::Params Perl 5 sub-package
  * Update RNA::Design Perl 5 sub-package
  * Simplified usage of v3.0 API with default options
  * Wrap more functions of v3.0 API in SWIG generated scripting language interfaces
  * Plugged some memory leaks in SWIG generated scripting language interfaces
  * Changed parameters of recursion status callback in vrna_fold_compound_t
  * Enable definition and binding of callback functions from within SWIG target language
  * Added optional subpackage Kinwalker
  * Added several configure options to ease building and packaging under MacOS X
  * Added new utility script RNAdesign.pl


### [v2.2.4](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.3...v2.2.4) (2016-02-19)
  * Fixed bug in RNAsubopt that occasionally produced cofolded structures twice
  * Removed debugging output in preparations of consensus structure prediction datastructures


### [v2.2.3](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.2...v2.2.3) (2016-02-13)
  * Added postscipt annotations for found ligand motifs in RNAfold
  * Added more documentation for the constraints features in RNAfold and RNAalifold
  * Restore backward compatibility of get_alipf_arrays()


### [v2.2.2](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.1...v2.2.2) (2016-02-08)
  * Fix regression bug that occasionally prevented backtracking with RNAcofold --noLP


### [v2.2.1](https://github.com/ViennaRNA/ViennaRNA/compare/v2.2.0...v2.2.1) (2016-02-06)
  * Fix regression bug that made RNAcofold -a unusable
  * Fix regression bug that prohibited RNAfold to compute the MEA structure when G-Quadruplex support was switched on
  * Fix bug in Kinfold to enable loading energy parameters from file
  * Fix potential use of uninitialized value in RNApdist
  * Add manpage for ct2db
  * Fix MEA computation when G-Quadruplex support is activated
  * Allow for vendor installation of the perl interface using INSTALLDIRS=vendor at configure time
  * Install architecture dependent and independent files of the perl and python interface to their correct file system locations


### [v2.2.0](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.9...v2.2.0) (2016-01-25)
  * RNAforester is now of version 2.0
  * New program RNApvmin to compute pseudo-energy pertubation vector that minimizes discrepancy between observed and predicted pairing probabilities
  * SHAPE reactivity support for RNAfold, RNAsubopt, and RNAalifold
  * Ligand binding to hairpin- and interior-loop motif support in RNAfold
  * New commandline option to limit maximum base pair span for RNAfold, RNAsubopt, RNAcofold, and RNAalifold
  * Bugfix in RNAheat to remove numerical instabilities
  * Bugfix in RNAplex to allow for computation of interactions without length limitation
  * Bugfix in RNAplot for simple layouts and hairpins of size 0
  * (generic) hard- and soft-constraints for MFE, partition function, base pair probabilities, stochastic backtracking, and suboptimal secondary structures of single sequences, sequence alignments, and sequence dimers
  * libsvm version as required for z-scoring in RNALfold is now 3.20
  * Stochastic backtracking for single sequences is faster due to usage of Boustrophedon scheme
  * First polymorphic functions vrna_mfe(), vrna_pf(), and vrna_pbacktrack().
  * The FLT_OR_DBL macro is now a typedef
  * New functions to convert between different secondary structure representations, such as helix lists, and RNAshapes abstractions
  * First object-oriented interface for new API functions in the scripting language interfaces
  * new ViennaRNA-perl submodule that augments the Perl interface to RNAlib
  * Ligand binding to hairpin- and interior-loop motif support in C-library and scripting language interfaces.
  * Libraries are generated using libtool
  * Linking of libraries and executables defaults to use Link Time Optimization (LTO)
  * Large changes in directory structure of the source code files


## Version 2.1.x

### [v2.1.9](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.8...v2.1.9)
  * Fixed integer underflow bug in RNALfold
  * Added Sequence Conservation index (SCI) option to RNAalifold
  * Fixed bug in energy evaluation of dangling ends / terminal mismatches of exterior loops and multibranch loops
  * Fixed bug in alifold partition function for circular RNAs
  * Fixed bug in alifold that scrambled backtracing with activated G-Quadruplex support
  * Fixed bug in alifold backtracking for larger G-Quadruplexes


### [v2.1.8](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.7...v2.1.8)
  * Repaired incorporation of RNAinverse user provided alphabet
  * Fix missing FASTA ID in RNAeval output
  * prevent race condition in parallel calls of Lfold()
  * Fixed memory bug in Lfold() that occured using long sequences and activated G-Quad support
  * Added latest version of switch.pl


### [v2.1.7](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.6...v2.1.7)
  * Fixed bug in RNALfold -z
  * Python and Perl interface are compiling again under MacOSX
  * Fixed handling of C arrays in Python interface
  * Added latest version of switch.pl
  * Make relplot.pl work with RNAcofold output


### [v2.1.6](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.5...v2.1.6)
  * New commandline switches allow for elimination of non-canonical base pairs from constraint structures in RNAfold, RNAalifold and RNAsubopt
  * updated moveset functions
  * final fix for discrepancy of tri-loop evaluation between partition function and mfe
  * pkg-config file now includes the OpenMP linker flag if necessary
  * New program ct2db allows for conversion of .ct files into dot-bracket notation (incl. pseudo-knot removal)


### [v2.1.5](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.4...v2.1.5)
  * Fix for discrepancy between special hairpin loop evaluation in partition functions and MFE


### [v2.1.4](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.3...v2.1.4)
  * Fix of G-quadruplex support in subopt()
  * Fix for discrepancy between special hairpin loop evaluation in partition functions and MFE


### [v2.1.3](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.2...v2.1.3)
  * RNAfold: Bugfix for ignoring user specified energy parameter files
  * RNAcofold: Bugfix for crashing upon constrained folding without specifying a constraint structure
  * RNAsubopt: Added G-quadruplex support
  * RNAalifold: Added parameter option to specify base pair probability threshold in dotplot
  * Fix of several G-quadruplex related bugs
  * Added G-quadruplex support in subopt()


### [v2.1.2](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.1...v2.1.2)
  * RNAfold: Bugfix for randomly missing probabilities in dot-plot during batch job execution
  * RNAeval: Bugfix for misinterpreted G-quadruplex containing sequences where the quadruplex starts at nucleotide 1
  * RNAsubopt: Slight changes to the output of stochastic backtracking and zuker subopt
  * Fix of some memory leaks
  * Bugfixes in zukersubopt(), assign_plist_from_pr()
  * New threadsafe variants of putoutpU_prob*() for LPfold()
  * Provision of python2 interface support. 


### [v2.1.1](https://github.com/ViennaRNA/ViennaRNA/compare/v2.1.0...v2.1.1)
  * Bugfix to restore backward compatibility with ViennaRNA Package 1.8.x API (this bug also affected proper usage of the the perl interface)


### [v2.1.0](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.7...v2.1.0)
  * G-Quadruplex support in RNAfold, RNAcofold, RNALfold, RNAalifold, RNAeval and RNAplot
  * LPfold got a new option to output its computations in split-mode
  * several G-Quadruplex related functions were introduced with this release
  * several functions for moves in an RNA landscape were introduced
  * new function in alipfold.c now enables access to the partition function matrices of alipf_fold()
  * different numeric approach was implement for concentration dependend co-folding to avoid instabilities which occured under certain circumstances


## Version 2.0.x

### [v2.0.7](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.6...v2.0.7)
  * Bugfix for RNAplfold where segfault happened upon usage of -O option
  * Corrected misbehavior of RNAeval and RNAplot in tty mode


### [v2.0.6](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.5...v2.0.6)
  * Bugfix for bad type casting with gcc under MacOSX (resulted in accidental "sequence too long" errors)
  * Bugfix for disappearing tri-/hexaloop contributions when read in from certain parameter files
  * Bugfix for RNALfold that segfaulted on short strange sequences like AT+ repeats
  * Change of RNA2Dfold output format for stochastic backtracking


### [v2.0.5](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.4...v2.0.5)
  * Restored z-score computation capabilities in RNALfold


### [v2.0.4](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.3...v2.0.4)
  * Bugfix for RNAcofold partition function
  * Perl wrapper compatibility to changed RNAlib has been restored
  * Backward compatibility for partition function calls has been restored 


### [v2.0.3](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.2...v2.0.3)
  * Bugfix for RNAalifold partition function and base pair probabilities in v2.0.3b
  * Added Boltzmann factor scaling in RNAsubopt, RNAalifold, RNAplfold and RNAcofold
  * Bugfix for alipfold() in v2.0.3b
  * Restored threadsafety of folding matrix access in LPfold.c, alipfold.c, part_func.c, part_func_co.c and part_func_up.c
  * Added several new functions regarding threadsafe function calls in terms of concurrently changing the model details
  * Added pkg-config file in the distribution to allow easy checks for certain RNAlib2 versions, compiler flags and linker flags.


### [v2.0.2](https://github.com/ViennaRNA/ViennaRNA/compare/v2.0.1...v2.0.2)
  * added support for Boltzmann factor scaling in RNAfold
  * fixed fastaheader to filename bug
  * plugged some memory leaks


### [v2.0.1]()
  * First official release of version 2.0
  * included latest bugfixes


## History

2011-03-10  Ronny Lorenz  <ronny@tbi.univie.ac.at>

  * new naming scheme for all shipped energy parameter files
  * fixed bugs that appear while compiling with gcc under MacOS X
  * fixed bug in RNAup --interaction-first where the longer of the first two
    sequences was taken as target
  * added full FASTA input support to RNAfold, RNAcofold, RNAheat, RNAplfold
    RNALfoldz, RNAsubopt and RNALfold


2010-11-24  Ronny Lorenz  <ronny@tbi.univie.ac.at>

  * first full pre-release of version 2.0


2009-11-03  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Fix memory corruption in PS_color_aln()


2009-09-09  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Fix bug in RNAplfold when -u and -L parameters are equal
  * Fix double call to free_arrays() in RNAfold.c
  * Improve drawing of cofolded structures


2009-05-14  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Fix occasional segfault in RNAalifold's print_aliout() 


2009-02-24  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Add -MEA options to RNAfold and RNAalifold
  * change energy_of_alistruct to return float not void


2009-02-24  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * RNAfold will draw structures unless -noPS is used (no more
    "structure too long" messages)
  * Restore the "alifold.out" output from RNAalifold -p
  * RNAalifold -circ did not work due to wrong return type
  * Accessibility calculation with RNAplfold would give wrong
    results for u<=30


2008-12-03  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Add zuker style suboptimals to RNAsubopt (-z)
  * get_line() should be much faster when reading huge sequences
    (e.g. whole chromosomes for RNALfold)


2008-08-12  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Add Ribosum matrices for covariance scoring in RNAalifold


2008-06-27  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Change RNAalifold to used berni's new energy evaluation w/o gaps
  * Add stochastic backtracking in RNAalifold


2008-07-04  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * modify output of RNAup (again).
    Program reading RNAup output will have to updated!


2008-07-02  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * RNAplfold now computes accessibilities for all regions up to a
    max length simultaneously. Slightly slower when only 1 value is
    needed, but much faster if all of them are wanted.
    This entails a new output format. Programs reading accessibility
    output from RNAplfold need to be updated!


2008-03-31  Stephan Bernhart  <berni@tbi.univie.ac.at>

  * add cofolding to RNAsubopt


2008-01-08  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * ensure circfold works even for open chain


2007-12-13  Ulli Mueckstein  <ulli@tbi.univie.ac.at>

  * upate RNAup related files
    RNAup can now include the intramolecular structure of both
    molecules and handles constraints.


2007-12-05  Ronny Lorenz  <ronny@tbi.univie.ac.at>

  * add circfold variants in part_func.c alipfold.c subopt.c


2007-09-19  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * compute the controid structure of the ensemble in RNAfold -p
  * fix a missing factor 2 in mean_bp_dist().
    CAUTION ensemble diversities returned by RNAfold -p are now
    twice as large as in earlier versions.


2007-09-04  Ivo Hofacker  <ivo@blini.tbi.univie.ac.at>

  * fix a bug in Lfold() where base number n-max-4 would never pair


2007-08-26  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add RNAaliduplex the alignment version of RNAduplex
  * introduce a minimal distance between hits produced by  duplex_subopt()


2007-07-03  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add a loop_energy() function to compute energy of a single loop


2007-06-23  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add aliLfold() and RNALalifold, alignment variant of Lfold()


2007-04-30  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add RNAup to distribution


2007-04-15  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * fix segfault in colorps output (thanks to Andres Varon)


2007-03-03  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * avoid unnormalized doubles in scale[], big speedup for pf_fold()
    on very long sequences


2007-02-03  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * RNAalifold can now produce colored structure plots and
    alignment plots


2007-02-01 Ivo Hofacker <ivo@tbi.univie.ac.at>

  * Fix segfault in RNAplfold because of missing prototype


2006-12-01 Ivo Hofacker <ivo@tbi.univie.ac.at>

  * RNAduplex would segfault when no structure base pairs are possible


2006-08-22  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add computation stacking probabilities using RNAfold -p2
  * add -noPS option for NRAfold to supress drawing structures


2006-08-09  Stephan Bernhart  <berni@tbi.univie.ac.at>

  * RNAplfold can now compute probabilites of unpaired regions
    (scanning version of RNAup)


2006-06-14  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * compile library with -fpic (if available) for use as shared
    library in the Perl module.
  * fix another bug when calling Lfold() repeatedly
  * fix switch cmdline parsing in RNAalifold (-mis implied -4)
  * fix bug in cofold() with dangles=0


2006-05-08  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * fix segfault in Lfold() when calling repeatedly
  * fix structure parsing in RNAstruct.c
    (thanks to Michael Pheasant for reporting both bugs)
  * add duplexfold() and alifold() to Perl module
  * distinguish window size and max pair span in LPfold


2006-04-05  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * fix performance bug in co_pf_fold()
  * use relative error for termination of Newton iteration


2006-03-02  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add circular folding in alifold()


2006-01-18  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * cleanup berni partition cofold code, including several bug fixes


2006-01-16  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * update RNAplfold to working version
  * add PS_dot_plot_turn() in  PS_dot.c


2005-11-07  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add new utilities colorna and coloraln


2005-10-11  Christoph Flamm  <xtof@tbi.univie.ac.at>

  * adapt PS_rna_plot() for drawing co-folded structures


2005-07-24  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * fix a few memory problems in structure comparison routines


2005-04-30  Ivo Hofacker  <ivo@blini.tbi.univie.ac.at>

  * add folding of circular RNAs


2005-03-11  Ivo Hofacker  <ivo@blini.tbi.univie.ac.at>

  * add -mis option to RNAalifold to give "most informative
    sequence" as consensus


2005-02-10  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * move alifold() into the library


2004-12-22  Stephan Bernhart  <berni@tbi.univie.ac.at>

  * add partition function version of RNAcofold


2004-12-23  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add RNApaln for fast structural alignments (RNApdist improvement)


2004-08-12  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * fix constrained folding in stochastic backtracking


2004-07-21  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add RNAduplex, to compute hybrid structures without
    intra-molecular pairs


2004-02-09  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * fix bug in fold that caused segfaults when using Intel compiler
  * add computation of ensemble diversity to RNAfold


2003-09-10  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add annotation options to RNAplot


2003-08-04  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * stochastic backtracking finally works. Try e.g.
    RNAsubopt -p 10


2003-07-18  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add relplot.pl and rotate_ss.pl utilities for reliability
    annotation and rotation of rna structure plots


2003-01-29  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add RNALfold program to compute locally optimal structures with
    maximum pair span.
  * add RNAcofold for computing hybrid structure


2002-11-07  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * change Make_bp_profile() and profile_edit_distance() to use
    simple (float *) arrays; makes Perl access much easier.
    RNApdist -B now works again


2002-10-28  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Improved Perl module with pod documentation; allow to write
    things like
    ($structure, $energy) = RNA::fold($seq);
    Compatibility warning: the ptrvalue() and related functions are
    gone, see the pod documentation for alternatives.


2002-10-29  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * added svg structure plots in PS_dot.c and RNAplot


2002-08-15  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * Improve reading of clustal files (alifold)
  * add a sample alifold.cgi script


2001-09-18  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * moved suboptimal folding into the library, thus it's now
    accessible from the Perl module


2001-08-31  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * added co-folding support in energy_of_struct(), and thus RNAeval


2001-04-30  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * switch from handcrafted makefiles to automake and autoconf


2001-04-05  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * added PS_rna_plot_a to produce structure plots with annotation


2001-03-03  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add alifold; predict consensus structures from alignment


2000-09-28  Ivo Hofacker  <ivo@tbi.univie.ac.at>

  * add -d3 option to RNAfold for co-axial stacking
