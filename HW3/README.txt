# Makefile

This makefile just contains a line to run my code with python3. It uses 
--fasta seq.fasta
--aln seqs.aln
--out out.txt
--match 1
--mismatch -1
--gap -2
as input arguments. If you want to use your own files or arguments you should change makefile.


## Usage

The argument list of the CLI is same as the example given in HW assignment.

$ python3 hw3.py ––fasta FASTA_FILE ––aln ALN_FILE ––out OUTPUT_FILE ––match MATCH_SCORE ––mismatch MISMATCH_SCORE ––gap GAP_PENALTY

for my files and scores it is:

$ python3 hw3.py --fasta seq.fasta --aln seqs.aln --out out.txt --match 1 --mismatch -1 --gap -2
(also specified in makefile)

## Classes and Methods

It imports seq2profile and numpy modules

sequence_to_profile: a method defined in seq2profile module
    Arguments: 
        seq:input sequence 
        freqs: occurence freqencies of each character in each location
        dct: a map from characters to integers for freqs array
        match: match score
        mismatch: mismatch score
        gap: gap penalty
    Returns:
        matrix: scores matrix of given sequence to profile
        direction: direction matrix of matrix used for back_track

back_track:
    Arguments:
        direction: specifies to direction of the score coming from (returned from sequence_to_profile)
        seq: input sequence
    Returns:
        new_seq: sequence aligned w.r.t. the profile
