import seq2profile as s2p
from sys import argv
from argparse import ArgumentParser
import numpy as np


parser = ArgumentParser()
parser.add_argument("--fasta", required=True)
parser.add_argument("--aln", required=True)
parser.add_argument("--out", required=True)
parser.add_argument("--match", required=True)
parser.add_argument("--mismatch", required=True)
parser.add_argument("--gap", required=True)
args = parser.parse_args()

fasta_address = args.fasta
aln_address = args.aln
out_address = args.out
match = int(args.match)
mismatch = int(args.mismatch)
gap = int(args.gap)

fasta = open(fasta_address, 'r')
fasta.readline()
seq = fasta.readline().strip()
fasta.close()

seqs = []
aln_file = open(aln_address, 'r')
for i in aln_file:
    seqs.append(i.strip().split('\t')[1])
aln_file.close()

freqs = np.zeros((5,len(seqs[0])))
dct = {'A': 0, 'C':1, 'G':2, 'T':3, '-':4}
for i in seqs:
    for j in range(len(i)):
        freqs[dct[i[j]], j] += 1
freqs /= len(seqs) # p


out = open(out_address, "w")
scores, direction = s2p.sequence_to_profile(seq, freqs, dct, match, mismatch, gap)
for i in range(len(seqs)):
    print("sequence%d\t%s"%(i+1, seqs[i]), file = out)
print("sequence\t%s"%s2p.back_track(direction, seq), file = out)
out.close()
