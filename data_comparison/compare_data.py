import numpy as np
import os

def load_file(fileName):
    with open(fileName, 'r') as f:
        lines = f.readlines()

    nlines = len(lines)
    words = lines[0].split(',')
    ncols = len(words) - 1

    myData = np.zeros((nlines, ncols))

    for iline in range(nlines):
        words = lines[iline].split(',')
        
        if (len(words) == ncols):
            for icol in range(ncols):
                myData[iline][icol] = float(words[icol])

    return myData, nlines, ncols

def compare_values(dataComp1, dataComp2, tol):
    diff = dataComp1 - dataComp2
    return np.count_nonzero(diff > tol)

def compare_files(folder, precision, math_type, bittage, tolerance):
    msvc_file = folder + "/cpu_" + precision + "_prec_results_" + math_type + "_" + bittage + "bit_MSVC.csv" 
    icc_file = folder + "/cpu_" + precision + "_prec_results_" + math_type + "_" + bittage + "bit_ICC.csv"

    msvc_data, msvc_rows, msvc_cols = load_file(msvc_file)
    icc_data, icc_rows, icc_cols = load_file(icc_file)

    if (msvc_rows != icc_rows) or (msvc_cols != icc_cols):
        print('Matrix dimension do NOT match...')
        print('Matrix Format: ')
        print('  {} x {}'.format(msvc_rows, msvc_cols))
        return
    
    ndiffs = compare_values(msvc_data, icc_data, tolerance)

    print('    {} bit, {} precision, {}: {} values differ more than {}'.format(bittage, precision, math_type, ndiffs, tolerance))

if __name__ == "__main__":
    print('Comparing MSVC and ICC compilers for: ')

    folders = ["../Win32/Release", "../x64/Release"]
    bittages = ['32', '64']

    for ij in range(2):
        folder = folders[ij]
        bittage = bittages[ij]
        for precision in ['single', 'double']:
            for math_type in ['mult', 'pow', 'tanh']:
                compare_files(folder, precision, math_type, bittage, 1e-8)


    print('\nDone.')
    print()


    
