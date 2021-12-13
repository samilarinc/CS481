import numpy as np

def sequence_to_profile(seq, freqs, dct, match, mismatch, gap):
    m = len(seq)
    n = len(freqs[0])
    
    matrix = np.zeros((m+1, n+1))
    direction = np.zeros((m+1, n+1), dtype = int    )

    for i in range(m+1):  # row
        for j in range(n+1): #column
            if i == 0 and j == 0:
                continue
            elif i == 0 and j > 0:
                matrix[i, j] = matrix[i, j-1] + gap
            elif j == 0 and i > 0:
                matrix[i, j] = matrix[i-1, j] + gap
            else:
                freq = freqs[dct[seq[i-1]], j-1]
                score = freq * match + (1-freq) * mismatch
                left = matrix[i, j-1] + gap
                diag = matrix[i-1, j-1] + score
                up = matrix[i-1, j] + gap
                matrix[i, j] = max(left, diag) # no way to upper because up means change profile
                direction[i, j] = np.argmax((diag, left)) #diag = 0, left = 1
    return matrix, direction

def back_track(direction, seq):
    m = len(direction)
    n = len(direction[0])
    new_seq = ""

    i = m-1
    j = n-1
    k = 0

    while i > 0 and j > 0:
        if direction[i, j] == 0: #diagonal
            new_seq += seq[i-1]
            i -= 1
            j -= 1
        elif direction[i, j] == 1: #left
            new_seq += '-'
            j -= 1
        else:
            print("Error occured")
            
    while j > 0:
        new_seq += '-'
        j -= 1
    return new_seq[::-1]
    